#include <fstream>
#include "framework.h"

#include "ExampleRemotePlugin.h"

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnLoad(uint32_t pid) {
	std::ofstream out("D:\\pluginDll.log", std::fstream::in | std::fstream::out | std::fstream::app);
	out << "Init plugin" << std::endl;
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnUnload() {
	return 0;
}

extern "C" int __declspec(dllexport) __stdcall DllPlugin_OnD3dRender() {
	static IPlugin* plugin = nullptr;

	if (nullptr == plugin) {
		//plugin = new ExampleRemotePlugin();
	}

	//plugin->onD3dRender();

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

