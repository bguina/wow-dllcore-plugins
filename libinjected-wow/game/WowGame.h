#pragma once

#include "observer/ARecurrentServerObserver.h"

#include "FileLogger.h"
#include "ObjectManager.h"
#include "SpellBook.h"

#include "AGame.h"

typedef uint64_t WowGameTime;

class IWindowController;

class WowGame : public AGame
{
public:
	WowGame(long pid, const uint8_t* baseAddr);
	~WowGame();

	const IWindowController* getWindowController() const;
	IWindowController* getWindowController() override;

	void update();

	bool isLoggedIn() const;
	bool isLoading() const;
	bool isInGame() const;

	const ObjectManager& getObjectManager() const;

	ObjectManager& getObjectManager();

	const SpellBook& getSpellBook() const;

	SpellBook& getSpellBook();

	const char* getVersionBuild() const;

	const char* getReleaseDate() const;

	const char* getVersion() const;

	int getInGameFlags() const;


	bool traceLine(const WowVector3f& from, const WowVector3f& to, uint64_t flags) const;
	bool addObserver(const std::string& name, const std::shared_ptr<ARecurrentServerObserver<WowGame>>& observer);
	bool removeObserver(const std::string& name);

private:
	FileLogger mDbg;
	std::map<std::string, std::shared_ptr<ARecurrentServerObserver<WowGame>>> mObservers;
	ObjectManager mObjMgr;
	SpellBook mSpellBook;
	std::unique_ptr<IWindowController> mWindowController;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowGame& obj
	)
{
	out << "[WowGame@" << (void*)obj.getAddress() << "]" << std::endl;
	ObjectManager objMgr = obj.getObjectManager();
	out << objMgr;

	if (obj.isInGame()) {
		SpellBook spellBookMgr = obj.getSpellBook();
		out << spellBookMgr;
	}

	return out;
}
