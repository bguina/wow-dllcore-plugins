#pragma once

#include "BenBot.h"

#include "../../objectmanager/WowActivePlayerObject.h"

class BenTravelBot : public BenBot
{
public:
	BenTravelBot(WowGame& game);
	virtual ~BenTravelBot();

	virtual void run() override;

protected:

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenTravelBot& obj
	)
{
	out << dynamic_cast<const WowBot&>(obj);
	return out;
}
