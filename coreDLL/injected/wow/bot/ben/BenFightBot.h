#pragma once

#include "BenBot.h"

class BenFightBot : public BenBot
{
public:
	BenFightBot(WowGame& game);
	virtual ~BenFightBot();

	virtual void pause(bool paused) override;
	virtual void run() override;

protected:

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenFightBot& obj
	)
{
	out << dynamic_cast<const WowBot&>(obj);
	return out;
}
