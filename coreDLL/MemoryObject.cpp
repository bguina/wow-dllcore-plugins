#include "pch.h"

#include "MemoryObject.h"

MemoryObject::MemoryObject(
	const uint8_t* baseAddr
) :
	mBaseAddr(baseAddr)
{}

const uint8_t* const MemoryObject::getBaseAddress() const {
	return mBaseAddr;
}

void MemoryObject::rebase(
	const uint8_t* baseAddr
) {
	mBaseAddr = baseAddr;
}
