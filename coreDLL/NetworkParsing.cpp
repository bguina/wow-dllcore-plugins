#include "pch.h"

#include "NetworkParsing.h"

size_t splitByDelimiter(
	const std::string input, 
	const std::string delimiter,
	std::vector<std::string>& result
) {
	std::string copyInput(input);
	size_t pos = 0;
	std::string token;
	std::vector<std::string> resultList;

	while ((pos = copyInput.find(delimiter)) != std::string::npos) {
		token = copyInput.substr(0, pos);
		result.push_back(token);
		copyInput.erase(0, pos + delimiter.length());
	}
	result.push_back(copyInput);
	return result.size();
}
