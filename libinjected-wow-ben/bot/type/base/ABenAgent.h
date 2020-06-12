#pragma once

#include <memory>
#include <string>

#include "bot/BaseWowBot.h"

#include "../../gameplay/IWowGameRecord.h"
#include "evaluator/WowBaseEvaluation.h"

class ISpellDefinition;
template<typename Model> class IBenWowGameEvaluator;

class ABenAgent : public BaseWowBot {
public:
	ABenAgent(std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>> gameplay, const std::string& tag);
	virtual ~ABenAgent();

	// BaseWowBot
	void attach(std::shared_ptr<WowGame> game) override;
	void onResume() override;
	void onPause() override;
	bool onEvaluate() final override;
	bool handleWowMessage(ServerWowMessage& cl) override;

	virtual bool faceAngleDegrees(int angle, int anglePrecision);
	virtual bool runAway();

protected:
	virtual void notifyGameplay();
	
	// game state
	virtual void onGameLoadingStart();
	virtual void onGameLoadingEnd();

	virtual void onGamePlayStart();
	virtual void onGamePlayStop();

	// game events
	virtual void onCombatStart();
	virtual void onCombatEnd();

	// game unit events
	virtual void onUnitPop(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitDepop(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitTap(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitUntap(const std::shared_ptr<const WowUnitObject>& object);
	
	// What would the bot do? does it handle the situation?
	virtual bool onEvaluatedIdle() = 0;

	std::list<ABenAgent*> mNestedAgents;			// we might want to iterate those when a server message is received
	std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>> mGameplay;	// gives vague, approximate translation of the current ongoing game
	std::shared_ptr<WowActivePlayerObject> mSelf;
};
