
#include "WowPlugin.h"
#include "BenAgents.h"
#include "bot/gameplay/BenGameRecord.h"
#include "bot/type/base/evaluator/WowGameBasicEvaluator.h"

WowPlugin* gContainedPlugin = nullptr;

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnLoad() {
	gContainedPlugin = new WowPlugin();
	const auto baseGameplay = std::make_shared<WowGameBasicEvaluator>(new BenGameRecord<2, 1>());
	const auto debug(true);

	if (debug)
	{
		auto* bot = new BenDebugAgent(baseGameplay);
		auto* patroller = new BenPathPatroller(baseGameplay, "BenPathPatroller", nullptr, bot);
		gContainedPlugin->attachBot(patroller);
	}
	else
	{
		//auto* root = new BenPolyAgent(nullptr, nullptr, patroller);
		auto* bot = new BenHunterChampion(baseGameplay);
		auto* patroller = new BenPathPatroller(baseGameplay, "BenPathPatroller", nullptr, bot);
		gContainedPlugin->attachBot(patroller);
	}
	
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnUnload() {

	if (gContainedPlugin) 
	{
		delete gContainedPlugin;
		gContainedPlugin = nullptr;
	}
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnD3dRender() {

	if (!gContainedPlugin->onD3dRender()) 
	{
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
