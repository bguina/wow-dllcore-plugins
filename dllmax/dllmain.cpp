// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <sstream>

#include "Client.h"
#include "d3dutils.h"
#include "FileLogger.h"

#include "DllCore.h"
#include "WowPlugin.h"
#include "MaxBot.h"

static bool gShouldStop = false;

void Render(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags) {
	static DllCore* sandbox = nullptr;

	if (!gShouldStop) {
		boolean stopSandbox = false;

		if (nullptr == sandbox) {
			sandbox = new DllCore(new WowPlugin(new WowMaxBot()));
		}

		stopSandbox = !sandbox->onFrameRender();
		drawSomeTriangle();

		if (stopSandbox) {

			delete sandbox;

			gShouldStop = true;
		}
	}
}

void MainThread(void* pHandle) {
	//FileDebugger dbg("MainThread");

	if (HookD3D(&Render)) {
		while (!gShouldStop && !GetAsyncKeyState(VK_END)) {

		}
	}

	//dbg << "deinject\n";
	UnhookD3D(pHandle);
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

