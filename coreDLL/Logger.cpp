#include "pch.h"

#include <fstream>
#include <sstream>

#include "logger.h"

Logger::Logger(madeSingleton)
	:
	Singleton<Logger>(),
	mOutputFile("D:\\nvtest.log")
{
	clear();
}

void Logger::clear() {
	//std::ofstream(g_logfilePath, std::fstream::in | std::fstream::out);
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

void Logger::log(
	const char* msg
) {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << msg << std::endl;
}
