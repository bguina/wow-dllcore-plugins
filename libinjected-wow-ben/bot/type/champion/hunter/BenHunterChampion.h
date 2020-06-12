#pragma once

#include <string>

#include "../../../gameplay/model/spell/static/hunter/AHunterRotation.h"
#include "../../../gameplay/model/spell/static/hunter/FeedPet.h"
#include "../base/ABenChampion.h"
#include "../base/ABenTamerChampion.h"

class BenHunterChampion : public ABenTamerChampion {
public:
	BenHunterChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay);
	BenHunterChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag);
	virtual ~BenHunterChampion();
	
	float getEngagementRange() const override;
	float getPreferredRange() const override;
	bool pullCombat(const std::shared_ptr<WowUnitObject>& unit) override;
	bool startCombat(const std::shared_ptr<WowUnitObject>& unit) override;

	bool feedPet();
	
protected:

	unsigned long long getPetNotificationPeriodMs() const override;
	int getPetSummonSpellId() override;
	int getPetDismissSpellId() override;
	virtual int getPetFoodItemId();

	void onPetSummoned(const std::shared_ptr<const WowUnitObject>& object) override;
	void onPetLost() override;
	
	bool onEvaluatedAsTamerChampion() override;
	
private:
	bool attack(const std::shared_ptr<const WowUnitObject>& unit);

	AHunterRotation* mSpells;
	FeedPet mFeedPet;
	uint64_t mLastPetFeed;
	std::shared_ptr<const WowUnitObject> mPet;
};
