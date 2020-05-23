#pragma once

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <mutex>
#include <iostream>

#include "MessageManager.h"

// Link with Ws2_32.lib, Mswsock.lib, Advapi32.lib static libraries
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class Client
{
public:
	Client();
	int joinServer();
	int disconnect();
	void sendMessage(std::string message);
	std::list<std::string> getMessageAvailable();

	//GETTERS
	bool isConnected();
	MessageManager getMessageManager() { return messageManager; };

private:
	PCSTR serverHost = "127.0.0.1";
	PCSTR serverPort = "27015";
	SOCKET activeSocket = NULL;
	HANDLE activeThread = NULL;

	MessageManager messageManager;

	fd_set read_fds;
	fd_set write_fds;
	fd_set except_fds;

	std::mutex mutexIsConnected;
	bool mIsConnected;
	void  updateIsConnected(bool value);

	std::string mMessageBuffer;
	int mTotalToRead = -1;
	int mCurrentRead = -1;

	void setTmpMessage(const std::string& _tmpMessge) { mMessageBuffer = _tmpMessge; };

	int getTotalToRead() { return mTotalToRead; };
	void setTotalToRead(int _totalToRead) { mTotalToRead = _totalToRead; };

	int getCurrentRead() { return mCurrentRead; };
	void setCurrentRead(int _currentRead) { mCurrentRead = _currentRead; };

	std::mutex mutexListMessageToSend;
	std::list<std::string> listMessageToSend;
	std::list<std::string> getMessageToSend(bool toClear);

	std::mutex mutexListMessageAvailable;
	std::list<std::string> listMessageAvailable;
	void addMessageAvailable(std::string message);

	static DWORD WINAPI _nvdll1254ClientThread(LPVOID dis);
	int build_fd_sets();
	int mainLoopClient();

	int cleanSocket();
	int sendMessageToServer();
	int readMessageFromServer();
};
