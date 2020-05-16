#include "pch.h"

#include <fstream>
#include <sstream>

#include "Debugger.h"

Debugger::Debugger(madeSingleton)
	:
	Singleton<Debugger>(),
	mOutputFile("D:\\nvtest.log")
{
	clear();
}

void Debugger::clear() {
	//std::ofstream(g_logfilePath, std::fstream::in | std::fstream::out);
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

void Debugger::log(
	const char* msg
) {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << msg << std::endl;
}
