#pragma once

#include "../base/ABenAgent.h"

class ABenTraveler : public ABenAgent
{
public:
	ABenTraveler(const std::string& tag);
	virtual ~ABenTraveler();

	// dynamically adapts path to avoid mobs  
	virtual bool supportsThreatsAvoidance() const;
};
