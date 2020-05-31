
#include <sstream>
#include <fstream>
#include "framework.h"

#include "WowPlugin.h"
#include "ben/BenTravelBot.h"

WowPlugin* gContainedPlugin = nullptr;

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnLoad() {
	FileLogger dbg("dllmain_ben");

	dbg << "DllPlugin_OnLoad" << std::endl;
	gContainedPlugin = new WowPlugin(new BenTravelBot());
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnUnload() {
	FileLogger dbg("dllmain_ben");

	dbg << "DllPlugin_OnUnload" << std::endl;
	if (gContainedPlugin) {
		delete gContainedPlugin;
		gContainedPlugin = nullptr;
	}
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnD3dRender() {
	FileLogger dbg("dllmain_ben");
	
	dbg << "DllPlugin_OnD3dRender" << std::endl;
	if (!gContainedPlugin->onD3dRender()) {
		dbg << "DllPlugin_OnD3dRender stop signal" << std::endl;
		return -1;
	}
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
