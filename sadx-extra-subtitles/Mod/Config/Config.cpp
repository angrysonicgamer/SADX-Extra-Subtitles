#include "pch.h"
#include "Config.h"
#include "Mod/Global/MyMod.h"
#include "IniFile.hpp"


std::string Config::SubtitlesMode;
bool Config::DisableMenuExtraSubs;
bool Config::DisplaySESubtitles;


void Config::Read()
{
	IniFile config(MyMod::Path + "\\config.ini");

	SubtitlesMode = config.getString("Options", "SubtitlesMode", "Auto");
	DisableMenuExtraSubs = config.getBool("Options", "DisableMenuExtraSubs", false);
	DisplaySESubtitles = config.getBool("Options", "DisplaySESubtitles", false);
}