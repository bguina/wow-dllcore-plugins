#include "pch.h"

#include <fstream>
#include <sstream>
#include <direct.h>

#include "FileDebugger.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

const std::string FileDebugger::normal = std::string(RESET);
const std::string FileDebugger::info = std::string(GREEN);
const std::string FileDebugger::warn = std::string(YELLOW);
const std::string FileDebugger::err = std::string(BOLDRED);

FileDebugger::FileDebugger(const std::string& fileName) :
	IDebugger(),
	mFolder("D:\\nvtest\\"),
	mOutputFile(mFolder + fileName + ".log")
{
	struct stat dirInfo;

	// make folder if it does not exist yet
	if (0 != stat(mFolder.c_str(), &dirInfo) && errno == ENOENT) {
		_mkdir(mFolder.c_str());
	}
}

FileDebugger::~FileDebugger()
{
	flush();
}

void FileDebugger::clear() {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

void FileDebugger::log(const std::string& msg) const {
	mBuff << msg << std::endl;
}

void FileDebugger::log(std::stringstream& msg) const {
	log(msg.str());
	msg.str("");
}

void FileDebugger::i(const std::string& msg) const {
	mBuff << info << "[i] " << msg << normal << std::endl;
}

void FileDebugger::w(const std::string& msg) const {
	mBuff << warn << "[w] " << msg << normal << std::endl;
}

void FileDebugger::e(const std::string& msg) const {
	mBuff << err << "[e] " << msg << normal << std::endl;
}

void FileDebugger::i(std::stringstream& msg) const {
	i(msg.str());
	msg.str("");
}

void FileDebugger::w(std::stringstream& msg) const {
	w(msg.str());
	msg.str("");
}

void FileDebugger::e(std::stringstream& msg) const {
	e(msg.str());
	msg.str("");
}

void FileDebugger::flush() {
	if (!mBuff.str().empty()) {
		std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << mBuff.str() << std::endl;
		mBuff.str(std::string());
	}
}
