#pragma once

#include <string>

#include "MemoryObject.h"

#include "windowcontroller/IWindowController.h"

class AGame : public MemoryObject
{
public:
	AGame(long pid, const uint8_t* baseAddr);
	virtual ~AGame();

	long getPid() const;

	const std::string& getName() const;

	virtual const IWindowController* getWindowController() const = 0;
	virtual IWindowController* getWindowController() = 0;

protected:
	const std::string mName;
	long mPid;
};
