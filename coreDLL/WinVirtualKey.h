#pragma once

enum class WinVirtualKey : uint32_t {
	// Those seem to be defined in "winuser.h" but could not use them for some reason...
	// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

	// Mouse
	WVK_LBUTTON = 0x01,
	WVK_RBUTTON = 0x02,
	WVK_CANCEL = 0x03,
	WVK_MBUTTON = 0x04,
	WVK_XBUTTON1 = 0x05,
	WVK_XBUTTON2 = 0x06,

	// Keyboard functionality keys
	WVK_BACK = 0x08,
	WVK_TAB = 0x09,
	WVK_CLEAR = 0x0C,
	WVK_RETURN = 0x0D,
	WVK_SHIFT = 0x10,
	WVK_CONTROL = 0x11,
	WVK_MENU = 0x12, // ALT key
	WVK_PAUSE = 0x13,
	WVK_CAPITAL = 0x14, // CAPS LOCK key

	// A-Z
	WVK_A = 0x41,
	WVK_B = 0x42,
	WVK_C = 0x43,
	WVK_D = 0x44,
	WVK_E = 0x45,
	WVK_F = 0x46,
	WVK_G = 0x47,
	WVK_H = 0x48,
	WVK_I = 0x49,
	WVK_J = 0x4A,
	WVK_K = 0x4B,
	WVK_L = 0x4C,
	WVK_M = 0x4D,
	WVK_N = 0x4E,
	WVK_O = 0x4F,
	WVK_P = 0x50,
	WVK_Q = 0x51,
	WVK_R = 0x52,
	WVK_S = 0x53,
	WVK_T = 0x54,
	WVK_U = 0x55,
	WVK_V = 0x56,
	WVK_W = 0x57,
	WVK_X = 0x58,
	WVK_Y = 0x59,
	WVK_Z = 0x5A,

};
