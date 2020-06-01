#pragma once

#include <memory>
#include <string>

#include "pathfinder/IPathFinder.h"
#include "game/Lua.h"

#include "bot/BaseWowBot.h"
#include "../../interpretation/BenGameInterpretation.h"
#include "../../interpretation/interpretator/IBenGameInterpretor.h"

class ABen : public BaseWowBot {
public:
	ABen(const std::string& tag);
	virtual ~ABen();

	// BaseWowBot
	virtual bool attach(std::shared_ptr<WowGame> game) override;
	virtual void onResume() final override;
	virtual void onEvaluate() final override;
	virtual void onPause() final override;
	virtual bool handleWowMessage(ServerWowMessage& cl) override;

	// Detect game state
	virtual void onGamePlayStarted();
	virtual void onGameplayStopped();
	
	// Detect events
	virtual void onTargetChanged();

	// combat lifecycle
	virtual void onCombatStart();
	virtual void onCombatRelief();
	virtual void onCombatEnd();

	// What would the bot do? does it handle the situation?
	virtual bool onEvaluatedIdle();

protected:
	std::unique_ptr<IBenGameInterpretor> mInterpret; // interpret?! interpret?!! what does this game tell?
	std::unique_ptr<IBenGameInterpretation> mContext; // interpret: *gives vague, approximate translation*
	std::unique_ptr<IPathFinder> mWaypoints;		 // we also we might have an idea where to walk
};
