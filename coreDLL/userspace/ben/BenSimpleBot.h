#pragma once

#include "ABenBot.h"


class BenSimpleBot : public ABenBot
{
public:
	BenSimpleBot(WowGame& game);
	virtual ~BenSimpleBot();

	virtual void onEvaluate() override;

protected:

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenSimpleBot& obj
	)
{
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
