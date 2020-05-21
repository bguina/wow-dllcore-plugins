// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <sstream>

#include "ServerSDK.h"
#include "d3d/d3d.h"
#include "Debugger.h"
#include "WowGame.h"
#include "WowBot.h"
#include "Sandbox.h"

static boolean shouldStop = false;

void MainThread(void* pHandle) {
	if (HookD3D()) {
		while (!shouldStop && !GetAsyncKeyState(VK_END)) {

		}
	}

	deinject(pHandle);
}

void releaseSandbox(Sandbox* sandbox, ServerSDK* server) {

}

void Render()
{
	static Sandbox* sandbox = nullptr;
	static ServerSDK* server = nullptr;

	if (!shouldStop)  {
		boolean stopSandbox = true;

		if (nullptr == server)
		{
			sandbox = new Sandbox();
			server = new ServerSDK();

			if (server->connectToServer()) {
				server->sendMessage(server->getMessageManager().builRequestdDLLInjectedMessage(sandbox->getGame().getPid()));
			}
		}

		if (server->getConnectionStatus()) {
			drawSomeTriangle();
		
			stopSandbox = !sandbox->run(*server);
		}
		else 
			stopSandbox = true;

		if (stopSandbox) {

			if (nullptr != server) {
				server->disconnect();
				delete server;
				delete sandbox;
				sandbox = nullptr;
				server = nullptr;
			}

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
