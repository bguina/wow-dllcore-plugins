#pragma once

#include "ABenBot.h"
#include "WowPlugin.h"

class BenTravelBot : public ABenBot
{
public:
	BenTravelBot();
	virtual ~BenTravelBot();

	virtual void onResume(WowGame& game) override;
	virtual void onPause(WowGame& game) override;
	virtual void onEvaluate(WowGame& game) override;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenTravelBot& obj
	)
{
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
