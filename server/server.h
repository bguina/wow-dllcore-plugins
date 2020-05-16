#pragma once

#ifndef SERVER_H
#define SERVER_H

#include "Ressources.h"
#include "ClientConnection.h"
#include "PeerClient.h";
#include "MessageManager.h"
#include "InjectorPrototypes.h"


class Server
{
private:
	const char* SERVER_HOST = "127.0.0.1";
	int SERVER_PORT = 27015;

	std::list<PeerClient*> listPeers;

	std::list<ClientConnection> listConnections;

	MessageManager messageManager;

	SOCKET serverSocket;
	fd_set ReadFDs;
	fd_set WriteFDs;
	fd_set ExceptFDs;

	SOCKET setUpListener();
	void acceptConnections();
	void setupFDSets();
	BOOL checkReadStatus(int totalRead, ClientConnection& client);
	BOOL ReadData(ClientConnection& client);

	PeerClient* checkIfClientExistInPeerList(SOCKET socket);
	PeerClient* checkIfClientExistInPeerList(int pid);

public:
	Server();
	int initialize();
};

#endif