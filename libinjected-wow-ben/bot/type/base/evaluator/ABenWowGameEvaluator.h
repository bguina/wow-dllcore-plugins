#pragma once

#include "EvaluatedWowInstant.h"
#include "IBenEvaluator.h"
#include "../../../gameplay/snapshot/IBenGameSnapshot.h"

class ABenWowGameEvaluator : public IBenEvaluator<EvaluatedWowInstant, WowGame> {
public:
	ABenWowGameEvaluator();
	virtual ~ABenWowGameEvaluator();

	virtual bool evaluateIsAttacked(IBenGameSnapshot::Timestamp time, const WowUnitObject& unit) const = 0;
	virtual float evaluatePositionThreat(IBenGameSnapshot::Timestamp time, const WowVector3f& position) const = 0;
	virtual float evaluateUnitThreat(IBenGameSnapshot::Timestamp time, const WowUnitObject& unit) const = 0;
	
protected:
	virtual void onUnitAppear(WowGuid128 guid) = 0;
	virtual void onUnitVanish(WowGuid128 guid) = 0;
	virtual void onUnitDeath(WowGuid128 guid) = 0;
	
	virtual void onUnitAggro(WowGuid128 guid) = 0;
	virtual void onUnitAggroLost(WowGuid128 guid) = 0;

};
