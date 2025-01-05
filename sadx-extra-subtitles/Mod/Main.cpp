#include "pch.h"
#include "ExtraSubs.h"
#include "Config.h"


extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* modPath, const HelperFunctions& helperFunctions)
	{
		Config::Read(modPath);
		ExtraSubs::Init(modPath);
	}

	__declspec(dllexport) void OnFrame()
	{
		ExtraSubs::OnFrame();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}