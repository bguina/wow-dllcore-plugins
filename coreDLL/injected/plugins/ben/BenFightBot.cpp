#include "pch.h"

#include "BenFightBot.h"

#include "../../process/wow/object/WowActivePlayerObject.h"

const std::string TAG = "BenFightBot";

BenFightBot::BenFightBot(WowGame& game) : 
	ABenFarmingBot(game, TAG)
{
}

BenFightBot::~BenFightBot()
{
}

void BenFightBot::_onRunning() {
	mDbg << "\n\n";
	mDbg << FileLogger::info << TAG << "::run()" << FileLogger::normal << std::endl;

	_logDebug();

	mDbg.flush();
}
