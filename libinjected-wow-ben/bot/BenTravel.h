#pragma once

#include "base/ABen.h"
#include "WowPlugin.h"

class BenTravel : public ABen
{
public:
	BenTravel();
	virtual ~BenTravel();

	virtual void onResume(WowGame& game) override;
	virtual void onPause(WowGame& game) override;
	virtual void onEvaluate(WowGame& game) override;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenTravel& obj
	)
{
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
