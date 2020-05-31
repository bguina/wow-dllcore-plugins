#pragma once

#include "plugin/IPlugin.h"
#include "game/WowGame.h"

/*
	Helpful reminder
	https://stackoverflow.com/questions/10334915/interface-vs-composition
*/

#include "bot/IWowBot.h"

class WowGame;
class Client;

class WowPlugin : public IPlugin
{
public:
	WowPlugin(IWowBot* bot);
	virtual ~WowPlugin();

	virtual std::string getTag() const;

	virtual bool onD3dRender() override;

protected:
	bool _readServerMessages();

	std::unique_ptr<Client> mClient;
	bool mBotPause;
	WowGame mGame;
	FileLogger mDbg;
	std::unique_ptr<IWowBot> mBot;
};

inline std::ostream& operator<<(std::ostream& out, const class WowPlugin& obj) {
	out << "[WowPlugin@"<< &obj << "]";
	return out;
}
