#pragma once

enum Codepage
{
	Latin = 1252,
	Japanese = 932
};

enum DisplayConditions
{
	Menu,
	Gameplay,
	Cutscene
};

struct SubtitleData
{
	const char* Text;
	int Duration;
	DisplayConditions Condition;
};

class ExtraSubs
{
public:
	static void Init(const char* modPath);
	static void OnFrame();
};