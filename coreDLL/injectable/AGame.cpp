#include "pch.h"

#include <Windows.h>

#include "AGame.h"

AGame::AGame(long pid, const uint8_t* baseAddr) :
	MemoryObject(baseAddr),
	mPid(pid)
{

}

AGame::~AGame() {

}

long AGame::getPid() const {
	return mPid;
}
