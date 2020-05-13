#include "windowQT.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	windowQT w;
	w.show();
	return a.exec();
}

/*
boolean startup(LPCTSTR lpApplicationName)
{
	// additional information
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	boolean result = CreateProcess(lpApplicationName,   // the path
		NULL,        // Command line argv[1]
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles.
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);

	return result;
}
*/

/*
	wchar_t buffer[512];

	if (GetCurrentDirectory(sizeof(buffer), buffer) == 0)
	{
		DWORD err = GetLastError();
		MessageBox(NULL, L"GetCurrentDirectory failed", L"Window1", MB_OK);
		ExitProcess(err);
	}

	std::wstring fullPathServer;
	fullPathServer.append(buffer);
	fullPathServer.append(L"\\server.exe");

	MessageBox(NULL, fullPathServer.c_str(), L"Window1", MB_OK);

	boolean result = startup(fullPathServer.c_str());


	if (result) {
		std::cout << "SUCCESS opening server.exe" << std::endl;
	}
	else {
		std::cout << "FAILED opening server.exe == " << std::endl;
	}

*/

/*
//HWND wowWindow = FindWindow(NULL, _T("World of Warcraft"));

			//0x57 -> W
			//0x51 -> Q
			//0x41 -> A

			//PostMessage(wowWindow, WM_KEYDOWN, 0x41, 0x00110001); //Flags found with spy++
			//PostMessage(notepad, WM_CHAR, 'w', 0);
			//Sleep(2000);
			//PostMessage(wowWindow, WM_KEYUP, 0x41, 0xC0110001);  //Flags found with spy++



			//SendMessage(notepad, WM_KEYDOWN, 'w', 0);
			//Sleep(3000);

			//PostMessage(notepad, WM_CHAR, 'w', 0);
*/

/*
std::list<DWORD> listPID = getAllProcessIdFromProcessName(L"WowClassic.exe");
std::cout << "BEGIN" << std::endl;
			for (std::list<DWORD>::iterator it = listPID.begin(); it != listPID.end(); ++it) {
				std::cout << "PID == " << *it << std::endl;
			}
*/
