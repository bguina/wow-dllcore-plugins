/***********************************************************************
 main.cpp - The main() routine for all the "Basic Winsock" suite of
	programs from the Winsock Programmer's FAQ.  This function parses
	the command line, starts up Winsock, and calls an external function
	called DoWinsock to do the actual work.

 This program is hereby released into the public domain.  There is
 ABSOLUTELY NO WARRANTY WHATSOEVER for this product.  Caveat hacker.
***********************************************************************/

#include "Server.h"


int main(int argc, char* argv[])
{
	Server server;

	int retval = server.initialize();

	// Shut Winsock back down and take off.
	WSACleanup();
	return retval;
}

