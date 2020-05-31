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
const std::string FileLogger::debug = std::string(CYAN) + "[d]";
const std::string FileLogger::verbose = std::string(WHITE) + "[v]";
const std::string FileLogger::info = std::string(GREEN) + "[i]";
const std::string FileLogger::warn = std::string(YELLOW) + "[w]";
const std::string FileLogger::err = std::string(BOLDRED) + "[e]";

FileLogger::FileLogger(const std::string& tag) :
	FileLogger(tag, std::string())
{
	struct stat dirInfo;

	// make folder if it does not exist yet
	if (0 != stat(mFolder.c_str(), &dirInfo) && errno == ENOENT) {
		_mkdir(mFolder.c_str());
	}
}

FileLogger::FileLogger(const FileLogger& o) :
	FileLogger(o.getTag())
{

}

FileLogger::FileLogger(const FileLogger& o, const std::string& prefix) :
	FileLogger(o.getTag(), prefix)
{

}

FileLogger::FileLogger(const std::string& tag, const std::string& prefix) :
	mFolder("D:\\nvtest\\"),
	mTag(tag),
	mPrefix(prefix + ": "),
	mOutputPath(mFolder + mTag + ".log"),
	mOfs(mOutputPath, std::fstream::in | std::fstream::out | std::fstream::app)
{

}

FileLogger::~FileLogger() {
	//mOfs.close();
}

const std::string& FileLogger::getTag() const {
	return mTag;
}

void FileLogger::clear() {
	std::ofstream(mOutputPath, std::fstream::in | std::fstream::out);
}

void FileLogger::log(const std::string& msg) {
	mOfs << msg << std::endl;
}

void FileLogger::log(std::stringstream& msg) {
	log(mPrefix + msg.str());
}

void FileLogger::i(const std::string& msg) {
	mOfs << info << mPrefix << msg << normal << std::endl;
}

void FileLogger::w(const std::string& msg) {
	mOfs << warn << mPrefix << msg << normal << std::endl;
}

void FileLogger::e(const std::string& msg) {
	mOfs << err << mPrefix << msg << normal << std::endl;
}

void FileLogger::i(std::stringstream& msg) {
	i(msg.str());
	msg.str("");
}

void FileLogger::w(std::stringstream& msg) {
	w(msg.str());
	msg.str("");
}

void FileLogger::e(std::stringstream& msg) {
	e(msg.str());
	msg.str("");
}
