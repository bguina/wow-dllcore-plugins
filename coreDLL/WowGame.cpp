#include "pch.h"

#include "WowGame.h"
#include "d3d/d3d.h"

WowGame::WowGame(const uint8_t* baseAddr) : AGame(baseAddr),
mObjMgr((const uint8_t**)(getAddress() + 0x2372D48))
{}

void WowGame::update() {

	mObjMgr.scan();

	for (std::map<std::string, IGameObserver<WowGame>*>::iterator it = mObservers.begin(); it != mObservers.end(); ++it) {
		it->second->capture(*this);
	}

}

const ObjectManager WowGame::getObjectManager() const {
	return mObjMgr;
}

ObjectManager WowGame::getObjectManager() {
	return mObjMgr;
}

bool WowGame::isObjectManagerActive() const {
	return NULL != mObjMgr.getBaseAddress();
}

const char* WowGame::getVersionBuild() const {
	return (const char*)(getAddress() + 0x1C3531C);
}

const char* WowGame::getReleaseDate() const {
	return (const char*)(getAddress() + 0x1C3531C);
}

const char* WowGame::getVersion() const {
	return (const char*)(getAddress() + 0x1C35314);
}

int  WowGame::getInGameFlags() const {
	return *(int*)(getAddress() + 0x2594F40);
}

int WowGame::getIsLoadingOrConnecting() const {
	return *(int*)(getAddress() + 0x2260D50);
}

typedef char(__fastcall* Intersect) (const Vector3f*, const Vector3f*, Vector3f*, __int64, int);

bool WowGame::traceLine(const Vector3f& from, const Vector3f& to, uint64_t flags) const {
	Intersect intersect = (Intersect)(getAddress() + 0x114AC10);
	Vector3f collision = Vector3f();

	return intersect(&to, &from, &collision, flags, 0);
}

void WowGame::addObserver(const std::string& name, IGameObserver<WowGame>* observer) {
	mObservers.insert(std::pair<std::string, IGameObserver<WowGame>*>(name, observer));
}

void WowGame::removeObserver(const std::string& name) {
	auto result = mObservers.find(name);

	if (result != mObservers.end()) {
		delete result->second;
		mObservers.erase(result);
	}
}
