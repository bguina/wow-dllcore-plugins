#include "pch.h"

#include "DllPlugin.h"

#include <windows.h>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>

DllPlugin::DllPlugin() :
	mDll(nullptr),
	mDbg("DllPlugin"),
	mDllOnLoadFunc(nullptr),
	mDllOnUnloadFunc(nullptr),
	mDllOnD3dRenderFunc(nullptr),
	mDllOnServerMessageFunc(nullptr)
{
}

DllPlugin::~DllPlugin() {
	freeLibrary();
}

bool DllPlugin::isLibraryLoaded() const {
	return mDll;
}

bool DllPlugin::loadLibrary(const std::wstring& dllPath) {
	freeLibrary();

	// string conversion
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	auto converted= converterX.to_bytes(dllPath);

	mDll = LoadLibraryW(LPWSTR(dllPath.c_str()));
	if (!mDll) {
		mDbg << FileLogger::err << converted.c_str() << ": failed to LoadLibrary" << FileLogger::normal << std::endl;
		return false;
	}

	// resolve functions
	mDllOnLoadFunc = (DllPlugin_OnLoad)GetProcAddress(mDll, "DllPlugin_OnLoad");
	mDllOnD3dRenderFunc = (DllPlugin_OnD3dRender)GetProcAddress(mDll, "DllPlugin_OnD3dRender");
	mDllOnUnloadFunc = (DllPlugin_OnUnload)GetProcAddress(mDll, "DllPlugin_OnUnload");
	mDllOnServerMessageFunc = (DllPlugin_OnServerMessage)GetProcAddress(mDll, "DllPlugin_OnServerMessage");

	if (!mDllOnLoadFunc) {
		freeLibrary();
		return false;
	}

	mDllOnLoadFunc(GetCurrentProcessId());
	mDbg << FileLogger::info << converted.c_str() << ": loaded" << FileLogger::normal << std::endl;
	mDbg.flush();
	return true;
}

void DllPlugin::freeLibrary() {
	if (mDll) {
		FreeLibrary(mDll);
		mDll = nullptr;
	}
}

bool DllPlugin::onD3dRender() {
	if (!isLibraryLoaded()) return true;

	if (mDllOnD3dRenderFunc) mDllOnD3dRenderFunc();
	else mDbg << FileLogger::err << "mDllD3dRender failure" << FileLogger::normal << std::endl;

	mDbg.flush();

	/*if (!isLibraryLoaded()) return false;

	if (mDllOnServerMessageFunc) return mDllOnServerMessageFunc(serverMsg);
	else mDbg << FileLogger::err << "mDllOnServerMessageFunc failure" << FileLogger::normal << std::endl;

	mDbg.flush();
	return true;*/
	return true;
}

