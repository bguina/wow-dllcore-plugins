#pragma once

#include <memory>
#include <string>

#include "bot/BaseWowBot.h"

#include "../../gameplay/IBenGameRecord.h"

class ABenWowGameEvaluator;
class IBenGameEvaluator;

class ABenAgent : public BaseWowBot {
public:
	
	ABenAgent(ABenWowGameEvaluator* gameplay, const std::string& tag);
	virtual ~ABenAgent();

	// BaseWowBot
	bool attach(std::shared_ptr<WowGame> game) override;
	void onResume() override;
	void onPause() override;
	bool onEvaluate() final override;
	bool handleWowMessage(ServerWowMessage& cl) override;

	// Detect game state
	virtual void onGameLoadingStart();
	virtual void onGameLoadingEnd();
	
	virtual void onGamePlayStart();
	virtual void onGamePlayStop();

	// combat lifecycle
	virtual void onCombatStart();
	virtual void onCombatEnd();

	virtual void onUnitAppear(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitVanish(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object);

	virtual bool runAway();

protected:

	// What would the bot do? does it handle the situation?
	virtual bool onEvaluatedIdle() = 0;
	
	std::list<IWowBot*> mNestedAgents;			// we might want to iterate those when a server message is received
	std::unique_ptr<ABenWowGameEvaluator> mGameplay;	// gives vague, approximate translation of the current ongoing game
	std::shared_ptr<WowActivePlayerObject> mSelf;
};
