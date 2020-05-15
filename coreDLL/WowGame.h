#pragma once

#include <iostream>
#include <Windows.h>
#include "MemoryObject.h"
#include "objectmanager/ObjectManager.h"

class WowGame : public MemoryObject
{
public:
	WowGame(
		const uint8_t* baseAddr
	) : MemoryObject(baseAddr),
		mObjMgr((const uint8_t**)(getBaseAddress() + 0x2372D48))
	{}

	const ObjectManager getObjectManager() const {
		return mObjMgr;
	}

	bool isObjectManagerActive() const {
		return NULL != mObjMgr.getBaseAddress();
	}

	const char* getVersionBuild() const {
		return (const char*)(getBaseAddress() + 0x1C3531C);
	}

	const char* getReleaseDate() const {
		return (const char*)(getBaseAddress() + 0x1C3531C);
	}

	const char* getVersion() const {
		return (const char*)(getBaseAddress() + 0x1C35314);
	}

	int getInGameFlags() const {
		return *(int*)(getBaseAddress() + 0x2594F40);
	}

	int getIsLoadingOrConnecting() const {
		return *(int*)(getBaseAddress() + 0x2260D50);
	}

	//bool traceLine(const Vector3f& from, const Vector3f& to, uint64_t flags) const {
	//	Vector3f collision = Vector3f();
	//	char(__fastcall * intersect) (const Vector3f*, const Vector3f*, Vector3f*, __int64, int) = (char(__fastcall*) (const Vector3f * to, const Vector3f * from, Vector3f * collision, __int64 flags, int uiOptional))(getBaseAddress() + 0x114AC10);
//
	//	return intersect(&to, &from, &collision, flags, 0);
	//}

private:
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
