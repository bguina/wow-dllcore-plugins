#pragma once

#include <memory>
#include <string>

#include "bot/BaseWowBot.h"

#include "../../gameplay/IBenGameplay.h"

class ABenAgent : public BaseWowBot {
public:
	ABenAgent(IBenGameplay* gameplay, const std::string& tag);
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
	virtual bool isInCombat() const;
	virtual void onCombatStart();
	virtual void onCombatEnd();

	virtual void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object);
	
	virtual bool runAway();

protected:
	// "on game saved"
	virtual bool updateFromSnapshot(const std::shared_ptr<const IBenGameSnapshot>& snapshot);

	// What would the bot do? does it handle the situation?
	virtual bool onEvaluatedIdle() = 0;
	
	std::list<IWowBot*> mNestedAgents;			// we might want to iterate those when a server message is received
	std::unique_ptr<IBenGameplay> mGameplay;	// gives vague, approximate translation of the current ongoing game
	std::shared_ptr<WowActivePlayerObject> mSelf;

private:
	// save current game state
	virtual bool snapGameFrame();
	virtual void notifyGamePlay(bool inGameNow);
	virtual bool notifyInCombat(bool inCombatNow);

	bool mInGame;
	bool mInCombat;
	IBenGameSnapshot::Timestamp mLastEvalTimestamp;
	std::list<std::shared_ptr<const WowUnitObject>> mAggroList;
};
