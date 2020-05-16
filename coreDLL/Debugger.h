#pragma once

#include "Singleton.h"

class Debugger : public Singleton<Debugger> {
public:
	Debugger(madeSingleton);

	void clear();
	void log(const char* msg);

private:
	const char* mOutputFile;

public:
	Debugger(Debugger const&) = delete;
	void operator=(Debugger const&) = delete;
};
