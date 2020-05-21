#pragma once

#include <sstream>

class Debugger {
public:
	Debugger(const std::string& tag);
	virtual ~Debugger();

	// clear previous file content
	void clear();

	// append output for the next file flush
	void log(const std::string& msg);
	Debugger& operator<<(const std::string& str);

	// append output with debug level for the next file flush
	void i(const std::string& msg);
	void w(const std::string& msg);
	void e(const std::string& msg);

	// append output with debug level for the next file flush
	// also, clears the stringstream after reading it.
	void i(std::stringstream& msg);
	void w(std::stringstream& msg);
	void e(std::stringstream& msg);

	// flush buffer to the log file
	void flush();

private:
	std::stringstream mBuff;
	const std::string mOutputFile;

public:
	Debugger(Debugger const&) = delete;
	void operator=(Debugger const&) = delete;
};
