#pragma once

#include <string>
#include <map>
#include <vector>

class CStringUtility
{
public:
	static std::vector<std::string> SplitString(const std::string& input, const char& separator);
	static void StringReplaceAll(std::string& str, const std::string& from, const std::string& to);
};

