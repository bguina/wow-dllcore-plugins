#pragma once

#include "AGame.h"
#include "observers/IGameObserver.h"
#include "objectmanager/ObjectManager.h"

class WowGame : public AGame
{
public:
	WowGame(const uint8_t* baseAddr);

	const ObjectManager getObjectManager() const;

	ObjectManager getObjectManager();

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
	void addObserver(const std::string& name, IGameObserver<WowGame>* observer);
	void removeObserver(const std::string& name);

private:
	std::map<std::string, IGameObserver<WowGame>*> mObservers;
	ObjectManager mObjMgr;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowGame& obj
	)
{
	out << "[WowGame@" << (void*)obj.getAddress() << "]" << std::endl;
	if (obj.isObjectManagerActive()) {
		ObjectManager objMgr = obj.getObjectManager();

		out << objMgr << std::endl;
	}

	return out;
}
