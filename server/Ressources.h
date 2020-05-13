#pragma once

#ifndef RESSOURCES_H
#define RESSOURCES_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#define _WINSOCKAPI_

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <list>

#include "ws-util.h"

//Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#endif // !RESSOURCES_H
