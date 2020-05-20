#pragma once

#include "MemoryObject.h"
#include "WindowController.h"

class AGame : public MemoryObject
{
public:
	AGame(const uint8_t* baseAddr);

	long getPid() const;

	const WindowController& getWindowController() const;
	WindowController& getWindowController();

	template<typename T>
	const T* get(uint64_t offset) const {
		return reinterpret_cast<T*>(getAddress() + offset);
	}

	virtual void update() = 0;

protected:
	long mPid;
	WindowController mWindowController;
};
