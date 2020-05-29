#pragma once

#include <string>
#include <list>

#include "framework.h"

#include "AvailableConfigurationObject.h"
#include "InjectObject.h"

#include "MessageType.h"

class MessageManager
{
private:
	std::string getMessageTypeString(MessageType type);
	MessageType getMessageTypeFromString(std::string type);
	std::list<std::string> splitByDelimiter(std::string input, std::string delimiter);

public:
	MessageManager() { };

	//GET MESSAGE TYPE
	MessageType getMessageType(std::string);

	//BUILDER OBJECT
	AvailableConfigurationObject* getAvailableConfigurationObject(std::string message);
	InjectObject* getInjectObject(std::string message);
	int getDLLInjectedObject(std::string message);
	std::list<std::string> getSubcribeObject(std::string message);
	std::pair<std::string, std::string> getInfoObject(std::string message);
	std::list<std::string> getWaypointsObject(std::string message);

	//BUILDER MESSAGE REQUEST
	std::string builRequestStartSubcribe(std::list<std::string> toSubscribe);
	std::string builRequestStopSubcribe(std::list<std::string> stopSubscribe);
	std::string builRequestdAvailableConfigationMessage();
	std::string builRequestdInjectMessage(int pid, std::string module);
	std::string builRequestdDLLInjectedMessage(int pid);
	std::string builRequestdDeinjecteMessage();
	std::string builRequestdStartBotMessage();
	std::string builRequestdStopBotMessage();

	//BUILDER MESSAGE RESPONSE
	std::string builResponseAvailableConfigationMessage(std::string pids, std::string modules);
	std::string builResponseInfo(std::string name, std::string value);

	//WAYPOINS FILE
	std::string buildWaypoinsFile(std::list<std::string> listWaypoint);
};
