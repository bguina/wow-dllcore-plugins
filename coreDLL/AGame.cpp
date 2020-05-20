#include "pch.h"

#include "framework.h"

#include "AGame.h"

AGame::AGame(const uint8_t* baseAddr) : MemoryObject(baseAddr),
	mPid(GetCurrentProcessId())
{}

long AGame::getPid() const {
	return mPid;
}
