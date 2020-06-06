#pragma once

typedef float ThreatPercent;

class IThreat
{
public:
	enum class level
	{
		mad,
		high,
		average,
		low,
		none
	};
	
	virtual ~IThreat() = default;

	virtual ThreatPercent getPercent() const = 0;
	
	virtual level getLevel() const = 0;

	virtual bool isMadThreat() const = 0;

	virtual bool isHighThreat() const = 0;

	virtual bool isAverageThreat() const = 0;

	virtual bool isLowThreat() const = 0;
};
