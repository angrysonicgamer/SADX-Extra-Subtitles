#include "pch.h"
#include "Json.h"
#include "Encoding.h"
#include "Mod/Global/MyMod.h"
#include <fstream>


json ReadJsonFile(const char* language, const char* type)
{
	char path[MAX_PATH];
	sprintf(path, "%s\\Languages\\%s\\%s.json", MyMod::Path.c_str(), language, type);

	std::ifstream jsonFile(path);
	return json::parse(jsonFile);
}

std::map<int, SubtitleData> Json::ReadExtraSubs(const char* language, const char* type, int codepage)
{
	std::map<std::string, DisplayConditions> displayModes
	{
		{ "Menu", Menu },
		{ "Gameplay", Gameplay },
		{ "Cutscene", Cutscene }
	};	

	json j = ReadJsonFile(language, type);
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

				extraSubs.insert({ voiceID, { Encode("\a" + text, codepage), duration, displayModes[mode] } });
			}
		}
	}

	return extraSubs;
}

std::vector<const char*> Json::ReadArray(const char* language, const char* key, int codepage)
{
	json j = ReadJsonFile(language, "Other");
	std::vector<const char*> subtitleArray;

	for (auto& subtitle : j[key])
	{
		std::string text = subtitle;

		if (text == "NULL")
		{
			subtitleArray.push_back(nullptr);
		}
		else
		{
			subtitleArray.push_back(Encode("\a" + text, codepage));
		}		
	}

	return subtitleArray;
}