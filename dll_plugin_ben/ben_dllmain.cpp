
#include "framework.h"

#include "WowPlugin.h"
#include "FileLogger.h"
#include "BenAgents.h"

WowPlugin* gContainedPlugin = nullptr;

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnLoad() {
	FileLogger dbg("ben_dllmain");

	dbg << "DllPlugin_OknLoad" << std::endl;
	auto* basic = new BenIdleAgent();

	auto* complex = new BenPolyAgent(
		nullptr,
		nullptr,
		basic
	);

	gContainedPlugin = new WowPlugin();
	gContainedPlugin->attachBot(basic);
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnUnload() {
	FileLogger dbg("ben_dllmain");

	dbg << "DllPlugin_OnUnload" << std::endl;
	if (gContainedPlugin) {
		delete gContainedPlugin;
		gContainedPlugin = nullptr;
	}
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnD3dRender() {
	FileLogger dbg("ben_dllmain");
	
	dbg << "DllPlugin_OnD3dRender" << std::endl;
	if (!gContainedPlugin->onD3dRender()) {
		dbg << "DllPlugin_OnD3dRender stop signal!" << std::endl;
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
