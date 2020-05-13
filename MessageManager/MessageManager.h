#pragma once

#include "framework.h"

#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include "PicoJSON.h"

#include "AvailableConfigurationObject.h"
#include "InjectObject.h"

#include <string>
#include <iostream>

enum class MessageType {
	UNKNOWN,
	MESSAGE_TYPE,
	AVAILABLE_CONFIGURATION,
	INJECT,
	DEINJECT,
	DLL_INJECTED,
};

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

	//BUILDER MESSAGE REQUEST
	std::string builRequestdAvailableConfigationMessage();
	std::string builRequestdInjectMessage(int pid, std::string module);
	std::string builRequestdDLLInjectedMessage(int pid);
	std::string builRequestdDeinjecteMessage();

	//BUILDER MESSAGE RESPONSE
	std::string builResponseAvailableConfigationMessage(std::string pids, std::string modules);

};
#endif // !MESSAGE_MANAGER_H




