#pragma once

#include <string>

#include "WowGame.h"

class Lua {
public:
	Lua(WowGame& game);
	virtual ~Lua();

	void run(const std::string& script);
	void run(const char* script);

protected:
	WowGame& mGame;
};
