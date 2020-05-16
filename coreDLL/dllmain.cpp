// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <sstream>

#include "ServerSDK.h"
#include "MessageManager.h"
#include "d3d/d3d.h"
#include "Debugger.h"
#include "WowGame.h"
#include "WowNavigator.h"
#include "Sandbox.h"

static int testVar = 1;

bool readMessageAvailable(ServerSDK* serverSDK, MessageManager* messageManager) {
	std::list<std::string> messages = serverSDK->getMessageAvailable();
	for (std::list<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
	{
		switch (messageManager->getMessageType((*it)))
		{
		case MessageType::START_SUBSCRIBE: {

			std::list<std::string> toSubscribe = messageManager->getStartSubcribeObject(*it);
			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());
			if (found)
				serverSDK->sendMessage(messageManager->builResponseInfo("position", "X,Y,Z"));

			//mutex
			testVar += 1;

			break;
		}
		case MessageType::STOP_SUBSCRIBE: {
			//serverSDK->sendMessage(messageManager->builResponseInfo("position", "X,Y,Z"));
			break;
		}
		case MessageType::DEINJECT: {
			return false;
			break;
		}
		default:
			break;
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
	//mutex
	testVar -= 1;

	drawSomeTriangle();

	Sandbox& sandbox = Sandbox::getInstance();

	if (!sandbox.isOverHeating())
		sandbox.run();
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
