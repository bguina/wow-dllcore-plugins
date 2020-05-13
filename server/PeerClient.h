#pragma once

#ifndef PEER_CONNECTION_H
#define PEER_CONNECTION_H

#include "ClientConnection.h"

class PeerClient
{
private:
	ClientConnection* client1 = NULL;
	ClientConnection* client2 = NULL;
	int pid = -1;

public:

	//Constructor
	PeerClient() { };

	//Getter
	ClientConnection* getClient1() { return client1; };
	ClientConnection* getClient2() { return client2; };
	int getPID() { return pid; };

	//Setter
	void setClient1(ClientConnection* client) { client1 = client; };
	void setClient2(ClientConnection* client) { client2 = client; };
	void setPID(int _pid) { pid = _pid; };
};

#endif // !PEER_CONNECTION_H




