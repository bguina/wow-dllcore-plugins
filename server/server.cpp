// server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "server.h"

Server::Server()
{
}

int Server::initialize() {
	WSAData wsaData;
	int nCode;
	if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
		std::cerr << "WSAStartup() returned error code " << nCode << "." << std::endl;
		return -1;
	}

	std::cout << "Establishing the listener..." << std::endl;
	serverSocket = setUpListener();
	if (serverSocket == INVALID_SOCKET) {
		std::cout << std::endl << WSAGetLastErrorMessage("establish listener") << std::endl;
		return -1;
	}

	std::cout << "Waiting for connections..." << std::endl;
	while (1) {
		acceptConnections();
		std::cout << "Acceptor restarting..." << std::endl;
	}

#if defined(_MSC_VER)
	return 0;   // warning eater
#endif
}



//// SetUpListener /////////////////////////////////////////////////////
// Sets up a listener on the given interface and port, returning the
// listening socket if successful; if not, returns INVALID_SOCKET.

SOCKET Server::setUpListener()
{
	u_long nInterfaceAddr = inet_addr(SERVER_HOST);
	if (nInterfaceAddr != INADDR_NONE) {
		SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
		if (sd != INVALID_SOCKET) {
			sockaddr_in sinInterface;
			sinInterface.sin_family = AF_INET;
			sinInterface.sin_addr.s_addr = nInterfaceAddr;
			sinInterface.sin_port = htons(SERVER_PORT);
			if (bind(sd, (sockaddr*)&sinInterface, sizeof(sockaddr_in)) != SOCKET_ERROR) {
				listen(sd, SOMAXCONN);
				return sd;
			}
			else {
				std::cerr << WSAGetLastErrorMessage("bind() failed") << std::endl;
			}
		}
	}

	return INVALID_SOCKET;
}


//// SetupFDSets ///////////////////////////////////////////////////////
// Set up the three FD sets used with select() with the sockets in the
// connection list.  Also add one for the listener socket, if we have
// one.

void Server::setupFDSets()
{
	FD_ZERO(&ReadFDs);
	FD_ZERO(&WriteFDs);
	FD_ZERO(&ExceptFDs);

	// Add the listener socket to the read and except FD sets, if there is one.
	if (serverSocket != INVALID_SOCKET) {
		FD_SET(serverSocket, &ReadFDs);
		FD_SET(serverSocket, &ExceptFDs);
	}

	// Add client connections
	std::list<ClientConnection>::iterator it = listConnections.begin();
	while (it != listConnections.end()) {

		FD_SET(it->getSocket(), &ReadFDs);
		//FD_SET(it->getSocket(), &WriteFDs);


		if (it->getListMessageToSend()->size() > 0) {
			// There's data still to be sent on this socket, so we need to be signalled when it becomes writable.
			FD_SET(it->getSocket(), &WriteFDs);
		}

		FD_SET(it->getSocket(), &ExceptFDs);
		++it;
	}
}


//// ReadData //////////////////////////////////////////////////////////
// Data came in on a client socket, so read it into the buffer.  Returns
// false on failure, or when the client closes its half of the
// connection.  (WSAEWOULDBLOCK doesn't count as a failure.)


BOOL Server::ReadData(ClientConnection& client)
{
	if (client.getCurrentRead() == -1) {
		int sizeToRead = 0;
		int totalRead = recv(client.getSocket(), (char*)(&sizeToRead), sizeof(int), 0);
		if (checkReadStatus(totalRead, client) == false)
			return false;
		client.setTotalToRead(sizeToRead);
		client.setCurrentRead(0);
	}
	else {
		char* buffer = (char*)malloc(client.getTotalToRead() + 1);
		memset(buffer, 0, client.getTotalToRead() + 1);
		int totalRead = recv(client.getSocket(), buffer, (int)client.getTotalToRead(), 0);
		if (checkReadStatus(totalRead, client) == false)
			return false;
		client.setCurrentRead(client.getCurrentRead() + totalRead);
		std::string message(buffer);
		client.setTmpMessage(client.getTmpMessage() + message);
		if (client.getTotalToRead() == client.getCurrentRead()) {
			client.setTotalToRead(-1);
			client.setCurrentRead(-1);
			std::cout << "FROM : " << client.getSocket() << " Message receive ==  " << client.getTmpMessage() << std::endl;

			if (messageManager.getMessageType(client.getTmpMessage()) == MessageType::AVAILABLE_CONFIGURATION)
			{
				//Push this client to PEER list for later
				if (checkIfClientExistInPeerList(client.getSocket()) == NULL) {
					PeerClient* peer = new PeerClient();
					peer->setClient1(&client);
					listPeers.push_back(peer);
					std::cout << "Adding window for Peer..." << std::endl;
				}

				std::list<std::string> listPID = getAllProcessIdFromProcessName(L"WowClassic.exe");
				std::string stringListPIDS = "";
				for (std::list<std::string>::iterator it = listPID.begin(); it != listPID.end(); ++it) {
					if (stringListPIDS.size() > 0)
						stringListPIDS += ",";
					stringListPIDS += (*it);
				}
				std::string messageToSend = messageManager.builResponseAvailableConfigationMessage(stringListPIDS, "dll1,dll2");
				client.getListMessageToSend()->push_back(messageToSend);
			}
			else if (messageManager.getMessageType(client.getTmpMessage()) == MessageType::INJECT)
			{
				InjectObject* object = messageManager.getInjectObject(client.getTmpMessage());
				if (object)
				{
					std::cout << "INJECTING PID == " << object->getPID() << " MODULE == " << object->getModule() << std::endl;

					if (inject(object->getPID(), object->getModule()) == 0)
					{
						std::cout << "SUCCESS INJECTION !!!!" << std::endl;
						PeerClient* peerClient = checkIfClientExistInPeerList(client.getSocket());
						if (peerClient != NULL)
						{
							std::cout << "FOUND CLIENT ! Add PID to this peer" << std::endl;
							peerClient->setPID(object->getPID());
						}
						else
						{
							std::cout << "[ERROR] CANNOT FOUND CLIENT........" << std::endl;
						}
					}
					else
					{
						std::cout << "[ERROR] DURING INJECTION.... " << std::endl;
					}
					free(object);
				}
			}
			else if (messageManager.getMessageType(client.getTmpMessage()) == MessageType::DLL_INJECTED)
			{
				int pid = messageManager.getDLLInjectedObject(client.getTmpMessage());
				if (pid != -1)
				{
					PeerClient* peerClient = checkIfClientExistInPeerList(pid);
					if (peerClient != NULL)
					{
						std::cout << "Link 2 PEER ! And sending back confirmation to client window" << std::endl;
						peerClient->setClient2(&client);
						peerClient->getClient1()->getListMessageToSend()->push_back(client.getTmpMessage());
					}
					else
					{
						std::cout << "[ERROR] CANNOT FOUND PEER with PID..........." << std::endl;
					}

				}
				else {
					std::cout << "[ERROR] PID == -1 .... " << std::endl;
				}

			}
			else if (messageManager.getMessageType(client.getTmpMessage()) == MessageType::DEINJECT ||
				messageManager.getMessageType(client.getTmpMessage()) == MessageType::SUBSCRIBE)
			{
				PeerClient* peerClient = checkIfClientExistInPeerList(client.getSocket());
				if (peerClient != NULL && peerClient->getClient2())
				{
					peerClient->getClient2()->getListMessageToSend()->push_back(client.getTmpMessage());
				}
				else {
					std::cout << "[ERROR] CANNOT FOUND PEER with SOCKET..........." << std::endl;
				}
			}
			else if (messageManager.getMessageType(client.getTmpMessage()) == MessageType::INFO) {
				PeerClient* peerClient = checkIfClientExistInPeerList(client.getSocket());
				if (peerClient != NULL && peerClient->getClient1())
				{
					peerClient->getClient1()->getListMessageToSend()->push_back(client.getTmpMessage());
				}
				else {
					std::cout << "[ERROR] CANNOT FOUND PEER with SOCKET..........." << std::endl;
				}
			}
			else {
				std::cout << ".........UNKNWON......." << std::endl;
				//client.getListMessageToSend()->push_back(".........UNKNWON.......");

			}
			client.setTmpMessage("");
			FD_SET(client.getSocket(), &WriteFDs);
		}
	}

	return true;
}

BOOL Server::checkReadStatus(int totalRead, ClientConnection& client) {
	if (totalRead <= 0) {
		std::cout << "Socket " << client.getSocket() << " was closed by the client. Shutting down." << std::endl;
		//Check is peer exist
		return false;
	}
	else if (totalRead == SOCKET_ERROR) {
		// Something bad happened on the socket.  It could just be a
		// "would block" notification, or it could be something more
		// serious.  Let caller handle the latter case.  WSAEWOULDBLOCK
		// can happen after select() says a socket is readable under
		// Win9x: it doesn't happen on WinNT/2000 or on Unix.
		int err;
		int errlen = sizeof(err);
		getsockopt(client.getSocket(), SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
		//return (err == WSAEWOULDBLOCK);
		return false;
	}
	return true;
}



//// WriteData /////////////////////////////////////////////////////////
// The connection is writable, so send any pending data.  Returns
// false on failure.  (WSAEWOULDBLOCK doesn't count as a failure.)

bool WriteData(ClientConnection& client)
{
	std::list<std::string>* listMessagesToSend = client.getListMessageToSend();

	for (std::list<std::string>::iterator it = listMessagesToSend->begin(); it != listMessagesToSend->end(); it++) {

		int sizeMessage = (*it).size();
		int bufferSize = sizeof(int) + sizeMessage;
		char* buffer = (char*)malloc(bufferSize);
		memset(buffer, 0, bufferSize);
		memcpy(buffer, &sizeMessage, sizeof(int));
		memcpy(buffer + sizeof(int), (*it).c_str(), sizeMessage);
		int totalSent = send(client.getSocket(), buffer, bufferSize, 0);
		free(buffer);
		std::cout << "TO : " << client.getSocket() << " Message : " << (*it) << std::endl;
		if (totalSent == SOCKET_ERROR) {
			// Something bad happened on the socket.  Deal with it.
			int err;
			int errlen = sizeof(err);
			getsockopt(client.getSocket(), SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
			return (err == WSAEWOULDBLOCK);
		}
	}
	listMessagesToSend->clear();

	return true;
}

PeerClient* Server::checkIfClientExistInPeerList(SOCKET socket) {
	for (std::list<PeerClient*>::iterator it = listPeers.begin(); it != listPeers.end(); it++) {
		if (((*it)->getClient1() && (*it)->getClient1()->getSocket() == socket) || ((*it)->getClient2() && (*it)->getClient2()->getSocket() == socket))
			return (*it);
	}
	return NULL;
}

PeerClient* Server::checkIfClientExistInPeerList(int pid) {
	for (std::list<PeerClient*>::iterator it = listPeers.begin(); it != listPeers.end(); it++) {
		if ((*it)->getPID() == pid)
			return (*it);
	}
	return NULL;
}


//// AcceptConnections /////////////////////////////////////////////////
// Spin forever handling connections.  If something bad happens, we
// return.

void Server::acceptConnections()
{
	sockaddr_in sinRemote;
	int nAddrSize = sizeof(sinRemote);

	while (1) {
		setupFDSets();

		if (select(0, &ReadFDs, &WriteFDs, &ExceptFDs, 0) > 0) {
			//// Something happened on one of the sockets.
			// Was it the listener socket?...
			if (FD_ISSET(serverSocket, &ReadFDs)) {
				SOCKET sd = accept(serverSocket, (sockaddr*)&sinRemote, &nAddrSize);
				if (sd != INVALID_SOCKET) {
					// Tell user we accepted the socket, and add it to
					// our connecition list.
					std::cout << "Accepted connection from " << inet_ntoa(sinRemote.sin_addr) << ":" << ntohs(sinRemote.sin_port) << ", socket " << sd << "." << std::endl;
					listConnections.push_back(ClientConnection(sd));

					if ((listConnections.size() + 1) > 64) {
						// For the background on this check, see
						// www.tangentsoft.net/wskfaq/advanced.html#64sockets
						// The +1 is to account for the listener socket.
						std::cout << "WARNING: More than 63 client connections accepted.  This will not work reliably on some Winsock stacks!" << std::endl;
					}

					// Mark the socket as non-blocking, for safety.
					u_long nNoBlock = 1;
					ioctlsocket(sd, FIONBIO, &nNoBlock);
				}
				else {
					std::cerr << WSAGetLastErrorMessage("accept() failed") << std::endl;
					return;
				}
			}
			else if (FD_ISSET(serverSocket, &ExceptFDs)) {
				int err;
				int errlen = sizeof(err);
				getsockopt(serverSocket, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
				std::cerr << WSAGetLastErrorMessage("Exception on listening socket: ", err) << std::endl;
				return;
			}

			// ...Or was it one of the client sockets?
			std::list<ClientConnection>::iterator it = listConnections.begin();
			while (it != listConnections.end()) {
				bool bOK = true;
				const char* pcErrorType = 0;

				// See if this socket's flag is set in any of the FD
				// sets.

				if (FD_ISSET(it->getSocket(), &ExceptFDs)) {
					bOK = false;
					pcErrorType = "General socket error";
					FD_CLR(it->getSocket(), &ExceptFDs);
				}
				else {
					if (FD_ISSET(it->getSocket(), &ReadFDs)) {
						bOK = ReadData(*it);
						pcErrorType = "Read error";
						FD_CLR(it->getSocket(), &ReadFDs);
					}
					if (FD_ISSET(it->getSocket(), &WriteFDs)) {
						bOK = WriteData(*it);
						pcErrorType = "Write error";
						FD_CLR(it->getSocket(), &WriteFDs);
					}
				}

				if (!bOK) {
					// Something bad happened on the socket, or the
					// client closed its half of the connection.  Shut
					// the conn down and remove it from the list.
					int err;
					int errlen = sizeof(err);
					getsockopt(it->getSocket(), SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
					if (err != NO_ERROR) {
						std::cerr << WSAGetLastErrorMessage(pcErrorType, err) << std::endl;
					}

					PeerClient* peerClient = checkIfClientExistInPeerList(it->getSocket());
					if (peerClient != NULL)
					{
						if (peerClient->getClient1() != NULL && peerClient->getClient1()->getSocket() == it->getSocket()) {
							std::cout << "Client QT Disconnect !" << std::endl;
							if (peerClient->getClient2() != NULL) {
								peerClient->getClient2()->getListMessageToSend()->push_back(messageManager.builRequestdDeinjecteMessage());
							}
							peerClient->setClient1(NULL);
						}
						else if (peerClient->getClient2() != NULL && peerClient->getClient2()->getSocket() == it->getSocket()) {
							std::cout << "Client DLL Disconnect !" << std::endl;
							if (peerClient->getClient1() != NULL) {
								//SEND DISCONNECT TO WINDOWS
								peerClient->getClient1()->getListMessageToSend()->push_back(messageManager.builRequestdDeinjecteMessage());
							}
							peerClient->setClient2(NULL);
						}
						if (peerClient->getClient1() == NULL && peerClient->getClient2() == NULL)
						{
							std::cout << "ALL PEER DISCONNECTED -> delete this pair element size list == " << listPeers.size() << std::endl;
							listPeers.remove(peerClient);
						}
					}

					ShutdownConnection(it->getSocket());
					listConnections.erase(it);
					it = listConnections.begin();
				}
				else {
					// Go on to next connection
					++it;
				}
			}
		}
		else {
			std::cerr << WSAGetLastErrorMessage("select() failed") << std::endl;
			return;
		}
	}
}