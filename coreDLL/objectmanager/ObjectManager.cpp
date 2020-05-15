#include "pch.h"

#include "ObjectManager.h"

ObjectManager::ObjectManager(
	const uint8_t** baseAddr
) :
	mPointerAddr(baseAddr)
{}

const uint8_t* ObjectManager::getBaseAddress() const {
	return *mPointerAddr;
}

const uint8_t* ObjectManager::firstObject() const {
	if (NULL == getBaseAddress()) return NULL;
	
	uint8_t* first = *(uint8_t**)(getBaseAddress() + 0x18);

	if (((uint64_t)first & 1)) return NULL;
	return *(const uint8_t**)(getBaseAddress() + 0x18);
}

const uint8_t* ObjectManager::nextObject(const uint8_t* currentObject) const {
	uint8_t* next = *(uint8_t**)(currentObject + 0x70);

	if (((uint64_t)next & 1)) return NULL;
	return next;
}

const uint8_t* ObjectManager::getActivePlayer() const {
	if (getBaseAddress() == NULL) return NULL;

	for (
		auto pObj = firstObject();
		NULL != pObj;
		pObj = nextObject(pObj)
		) {
		WowObject obj(pObj);

		if (obj.getType() == WowObject::ActivePlayer)
			return pObj;
	}
	return NULL;
}

const uint8_t* ObjectManager::getSomeBoar() const {
	if (getBaseAddress() == NULL) return NULL;

	for (
		auto pObj = firstObject();
		NULL != pObj;
		pObj = nextObject(pObj)
		) {
		WowObject obj(pObj);

		if (obj.getType() == WowObject::Unit)
			return pObj;
	}
	return NULL;
}
