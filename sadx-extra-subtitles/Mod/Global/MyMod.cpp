#include "pch.h"
#include "MyMod.h"


const std::string MyMod::Name = "SADX Extra Subtitles";
std::string MyMod::Path;
HelperFunctions MyMod::Helper;

void MyMod::Init(const char* modPath, const HelperFunctions& helperFunctions)
{
	Path = modPath;
	Helper = helperFunctions;
}