#pragma once

#include "../APausablePlugin.h"
#include "../../process/wow/WowGame.h"

/*
	Helpful reminder
	https://stackoverflow.com/questions/10334915/interface-vs-composition
*/

class IPathFinder;
class WowGame;

class AWowBot : public APausablePlugin
{
public:
	AWowBot(WowGame& game, const std::string& tag);
	virtual ~AWowBot();

	virtual void onD3dRender();

	virtual bool pause(bool paused) override;
	virtual bool isPaused() const;

protected:
	virtual void _logDebug() const;

	virtual void _onResumed();
	virtual void _onPaused();
	virtual void _onRunning() = 0;

	WowGame& mGame;

};

inline std::ostream& operator<<(std::ostream& out, const class AWowBot& obj) {
	out << "[AWowBot:" << (obj.isPaused() ? "paused" : "running") << "]";
	return out;
}
