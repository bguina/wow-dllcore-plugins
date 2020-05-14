#pragma once

#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include "ObjectManager.h"
#include "MemoryObject.h"

class WowGame : public MemoryObject
{
public:
	WowGame(
		const uint8_t* baseAddr
	) : MemoryObject(baseAddr) {}

	const HWND getWindowHandle() const {
		return FindWindow(NULL, _T("World of Warcraft"));
	}

	const ObjectManager getObjectManager() const {
		return ObjectManager(*(const uint8_t**)(baseAddress() + 0x2372D48));
	}

	/*

		// BuildVerion
		if (false) {
			char* pGameBuild = (char*)pModuleBaseAddr + 0x1c46f0c;

			ss << "Build";
			ss << pGameBuild;
			ss << ":";
		}

		// GameState?
		if (false) {
			ss << " GameState ";
			ss << *(PUINT16*)(pModuleBaseAddr + 0x25A9E60);
			// fun fact: we see boolean bit "isFalling"
		}

		// Realm name
		{
			uint8_t** pRealm = *(uint8_t***)(pModuleBaseAddr + 0x2688058);

			if (nullptr != pRealm) {
				ss << " Realm ";
				ss << ((char*)pRealm + 0x420); // realm name at RealmObj+0x420
			}
		}

		// Zone name
		{
			char* pZoneText = *(char**)(pModuleBaseAddr + 0x25A8C40);

			if (nullptr != pZoneText) {
				ss << " Zone ";
				ss << pZoneText;
			}
		}

		// Player name
		{
			char* pPlayerName = (char*)(pModuleBaseAddr + 0x2688828);

			if (*pPlayerName != '\0') {
				ss << " Player ";
				ss << pPlayerName;
			}
		}

		// Player GUID
		uint64_t* pLocalPlayerGuid = (uint64_t*)(pModuleBaseAddr + 0x2688810);
		ss << " LocalGuid=";
		ss << std::hex << pLocalPlayerGuid[0] << pLocalPlayerGuid[1];

		// Mouse-over object GUID
		uint64_t* pMouseoverGuid = (uint64_t*)(pModuleBaseAddr + 0x25A9E68);

		ss << " MouseOverGUID=";
		ss << std::hex << pMouseoverGuid[0] << pMouseoverGuid[1];

		uint64_t* pTargetGuid = (uint64_t*)(pModuleBaseAddr + 0x21E28A0);

		ss << " TargetGUID=";
		ss << std::hex << pTargetGuid[0] << pTargetGuid[1] << std::endl;
	*/
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowGame& obj
	)
{
	ObjectManager objMgr = obj.getObjectManager();

	out << objMgr << std::endl;

	return out;
}
