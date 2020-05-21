#pragma once

#include <sstream>

class Debugger  {
public:
	Debugger(const char* logPath);
	virtual ~Debugger();

	// clear previous file content
	void clear();

	// add output for next file flush
	//void log(const char* msg);
	void log(const std::string& msg);

	// flush buffer to the log file
	void flush();

private:
	std::stringstream mBuff;
	const char* mOutputFile;

public:
	Debugger(Debugger const&) = delete;
	void operator=(Debugger const&) = delete;
};
