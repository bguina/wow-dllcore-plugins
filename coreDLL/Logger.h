#pragma once

#include <string>

class Logger {
public:
	Logger(const std::string& outputFile);

	void reset();
	void log(std::string msg);

private:
	std::string mOutputFile;
};
