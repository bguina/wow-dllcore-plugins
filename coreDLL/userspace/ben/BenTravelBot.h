#pragma once

#include "ABenBot.h"

#include "../../injectable/wow/game/object/WowActivePlayerObject.h"

class BenTravelBot : public ABenBot
{
public:
	BenTravelBot(WowGame& game);
	virtual ~BenTravelBot();

	virtual void onResume() override;
	virtual void onPause() override;
	virtual void onEvaluate() override;

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenTravelBot& obj
	)
{
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
