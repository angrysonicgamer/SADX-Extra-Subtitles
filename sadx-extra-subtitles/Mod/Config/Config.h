#pragma once

class Config
{
public:
	static std::string SubtitlesMode;	
	static bool DisableMenuExtraSubs;	
	static bool DisplaySESubtitles;

	static void Read();
};
