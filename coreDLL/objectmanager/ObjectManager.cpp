#include "pch.h"

#include <set>
#include "ObjectManager.h"
#include "WowObject.h"
#include "WowContainerObject.h"
#include "WowItemObject.h"
#include "WowLootObject.h"
#include "WowCorpseObject.h"
#include "WowUnitObject.h"
#include "WowPlayerObject.h"
#include "WowActivePlayerObject.h"
#include "WowUnimplementedObject.h"

ObjectManager::ObjectManager(
	const uint8_t** baseAddr
) :
	mPointerAddr(baseAddr)
{}

bool ObjectManager::isEnabled() const {
	return NULL != getBaseAddress();
}

#include "../Debugger.h"

void ObjectManager::scan() {
	if (isEnabled()) {
		std::set<WowGuid64> oldGuids;

		for (std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator it = mObjects.begin();
			it != mObjects.end(); ++it) {
			oldGuids.insert(it->first);
		}

		std::stringstream ss;
		Debugger dbg("D:\\mtlog.txt");

		// Manually walk through the native ObjectManager linked list
		for (auto pObj = *(uint8_t**)(getBaseAddress() + 0x18);
			NULL != pObj && !((uint64_t)pObj & 1);
			pObj = *(uint8_t**)(pObj + 0x70))
		{
			WowObject thisObj = WowObject(pObj);
			std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator oldObjInstance = mObjects.find(thisObj.getGuid2());

			if (oldObjInstance == mObjects.end()) {
				std::shared_ptr<WowObject> finalObj = nullptr;

				switch (thisObj.getType()) {
				case WowObject::Object:
					finalObj = std::make_shared<WowObject>(pObj);
					break;
				case WowObject::Item:
					finalObj = std::make_shared<WowItemObject>(pObj);
					break;
				case WowObject::Container:
					finalObj = std::make_shared<WowContainerObject>(pObj);
					break;
				case WowObject::Unit:
					finalObj = std::make_shared<WowUnitObject>(pObj);
					break;
				case WowObject::Player:
					finalObj = std::make_shared<WowPlayerObject>(pObj);
					break;
				case WowObject::ActivePlayer:
					finalObj = std::make_shared<WowActivePlayerObject>(pObj);
					break;
				case WowObject::Corpse:
					finalObj = std::make_shared<WowCorpseObject>(pObj);
					break;
				case WowObject::Loot:
				case WowObject::GameObject:
				case WowObject::DynamicObject:
				case WowObject::Conversation:
				case WowObject::Invalid: // Type "Invalid"? based on which wtf Blizzard
					finalObj = std::make_shared<WowUnimplementedObject>(pObj);
					break;
				default:
					// Invalid WowObject enum value!!
					break;
				}

				if (nullptr != finalObj)
					mObjects.insert(std::pair<WowGuid64, std::shared_ptr<WowObject>>(thisObj.getGuid2(), finalObj));
			}
			else {
				// WowObject still present in memory, rebase to found address
				// TODO: Define WHEN/HOW a same object would be rebase, when we immediately clear any missing previous object?
				// in current implementation, we don't persist objects went missing so this currently has no real purpose.
				oldObjInstance->second->rebase(pObj);
				oldGuids.erase(oldObjInstance->second->getGuid2());
			}
		}

		for (std::set<WowGuid64>::const_iterator it = oldGuids.begin(); it != oldGuids.end(); ++it) {
			mObjects.erase(*it);
		}

		dbg.log(ss.str().c_str());
		dbg.flush();
	}
	else {
		mObjects.clear();
	}
}

const uint8_t* ObjectManager::getBaseAddress() const {
	return *mPointerAddr;
}

std::map<uint64_t, std::shared_ptr<WowObject>>::const_iterator ObjectManager::begin() const {
	return mObjects.begin();
}

std::map<uint64_t, std::shared_ptr<WowObject>>::iterator ObjectManager::begin() {
	return mObjects.begin();
}

std::map<uint64_t, std::shared_ptr<WowObject>>::const_iterator ObjectManager::end() const {
	return mObjects.end();
}

std::map<uint64_t, std::shared_ptr<WowObject>>::iterator ObjectManager::end() {
	return mObjects.end();
}

const WowActivePlayerObject* ObjectManager::getActivePlayer() const {
	return anyOfType<const WowActivePlayerObject>(WowObject::ActivePlayer);
}

WowActivePlayerObject* ObjectManager::getActivePlayer() {
	return const_cast<WowActivePlayerObject*>(std::as_const(*this).getActivePlayer());
}
