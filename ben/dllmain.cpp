
#include <sstream>
#include <fstream>
#include "framework.h"

#include "injectable/wow/WowPlugin.h"

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnLoad(uint32_t pid) {
	std::stringstream ss("D:\\");
	ss << "nvtest\\remote_" << pid << ".log" << std::endl;
	std::ofstream out(ss.str(), std::fstream::in | std::fstream::out | std::fstream::app);
	out << "DllPlugin_OnLoad:" << ss.str() << std::endl;
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnUnload() {
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnD3dRender() {
	static WowPlugin* plugin = nullptr;

	//if (nullptr == plugin) {
	//	plugin = new WowPlugin(99);
	//}

	//plugin->onD3dRender();
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
