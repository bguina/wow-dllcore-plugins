#pragma once

#include <string>

#include "MemoryObject.h"

#include "../windowcontroller/IWindowController.h"

class AGame : public MemoryObject
{
public:
	AGame(const std::string& name, const uint8_t* baseAddr);
	virtual ~AGame();

	const std::string& getName() const;

	long getPid() const;

	virtual const IWindowController* getWindowController() const = 0;
	virtual IWindowController* getWindowController() = 0;

	template<typename T>
	const T* get(uint64_t offset) const {
		return reinterpret_cast<const T*>(getAddress() + offset);
	}

	template<typename T>
	T* getWritable(uint64_t offset)  {
		return reinterpret_cast<T*>(getAddress() + offset);
	}

protected:
	const std::string mName;
	long mPid;
};
