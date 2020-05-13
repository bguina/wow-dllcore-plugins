#include "include/wow.h"

BOOL loadRemoteDLL(HANDLE hProcess, const char* dllPath) {
	printf("Enter any key to attempt DLL injection.");
	getchar();

	// Allocate memory for DLL's path name to remote process
	LPVOID dllPathAddressInRemoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (dllPathAddressInRemoteMemory == NULL) {
		cout << GetLastError() << endl;
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
		LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(LPSTR("kernel32.dll")), "LoadLibraryA");
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

BOOL GetProcessIdFromProcessName
(
	IN CONST LPSTR pszProcessName
)
{
	BOOL bRet = FALSE;

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
					cout << L"found process " << processEntry.szExeFile << ": pid is " << processEntry.th32ProcessID << "\n";
					return  processEntry.th32ProcessID;
				}
				else
				{
					bEntry = Process32Next(hSnapshot, &processEntry);
				}
			}

			CloseHandle(hSnapshot);
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int wowPid = GetProcessIdFromProcessName(LPSTR("WowClassic.exe"));
	LPSTR crazyStuff = LPSTR("D:\\nvtest.dll");

	if (wowPid > 0) {
		HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD |
			PROCESS_QUERY_INFORMATION |
			PROCESS_VM_OPERATION |
			PROCESS_VM_WRITE |
			PROCESS_VM_READ,
			FALSE,
			wowPid
		);

		if (!GetDebugPrivileges()) {
			return 1;
		}

		loadRemoteDLL(hProcess, crazyStuff);
	}
	else {
		cerr << "WowClassic.exe not running\n";
	}

	// Freeze app to see output result.
	cin.get();
	return 0;
}
