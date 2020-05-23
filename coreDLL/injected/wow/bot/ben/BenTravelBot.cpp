#include "pch.h"


#include "BenTravelBot.h"


const std::string TAG = "BenTravelBot";

BenTravelBot::BenTravelBot(WowGame& game) : 
	BenBot(game, TAG)
{
}

BenTravelBot::~BenTravelBot()
{
}

void BenTravelBot::run() {
	mDbg << "\n\n";
	mDbg << FileDebugger::info << TAG << "::run()" << FileDebugger::normal << std::endl;

	logDebug();
}
