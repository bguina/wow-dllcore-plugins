#pragma once

#include "IDllPlugin.h"

#include <Windows.h>
#include "FileLogger.h"

class DllPlugin : public IDllPlugin
{
public:
	DllPlugin();
	virtual ~DllPlugin();
	virtual std::string getTag() const override;

	virtual bool isLibraryLoaded() const;
	virtual std::string getLibraryPath() const;
	virtual uint64_t getLibraryVersion() const;

	virtual bool loadLibrary(const std::wstring& dllPath);
	virtual void freeLibrary();

	virtual bool onD3dRender() override;

	typedef int(__stdcall* DllPlugin_OnLoad)();
	typedef int(__stdcall* DllPlugin_OnUnload)();
	typedef int(__stdcall* DllPlugin_OnD3dRender)();

protected:
	std::wstring stringConvertToWstring(const std::string& w) const;
	std::string wstringConvertToString(const std::wstring& w) const;

	std::wstring mFilePath;
	HINSTANCE mDll;
	FileLogger mDbg;
	DllPlugin_OnLoad mDllOnLoadFunc;
	DllPlugin_OnUnload mDllOnUnloadFunc;
	DllPlugin_OnD3dRender mDllOnD3dRenderFunc;
};
