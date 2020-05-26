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
}

void BenFightBot::onEvaluate() {
	ABenBot::onEvaluate();

	if (false) {
		mDbg << "\n\n";
		mDbg << FileLogger::info << TAG << "::run()" << FileLogger::normal << std::endl;

		_logDebug();

	}
}

void BenFightBot::onPause() {
	ABenBot::onPause();
}
