// dllmain.cpp : Defines the entry point for the DLL application.

#include <fstream>

#include "ExampleRemotePlugin.h"

ExampleRemotePlugin::ExampleRemotePlugin() {
};

ExampleRemotePlugin::~ExampleRemotePlugin() {
};

void ExampleRemotePlugin::onD3dRender() {
	std::ofstream out("D:\\nvtest\\pluginDll.log", std::fstream::in | std::fstream::out | std::fstream::app);

	out << "RemotePlugin::onD3dRender" << std::endl;
}

