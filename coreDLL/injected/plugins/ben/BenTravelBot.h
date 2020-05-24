#pragma once

#include "ABenBot.h"

#include "../../process/wow/object/WowActivePlayerObject.h"

class BenTravelBot : public ABenBot
{
public:
	BenTravelBot(WowGame& game);
	virtual ~BenTravelBot();

protected:
	virtual void _onRunning() override;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenTravelBot& obj
	)
{
	out << dynamic_cast<const AWowBot&>(obj);
	return out;
}
