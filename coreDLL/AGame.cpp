#include "pch.h"

#include "framework.h"
#include "d3d/d3d.h"

#include "AGame.h"

AGame::AGame(const uint8_t* baseAddr) : 
	MemoryObject(baseAddr),
	mPid(GetCurrentProcessId()),
	mWindowController(FindMainWindow(GetCurrentProcessId()))
{}

long AGame::getPid() const {
	return mPid;
}

const WindowController& AGame::getWindowController() const {
	return mWindowController;
}

WindowController& AGame::getWindowController() {
	return mWindowController;
}