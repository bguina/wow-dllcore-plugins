#pragma once

#include <list>

#include "ThreatZone.h"

class IBenGameInterpretation  {
public:
	virtual ~IBenGameInterpretation();

	virtual std::list<RadiusThreatZone> listThreatZone() = 0;
};
