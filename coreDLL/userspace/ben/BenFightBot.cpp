#include "pch.h"

#include "BenFightBot.h"

#include "../../injectable/wow/game/object/WowActivePlayerObject.h"

const std::string TAG = "BenFightBot";

BenFightBot::BenFightBot(WowGame& game) :
	ABenFarmingBot(game, TAG)
{
}

BenFightBot::~BenFightBot()
{
}

void BenFightBot::onResume() {
	ABenBot::onResume();

	mDbg << FileLogger::info << getTag() << ": onResume() " << FileLogger::normal << std::endl;
	mDbg.flush();
}

void BenFightBot::onPause() {
	ABenBot::onPause();

	mDbg << FileLogger::info << getTag() << ": onPause() " << FileLogger::normal << std::endl;
	mDbg.flush();
}

void BenFightBot::onEvaluate() {
	ABenBot::onEvaluate();

	if (false) {
		mDbg << "\n\n";
		mDbg << FileLogger::info << TAG << "::onEvaluate()" << FileLogger::normal << std::endl;

		_logDebug();
	}
}

