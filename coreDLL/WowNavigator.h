#pragma once

#include <iostream>
#include "WowGame.h"

class WowNavigator
{
public:
	WowNavigator(
		WowGame game
	){}

protected:

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowNavigator& obj
	)
{
	// TODO print navigator state
	return out;
}
