#pragma once

#include <sstream>

class ILogger {
public:
	virtual ~ILogger() {};

	void operator=(ILogger const&) = delete;

	virtual const std::string& getTag() const = 0;

	// append output for the next file flush
	virtual void log(const std::string& msg)  = 0;

	// append output with debug level for the next file flush
	virtual void i(const std::string& msg)  = 0;
	virtual void w(const std::string& msg)  = 0;
	virtual void e(const std::string& msg)  = 0;
};
