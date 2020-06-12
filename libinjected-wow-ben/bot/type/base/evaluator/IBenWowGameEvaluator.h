#pragma once

#include "IBenEvaluator.h"
#include "../../../gameplay/snapshot/IBenWowGameSnapshot.h"

template<typename Evaluation>
class IBenWowGameEvaluator : public IBenEvaluator<Evaluation, WowGame> {
public:
	virtual ~IBenWowGameEvaluator() = default;

	virtual bool evaluateIsAttacked(IBenWowGameSnapshot::Timestamp time, const WowUnitObject& unit) const = 0;
	virtual float evaluatePositionThreat(IBenWowGameSnapshot::Timestamp time, const WowVector3f& position) const = 0;
	virtual float evaluateUnitThreat(IBenWowGameSnapshot::Timestamp time, const WowUnitObject& unit) const = 0;
	
protected:
	virtual void onUnitAppear(WowGuid128 guid) = 0;
	virtual void onUnitVanish(WowGuid128 guid) = 0;
	virtual void onUnitDeath(WowGuid128 guid) = 0;
	
	virtual void onUnitAggro(WowGuid128 guid) = 0;
	virtual void onUnitAggroLost(WowGuid128 guid) = 0;

};
