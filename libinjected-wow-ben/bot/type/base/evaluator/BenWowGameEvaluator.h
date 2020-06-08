#pragma once

#include "ABenWowGameEvaluator.h"
#include "../../../gameplay/BenGameRecord.h"
#include "../../../gameplay/snapshot/IBenGameSnapshot.h"

class BenWowGameEvaluator final : public ABenWowGameEvaluator {
public:
	BenWowGameEvaluator(IBenGameRecord* record);
	virtual ~BenWowGameEvaluator();

	bool read(const WowGame& game) override;

	bool evaluateIsAttacked(IBenGameSnapshot::Timestamp time,const WowUnitObject& unit) const override;
	float evaluateUnitThreat(IBenGameSnapshot::Timestamp time,const WowUnitObject& unit) const override;
	float evaluatePositionThreat(IBenGameSnapshot::Timestamp time,const WowVector3f& position) const override;

protected:
	bool evaluate() override;

	void onUnitAppear(WowGuid128 guid) override;
	void onUnitVanish(WowGuid128 guid) override;
	void onUnitDeath(WowGuid128 guid) override;
	
	void onUnitAggro(WowGuid128 guid) override;
	void onUnitAggroLost(WowGuid128 guid) override;
	
public:
	const EvaluatedWowInstant& getResult() const override;
	
private:
	mutable FileLogger mDbg;
	BenGameRecord<2, 1> mInstantRecord;
	std::shared_ptr<const WowActivePlayerObject> mSelf;
	const std::unique_ptr<IBenGameRecord> mLengthyRecord;
	IBenGameSnapshot::Timestamp mLastEvalTimestamp;
	EvaluatedWowInstant mInstant;
};
