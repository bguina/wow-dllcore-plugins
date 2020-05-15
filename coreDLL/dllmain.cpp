// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <sstream>

#include "ServerSDK.h"
#include "MessageManager.h"
#include "d3d/d3d.h"
#include "logger.h"
#include "WowGame.h"
#include "WowNavigator.h"
#include "Sandbox.h"

bool readMessageAvailable(ServerSDK* serverSDK, MessageManager* messageManager) {
	std::list<std::string> messages = serverSDK->getMessageAvailable();
	for (std::list<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
	{
		if (messageManager->getMessageType((*it)) == MessageType::DEINJECT) {
			return false;
		}
	}
	return true;
}

void MainThread(void* pHandle) {

	ServerSDK serverSDK;
	MessageManager messageManager;

	if (serverSDK.connectToServer())
	{
		serverSDK.sendMessage(messageManager.builRequestdDLLInjectedMessage(GetCurrentProcessId()));
		if (HookD3D()) {
			//isConnected(-1) &&
			while (serverSDK.getConnectionStatus() && !GetAsyncKeyState(VK_END)) {

				if (!readMessageAvailable(&serverSDK, &messageManager))
				{
					break;
				}

			}
			serverSDK.disconnect();
		}
	}
	else {
		//SERVER NOT RUNNING DEINJECT
	}
	deinject(pHandle);

}

void Render()
{
	static ULONG64 bootTime = 0;
	static ULONG64 lastPulse = 0;

	drawSomeTriangle();

	auto uptime = GetTickCount64();
	if (lastPulse + 500 > uptime)
		return;

	lastPulse = uptime;
	Sandbox::getInstance().run();
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
