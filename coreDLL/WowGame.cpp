#include "pch.h"

#include "WowGame.h"
#include "d3d/d3d.h"

WowGame::WowGame(const uint8_t* baseAddr) :
	MemoryObject(baseAddr),
	mPid(GetCurrentProcessId()),
	mWindow(FindMainWindow(mPid)),
	mObjMgr((const uint8_t**)(getBaseAddress() + 0x2372D48))
{}

long WowGame::getPid() const {
	return mPid;
}

HWND WowGame::getWindow() const {
	return mWindow;
}

const ObjectManager WowGame::getObjectManager() const {
	return mObjMgr;
}

bool WowGame::isObjectManagerActive() const {
	return NULL != mObjMgr.getBaseAddress();
}

const char* WowGame::getVersionBuild() const {
	return (const char*)(getBaseAddress() + 0x1C3531C);
}

const char* WowGame::getReleaseDate() const {
	return (const char*)(getBaseAddress() + 0x1C3531C);
}

const char* WowGame::getVersion() const {
	return (const char*)(getBaseAddress() + 0x1C35314);
}

int  WowGame::getInGameFlags() const {
	return *(int*)(getBaseAddress() + 0x2594F40);
}

int WowGame::getIsLoadingOrConnecting() const {
	return *(int*)(getBaseAddress() + 0x2260D50);
}

typedef char(__fastcall* Intersect) (const Vector3f*, const Vector3f*, Vector3f*, __int64, int);

bool WowGame::traceLine(const Vector3f& from, const Vector3f& to, uint64_t flags) const {
	Intersect intersect = (Intersect)(getBaseAddress() + 0x114AC10);
	Vector3f collision = Vector3f();

	return intersect(&to, &from, &collision, flags, 0);
}
