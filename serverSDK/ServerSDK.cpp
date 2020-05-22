#include "pch.h"

#include "framework.h"
#include "ServerSDK.h"

int ServerSDK::connectToServer() {
	WSADATA wsaData;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	//const char* sendbuf = "this is a test";
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed with error : " << iResult << std::endl;
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(serverHost, serverPort, &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed with error : " << iResult << std::endl;
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		activeSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (activeSocket == INVALID_SOCKET) {
			std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(activeSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(activeSocket);
			activeSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (activeSocket == INVALID_SOCKET) {
		std::cout << "Unable to connect to server!" << std::endl;
		WSACleanup();
		return false;
	}


	DWORD dwThreadID;

	activeThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		MyThreadFunction,       // thread function name
		(void*)this,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadID);   // returns the thread identifier 

	updateIsConnected(true);

	return getConnectionStatus();
}

int ServerSDK::mainLoopClient() {
	while (getConnectionStatus()) {
		build_fd_sets();
		struct timeval tv = { 0, 1000 };   // sleep for ten minutes!
		int activity = select((int)(activeSocket + 1), &read_fds, &write_fds, &except_fds, &tv);
		switch (activity) {
		case -1:
			printf("select()");
			updateIsConnected(false);
			//shutdown_properly(EXIT_FAILURE);

		case 0:
			//SELECT TIMEOUT
		default:
			/* All fd_set's should be checked. */
			if (FD_ISSET(activeSocket, &read_fds)) {
				if (readMessageFromServer() != 0) {
					//shutdown_properly(EXIT_FAILURE);
					updateIsConnected(false);
				}
			}

			if (FD_ISSET(activeSocket, &write_fds)) {
				if (sendMessageToServer() != 0) {
					//shutdown_properly(EXIT_FAILURE);
					updateIsConnected(false);
				}
			}

			if (FD_ISSET(activeSocket, &except_fds)) {
				printf("except_fds for server.\n");
				//shutdown_properly(EXIT_FAILURE);
				updateIsConnected(false);
			}
		}
	}
	return isConnected;
}

int ServerSDK::readMessageFromServer() {
	if (getCurrentRead() == -1) {
		int sizeToRead = 0;
		int totalRead = recv(activeSocket, (char*)(&sizeToRead), sizeof(int), 0);
		if (totalRead <= 0)
			return -1;
		setTotalToRead(sizeToRead);
		setCurrentRead(0);
	}
	else {
		char* buffer = (char*)malloc(getTotalToRead() + 1);
		memset(buffer, 0, getTotalToRead() + 1);
		int totalRead = recv(activeSocket, buffer, (int)getTotalToRead(), 0);
		if (totalRead <= 0)
			return -1;
		setCurrentRead(getCurrentRead() + totalRead);
		std::string message(buffer);
		setTmpMessage(getTmpMessage() + message);
		if (getTotalToRead() == getCurrentRead()) {
			setTotalToRead(-1);
			setCurrentRead(-1);
			std::cout << "Message received (pushing in listMessageAvailable) ==  " << getTmpMessage() << std::endl;
			addMessageAvailable(getTmpMessage());
			setTmpMessage("");
		}
	}

	return 0;
}

int ServerSDK::sendMessageToServer() {

	std::list<std::string> listMessagesToSend = getMessageToSend(true);

	for (std::list<std::string>::iterator it = listMessagesToSend.begin(); it != listMessagesToSend.end(); it++) {
		int messageSize = (int)(*it).size();
		int bufferSize = sizeof(int) + messageSize;
		char* buffer = (char*)malloc(bufferSize);
		memset(buffer, 0, bufferSize);
		memcpy(buffer, &messageSize, sizeof(int));
		memcpy(buffer + sizeof(int), (*it).c_str(), messageSize);

		int totalSent = send(activeSocket, buffer, bufferSize, 0);
		std::cout << "Sending message : ==  " << (*it) << std::endl;
		free(buffer);
		if (totalSent == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			cleanSocket();
			return -1;
		}
	}
	return 0;
}

void ServerSDK::sendMessage(std::string message) {
	std::lock_guard<std::mutex> lock(mutexListMessageToSend);
	listMessageToSend.push_back(message);
}

std::list<std::string> ServerSDK::getMessageToSend(bool toDelete) {
	std::lock_guard<std::mutex> lock(mutexListMessageToSend);

	std::list<std::string> copyList(listMessageToSend);
	if (toDelete)
		listMessageToSend.clear();
	return copyList;
}

std::list<std::string> ServerSDK::getMessageAvailable() {
	std::lock_guard<std::mutex> lock(mutexListMessageAvailable);

	std::list<std::string> copyList(listMessageAvailable);
	listMessageAvailable.clear();
	return copyList;
}

void ServerSDK::addMessageAvailable(std::string message) {
	std::lock_guard<std::mutex> lock(mutexListMessageAvailable);
	listMessageAvailable.push_back(message);
}

int ServerSDK::disconnect() {
	// shutdown the connection since no more data will be sent

	int iResult = shutdown(activeSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		std::cout << "shutdown failed with error: == " << WSAGetLastError() << std::endl;
		closesocket(activeSocket);
		WSACleanup();
		return 1;
	}

	WaitForSingleObject(activeThread, INFINITE);
	return 0;
}

int ServerSDK::cleanSocket() {
	closesocket(activeSocket);
	WSACleanup();
	return 1;
}

void ServerSDK::updateIsConnected(BOOL value) {
	std::lock_guard<std::mutex> lock(mutexIsConnected);
	isConnected = value;
}

BOOL ServerSDK::getConnectionStatus() {
	std::lock_guard<std::mutex> lock(mutexIsConnected);
	BOOL copyIsConnected = isConnected;
	return copyIsConnected;
}

int ServerSDK::build_fd_sets()
{
	FD_ZERO(&read_fds);
	FD_SET(activeSocket, &read_fds);

	FD_ZERO(&write_fds);
	//std::cout << "Message Added " << getMessageToSend(false).size() << std::endl;
	if (getMessageToSend(false).size() > 0) {
		FD_SET(activeSocket, &write_fds);
	}


	FD_ZERO(&except_fds);
	FD_SET(activeSocket, &except_fds);

	return 0;
}

DWORD WINAPI ServerSDK::MyThreadFunction(LPVOID lpParam)
{
	ServerSDK* This = (ServerSDK*)lpParam;
	This->mainLoopClient();
	return 0;
}