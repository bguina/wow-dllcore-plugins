#pragma once

#include "IBenGameInterpretation.h"

class BenGameInterpretation : IBenGameInterpretation  {
public:
	BenGameInterpretation();
	virtual ~BenGameInterpretation();

protected:
	struct Hostile {
		WowGuid128 guid;
		RadiusThreatZone aggro;
	};

	struct HostileList {
		std::list<WowGuid128> units;
		std::list<WowGuid128> keepAway;
	};

	HostileList mAggroList;
	HostileList mInterestList;
	HostileList mHostileList;

	std::vector<RadiusThreatZone> mDangerZones;
};

