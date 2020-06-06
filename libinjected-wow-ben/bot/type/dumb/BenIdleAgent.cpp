
#include "BenIdleAgent.h"
#include "../../gameplay/BenRecordedGameplay.h"
#include "../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

const std::string TAG = "BenIdleAgent";

BenIdleAgent::BenIdleAgent() :
	ABenChampion(new BenRecordedGameplay<20000, 1000>(new BenGameSnapshotEvaluator()), TAG, nullptr)
{
}

BenIdleAgent::~BenIdleAgent() = default;

void BenIdleAgent::onResume()
{
}

void BenIdleAgent::onPause()
{
}

void BenIdleAgent::onGamePlayStart()
{
	FileLogger dbg(mDbg, "onGamePlayStart");

	dbg << dbg.i() << "call" << dbg.endl();
}

void BenIdleAgent::onGamePlayStop()
{
	FileLogger dbg(mDbg, "onGamePlayStop");

	dbg << dbg.i() << "call" << dbg.endl();
}

void BenIdleAgent::onCombatStart()
{
	FileLogger dbg(mDbg, "onCombatStart");

	dbg << dbg.i() << "call" << dbg.endl();
}

void BenIdleAgent::onCombatEnd()
{
	FileLogger dbg(mDbg, "onCombatEnd");

	dbg << dbg.i() << "call" << dbg.endl();
}

bool BenIdleAgent::isPositionSatisfying(const WowVector3f & position)
{
	return true;
}

bool BenIdleAgent::attack(const WowUnitObject & unit)
{
	return false;
}

void BenIdleAgent::onUnitTap(const WowUnitObject & object)
{
	FileLogger dbg(mDbg, "onUnitTap");

	dbg << dbg.v() << "onUnitTap " << dbg.endl();
}

void BenIdleAgent::onUnitAggro(const WowUnitObject & object)
{
	FileLogger dbg(mDbg, "onUnitAggro");

	dbg << dbg.w() << "onUnitAggro " << dbg.endl();
}

void BenIdleAgent::onUnitUnaggro(const WowUnitObject & object)
{
	FileLogger dbg(mDbg, "onUnitUnaggro");

	dbg << dbg.w() << "onUnitUnaggro " << dbg.endl();
}

void BenIdleAgent::onUnitKill(const WowUnitObject & object)
{
	FileLogger dbg(mDbg, "onUnitKill");

	dbg << dbg.i() << "onUnitKill" << dbg.endl();
}

float BenIdleAgent::getEngagementRange() const
{
	return 0;
}

float BenIdleAgent::getPreferredRange() const
{
	return 30;
}

bool BenIdleAgent::onEvaluatedInFight()
{
	const auto& frame(mGameplay->getLastFrame());
	FileLogger dbg(mDbg, "onEvaluatedIdle");

	if (isInCombat())
	{
		// I was in combat before?
		const auto targetTimestamp(GetTickCount64() - 2000);
		const auto& snapshot((*mGameplay)[targetTimestamp]);

		if (nullptr != snapshot)
		{
			dbg << dbg.i() << "found snapshot with delta" << IBenGameplay::timestampDelta(targetTimestamp, snapshot->getTimestamp()) << dbg.endl();
		}
	}
	else
	{
		//const auto targetTimestamp(GetTickCount64() - 2000);
		//const auto& snapshot((*mGameplay)[targetTimestamp]);

		//dbg << dbg.i() << "current" << mGameplay->getRecordedDuration() << dbg.endl();
		//dbg << dbg.i() << "max    " << mGameplay->getMaxRecordDuration() << dbg.endl();
	}

	

	//dbg << dbg.d() << "idle success at frame" << frame.getTimestamp() << dbg.endl();
	return true;
}

