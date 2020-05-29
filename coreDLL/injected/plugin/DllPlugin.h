#pragma once

#include "IDllPlugin.h"

#include <Windows.h>
#include "../../logger/FileLogger.h"

class DllPlugin : public IDllPlugin
{
public:
	DllPlugin();
	virtual ~DllPlugin();

	virtual bool loadLibrary(const std::wstring& dllPath);
	virtual bool isLibraryLoaded() const;
	virtual void freeLibrary();

	virtual void onD3dRender() override;

	typedef int(__stdcall* DllPlugin_OnLoad)(uint32_t pid);
	typedef int(__stdcall* DllPlugin_OnUnload)();
	typedef int(__stdcall* DllPlugin_OnD3dRender)();

protected:
	HINSTANCE mDll;
	FileLogger mDbg;
	DllPlugin_OnLoad mDllOnLoadFunc;
	DllPlugin_OnUnload mDllOnUnloadFunc;
	DllPlugin_OnD3dRender mDllOnD3dRenderFunc;
};
