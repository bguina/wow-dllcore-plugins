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
	mDbg << FileDebugger::info << TAG << "::run()" << FileDebugger::normal << std::endl;

	_logDebug();
}
