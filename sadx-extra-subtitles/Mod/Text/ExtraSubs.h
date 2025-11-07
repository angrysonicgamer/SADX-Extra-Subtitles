#pragma once

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
	static void Init();
	static void OnFrame();
};