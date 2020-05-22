#include "pch.h"

#include "../d3d/d3d.h"

#include "AGame.h"

AGame::AGame(const std::string& name, const uint8_t* baseAddr) : 
	MemoryObject(baseAddr),
	mName(name),
	mPid(GetCurrentProcessId())
{

}

AGame::~AGame() {

}

const std::string& AGame::getName() const {
	return mName;
}

long AGame::getPid() const {
	return mPid;
}
