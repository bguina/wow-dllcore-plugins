#pragma once

#include <memory>
#include <string>

#include "../bot/BaseWowBot.h"
#include "../../injectable/wow/game/object/WowGuid128.h"

class IPathFinder;

class ABenBot : public BaseWowBot
{
public:
	ABenBot(WowGame& game, const std::string& tag);
	virtual ~ABenBot();

	virtual void onResume() override;
	virtual void onEvaluate() override;
	virtual void onPause() override;

	virtual bool handleServerMessage(ClientMessage& serverMessage);

protected:

	std::unique_ptr<IPathFinder> mPathFinder;

	virtual void _logDebug() const override;
};

inline std::ostream& operator<<(std::ostream& out,const class ABenBot& obj) {
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
