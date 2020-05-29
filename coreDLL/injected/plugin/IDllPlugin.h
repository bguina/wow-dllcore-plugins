#pragma once

#include "IPlugin.h"

#define DLLPLUGIN_LOADED DllLoaded
#define DLLPLUGIN_D3DRENDER DllOnRender
#define DLLPLUGIN_HANDLEMESSAGE DllHandleMessage

#define DLLPLUGIN_LOADED_FUNC int __declspec(dllexport) __stdcall DLLPLUGIN_LOADED
#define DLLPLUGIN_D3DRENDER_FUNC int __declspec(dllexport) __stdcall DLLPLUGIN_D3DRENDER
#define DLLPLUGIN_HANDLEMESSAGE_FUNC int __declspec(dllexport) __stdcall DLLPLUGIN_HANDLEMESSAGE

typedef int (__stdcall* DllPlugin_Loaded)();
typedef int (__stdcall* DllPlugin_D3dRender)();
typedef int (__stdcall* DllPlugin_Unloaded)();

class IDllPlugin : public IPlugin
{
public:
	virtual ~IDllPlugin() {};
	virtual void onD3dRender() = 0;
};
