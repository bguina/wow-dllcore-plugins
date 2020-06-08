#include "BenWowGameEvaluator.h"

#include <algorithm>

#include "IBenEvaluator.h"
#include "../../../gameplay/IBenGameRecord.h"

BenWowGameEvaluator::BenWowGameEvaluator(IBenGameRecord* record) :
	ABenWowGameEvaluator(),
	mDbg("BenWowGameEvaluator"),
	mLengthyRecord(record),
	mLastEvalTimestamp(0),
	mInstant()
{
}

BenWowGameEvaluator::~BenWowGameEvaluator() = default;

bool BenWowGameEvaluator::read(const WowGame & game)
{
	FileLogger dbg(mDbg, "extractInfo");

	// get current frame info
	if (!mInstantRecord.record(game))
	{
		// fixme fatal error!
		return false;
	}
	if (nullptr != mLengthyRecord && !mLengthyRecord->record(game))
	{
		// lost memory :( getting old!
		return false;
	}
	mSelf = game.getObjectManager().getActivePlayer();
	return true;
}

bool BenWowGameEvaluator::evaluate()
{
	FileLogger dbg(mDbg, "evaluate");
	const auto backSnapshot = mInstantRecord.back();
	const auto frontSnapshot = mInstantRecord.front();

	auto self(mSelf);
	mInstant.reset();

	// trigger onGamePlayStart / onGamePlayStop events
	if (nullptr != backSnapshot)
	{
		const auto gameToggle(backSnapshot->isInGame() != (nullptr != self));
		const auto combatToggle(nullptr != self && backSnapshot->isInCombat() != self->isInCombat());
		if (gameToggle)
			dbg << dbg.w() << "game toggle" << dbg.endl();
		if (combatToggle)
			dbg << dbg.w() << "combat toggle" << dbg.endl();

		if (gameToggle)
		{
			mInstant.gameEntered = (nullptr != self);
			mInstant.gameLeft = (nullptr == self);
		}
		
		if (combatToggle)
		{
			mInstant.combatEntered = self->isInCombat();
			mInstant.combatLeft = !self->isInCombat();
		}

		//dbg << dbg.w() << "evaluate" << backSnapshot->getTimestamp() << dbg.endl();

		auto prevEnemies = backSnapshot->getHostileList();
		auto enemies = frontSnapshot->getHostileList();

		{
			IBenGameSnapshot::UnitList oldAggroList;
			auto selfGuid(self->getGuid());
			auto predicate = [&selfGuid](const std::shared_ptr<const WowUnitSnapshot>& v)->bool {
				return v->getTargetGuid() == selfGuid;
			};

			std::copy_if(prevEnemies.begin(), prevEnemies.end(), std::back_inserter(oldAggroList), predicate);

			// trigger onUnitAggroLost
			dbg << dbg.w() << "previous aggro list of " << oldAggroList.size() << dbg.endl();
			for (auto it = oldAggroList.begin(); it != oldAggroList.end(); ++it)
			{
				const auto liveAggroUnit = frontSnapshot->getUnitByGuid((*it)->getGuid());

				if (nullptr == liveAggroUnit || liveAggroUnit->getTargetGuid() != self->getGuid())
				{
					dbg << dbg.w() << " onUnitAggroLost ts " << backSnapshot->getTimestamp() << dbg.endl();
					onUnitAggroLost(liveAggroUnit->getGuid());
				}
			}

			// trigger onUnitAggro
			for (auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				const auto& liveAggroUnit(*it);

				if (liveAggroUnit->getTargetGuid() == self->getGuid())
				{
					auto oldVersion(backSnapshot->getUnitByGuid(liveAggroUnit->getGuid()));

					if (nullptr == oldVersion || oldVersion->getTargetGuid() != self->getGuid())
					{
						dbg << dbg.w() << " onUnitAggro ts " << backSnapshot->getTimestamp() << dbg.endl();
						onUnitAggro(liveAggroUnit->getGuid());
					}
					//else
					//	dbg << dbg.w() << " target previously already in aggro? " << oldVersion << dbg.endl();

				}
			}

			// trigger onUnitDeath
			for (auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				const auto& enemy(*it);

				if (enemy->isDead())
				{
					auto oldVersion(backSnapshot->getUnitByGuid(enemy->getGuid()));

					if (nullptr == oldVersion || !oldVersion->isDead())
					{
						onUnitDeath(enemy->getGuid());
					}
				}
			}
		}
	}
	else
	{
		dbg << dbg.w() << "mBackSnapshot is null" << dbg.endl();
	}

	if (nullptr != mLengthyRecord) {
		const auto& lastMemory(mLengthyRecord->previous());
		// throttle some computation
		if (nullptr != lastMemory && backSnapshot->getTimestamp() > mLastEvalTimestamp)
		{
			// todo do stuff here?
			mLastEvalTimestamp = backSnapshot->getTimestamp();
		}
	}
	
	return true;
}

const EvaluatedWowInstant& BenWowGameEvaluator::getResult() const
{
	return mInstant;
}

void BenWowGameEvaluator::onUnitAppear(WowGuid128 guid)
{
	mInstant.appearList.push_back(guid);
}

void BenWowGameEvaluator::onUnitVanish(WowGuid128 guid)
{
	mInstant.vanishList.push_back(guid);
}

void BenWowGameEvaluator::onUnitAggro(WowGuid128 guid)
{
	mInstant.aggroList.push_back(guid);
}

void BenWowGameEvaluator::onUnitAggroLost(WowGuid128 guid)
{
	mInstant.aggroLostList.push_back(guid);
}

void BenWowGameEvaluator::onUnitDeath(WowGuid128 guid)
{
	mInstant.deathList.push_back(guid);
}

bool BenWowGameEvaluator::evaluateIsAttacked(IBenGameSnapshot::Timestamp time, const WowUnitObject& unit) const
{
	return false;
}

float BenWowGameEvaluator::evaluateUnitThreat(IBenGameSnapshot::Timestamp time, const WowUnitObject& unit) const
{
	/*auto* threat = new Threat(0);*/
	return 0;// new Threat(0);
}

float BenWowGameEvaluator::evaluatePositionThreat(IBenGameSnapshot::Timestamp time, const WowVector3f& position) const
{
	//auto* threat = new Threat();
	return 0;// new Threat(0);
}