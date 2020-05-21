#include "pch.h"

#include <fstream>
#include <sstream>

#include "Debugger.h"

Debugger::Debugger(const char* logPath)
	: mOutputFile(logPath)
{
}

void Debugger::clear() {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

void Debugger::log(const std::string& msg) {
	mBuff << msg;
}

void Debugger::log(const char* msg) {
	mBuff << msg;
}

void Debugger::flush() {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << mBuff.str() << std::endl << std::endl;
	mBuff.str(std::string());
}
