#include "pch.h"

#include "MemoryObject.h"

MemoryObject::MemoryObject(
	const uint8_t* baseAddr
) :
	mBaseAddr(baseAddr)
{

}

MemoryObject:: ~MemoryObject() {

}

const uint8_t* MemoryObject::getAddress() const {
	return mBaseAddr;
}

void MemoryObject::rebase(
	const uint8_t* baseAddr
) {
	mBaseAddr = baseAddr;
}

bool MemoryObject::isPresent() const {
	return NULL != mBaseAddr;
}
