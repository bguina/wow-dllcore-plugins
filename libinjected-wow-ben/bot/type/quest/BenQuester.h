#pragma once

#include "../base/ABenAgent.h"
#include "WowPlugin.h"

class BenQuester : public ABenAgent
{
public:
	BenQuester(WowGame& game);
	virtual ~BenQuester();

	virtual void onResume(WowGame& game) override;
	virtual void onPause(WowGame& game) override;
	virtual void onEvaluate(WowGame& game) override;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenQuester& obj
	)
{
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
