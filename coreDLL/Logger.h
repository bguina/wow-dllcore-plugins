#pragma once

#include "Singleton.h"

class Logger : public Singleton<Logger> {
public:
	Logger(madeSingleton);

	void clear();
	void log(const char* msg);

private:
	const char* mOutputFile;

public:
	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;
};
