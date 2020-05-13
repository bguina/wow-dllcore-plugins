#pragma once

#ifndef INJECTOR_PROTOTYPES
#define INJECTOR_PROTOTYPES

#include <list>
#include <string>
#include <Windows.h>

//PROTOTYPES
std::list<std::string> getAllProcessIdFromProcessName(IN CONST LPCWSTR pszProcessName);
int inject(int wowPid, std::string module);

#endif // !INJECTOR_PROTOTYPES


