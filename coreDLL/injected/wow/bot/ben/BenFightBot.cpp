#include "pch.h"

#include "BenFightBot.h"

#include "../../objectmanager/WowActivePlayerObject.h"

const std::string TAG = "BenFightBot";

BenFightBot::BenFightBot(WowGame& game) : 
	BenBot(game, TAG)
{
}

BenFightBot::~BenFightBot()
{
}

void BenFightBot::pause(bool paused) {
	WowBot::pause(paused);

	mDbg << FileDebugger::info << TAG << " " << (mPaused ? "paused" : "running") << FileDebugger::normal << std::endl;
}

void BenFightBot::run() {
	mDbg << "\n\n";
	mDbg << FileDebugger::info << TAG << "::run()" << FileDebugger::normal << std::endl;

	logDebug();


}
