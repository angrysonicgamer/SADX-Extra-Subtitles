#include "pch.h"
#include "DebugMessage.h"
#include "Mod/Global/MyMod.h"


void DebugMessage::Success(const char* language)
{
	PrintDebug("%s subtitles have been loaded.\n", language);
}

void DebugMessage::Error(const char* language)
{
	PrintDebug("Error loading %s subtitles. There might be something wrong with the files.\n", language);
}

void DebugMessage::Loading()
{
	PrintDebug("\n[%s] Loading text from json files...\n", MyMod::Name.c_str());
}

void DebugMessage::Finished()
{
	PrintDebug("[%s] Finished loading text.\n\n", MyMod::Name.c_str());
}