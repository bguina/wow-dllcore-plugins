#pragma once

#include <sstream>

class ILogger {
public:
	virtual ~ILogger() {};

	void operator=(ILogger const&) = delete;

	// append output for the next file flush
	virtual void log(const std::string& msg) const = 0;
	virtual void log(std::stringstream& msg) const = 0;

	// append output with debug level for the next file flush
	virtual void i(const std::string& msg) const = 0;
	virtual void w(const std::string& msg) const = 0;
	virtual void e(const std::string& msg) const = 0;

	// append output with debug level for the next file flush
	// also, clears the stringstream after reading it.
	virtual void i(std::stringstream& msg) const = 0;
	virtual void w(std::stringstream& msg) const = 0;
	virtual void e(std::stringstream& msg) const = 0;
};
