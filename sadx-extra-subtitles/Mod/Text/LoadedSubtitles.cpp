#include "pch.h"
#include "LoadedSubtitles.h"
#include "Json.h"
#include "Mod/Messages/Message.h"


void LoadedSubtitles::Load(const char* language, int codepage)
{
	try
	{
		Main = Json::ReadExtraSubs(language, "Main", codepage);
		SE = Json::ReadExtraSubs(language, "SE", codepage);
		SkyChase1 = Json::ReadArray(language, "SkyChase1", codepage);
		SkyChase2 = Json::ReadArray(language, "SkyChase2", codepage);
		TwinklePark = Json::ReadArray(language, "TwinklePark", codepage);

		Message::Success(language);
	}
	catch (std::exception& ex)
	{
		Message::Error(language);
	}
}

void LoadedSubtitles::LoadRetranslated(const char* language, int codepage)
{
	try
	{
		MainRetranslated = Json::ReadExtraSubs(language, "Main", codepage);
		SkyChase1Retranslated = Json::ReadArray(language, "SkyChase1", codepage);
		SkyChase2Retranslated = Json::ReadArray(language, "SkyChase2", codepage);

		Message::Success(language);
	}
	catch (std::exception& ex)
	{
		Message::Error(language);
	}
}