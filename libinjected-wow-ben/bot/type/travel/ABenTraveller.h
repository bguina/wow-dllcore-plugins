#pragma once

#include "../base/ABenAgent.h"
#include "WowPlugin.h"

class ABenTraveller : public ABenAgent
{
public:
	ABenTraveller(const std::string& tag);
	virtual ~ABenTraveller();

	// dynamically adapts path to avoid mobs  
	virtual bool supportsThreatsAvoidance() const;
};
