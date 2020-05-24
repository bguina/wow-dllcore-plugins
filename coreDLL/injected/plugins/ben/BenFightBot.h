#pragma once

#include "ABenFarmingBot.h"

class BenFightBot : public ABenFarmingBot
{
public:
	BenFightBot(WowGame& game);
	virtual ~BenFightBot();

protected:
	virtual void _onRunning() override;
};

inline std::ostream& operator<<(std::ostream& out,const class BenFightBot& obj) {
	out << dynamic_cast<const AWowBot&>(obj);
	return out;
}
