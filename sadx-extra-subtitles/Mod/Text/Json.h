#pragma once

#include "nlohmann-json/json.hpp"
#include "ExtraSubs.h"
using json = nlohmann::json;

class Json
{
public:
	static std::map<int, SubtitleData> ReadExtraSubs(const char* language, const char* type, int codepage);
	static std::vector<const char*> ReadArray(const char* language, const char* key, int codepage);
};