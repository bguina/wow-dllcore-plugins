#pragma once

#ifndef INJECT_OBJECT_H
#define INJECT_OBJECT_H

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

#endif // !INJECT_OBJECT_H