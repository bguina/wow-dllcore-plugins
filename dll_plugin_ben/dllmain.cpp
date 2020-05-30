
#include <sstream>
#include <fstream>
#include "framework.h"

#include "WowPlugin.h"
#include "ben/BenTravelBot.h"

 WowPlugin* gContainedPlugin = nullptr;

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnLoad(uint32_t pid) {
	gContainedPlugin = new WowPlugin(new BenTravelBot(), "DllBenTravelBot");
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnUnload() {
	delete gContainedPlugin;
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnD3dRender() {
	 gContainedPlugin->onD3dRender();
	 return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnServerMessage(void* param) {
	gContainedPlugin->onD3dRender();
	return 0;
}

//BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
//{
//	switch (ul_reason_for_call) {
//	case DLL_PROCESS_ATTACH:
//		break;
//	case DLL_THREAD_ATTACH:
//		break;
//	case DLL_THREAD_DETACH:
//		break;
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//}
//
