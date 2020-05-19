#pragma once

#include "framework.h"

#include "MemoryObject.h"
#include "objectmanager/ObjectManager.h"

class WowGame : public MemoryObject
{
public:
	WowGame(const uint8_t* baseAddr);

	long getPid() const;

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

private:
	long mPid;
	ObjectManager mObjMgr;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowGame& obj
	)
{
	out << "[WowGame@" << (void*)obj.getBaseAddress() << "]" << std::endl;

	if (obj.isObjectManagerActive()) {
		ObjectManager objMgr = obj.getObjectManager();

		out << objMgr << std::endl;
	}

	return out;
}
