#include "BenHunterChampion.h"

#include "../../../gameplay/model/spell/static/hunter/FeedPet.h"

BenHunterChampion::BenHunterChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay) :
	BenHunterChampion(gameplay, "BenHunterChampion")
{
}

BenHunterChampion::BenHunterChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag) :
	ABenTamerChampion(gameplay, tag),
	mSpells(new AHunterRotation()),
	mLastPetFeed(GetTickCount64())
{
}


BenHunterChampion::~BenHunterChampion() = default;

float BenHunterChampion::getEngagementRange() const
{
	return 34;
}

float BenHunterChampion::getPreferredRange() const
{
	return 32;
}

bool BenHunterChampion::pullCombat(const std::shared_ptr<WowUnitObject> & unit)
{
	FileLogger dbg(mLog, "BenHunterChampion:pullCombat");

	// todo check stuff before adding to pull list
	//if (unit->getDistanceTo(*mSelf) > getEngagementRange())
	//{
	//	dbg << dbg.w() << "out of range, move forward to " << unit->getTargetGuid() << dbg.endl();
	//	// out of range, move forward
	//	mSelf->moveTo(*mGame, unit->getPosition());
	//	return true;
	//}

	// todo check applied aspect

	dbg << dbg.w() << "startCombat " << *unit->getTargetGuid() << dbg.endl();
	//mSelf->interactWith(unit->getTargetGuid());
	//mGame->getSpellBook().orderPetToAttackTarget(*mGame, unit->getGuid());
	return startCombat(unit);
}

bool BenHunterChampion::startCombat(const std::shared_ptr<WowUnitObject> & unit)
{
	// todo check stuff before adding to combat list
	mCombatUnits.push_back(unit);
	
	return attack(unit);
}

bool BenHunterChampion::feedPet()
{
	if (nullptr != mPet)
	{
		mLastPetFeed = GetTickCount64();
		mGame->getSpellBook().castSpell(*mGame, mFeedPet.getRankSpellId(0), getPetFoodItemId(), mPet->getGuid());
		return true;
	}
	return false;
}

unsigned long long BenHunterChampion::getPetNotificationPeriodMs() const
{
	return 1000 * 60;
}

int BenHunterChampion::getPetSummonSpellId()
{
	return 883;
}

int BenHunterChampion::getPetDismissSpellId()
{
	return 2641;
}

int BenHunterChampion::getPetFoodItemId()
{
	return 4540;
}

void BenHunterChampion::onPetSummoned(const std::shared_ptr<const WowUnitObject>& object)
{
	FileLogger dbg(mLog, "BenHunterChampion:onPetSummoned");
	
	mPet = object;
	dbg << dbg.v() << "onPetSummoned() " << dbg.endl();
}

void BenHunterChampion::onPetLost()
{
	FileLogger dbg(mLog, "BenHunterChampion:onPetLost");
	
	mPet = nullptr;
	dbg << dbg.w() << "onPetLost() " << dbg.endl();
}

bool BenHunterChampion::onEvaluatedAsTamerChampion()
{
	FileLogger dbg(mLog, "BenHunterChampion:onEvaluatedAsChampion");

	if (mCombatUnits.empty()) return false;

	if (!isInCombat())
	{
		if (nullptr == mPet)
		{
			if (summonPet())
			{
				dbg << dbg.i() << "summonPet() " << dbg.endl();
				return true;
			}
		}

		if (nullptr != mPet && mLastPetFeed + 1000*60*5 < GetTickCount64())
		{
			// todo check pet feed
			dbg << dbg.i() << "feedPet() " << dbg.endl();
			feedPet();
		}
	}

	if (nullptr == mSelf)
	{
		dbg << dbg.e() << "nullptr == mSelf" << dbg.endl();
		return false;
	}

	const auto& unit(*mCombatUnits.begin());
	if (unit->getDistanceTo(*mSelf) > getEngagementRange())
	{
		//dbg << dbg.w() << "out of range, move forward to " << unit->getTargetGuid() << dbg.endl();
		// out of range, move forward
		mSelf->moveTo(*mGame, unit->getPosition());
	}
	else
	{
		const auto targetFacing(mSelf->getPosition().getFacingDegreesTo(unit->getPosition()));
		if (!faceAngleDegrees(targetFacing, 10))
		{
			dbg << dbg.w() << "adjusting to face " << targetFacing << dbg.endl();
			return true;
		}

		// todo make onTargetInRange logic
		// todo check applied aspect
		return true;

		dbg << dbg.w() << "startCombat " << *unit->getGuid() << dbg.endl();
		mSelf->interactWith(unit->getGuid());
		if (nullptr != mSpells)
		{
			auto* spell(mSpells->getNextSpell(*mGame, *unit));

			dbg << dbg.i() << "mSpells good" << dbg.endl();
			if (nullptr != spell)
			{
				if (spell->cast(*mGame, unit->getGuid()))
				{
					dbg << dbg.i() << "cast " << spell->getSpellId() << " on unit " << *unit->getGuid() << " dead? " << unit->isDead() << dbg.endl();
				}
				else
				{
					dbg << dbg.w() << "failed to cast spell " << spell->getSpellId() << " on unit " << *unit->getGuid() << dbg.endl();
				}
			}
			else
			{
				dbg << dbg.w() << "failed to cast any spell :( on unit " << *unit->getGuid() << dbg.endl();
			}
		}
		else
		{
			dbg << dbg.w() << "no spell to cast" << dbg.endl();
		}
	}

	return true;
}

bool BenHunterChampion::attack(const std::shared_ptr<const WowUnitObject> & unit)
{
	FileLogger dbg(mLog, "BenHunterChampion:attack");
	dbg << dbg.i() << "Attacking unit " << *unit->getGuid() << dbg.endl();
	auto* spell = mSpells->getNextSpell(*mGame, *unit);

	if (nullptr != spell)
	{
		dbg << dbg.i() << "casting " << spell->getSpellId() << " on unit " << *unit->getGuid() << dbg.endl();
		return spell->cast(*mGame, unit->getGuid());
	}
	dbg << dbg.e() << "No spell found " << dbg.endl();

	return false;
}
