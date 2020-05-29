#pragma once

#include "../../injected/plugin/IPlugin.h"
#include "game/WowGame.h"

/*
	Helpful reminder
	https://stackoverflow.com/questions/10334915/interface-vs-composition
*/

class WowGame;
class IWowBot;
class Client;

class WowPlugin : public IPlugin
{
public:
	WowPlugin(long botId);
	virtual ~WowPlugin();

	virtual void onD3dRender() override;

	bool handleClient(Client& cl);

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
