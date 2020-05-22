#pragma once

#include <sstream>

#include "IDebugger.h"

class FileDebugger : public IDebugger {
public:
	FileDebugger(const std::string& tag);
	virtual ~FileDebugger();

	// append output for the next file flush
	virtual void log(const std::string& msg) override;
	virtual void log(std::stringstream& msg) override;

	// append output with debug level for the next file flush
	virtual void i(const std::string& msg) override;
	virtual void w(const std::string& msg) override;
	virtual void e(const std::string& msg) override;

	// append output with debug level for the next file flush
	// also, clears the stringstream after reading it.
	virtual void i(std::stringstream& msg) override;
	virtual void w(std::stringstream& msg) override;
	virtual void e(std::stringstream& msg) override;

	// clear previous file content
	void clear();

	// flush buffer to the log file
	void flush();

	template<typename T>
	std::ostream& operator<<(const T& obj) {
		mBuff << obj;
		return mBuff;
	}

private:
	std::stringstream mBuff;
	const std::string mFolder;
	const std::string mOutputFile;

public:
	FileDebugger(FileDebugger const&) = delete;
	void operator=(FileDebugger const&) = delete;

	static const std::string normal;
	static const std::string info;
	static const std::string warn;
	static const std::string err;
};
