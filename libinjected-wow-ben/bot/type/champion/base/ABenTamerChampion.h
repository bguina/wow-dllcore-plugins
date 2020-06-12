#pragma once

#include <string>

#include "ABenDistantChampion.h"
#include "ABenChampion.h"

template<typename Evaluation> class IBenWowGameEvaluator;

class ABenTamerChampion : public ABenDistantChampion {
public:
	ABenTamerChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag);
	virtual ~ABenTamerChampion();
	
	void onGamePlayStart() override;
	void onGamePlayStop() override;

	// return false if summoning pet is not possible
	virtual bool summonPet();
	
	virtual void dismissPet();

protected:
	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;

	bool onEvaluatedAsDistantChampion() final override;

	virtual unsigned long long getPetNotificationPeriodMs() const = 0;
	virtual int getPetSummonSpellId() = 0;
	virtual int getPetDismissSpellId() = 0;
	
	virtual void onPetSummoned(const std::shared_ptr<const WowUnitObject>& object) = 0;
	virtual	void onPetLost() = 0;
	
	virtual bool onEvaluatedAsTamerChampion() = 0;

private:
	bool notifyPetPresence();

	unsigned long long mLastSummonAttemptTimestamp;
	std::shared_ptr<const WowUnitObject> mSummonedPet;
};
