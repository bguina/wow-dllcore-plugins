#include "pch.h"

#include "Bot.h"

Bot::Bot() :
	mPaused(true)
{
}

Bot::~Bot()
{
}

void Bot::pause(bool paused) {
	mPaused = paused;
}

bool Bot::isPaused() const {
	return mPaused;
}
