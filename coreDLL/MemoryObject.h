#pragma once

#include <cstdint>

class MemoryObject
{
public:
	MemoryObject(const uint8_t* baseAddr);

	const uint8_t* const getBaseAddress() const;

	void rebase(const uint8_t* baseAddr);

private:
	const uint8_t* mBaseAddr;
};
