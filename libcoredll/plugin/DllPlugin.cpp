#include "pch.h"

#include "DllPlugin.h"

#include <windows.h>
#include <string>
#include <iostream>
#include <codecvt>

DllPlugin::DllPlugin() :
	mDll(nullptr),
	mDbg("DllPlugin"),
	mDllOnLoadFunc(nullptr),
	mDllOnUnloadFunc(nullptr),
	mDllOnD3dRenderFunc(nullptr)
{
}

DllPlugin::~DllPlugin() {
	freeLibrary();
}

std::string DllPlugin::getTag() const {
	return mDbg.getTag();
}

bool DllPlugin::isLibraryLoaded() const {
	return nullptr != mDll;
}

std::string DllPlugin::getLibraryPath() const {
	return wstringConvertToString(mFilePath);
}

uint64_t DllPlugin::getLibraryVersion() const {
	return isLibraryLoaded() ? 1 : 0;
}

#define DLL_FUNC(dll, func) (func)GetProcAddress(mDll, TEXT(func));

bool DllPlugin::loadLibrary(const std::wstring& dllPath) {
	FileLogger dbg(mDbg, "loadLibrary");
	std::string path(wstringConvertToString(dllPath));

	freeLibrary();

	if (!dllPath.empty()) {
		dbg << FileLogger::verbose << "LoadLibrary(" << path << ")" << FileLogger::normal << std::endl;
		mDll = LoadLibraryW(LPWSTR(dllPath.c_str()));

		if (mDll) {

			// resolve functions
			mFilePath = dllPath;
			mDllOnLoadFunc = (DllPlugin_OnLoad)GetProcAddress(mDll, "DllPlugin_OnLoad");
			mDllOnD3dRenderFunc = (DllPlugin_OnD3dRender)GetProcAddress(mDll, "DllPlugin_OnD3dRender");
			mDllOnUnloadFunc = (DllPlugin_OnUnload)GetProcAddress(mDll, "DllPlugin_OnUnload");
			dbg << FileLogger::verbose << "resolved" << path << FileLogger::normal << std::endl;

			if (!mDllOnLoadFunc) {
				dbg << FileLogger::err << "missing DllPlugin_OnLoad export in library " << path << FileLogger::normal << std::endl;
				freeLibrary();
			}
		}
		else {
			dbg << FileLogger::err << "Failed to LoadLibrary(" << path << ") errno " << GetLastError() << FileLogger::normal << std::endl;
		}
	}

	if (mDllOnLoadFunc) {
		dbg << FileLogger::verbose << "initializing " << path << FileLogger::normal << std::endl;
		mDllOnLoadFunc();
		dbg << FileLogger::info << "loaded " << path << FileLogger::normal << std::endl;
	}

	return isLibraryLoaded();
}

void DllPlugin::freeLibrary() {
	FileLogger dbg(mDbg, "freeLibrary");

	if (mDll) {
		dbg << FileLogger::verbose << "freeing library" << FileLogger::normal << std::endl;
		mDllOnUnloadFunc();
		FreeLibrary(mDll);
		dbg << FileLogger::verbose << "freed library" << FileLogger::normal << std::endl;
		mDll = nullptr;
	} else
		dbg << FileLogger::warn << "no library to free" << FileLogger::normal << std::endl;

	mFilePath = std::wstring();
	mDllOnD3dRenderFunc = nullptr;
}

bool DllPlugin::onD3dRender() {
	FileLogger dbg(mDbg, "onD3dRender");
	if (!isLibraryLoaded()) return true; // fixme might not be loaded YET?

	if (mDllOnD3dRenderFunc) {
		dbg << FileLogger::debug << "call to library" << FileLogger::normal << std::endl;
		mDllOnD3dRenderFunc();
	}
	else {
		dbg << FileLogger::err << "mDllD3dRender failure" << FileLogger::normal << std::endl;
		return false;
	}

	return true;
}

std::wstring DllPlugin::stringConvertToWstring(const std::string& w) const {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(w);
}

std::string DllPlugin::wstringConvertToString(const std::wstring& w) const {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(w);
}