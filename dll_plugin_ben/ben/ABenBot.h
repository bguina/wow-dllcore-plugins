#pragma once

#include <memory>
#include <string>

#include "pathfinder/IPathFinder.h"
#include "game/Lua.h"

#include "bot/BaseWowBot.h"

class ABenBot : public BaseWowBot
{
public:
	ABenBot(const std::string& tag);
	virtual ~ABenBot();

	virtual void onResume(WowGame& game) override;
	virtual void onEvaluate(WowGame& game) override;
	virtual void onPause(WowGame& game) override;

	virtual bool handleWowMessage(ServerWowMessage& cl);

	// Game has objectmanager.getActivePlayer ready
	//virtual void _onGameplayResumed() = 0;
	//virtual void _onGameplayStopped() = 0;

	//virtual void _onTargetChanged() = 0;

	//virtual void _onCombatStart() = 0;
	//virtual void _onCombatEnd() = 0;

protected:
	std::unique_ptr<IPathFinder> mPathFinder;

	virtual void _logDebug(const WowGame& game) const override;
};

