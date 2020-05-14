#pragma once

#include <iostream>
#include "WowGame.h"

class WowNavigator
{
public:
	WowNavigator(
		WowGame game
	){}

protected:
    /*HWND wowWindow = FindWindow(NULL, _T("World of Warcraft"));

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

};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowNavigator& obj
	)
{
	// TODO print navigator state
	return out;
}
