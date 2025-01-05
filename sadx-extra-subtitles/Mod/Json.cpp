#include "pch.h"
#include "ExtraSubs.h"
#include "Json.h"
#include "TextConv.hpp"
#include <fstream>


const char* ConvertToCodepage(std::string& text, Codepage codepage)
{
	return UTF8toCodepage(("\a" + text).c_str(), (int)codepage);
}

json ReadJsonFile(const char* modPath, const char* language, const char* type)
{
	char path[MAX_PATH];
	sprintf(path, "%s\\Languages\\%s\\%s.json", modPath, language, type);

	std::ifstream jsonFile(path);
	return json::parse(jsonFile);
}

std::map<int, SubtitleData> Json::ReadExtraSubs(const char* modPath, const char* language, const char* type, Codepage codepage)
{
	std::map<std::string, DisplayConditions> displayModes
	{
		{ "Menu", Menu },
		{ "Gameplay", Gameplay },
		{ "Cutscene", Cutscene }
	};	

	json j = ReadJsonFile(modPath, language, type);
	std::map<int, SubtitleData> extraSubs;

	for (auto& subtitleArray : j["Subtitles"])
	{
		for (auto& value : subtitleArray)
		{
			for (auto& subtitle : value)
			{
				int voiceID = subtitle["VoiceID"];
				std::string text = subtitle["Text"];
				int duration = subtitle["Duration"];
				std::string mode = subtitle["Mode"];

				extraSubs.insert({ voiceID, { ConvertToCodepage(text, codepage), duration, displayModes[mode]} });
			}
		}
	}

	return extraSubs;
}

std::vector<const char*> Json::ReadArray(const char* modPath, const char* language, const char* key, Codepage codepage)
{
	json j = ReadJsonFile(modPath, language, "Other");
	std::vector<const char*> subtitleArray;

	for (auto& subtitle : j[key])
	{
		std::string text = subtitle;

		if (text == "NULL")
		{
			subtitleArray.push_back(NULL);
		}
		else
		{
			subtitleArray.push_back(ConvertToCodepage(text, codepage));
		}		
	}

	return subtitleArray;
}