#pragma once

#include "base/ABen.h"

class BenSimple : public ABen
{
public:
	BenSimple();
	virtual ~BenSimple();

protected:

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenSimple& obj
	)
{
	//out << dynamic_cast<const ABen&>(obj);
	return out;
}
