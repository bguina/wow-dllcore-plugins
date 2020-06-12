#pragma once

#include "IThreat.h"

class Threat : public IThreat
{
public:
	Threat(float percent);
	Threat(float percent, float maxPercent);
	~Threat();
	
	ThreatPercent getPercent() const override;
	
	level getLevel() const override;

	bool isMadThreat() const override;

	bool isHighThreat() const override;

	bool isAverageThreat() const override;

	bool isLowThreat() const override;

protected:
	level defaultEstimateThreatLevel() const;
	
	float mPercent;
	float mMaxPercent;
	level mLevel;
};

