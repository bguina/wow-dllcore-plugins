#include "pch.h"

#include <fstream>
#include <sstream>
#include <direct.h>

#include "FileLogger.h"

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

const std::string FileLogger::normal = std::string(RESET);
const std::string FileLogger::debug = std::string(CYAN);
const std::string FileLogger::verbose = std::string(WHITE);
const std::string FileLogger::info = std::string(GREEN);
const std::string FileLogger::warn = std::string(YELLOW);
const std::string FileLogger::err = std::string(BOLDRED);

FileLogger::FileLogger(const std::string& tag) :
	ILogger(),
	mFolder("D:\\nvtest\\"),
	mTag(tag),
	mOutputFile(mFolder + mTag + ".log")
{
	struct stat dirInfo;

	// make folder if it does not exist yet
	if (0 != stat(mFolder.c_str(), &dirInfo) && errno == ENOENT) {
		_mkdir(mFolder.c_str());
	}
}

FileLogger::~FileLogger()
{
	flush();
}

const std::string& FileLogger::getTag() const {
	return mTag;
}

void FileLogger::clear() {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

void FileLogger::log(const std::string& msg) const {
	mBuff << msg << std::endl;
}

void FileLogger::log(std::stringstream& msg) const {
	log(msg.str());
	msg.str("");
}

void FileLogger::i(const std::string& msg) const {
	mBuff << info << "[i] " << msg << normal << std::endl;
}

void FileLogger::w(const std::string& msg) const {
	mBuff << warn << "[w] " << msg << normal << std::endl;
}

void FileLogger::e(const std::string& msg) const {
	mBuff << err << "[e] " << msg << normal << std::endl;
}

void FileLogger::i(std::stringstream& msg) const {
	i(msg.str());
	msg.str("");
}

void FileLogger::w(std::stringstream& msg) const {
	w(msg.str());
	msg.str("");
}

void FileLogger::e(std::stringstream& msg) const {
	e(msg.str());
	msg.str("");
}

void FileLogger::flush() {
	if (!mBuff.str().empty()) {
		std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << mBuff.str() << std::endl;
		mBuff.str(std::string());
	}
}
