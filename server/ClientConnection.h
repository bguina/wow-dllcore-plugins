#pragma once

#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H 

#include "Ressources.h"

class ClientConnection
{
private:
	SOCKET socket;
	std::list<std::string> listMessageToSend;
	std::string tmpMessage = "";
	int totalToRead = -1;
	int currentRead = -1;
public:
	ClientConnection(SOCKET);

	//GETTER
	SOCKET getSocket() { return socket; };
	std::list<std::string>* getListMessageToSend() { return &listMessageToSend; };

	std::string getTmpMessage() { return tmpMessage; };
	void setTmpMessage(std::string _tmpMessge) { tmpMessage = _tmpMessge; };

	int getTotalToRead() { return totalToRead; };
	void setTotalToRead(int _totalToRead) { totalToRead = _totalToRead; };

	int getCurrentRead() { return currentRead; };
	void setCurrentRead(int _currentRead) { currentRead = _currentRead; };

};

#endif // !CLIENT_CONNECTION_H






