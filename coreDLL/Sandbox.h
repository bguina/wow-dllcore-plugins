#pragma once

#include "WowGame.h"
#include "WowNavigator.h"
#include "Logger.h"
#include "Singleton.h"

class Sandbox : public Singleton<Sandbox>
{
public:
	Sandbox(madeSingleton);

	bool isOverHeating() const;

	void run();

private: 
	HMODULE mModuleBaseAddr;
	WowGame mGame;
	WowNavigator mBot;

public:
	Sandbox(Sandbox const&) = delete;
	void operator=(Sandbox const&) = delete;
};