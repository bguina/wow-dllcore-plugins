#pragma once

#include "RadiusZone.h"
#include "Threat.h"

class RadiusThreatZone : public RadiusZone {
public:
	RadiusThreatZone() :
		mThreat()
	{

	}

protected:
	ThreatPercent mThreat;
};
