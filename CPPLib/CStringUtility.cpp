#include "pch.h"
#include <sstream>
#include "StringUtility.h"

std::vector<std::string> CStringUtility::SplitString(const std::string& input, const char& separator)
{
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;

	while (std::getline(ss, token, separator)) {
		tokens.push_back(token);
	}

	return tokens;
}

void CStringUtility::StringReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
	if (from.empty())
		return;

	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}