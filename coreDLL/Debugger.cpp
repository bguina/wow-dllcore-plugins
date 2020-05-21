#include "pch.h"

#include <fstream>
#include <sstream>

#include "Debugger.h"

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

Debugger::Debugger(const std::string& fileName)
	: mOutputFile(std::string("D:\\nvtest\\") + fileName + ".log")
{
}

Debugger::~Debugger()
{
	flush();
}

void Debugger::clear() {
	std::ofstream(mOutputFile, std::fstream::in | std::fstream::out);
}

void Debugger::log(const std::string& msg) {
	mBuff << msg << std::endl;
}



void Debugger::i(const std::string& msg) {
	mBuff << GREEN << "[i]" << msg << RESET << std::endl;
}

void Debugger::w(const std::string& msg) {
	mBuff << RED << "[w]" << msg << RESET << std::endl;
}

void Debugger::e(const std::string& msg) {
	mBuff << BOLDRED << "[e]" << msg << RESET << std::endl;
}

void Debugger::i(std::stringstream& msg) {
	i(msg.str());
	msg.str("");
}

void Debugger::w(std::stringstream& msg) {
	w(msg.str());
	msg.str("");
}

void Debugger::e(std::stringstream& msg) {
	e(msg.str());
	msg.str("");
}

Debugger& Debugger::operator<<(const std::string& str) {
	log(str);
	return *this;
}

void Debugger::flush() {
	if (!mBuff.str().empty()) {
		std::ofstream(mOutputFile, std::fstream::in | std::fstream::out | std::fstream::app) << mBuff.str() << std::endl;
		mBuff.str(std::string());
	}
}
