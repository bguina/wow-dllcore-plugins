#pragma once

#include <list>
#include <string>

class InjectObject
{
private:
	int pid;
	std::string module;
public:
	InjectObject(int _pid, std::string _module) { pid = _pid; module = _module; };

	int getPID() { return pid; };
	std::string getModule() { return module; };
};
