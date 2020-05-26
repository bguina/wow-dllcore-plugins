#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <Psapi.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>

#include <iostream>
#include <string>
#include <vector>

bool loadRemoteDLL(HANDLE hProcess, const char* dllPath) {
	// Allocate memory for DLL's path name to remote process
	LPVOID dllPathAddressInRemoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (dllPathAddressInRemoteMemory == NULL) {
		std::cerr << "VirtualAllocEx error " << GetLastError() << std::endl;
		printf("[---] VirtualAllocEx unsuccessful.\n");
		getchar();
		return FALSE;
	}

	// Write DLL's path name to remote process
	BOOL succeededWriting = WriteProcessMemory(hProcess, dllPathAddressInRemoteMemory, dllPath, strlen(dllPath) + 1, NULL);

	if (!succeededWriting) {
		printf("[---] WriteProcessMemory unsuccessful.\n");
		getchar();
		return FALSE;
	}
	else {
		// Returns a pointer to the LoadLibrary address. This will be the same on the remote process as in our current process.
		LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		if (loadLibraryAddress == NULL) {
			printf("[---] LoadLibrary not found in process.\n");
			getchar();
			return FALSE;
		}
		else {
			HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddressInRemoteMemory, NULL, NULL);
			if (remoteThread == NULL) {
				printf("[---] CreateRemoteThread unsuccessful. %p \n", loadLibraryAddress);
				return FALSE;
			}
			else {
				printf("kernel32.dll: LoadLibraryA() success. %p \n", loadLibraryAddress);
			}
		}
	}

	CloseHandle(hProcess);
	return TRUE;
}

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
		//printf("LookupPrivilegeValue error: %u\n", GetLastError() );
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) {
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}

BOOL GetDebugPrivileges() {
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		std::cout << "OpenProcessToken() failed, error\n>> " << GetLastError() << std::endl;
		return FALSE;
	}

	if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE)) {
		std::cout << "Failed to enable privilege, error:\n>> " << GetLastError() << std::endl;
		return FALSE; //
	}

	return TRUE;
}

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

int inject(int processId, const std::string& module) {

	// TODO use "module" dll path variable to select the real DLL to inject
	//LPSTR crazyStuff = LPSTR("D:\\nvtest.dll");

	//LPSTR crazyStuff = LPSTR("C:\\Users\\maxim\\Documents\\wow01\\Debug\\nvtest.dll");

	wchar_t buffer[512];

	if (GetCurrentDirectory(sizeof(buffer), buffer) == 0)
	{
		DWORD err = GetLastError();
		MessageBox(NULL, L"GetCurrentDirectory failed", L"Window1", MB_OK);
		ExitProcess(err);
	}

	std::wstring fullPathDLL;
	fullPathDLL.append(buffer);
	fullPathDLL.append(L"\\nvtest.dll");

	std::string str(fullPathDLL.begin(), fullPathDLL.end());

	MessageBox(NULL, fullPathDLL.c_str(), L"Window1", MB_OK);

	LPSTR crazyStuff = _strdup(str.c_str());



	if (processId <= 0) {
		std::cerr << "processId <= 0" << std::endl;
		return -1;
	}

	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ,
		FALSE,
		processId
	);

	if (!GetDebugPrivileges()) {
		std::cerr << "GetDebugPrivileges failed" << std::endl;
		return 1;
	}

	if (hProcess <= 0) {
		std::cerr << "OpenProcess: returned hProcess <= 0" << std::endl;
		return -1;
	}

	std::cout << "LOADING ..." << std::endl;

	loadRemoteDLL(hProcess, crazyStuff);

	// Freeze app to see output result.
	//cin.get();
	return 0;
}
