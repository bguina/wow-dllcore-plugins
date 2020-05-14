// pch.cpp: source file corresponding to the pre-compiled header
#include "pch.h"

#include <fstream>
#include <sstream>
#include "logger.h"

Logger::Logger(
	const std::string& outputFile
) : mOutputFile(outputFile) {

}

void Logger::reset() {
	//std::ofstream(g_logfilePath, std::fstream::in | std::fstream::out);
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

void Logger::log(
	std::string msg
) {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << msg << std::endl;
}
