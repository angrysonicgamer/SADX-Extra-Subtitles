#include "pch.h"
#include "Mod/Global/MyMod.h"
#include "Mod/Config/Config.h"
#include "Mod/Text/ExtraSubs.h"


extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* modPath, const HelperFunctions& helperFunctions)
	{
		MyMod::Init(modPath, helperFunctions);
		Config::Read();
		ExtraSubs::Init();
	}

	__declspec(dllexport) void OnFrame()
	{
		ExtraSubs::OnFrame();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}