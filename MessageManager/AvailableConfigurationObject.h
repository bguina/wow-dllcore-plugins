#pragma once

#include <list>
#include <string>

class AvailableConfigurationObject
{
private:
	std::list<std::string> listPID;
	std::list<std::string> listModule;
public:
	AvailableConfigurationObject(std::list<std::string> _listPID, std::list<std::string> _listModule) { listPID = _listPID; listModule = _listModule; };

	std::list<std::string> getListPID() { return listPID; };
	std::list<std::string> getListModule() { return listModule; };
};
