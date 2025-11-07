#pragma once

#include "ExtraSubs.h"

class LoadedSubtitles
{
public:
	std::map<int, SubtitleData> Main;
	std::map<int, SubtitleData> SE;
	std::vector<const char*> SkyChase1;
	std::vector<const char*> SkyChase2;
	std::vector<const char*> TwinklePark;

	std::map<int, SubtitleData> MainRetranslated;
	std::vector<const char*> SkyChase1Retranslated;
	std::vector<const char*> SkyChase2Retranslated;

	void Load(const char* language, int codepage);
	void LoadRetranslated(const char* language, int codepage);
};