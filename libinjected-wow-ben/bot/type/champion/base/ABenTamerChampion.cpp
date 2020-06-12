
#include "ABenTamerChampion.h"
#include "../../../gameplay/AWowGameRecord.h"

ABenTamerChampion::ABenTamerChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag) :
	ABenDistantChampion(gameplay, tag),
	mLastSummonAttemptTimestamp(0)
{
}

ABenTamerChampion::~ABenTamerChampion() = default;

void ABenTamerChampion::onGamePlayStart()
{
	FileLogger dbg(mLog, "ABenTamerChampion:onGamePlayStart");
	ABenDistantChampion::onGamePlayStart();

	// resume with onPetSummoned / onPetLost
	if (!notifyPetPresence())
	{
		onPetLost();
	}
}

void ABenTamerChampion::onGamePlayStop()
{
	FileLogger dbg(mLog, "ABenTamerChampion:onGamePlayStop");
	mSummonedPet = nullptr;
	dbg << dbg.d() << "pet cleared" << dbg.endl();
	ABenDistantChampion::onGamePlayStop();
}

bool ABenTamerChampion::summonPet()
{
	FileLogger dbg(mLog, "ABenTamerChampion:summonPet");

	const auto summonCooldown(mGame->getSpellBook().getSpellCooldownTime(*mGame, getPetSummonSpellId()));

	if (summonCooldown - mGame->getTime() < 500)
	{
		mLastSummonAttemptTimestamp = GetTickCount64();
		mGame->getSpellBook().castSpell(*mGame, getPetSummonSpellId(), nullptr);
		const auto delta(mGame->getSpellBook().getSpellCooldownTime(*mGame, getPetSummonSpellId()) - mGame->getTime());
		const auto cast(delta > 0 && delta < 500);
		dbg << dbg.i() << "summonPet() cast? " << summonCooldown << "/" << cast << dbg.endl();

		return cast;
	}
	return false;
}

void ABenTamerChampion::dismissPet()
{
	mGame->getSpellBook().castSpell(*mGame, getPetDismissSpellId(), nullptr);
	mLastSummonAttemptTimestamp = 0;
}

void ABenTamerChampion::onUnitAggro(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenTamerChampion::onUnitTap(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenTamerChampion::onUnitDeath(const std::shared_ptr<const WowUnitObject> & object)
{
}

bool ABenTamerChampion::onEvaluatedAsDistantChampion()
{


	if (nullptr == mSummonedPet)
	{
		if (mLastSummonAttemptTimestamp + getPetNotificationPeriodMs() < GetTickCount64())
		{
			notifyPetPresence();
		}
	}
	return onEvaluatedAsTamerChampion();
}

bool ABenTamerChampion::notifyPetPresence()
{
	FileLogger dbg(mLog, "ABenTamerChampion:notifyPetPresence");

	const auto hadPet(nullptr != mSummonedPet);
	if (nullptr != mSelf)
	{
		mSummonedPet = mGame->getObjectManager().getObjectBySummonedGuid<WowUnitObject>(*mSelf->getGuid());
		const auto petToggle(hadPet != (nullptr != mSummonedPet));
		dbg << dbg.i() << "petToggle " << petToggle << "/" << (nullptr != mSummonedPet) << dbg.endl();
		if (petToggle)
		{
			if (hadPet) onPetLost();
			else onPetSummoned(mSummonedPet);
		}
	}
	return nullptr != mSummonedPet;
}
