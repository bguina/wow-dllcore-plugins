#pragma once

#ifndef SERVER_SDK
#define SERVER_SDK

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <mutex>
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ServerSDK
{
private:
	PCSTR serverHost = "127.0.0.1";
	PCSTR serverPort = "27015";
	SOCKET activeSocket = NULL;
	HANDLE activeThread = NULL;

	fd_set read_fds;
	fd_set write_fds;
	fd_set except_fds;

	std::mutex mutexIsConnected;
	BOOL isConnected = 0;
	void  updateIsConnected(BOOL value);

	std::string tmpMessage = "";
	int totalToRead = -1;
	int currentRead = -1;

	std::string getTmpMessage() { return tmpMessage; };
	void setTmpMessage(std::string _tmpMessge) { tmpMessage = _tmpMessge; };

	int getTotalToRead() { return totalToRead; };
	void setTotalToRead(int _totalToRead) { totalToRead = _totalToRead; };

	int getCurrentRead() { return currentRead; };
	void setCurrentRead(int _currentRead) { currentRead = _currentRead; };

	std::mutex mutexListMessageToSend;
	std::list<std::string> listMessageToSend;
	std::list<std::string> getMessageToSend(bool toClear);

	std::mutex mutexListMessageAvailable;
	std::list<std::string> listMessageAvailable;
	void addMessageAvailable(std::string message);

	static DWORD WINAPI MyThreadFunction(LPVOID lpParam);
	int build_fd_sets();
	int mainLoopClient();

	int cleanSocket();
	int sendMessageToServer();
	int readMessageFromServer();

public:
	ServerSDK() { };
	int connectToServer();
	int disconnect();
	void sendMessage(std::string message);
	std::list<std::string> getMessageAvailable();

	//GETTERS
	BOOL getConnectionStatus();
};

#endif