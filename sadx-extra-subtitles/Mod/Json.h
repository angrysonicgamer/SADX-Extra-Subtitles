#pragma once

#include "nlohmann-json/json.hpp"
using json = nlohmann::json;

class Json
{
public:
	static std::map<int, SubtitleData> ReadExtraSubs(const char* modPath, const char* language, const char* type, Codepage codepage);
	static std::vector<const char*> ReadArray(const char* modPath, const char* language, const char* key, Codepage codepage);
};