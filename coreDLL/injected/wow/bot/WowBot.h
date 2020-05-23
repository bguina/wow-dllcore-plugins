#pragma once

#include "../../bot/Bot.h"

#include "../../../debugger/FileDebugger.h"

class IPathFinder;
class WowGame;

class WowBot : public Bot
{
public:
	WowBot(WowGame& game, const std::string& tag);
	virtual ~WowBot();

	virtual void run() = 0;

	virtual void pause(bool paused) override;
	virtual bool isPaused() const;



protected:
	virtual void logDebug() const;

	WowGame& mGame;
	FileDebugger mDbg;
};

inline std::ostream& operator<<(std::ostream& out, const class WowBot& obj) {
	out << "[WowBot:" << (obj.isPaused() ? "paused" : "running") << "]";
	return out;
}
