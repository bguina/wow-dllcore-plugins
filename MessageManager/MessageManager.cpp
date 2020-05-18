// MessageManager.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "MessageManager.h"

std::list<std::string> MessageManager::getWaypoinsObject(std::string message) {
	picojson::value rootJSON;
	std::list<std::string> listWaypoint;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::WAYPOINTS) {
		if (rootJSON.get("data").is<picojson::array>())
		{
			picojson::array arr = rootJSON.get("data").get<picojson::array>();
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
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::WAYPOINTS));
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
	else if (getMessageType(message) == MessageType::INFO) {
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
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::INFO));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["name"] = picojson::value(name);
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["value"] = picojson::value(value);
	return rootJSON.serialize();
}

//START_SUBSCRIBE
std::string MessageManager::builRequestStartSubcribe(std::list<std::string> toSubscribe) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::START_SUBSCRIBE));
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
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::STOP_SUBSCRIBE));
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
	else if (getMessageType(message) == MessageType::START_SUBSCRIBE ||
		getMessageType(message) == MessageType::STOP_SUBSCRIBE) {
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
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::DEINJECT));
	return rootJSON.serialize();
}

int MessageManager::getDLLInjectedObject(std::string message) {
	picojson::value rootJSON;
	int pid = -1;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::DLL_INJECTED) {
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
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::DLL_INJECTED));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["pid"] = picojson::value(std::to_string(pid));
	return rootJSON.serialize();
}

//INJECT
InjectObject* MessageManager::getInjectObject(std::string message) {
	picojson::value rootJSON;
	InjectObject* object = NULL;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else if (getMessageType(message) == MessageType::INJECT) {
		if (rootJSON.get("data").is<picojson::object>()
			&& rootJSON.get("data").get("pid").is<std::string>()
			&& rootJSON.get("data").get("module").is<std::string>())
		{
			std::string strPID = rootJSON.get("data").get("pid").to_str();
			std::string strModule = rootJSON.get("data").get("module").to_str();

			int pid = std::stoi(strPID);

			object = new InjectObject(pid, strModule);
			return object;
		}
	}
	return object;
}

std::string MessageManager::builRequestdInjectMessage(int pid, std::string module) {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::INJECT));
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
	else if (getMessageType(message) == MessageType::AVAILABLE_CONFIGURATION) {
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
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::AVAILABLE_CONFIGURATION));
	rootJSON.get<picojson::object>()["data"] = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["pids"] = picojson::value(pids);
	rootJSON.get<picojson::object>()["data"].get<picojson::object>()["modules"] = picojson::value(modules);

	return rootJSON.serialize();
}

std::string MessageManager::builRequestdAvailableConfigationMessage() {
	picojson::value rootJSON = picojson::value(picojson::object());
	rootJSON.get<picojson::object>()[getMessageTypeString(MessageType::MESSAGE_TYPE)] = picojson::value(getMessageTypeString(MessageType::AVAILABLE_CONFIGURATION));
	return rootJSON.serialize();
}


//BASE OBJECT

MessageType MessageManager::getMessageType(std::string message) {
	picojson::value rootJSON;
	std::string err = picojson::parse(rootJSON, message);
	if (!err.empty()) {
		std::cout << err << std::endl;
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
		return MessageType::AVAILABLE_CONFIGURATION;
	}
	else if (type == "INJECT")
	{
		return MessageType::INJECT;
	}
	else if (type == "DEINJECT")
	{
		return MessageType::DEINJECT;
	}
	else if (type == "DLL_INJECTED")
	{
		return MessageType::DLL_INJECTED;
	}
	else if (type == "INFO")
	{
		return MessageType::INFO;
	}
	else if (type == "START_SUBSCRIBE")
	{
		return MessageType::START_SUBSCRIBE;
	}
	else if (type == "STOP_SUBSCRIBE")
	{
		return MessageType::STOP_SUBSCRIBE;
	}
	else if (type == "START_BOT")
	{
		return MessageType::START_BOT;
	}
	else if (type == "STOP_BOT")
	{
		return MessageType::STOP_BOT;
	}
	else if (type == "WAYPOINTS")
	{
		return MessageType::WAYPOINTS;
	}
	else {
		std::cout << "Unknown type..." << std::endl;
		return MessageType::UNKNOWN;
	}
}

std::string MessageManager::getMessageTypeString(MessageType type) {
	switch (type)
	{
	case MessageType::UNKNOWN: { return("UNKNOWN"); } break;
	case MessageType::MESSAGE_TYPE: { return("MESSAGE_TYPE"); } break;
	case MessageType::AVAILABLE_CONFIGURATION: { return("AVAILABLE_CONFIGURATION"); } break;
	case MessageType::INJECT: { return ("INJECT"); } break;
	case MessageType::DEINJECT: { return ("DEINJECT"); } break;
	case MessageType::DLL_INJECTED: { return ("DLL_INJECTED"); } break;
	case MessageType::INFO: { return ("INFO"); } break;
	case MessageType::START_SUBSCRIBE: { return ("START_SUBSCRIBE"); } break;
	case MessageType::STOP_SUBSCRIBE: { return ("STOP_SUBSCRIBE"); } break;
	case MessageType::START_BOT: { return ("START_BOT"); } break;
	case MessageType::STOP_BOT: { return ("STOP_BOT"); } break;
	case MessageType::WAYPOINTS: { return ("WAYPOINTS"); } break;
	default:
		std::cout << "Unknown type..." << std::endl;
		return NULL;
		break;
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
