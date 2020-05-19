#include "pch.h"

#include <fstream>
#include <sstream>

#include "Debugger.h"

Debugger::Debugger(const char* logPath)
	: mOutputFile(logPath)
{
}

// clear previous file content
void Debugger::clear() {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

// add output for next file flush
void Debugger::log(const char* msg) {
	mBuff << msg;
}

// flush buffer to the log file
void Debugger::flush() {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << mBuff.str() << std::endl << std::endl;
	mBuff.str(std::string());
}
