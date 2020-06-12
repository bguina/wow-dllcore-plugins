#pragma once

#include "WowBaseEvaluation.h"
#include "IBenWowGameEvaluator.h"
#include "../../../gameplay/BenGameRecord.h"
#include "../../../gameplay/snapshot/IBenWowGameSnapshot.h"

class WowGameBasicEvaluator final : public IBenWowGameEvaluator<WowBaseEvaluation> {
public:
	WowGameBasicEvaluator(BenWowGameBackBuffer* record);
	virtual ~WowGameBasicEvaluator();

	bool read(const WowGame& game) override;

	bool evaluateIsAttacked(IBenWowGameSnapshot::Timestamp time,const WowUnitObject& unit) const override;
	float evaluateUnitThreat(IBenWowGameSnapshot::Timestamp time,const WowUnitObject& unit) const override;
	float evaluatePositionThreat(IBenWowGameSnapshot::Timestamp time,const WowVector3f& position) const override;

protected:
	bool evaluate() override;

	void onUnitAppear(WowGuid128 guid) override;
	void onUnitVanish(WowGuid128 guid) override;
	void onUnitDeath(WowGuid128 guid) override;
	
	void onUnitAggro(WowGuid128 guid) override;
	void onUnitAggroLost(WowGuid128 guid) override;
	
public:
	const WowBaseEvaluation& getResult() const override;
	
private:
	mutable FileLogger mDbg;
	BenGameRecord<2, 1> mInstantRecord;
	std::shared_ptr<const WowActivePlayerObject> mSelf;
	const std::unique_ptr<IBenWowGameRecord> mLengthyRecord;
	IBenWowGameSnapshot::Timestamp mLastEvalTimestamp;
	WowBaseEvaluation mInstant;
};
