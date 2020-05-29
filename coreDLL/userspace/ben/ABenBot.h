#pragma once

#include <memory>
#include <string>

#include "../bot/BaseWowBot.h"
#include "../../injectable/wow/game/Lua.h"
#include "../../injectable/wow/pathfinder/IPathFinder.h"

class ABenBot : public BaseWowBot
{
public:
	ABenBot(WowGame& game, const std::string& tag);
	virtual ~ABenBot();

	virtual void onResume() override;
	virtual void onEvaluate() override;
	virtual void onPause() override;

	virtual bool handleWowMessage(ServerWowMessage& cl);

protected:
	Lua mLua;
	std::unique_ptr<IPathFinder> mPathFinder;

	virtual void _logDebug() const override;
};

inline std::ostream& operator<<(std::ostream& out,const class ABenBot& obj) {
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
