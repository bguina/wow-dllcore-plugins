#pragma once

#include <memory>
#include <string>

#include "bot/BaseWowBot.h"

#include "../../gameplay/IBenGameplay.h"
#include "pathfinder/IPathFinder.h"

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
	virtual void onCombatStart();
	virtual void onCombatEnd();
	virtual bool isInCombat() const;

	// What would the bot do? does it handle the situation?
	virtual bool onEvaluatedIdle() = 0;
	
	virtual bool runAway();

protected:
	std::unique_ptr<IBenGameplay> mGameplay; // gives vague, approximate translation of the current ongoing game
	std::unique_ptr<IPathFinder> mWaypoints; // we also might have an idea of where to walk
	std::shared_ptr<WowActivePlayerObject> mSelf;

private:
	// save current game state
	virtual bool snapGameFrame();
	virtual bool notifyGamePlay(bool inGameNow);
	virtual bool notifyInCombat(bool inCombatNow);

	bool mInGame;
	bool mInCombat;
};
