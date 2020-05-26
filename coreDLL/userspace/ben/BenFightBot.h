#pragma once

#include "farm/ABenFarmingBot.h"

class BenFightBot : public ABenFarmingBot
{
public:
	BenFightBot(WowGame& game);
	virtual ~BenFightBot();

	virtual void onResume() override;
	virtual void onEvaluate() override;
	virtual void onPause() override;

protected:
};

inline std::ostream& operator<<(std::ostream& out,const class BenFightBot& obj) {
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
