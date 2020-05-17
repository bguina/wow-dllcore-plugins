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
	INFO,
	START_SUBSCRIBE,
	STOP_SUBSCRIBE,
	START_BOT,
	STOP_BOT
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
	std::list<std::string> getSubcribeObject(std::string message);

	//BUILDER MESSAGE REQUEST
	std::string builRequestStartSubcribe(std::list<std::string> toSubscribe);
	std::string builRequestdAvailableConfigationMessage();
	std::string builRequestdInjectMessage(int pid, std::string module);
	std::string builRequestdDLLInjectedMessage(int pid);
	std::string builRequestdDeinjecteMessage();

	//BUILDER MESSAGE RESPONSE
	std::string builResponseAvailableConfigationMessage(std::string pids, std::string modules);
	std::string builResponseInfo(std::string name, std::string value);

};
#endif // !MESSAGE_MANAGER_H




