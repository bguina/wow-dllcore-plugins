#pragma once

#include "debugger/FileDebugger.h"
#include "AGame.h"
#include "observers/IGameObserver.h"
#include "objectmanager/ObjectManager.h"
#include "spellbook/SpellBookManager.h"

class WowGame : public AGame
{
public:
	WowGame(const uint8_t* baseAddr);

	const ObjectManager getObjectManager() const;

	ObjectManager getObjectManager();

	const SpellBookManager getSpellBookManager() const;

	bool isObjectManagerActive() const;

	void update();

	const char* getVersionBuild() const;

	const char* getReleaseDate() const;

	const char* getVersion() const;

	int getInGameFlags() const;

	int getIsLoadingOrConnecting() const;

	bool traceLine(const Vector3f& from, const Vector3f& to, uint64_t flags) const;

	/**
		Give WowGame ownership of an observer object.
		Observer gets freed upon removal.
	*/
	bool addObserver(const std::string& name, const std::shared_ptr<IGameObserver<WowGame>>& observer);
	bool removeObserver(const std::string& name);

private:
	FileDebugger mDbg;
	std::map<std::string, std::shared_ptr<IGameObserver<WowGame>>> mObservers;
	ObjectManager mObjMgr;
	SpellBookManager mSpellBookMgr;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowGame& obj
	)
{
	out << "[WowGame@" << (void*)obj.getAddress() << "]" << std::endl;
	ObjectManager objMgr = obj.getObjectManager();
	out << objMgr;

	if (obj.isObjectManagerActive()) {
		SpellBookManager spellBookMgr = obj.getSpellBookManager();
		out << spellBookMgr;
	}
	return out;
}
