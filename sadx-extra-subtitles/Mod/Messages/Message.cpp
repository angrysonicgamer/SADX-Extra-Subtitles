#include "pch.h"
#include "Message.h"
#include "Mod/Global/MyMod.h"


void Message::Success(const char* language)
{
	PrintDebug("%s subtitles have been loaded.\n", language);
}

void Message::Error(const char* language)
{
	PrintDebug("Error loading %s subtitles. There might be something wrong with the files.\n", language);
}

void Message::Loading()
{
	PrintDebug("\n[%s] Loading text from json files...\n", MyMod::Name.c_str());
}

void Message::Finished()
{
	PrintDebug("[%s] Finished loading text.\n\n", MyMod::Name.c_str());
}