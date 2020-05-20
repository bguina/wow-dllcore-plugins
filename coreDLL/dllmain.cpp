// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <sstream>

#include "ServerSDK.h"
#include "d3d/d3d.h"
#include "Debugger.h"
#include "WowGame.h"
#include "WowNavigator.h"
#include "Sandbox.h"

static BOOL shouldStop = false;

void MainThread(void* pHandle) {

	if (HookD3D()) {
		while (!shouldStop && !GetAsyncKeyState(VK_END)) {
		}
	}

	deinject(pHandle);
}


void Render()
{
	static Sandbox* sandbox = nullptr;
	static ServerSDK* server = nullptr;

	if (shouldStop) {

		if (nullptr != server) {
			server->disconnect();
			delete server;
			delete sandbox;
			server = nullptr;
			sandbox = nullptr;
		}

	}
	else {
		if (nullptr == server)
		{
			sandbox = new Sandbox();
			server = new ServerSDK();
			if (server->connectToServer())
				server->sendMessage(server->getMessageManager().builRequestdDLLInjectedMessage(GetCurrentProcessId()));
		}


		if (server->getConnectionStatus()) {
			drawSomeTriangle();

			if (!sandbox->isOverHeating()) {
				if (!sandbox->run(*server))
					shouldStop = true;
			}
		}
		else {
			shouldStop = true;
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
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
