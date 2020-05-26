#pragma once

#include "../../injected/plugin/IMessageablePlugin.h"
#include "game/WowGame.h"

/*
	Helpful reminder
	https://stackoverflow.com/questions/10334915/interface-vs-composition
*/

class WowGame;
class IWowBot;

class WowPlugin : public IMessageablePlugin
{
public:
	WowPlugin(long pid, HMODULE moduleBaseAddress);
	virtual ~WowPlugin();

	virtual void onD3dRender() override;

	virtual bool handleServerMessage(ClientMessage& serverMessage) override;

protected:
	bool mBotPause;
	WowGame mGame;
	FileLogger mDbg;
	std::unique_ptr<IWowBot> mBot;
};

inline std::ostream& operator<<(std::ostream& out, const class WowPlugin& obj) {
	out << "[WowPlugin@"<< &obj << "]";
	return out;
}
