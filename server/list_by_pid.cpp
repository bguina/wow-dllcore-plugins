#include "pch.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <Psapi.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>

#include <iostream>
#include <string>
#include <vector>

#include "list_by_pid.h"

std::vector<int> listPids(IN CONST LPCWSTR pszProcessName) {
	std::vector<int> list;

	if (pszProcessName)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnapshot)
		{
			PROCESSENTRY32 processEntry = { 0 };
			processEntry.dwSize = sizeof(PROCESSENTRY32);

			BOOL bEntry = Process32First(hSnapshot, &processEntry);

			while (bEntry)
			{
				if (lstrcmpi(processEntry.szExeFile, pszProcessName) == 0)
				{
					list.push_back(processEntry.th32ProcessID);
				}
				bEntry = Process32Next(hSnapshot, &processEntry);
			}
			CloseHandle(hSnapshot);
		}
	}

	return list;
}
