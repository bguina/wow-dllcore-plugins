#pragma once

#include <sstream>

class Debugger  {
public:
	Debugger(const char* logPath);

	void clear();
	void log(const char* msg);
	void flush();

private:
	std::stringstream mBuff;
	const char* mOutputFile;

public:
	Debugger(Debugger const&) = delete;
	void operator=(Debugger const&) = delete;
};
