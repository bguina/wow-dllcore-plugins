#pragma once

#include "base/ABen.h"

class BenSimple : public ABen
{
public:
	BenSimple();
	virtual ~BenSimple();

	virtual void onEvaluate(WowGame& game) override;

protected:

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenSimple& obj
	)
{
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
