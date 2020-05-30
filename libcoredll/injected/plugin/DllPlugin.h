#pragma once

#include "IDllPlugin.h"

#include <Windows.h>
#include "FileLogger.h"

class DllPlugin : public IDllPlugin
{
public:
	DllPlugin();
	virtual ~DllPlugin();

	virtual bool loadLibrary(const std::wstring& dllPath);
	virtual bool isLibraryLoaded() const;
	virtual void freeLibrary();

	virtual bool onD3dRender() override;

	typedef int(__stdcall* DllPlugin_OnLoad)(uint32_t pid);
	typedef int(__stdcall* DllPlugin_OnUnload)();
	typedef int(__stdcall* DllPlugin_OnD3dRender)();
	typedef int(__stdcall* DllPlugin_OnServerMessage)(void* param);

protected:
	HINSTANCE mDll;
	FileLogger mDbg;
	DllPlugin_OnLoad mDllOnLoadFunc;
	DllPlugin_OnUnload mDllOnUnloadFunc;
	DllPlugin_OnD3dRender mDllOnD3dRenderFunc;
	DllPlugin_OnServerMessage mDllOnServerMessageFunc;

};
