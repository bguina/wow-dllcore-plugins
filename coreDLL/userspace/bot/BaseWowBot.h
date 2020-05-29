#pragma once

#include "../../logger/FileLogger.h"

#include "../../injectable/wow/ServerWowMessage.h"
#include "../../injectable/wow/IWowBot.h"
#include "../../injectable/wow/game/WowGame.h"
#include "../../injectable/wow/game/object/WowGuid128.h"

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
