#pragma once

#include "ABenBot.h"

class BenSimpleBot : public ABenBot
{
public:
	BenSimpleBot();
	virtual ~BenSimpleBot();

	virtual void onEvaluate(WowGame& game) override;

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
