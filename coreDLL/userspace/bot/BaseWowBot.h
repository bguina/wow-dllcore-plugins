#pragma once

#include "../../injectable/wow/game/WowGame.h"
#include "../../logger/FileLogger.h"

#include "../../injectable/wow/IWowBot.h"

/*
	Helpful reminder
	https://stackoverflow.com/questions/10334915/interface-vs-composition
*/

#include "../../injectable/wow/game/WowGame.h"
#include "../../injected/ClientMessage.h"

class BaseWowBot : public IWowBot
{
public:
	BaseWowBot(WowGame& game, const std::string& tag);
	virtual ~BaseWowBot();

	virtual const char* getTag() const override;

protected:
	virtual void _logDebug() const; 
	
	WowGame& mGame;
	FileLogger mDbg;
};

inline std::ostream& operator<<(std::ostream& out, const class IWowBot& obj) {
	out << "[IWowBot:" << obj.getTag() << "]";
	return out;
}
