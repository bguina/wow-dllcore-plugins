// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <sstream>

#include "Client.h"
#include "d3d/d3d.h"
#include "logger/FileLogger.h"

#include "injected/InjectedClient.h"

static boolean gShouldStop = false;

void MainThread(void* pHandle) {
	//FileDebugger dbg("MainThread");

	if (HookD3D()) {
		while (!gShouldStop && !GetAsyncKeyState(VK_END)) {

		}
	}

	//dbg << "deinject\n";
	deinject(pHandle);
}

void Render() {
	static InjectedClient* sandbox = nullptr;

	if (!gShouldStop) {
		boolean stopSandbox = false;

		if (nullptr == sandbox) {
			sandbox = new InjectedClient(); 
		}

		stopSandbox = !sandbox->run();
		drawSomeTriangle();

		if (stopSandbox) {

			delete sandbox;

			gShouldStop = true;
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hinstDLL, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

