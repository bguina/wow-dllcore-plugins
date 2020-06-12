
#include "Threat.h"

Threat::Threat(const float percent) :
	Threat(percent, 100)
{
}

Threat::Threat(const float percent, const float maxPercent) :
	mPercent(percent),
	mMaxPercent(maxPercent),
	mLevel(level::none)
{
	//defaultEstimateThreatLevel();
}

Threat::~Threat() = default;

ThreatPercent Threat::getPercent() const
{
	return mPercent;
}

Threat::level Threat::getLevel() const
{
	const auto percent(mPercent / mMaxPercent);

	if (percent > .8)
		return level::mad;
	if (percent > .66)
		return level::high;
	if (percent > .33)
		return level::average;
	if (percent > .10)
		return level::low;
	return level::none;
}

bool Threat::isMadThreat() const
{
	return level::mad == getLevel();
}

bool Threat::isHighThreat() const
{
	return level::high == getLevel();
}

bool Threat::isAverageThreat() const
{
	return level::average == getLevel();
}

bool Threat::isLowThreat() const
{
	const auto currentLevel(getLevel());

	return currentLevel == level::low || currentLevel == level::none;
}

IThreat::level Threat::defaultEstimateThreatLevel() const
{
	const auto percent(mPercent / mMaxPercent);

	if (percent > .8)
		return level::mad;
	if (percent > .66)
		return level::high;
	if (percent > .33)
		return level::average;
	if (percent > .10)
		return level::low;
	return level::none;
}
