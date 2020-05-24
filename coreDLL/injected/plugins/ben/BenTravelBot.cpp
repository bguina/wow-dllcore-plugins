#include "pch.h"

#include "BenTravelBot.h"

const std::string TAG = "BenTravelBot";

BenTravelBot::BenTravelBot(WowGame& game) : 
	ABenBot(game, TAG)
{
}

BenTravelBot::~BenTravelBot()
{
}

void BenTravelBot::_onRunning() {
	mDbg << "\n\n";
	mDbg << FileLogger::info << TAG << "::run()" << FileLogger::normal << std::endl;

	_logDebug();
}
