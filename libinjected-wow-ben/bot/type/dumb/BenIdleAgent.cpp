
#include "BenIdleAgent.h"
#include "../../gameplay/BenGameRecord.h"
#include "../base/evaluator/BenWowGameEvaluator.h"

const std::string TAG = "BenIdleAgent";

BenIdleAgent::BenIdleAgent() :
	ABenChampion(new BenWowGameEvaluator(new BenGameRecord<20000, 1000>()), TAG, nullptr)
{
}

BenIdleAgent::~BenIdleAgent() = default;

void BenIdleAgent::onResume()
{
	ABenAgent::onResume();
}

void BenIdleAgent::onPause()
{
	ABenAgent::onPause();
}

void BenIdleAgent::onGamePlayStart()
{
	FileLogger dbg(mDbg, "onGamePlayStart");

	dbg << dbg.i() << dbg.endl();
}

void BenIdleAgent::onGamePlayStop()
{
	FileLogger dbg(mDbg, "onGamePlayStop");

	dbg << dbg.i() << dbg.endl();
}

void BenIdleAgent::onCombatStart()
{
	FileLogger dbg(mDbg, "onCombatStart");

	dbg << dbg.i() << dbg.endl();
}

void BenIdleAgent::onCombatEnd()
{
	FileLogger dbg(mDbg, "onCombatEnd");

	dbg << dbg.i() << dbg.endl();
}

bool BenIdleAgent::isPositionSatisfying(const WowVector3f& position)
{
	return true;
}

bool BenIdleAgent::pull(const WowUnitObject& unit)
{
	return false;
}

bool BenIdleAgent::engage(const WowUnitObject& unit)
{
	return false;
}

void BenIdleAgent::onUnitTap(const std::shared_ptr<const WowUnitObject>& object)
{
	FileLogger dbg(mDbg, "onUnitTap");

	dbg << dbg.v() << "onUnitTap " << dbg.endl();
}

void BenIdleAgent::onUnitAggro(const std::shared_ptr<const WowUnitObject>& object)
{
	FileLogger dbg(mDbg, "onUnitAggro");

	dbg << dbg.w() << "onUnitAggro " << dbg.endl();
}

void BenIdleAgent::onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object)
{
	FileLogger dbg(mDbg, "onUnitAggroLost");

	dbg << dbg.w() << "onUnitAggroLost " << dbg.endl();
}

void BenIdleAgent::onUnitDeath(const std::shared_ptr<const WowUnitObject>& object)
{
	FileLogger dbg(mDbg, "onUnitDeath");

	dbg << dbg.i() << "onUnitDeath" << dbg.endl();
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
	//FileLogger dbg(mDbg, "onEvaluatedIdle");

	if (nullptr != mSelf && mSelf->isInCombat())
	{
		// I was in combat before?
		const auto targetTimestamp(GetTickCount64() - 2000);
		//const auto& snapshot((*mGameplay)[targetTimestamp]);

		//dbg << dbg.i() << "found snapshot with delta" << IBenGameRecord::timestampDelta(targetTimestamp, snapshot->getTimestamp()) << dbg.endl();
	}
	else
	{
		const auto targetTimestamp(GetTickCount64() - 2000);
		//const auto& snapshot((*mGameplay)[targetTimestamp]);

		//dbg << dbg.i() << "current delta " << IBenGameRecord::timestampDelta(targetTimestamp, snapshot->getTimestamp()) << dbg.endl();
	}

	//const auto& frame(mGameplay->front());
	//dbg << dbg.d() << "idle success at frame" << frame.getTimestamp() << dbg.endl();
	return false;
}

