#pragma once

#include <cstdint>

class MemoryObject
{
public:
	MemoryObject(
		const uint8_t* baseAddr
	) : mBaseAddr(baseAddr) {
	}

	void rebase(
		const uint8_t* baseAddr
	) {
		this->mBaseAddr = baseAddr;
	}

	const uint8_t* const getBaseAddress() const {
		return mBaseAddr;
	}

private:
	const uint8_t* mBaseAddr;
};