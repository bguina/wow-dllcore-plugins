#include "pch.h"

#include <iostream>

#include "PicoJSON.h"

#include "MessageManager.h"

std::list<std::string> MessageManager::getWaypointsObject(std::string message) {
	picojson::value rootJSON;
	std::list<std::string> listWaypoint;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::POST_DLL_DATA_3DPATH) {
		if (rootJSON.get("waypoints").is<picojson::array>())
		{
			picojson::array arr = rootJSON.get("waypoints").get<picojson::array>();
			picojson::array::iterator it;
			for (it = arr.begin(); it != arr.end(); it++) {
				listWaypoint.push_back(it->get<std::string>());
			}
			return listWaypoint;
		}
	}
	return listWaypoint;
}

std::string MessageManager::buildWaypoinsFile(std::list<std::string> listWaypoint) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::POST_DLL_DATA_3DPATH));
	picojson::array arr;
	for (std::list<std::string>::iterator it = listWaypoint.begin(); it != listWaypoint.end(); it++)
	{
		arr.push_back(picojson::value(*it));
	}
	rootJSON.get<picojson::object>()["waypoints"] = picojson::value(arr);
	return rootJSON.serialize();
}

//INFO
std::pair<std::string, std::string> MessageManager::getInfoObject(std::string message) {
	picojson::value rootJSON;
	std::pair<std::string, std::string> infoObject;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::DATA_PROMPT_DLL) {
		if (rootJSON.get("data").is<picojson::object>()
			&& rootJSON.get("data").get("name").is<std::string>()
			&& rootJSON.get("data").get("value").is<std::string>())
		{
			std::string name = rootJSON.get("data").get("name").to_str();
			std::string value = rootJSON.get("data").get("value").to_str();

			infoObject = std::pair<std::string, std::string>(name, value);
			return infoObject;
		}
	}
	return infoObject;
}


std::string MessageManager::builResponseInfo(std::string name, std::string value) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::DATA_PROMPT_DLL));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["name"] = picojson::value(name);
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["value"] = picojson::value(value);
	return rootJSON.serialize();
}

//START_SUBSCRIBE
std::string MessageManager::builRequestStartSubcribe(std::list<std::string> toSubscribe) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::SUBSCRIBE_DLL_UPDATES));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	std::string to("");
	for (std::list<std::string>::iterator it = toSubscribe.begin(); it != toSubscribe.end(); it++)
	{
		if (!to.empty())
			to += ",";
		to += (*it);
	}
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["to"] = picojson::value(to);
	return rootJSON.serialize();
}

//STOP_SUBSCRIBE
std::string MessageManager::builRequestStopSubcribe(std::list<std::string> stopSubscribe) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::UNSUBSCRIBE_DLL_UPDATES));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	std::string to("");
	for (std::list<std::string>::iterator it = stopSubscribe.begin(); it != stopSubscribe.end(); it++)
	{
		if (!to.empty())
			to += ",";
		to += (*it);
	}
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["to"] = picojson::value(to);
	return rootJSON.serialize();
}


std::list<std::string> MessageManager::getSubcribeObject(std::string message) {
	picojson::value rootJSON;
	std::list<std::string> toSubscribe;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::SUBSCRIBE_DLL_UPDATES ||
		getMessageType(message) == MessageType::UNSUBSCRIBE_DLL_UPDATES) {
		if (rootJSON.get("data").is<picojson::object>()
			&& rootJSON.get("data").get("to").is<std::string>())
		{
			std::string to = rootJSON.get("data").get("to").to_str();

			toSubscribe = splitByDelimiter(to, ",");
			return toSubscribe;
		}
	}
	return toSubscribe;
}

//DEINJECT
std::string MessageManager::builRequestdDeinjecteMessage() {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::POST_SERVER_EJECTION));
	return rootJSON.serialize();
}

std::string MessageManager::builRequestdStartBotMessage() {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::RESUME));
	return rootJSON.serialize();
}

std::string MessageManager::builRequestdStopBotMessage() {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::PAUSE));
	return rootJSON.serialize();
}

int MessageManager::getDLLInjectedObject(std::string message) {
	picojson::value rootJSON;
	int pid = -1;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::DLL_RESPONSE_INJECTED) {
		if (rootJSON.get("data").is<picojson::object>()
			&& rootJSON.get("data").get("pid").is<std::string>())
		{
			std::string strPID = rootJSON.get("data").get("pid").to_str();
			pid = std::stoi(strPID);
			return pid;
		}
	}
	return pid;
}

//DLL_INJECTED
std::string MessageManager::builRequestdDLLInjectedMessage(int pid) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::DLL_RESPONSE_INJECTED));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["pid"] = picojson::value(std::to_string(pid));
	return rootJSON.serialize();
}

//INJECT
InjectObject* MessageManager::getInjectObject(std::string message) {
	picojson::value rootJSON;
	std::string err(picojson::parse(rootJSON, message));

	if (!err.empty()) {
		std::cerr << err << std::endl;
		return nullptr;
	}

	if (getMessageType(message) == MessageType::POST_SERVER_INJECTION) {

		if (rootJSON.get("data").is<picojson::object>()
			&& rootJSON.get("data").get("pid").is<std::string>()
			&& rootJSON.get("data").get("module").is<std::string>())
		{
			std::string strPID = rootJSON.get("data").get("pid").to_str();
			std::string strModule = rootJSON.get("data").get("module").to_str();

			return new InjectObject(std::stoi(strPID), strModule);
		}

	}
	return nullptr;
}

std::string MessageManager::builRequestdInjectMessage(int pid, std::string module) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::POST_SERVER_INJECTION));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["pid"] = picojson::value(std::to_string(pid));
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["module"] = picojson::value(module);
	return rootJSON.serialize();
}

//AVAILABLE_CONFIGURATION
AvailableConfigurationObject* MessageManager::getAvailableConfigurationObject(std::string message) {
	picojson::value rootJSON;
	AvailableConfigurationObject* object = NULL;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::GET_SERVER_OPTIONS) {
		if (rootJSON.get("data").is<picojson::object>()
			&& rootJSON.get("data").get("pids").is<std::string>()
			&& rootJSON.get("data").get("modules").is<std::string>())
		{
			std::string strPIDS = rootJSON.get("data").get("pids").to_str();
			std::string strModules = rootJSON.get("data").get("modules").to_str();

			std::list<std::string> pids = splitByDelimiter(strPIDS, ",");
			std::list<std::string> modules = splitByDelimiter(strModules, ",");

			object = new AvailableConfigurationObject(pids, modules);
			return object;
		}
	}
	return object;
}

std::string MessageManager::builResponseAvailableConfigationMessage(std::string pids, std::string modules) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::GET_SERVER_OPTIONS));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["pids"] = picojson::value(pids);
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["modules"] = picojson::value(modules);

	return rootJSON.serialize();
}

std::string MessageManager::builRequestdAvailableConfigationMessage() {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::GET_SERVER_OPTIONS));
	return rootJSON.serialize();
}


//BASE OBJECT

MessageType MessageManager::getMessageType(std::string message) {
	picojson::value rootJSON;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	if (rootJSON.is<picojson::object>()) {
		picojson::value type = rootJSON.get(getMessageTypeString(MessageType::MESSAGE_TYPE));
		if (type.is<std::string>())
		{
			return getMessageTypeFromString(type.to_str());
		}
		else {
			return MessageType::UNKNOWN;
		}
	}

	return MessageType::UNKNOWN;
}

MessageType MessageManager::getMessageTypeFromString(std::string type) {
	if (type == "MESSAGE_TYPE")
	{
		return MessageType::MESSAGE_TYPE;
	}
	else if (type == "AVAILABLE_CONFIGURATION")
	{
		return MessageType::GET_SERVER_OPTIONS;
	}
	else if (type == "INJECT")
	{
		return MessageType::POST_SERVER_INJECTION;
	}
	else if (type == "DEINJECT")
	{
		return MessageType::POST_SERVER_EJECTION;
	}
	else if (type == "DLL_INJECTED")
	{
		return MessageType::DLL_RESPONSE_INJECTED;
	}
	else if (type == "INFO")
	{
		return MessageType::DATA_PROMPT_DLL;
	}
	else if (type == "START_SUBSCRIBE")
	{
		return MessageType::SUBSCRIBE_DLL_UPDATES;
	}
	else if (type == "STOP_SUBSCRIBE")
	{
		return MessageType::UNSUBSCRIBE_DLL_UPDATES;
	}
	else if (type == "START_BOT")
	{
		return MessageType::RESUME;
	}
	else if (type == "STOP_BOT")
	{
		return MessageType::PAUSE;
	}
	else if (type == "WAYPOINTS")
	{
		return MessageType::POST_DLL_DATA_3DPATH;
	}
	else {
		std::cerr << "Unknown type..." << std::endl;
		return MessageType::UNKNOWN;
	}
}

std::string MessageManager::getMessageTypeString(MessageType type) {
	switch (type)
	{
	case MessageType::UNKNOWN: { return("UNKNOWN"); } break;
	case MessageType::MESSAGE_TYPE: { return("MESSAGE_TYPE"); } break;
	case MessageType::GET_SERVER_OPTIONS: { return("AVAILABLE_CONFIGURATION"); } break;
	case MessageType::POST_SERVER_INJECTION: { return ("INJECT"); } break;
	case MessageType::POST_SERVER_EJECTION: { return ("DEINJECT"); } break;
	case MessageType::DLL_RESPONSE_INJECTED: { return ("DLL_INJECTED"); } break;
	case MessageType::DATA_PROMPT_DLL: { return ("INFO"); } break;
	case MessageType::SUBSCRIBE_DLL_UPDATES: { return ("START_SUBSCRIBE"); } break;
	case MessageType::UNSUBSCRIBE_DLL_UPDATES: { return ("STOP_SUBSCRIBE"); } break;
	case MessageType::RESUME: { return ("START_BOT"); } break;
	case MessageType::PAUSE: { return ("STOP_BOT"); } break;
	case MessageType::POST_DLL_DATA_3DPATH: { return ("WAYPOINTS"); } break;
	default:
		std::cerr << "Unknown type..." << std::endl;
		return NULL;
	}
}

std::list<std::string> MessageManager::splitByDelimiter(const std::string input, const std::string delimiter) {
	std::string copyInput(input);
	size_t pos = 0;
	std::string token;
	std::list<std::string> resultList;
	while ((pos = copyInput.find(delimiter)) != std::string::npos) {
		token = copyInput.substr(0, pos);
		resultList.push_back(token);
		copyInput.erase(0, pos + delimiter.length());
	}
	resultList.push_back(copyInput);
	return resultList;
}
