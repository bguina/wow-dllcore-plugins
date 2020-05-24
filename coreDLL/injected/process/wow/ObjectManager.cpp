#include "pch.h"

#include <set>

#include "ObjectManager.h"

#include "../../../logger/FileLogger.h"
#include "dump/WowGameDescriptors.h"
#include "dump/WowGameOffsets.h"

#include "object/WowContainerObject.h"
#include "object/WowItemObject.h"
#include "object/WowLootObject.h"
#include "object/WowCorpseObject.h"
#include "object/WowUnitObject.h"
#include "object/WowPlayerObject.h"
#include "object/WowActivePlayerObject.h"
#include "object/WowUnimplementedObject.h"

ObjectManager::ObjectManager(
	const uint8_t** baseAddr
) :
	mPointerAddr(baseAddr)
{}

bool ObjectManager::isEnabled() const {
	return NULL != getBaseAddress();
}

void ObjectManager::scan() {
	if (isEnabled()) {
		std::set<WowGuid64> oldGuids;

		for (std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator it = mObjects.begin();
			it != mObjects.end(); ++it) {
			oldGuids.insert(it->first);
		}

		// Manually walk through the native ObjectManager linked list
		for (auto pObj = *(uint8_t**)(getBaseAddress() + 0x18);
			NULL != pObj && !((uint64_t)pObj & 1);
			pObj = *(uint8_t**)(pObj + WowGameOffsets::WowObjectManager::OffsetNextObject))
		{
			WowObject thisObj = WowObject(pObj);
			std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator oldObjInstance = mObjects.find(thisObj.getGuid());

			if (oldObjInstance == mObjects.end()) {
				std::shared_ptr<WowObject> finalObj = nullptr;

				switch (thisObj.getType()) {
				case WowObjectType::Object:
					finalObj = std::make_shared<WowObject>(pObj);
					break;
				case WowObjectType::Item:
					finalObj = std::make_shared<WowItemObject>(pObj);
					break;
				case WowObjectType::Container:
					finalObj = std::make_shared<WowContainerObject>(pObj);
					break;
				case WowObjectType::Unit:
					finalObj = std::make_shared<WowUnitObject>(pObj);
					break;
				case WowObjectType::Player:
					finalObj = std::make_shared<WowPlayerObject>(pObj);
					break;
				case WowObjectType::ActivePlayer:
					finalObj = std::make_shared<WowActivePlayerObject>(pObj);
					break;
				case WowObjectType::Corpse:
					finalObj = std::make_shared<WowCorpseObject>(pObj);
					break;
				case WowObjectType::Loot:
				case WowObjectType::GameObject:
				case WowObjectType::DynamicObject:
				case WowObjectType::Conversation:
				case WowObjectType::Invalid: // Type "Invalid"? based on which wtf Blizzard
					finalObj = std::make_shared<WowUnimplementedObject>(pObj);
					break;
				default:
					// Invalid WowObject enum value!!
					break;
				}

				if (nullptr != finalObj) {
					mObjects.insert(std::pair<WowGuid64, std::shared_ptr<WowObject>>(finalObj->getGuid(), finalObj));
				}
			}
			else {
				// WowObject still present in memory, rebase to found address
				// TODO: Define WHEN/HOW a same object would be rebase, when we immediately clear any missing previous object?
				// in current implementation, we don't persist objects went missing so this currently has no real purpose.
				oldObjInstance->second->rebase(pObj);
				oldGuids.erase(oldObjInstance->second->getGuid());
			}
		}

		for (std::set<WowGuid64>::const_iterator it = oldGuids.begin(); it != oldGuids.end(); ++it) {
			mObjects.erase(*it);
		}
	}
	else {
		mObjects.clear();
	}
}

const uint8_t* ObjectManager::getBaseAddress() const {
	return *mPointerAddr;
}

std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator ObjectManager::begin() const {
	return mObjects.begin();
}

std::map<WowGuid64, std::shared_ptr<WowObject>>::iterator ObjectManager::begin() {
	return mObjects.begin();
}

std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator ObjectManager::end() const {
	return mObjects.end();
}

std::map<WowGuid64, std::shared_ptr<WowObject>>::iterator ObjectManager::end() {
	return mObjects.end();
}

const std::shared_ptr<const WowActivePlayerObject> ObjectManager::getActivePlayer() const {
	return anyOfType<const WowActivePlayerObject>(WowObjectType::ActivePlayer);
}

std::shared_ptr<WowActivePlayerObject> ObjectManager::getActivePlayer() {
	return anyOfType<WowActivePlayerObject>(WowObjectType::ActivePlayer);
}

size_t ObjectManager::getObjectsCount() const {
	return mObjects.size();
}