#include "pch.h"
#include "Utils.h"

std::vector<std::string> splitByDelimiter(const std::string input, const std::string delimiter) {
	std::string copyInput(input);
	size_t pos = 0;
	std::string token;
	std::vector<std::string> resultList;
	while ((pos = copyInput.find(delimiter)) != std::string::npos) {
		token = copyInput.substr(0, pos);
		resultList.push_back(token);
		copyInput.erase(0, pos + delimiter.length());
	}
	resultList.push_back(copyInput);
	return resultList;
}