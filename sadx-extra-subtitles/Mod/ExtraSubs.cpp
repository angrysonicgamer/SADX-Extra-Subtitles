#include "pch.h"
#include "Config.h"
#include "ExtraSubs.h"
#include "Json.h"
#include "TextConv.hpp"
#include "FunctionHook.h"


FunctionHook<int, int, void*, int, void*> PlaySound_Hook(0x423D70);

const char* Buffer[] = { nullptr, nullptr };
const char* TextBuffer = nullptr;
int SubtitleDisplayFrameCount = 0;
int SubtitleDuration = 0;
int EggCannonFrameCount = 0;


// English

static std::map<int, SubtitleData> ExtraSubs_English;
static std::map<int, SubtitleData> ExtraSubs_SE_English;
static std::vector<const char*> SkyChase1_English;
static std::vector<const char*> SkyChase2_English;
static std::vector<const char*> WelcomeToTwinklePark_English;

static std::map<int, SubtitleData> ExtraSubs_English_Retranslated;
static std::vector<const char*> SkyChase1_English_Retranslated;
static std::vector<const char*> SkyChase2_English_Retranslated;

// French

static std::map<int, SubtitleData> ExtraSubs_French;
static std::map<int, SubtitleData> ExtraSubs_SE_French;
static std::vector<const char*> SkyChase1_French;
static std::vector<const char*> SkyChase2_French;
static std::vector<const char*> WelcomeToTwinklePark_French;

static std::map<int, SubtitleData> ExtraSubs_French_Retranslated;
static std::vector<const char*> SkyChase1_French_Retranslated;
static std::vector<const char*> SkyChase2_French_Retranslated;

// Japanese

static std::map<int, SubtitleData> ExtraSubs_Japanese;
static std::map<int, SubtitleData> ExtraSubs_SE_Japanese;
static std::vector<const char*> SkyChase1_Japanese;
static std::vector<const char*> SkyChase2_Japanese;
static std::vector<const char*> WelcomeToTwinklePark_Japanese;


// Subtitle data

static std::vector<const char*>* SkyChase1[]
{
	&SkyChase1_Japanese,
	&SkyChase1_English,
	&SkyChase1_French,
	nullptr,	// Spanish
	nullptr		// German
};

static std::vector<const char*>* SkyChase2[]
{
	&SkyChase2_Japanese,
	&SkyChase2_English,
	&SkyChase2_French,
	nullptr,	// Spanish
	nullptr		// German
};

static std::vector<const char*>* WelcomeToTwinkleParkCutscene[]
{
	&WelcomeToTwinklePark_Japanese,
	&WelcomeToTwinklePark_English,
	&WelcomeToTwinklePark_French,
	nullptr,	// Spanish
	nullptr		// German
};

static std::map<int, SubtitleData>* ExtraSubs[]
{
	&ExtraSubs_Japanese,
	&ExtraSubs_English,
	&ExtraSubs_French,
	nullptr,	// Spanish
	nullptr		// German
};

static std::map<int, SubtitleData>* ExtraSubs_SE[]
{
	&ExtraSubs_SE_Japanese,
	&ExtraSubs_SE_English,
	&ExtraSubs_SE_French,
	nullptr,	// Spanish
	nullptr		// German
};


// Subtitles for normal voices

void DisplayGameplaySubtitle(int id)
{
	Buffer[0] = ExtraSubs[TextLanguage]->at(id).Text;
	DisplayHintText(Buffer, ExtraSubs[TextLanguage]->at(id).Duration);
}

void SetUpMenuSubtitle(int id)
{
	TextBuffer = ExtraSubs[TextLanguage]->at(id).Text;
	SubtitleDisplayFrameCount = 1;
	SubtitleDuration = ExtraSubs[TextLanguage]->at(id).Duration;
}

void DisplayCutsceneSubtitle(int id) // for post-Egg Walker cutscene specifically
{
	if (VoiceLanguage == Languages_English && (id == 822 || id == 824)) return; // voice language can only be English or Japanese
	
	EV_Msg(ExtraSubs[TextLanguage]->at(id).Text);

	if (id == 823)
	{
		EV_Wait(ExtraSubs[TextLanguage]->at(id).Duration);
		EV_MsgClose();
	}
}

void SetUpSkyChase1Subtitles()
{
	if (SkyChase1[TextLanguage] != nullptr)
	{
		EggCannonFrameCount = 1;
	}
}

void DisplaySkyChase2Subtitles()
{
	if (SkyChase2[TextLanguage] != nullptr)
	{
		DisplayHintText(SkyChase2[TextLanguage]->data(), 270);
	}
}

void SetSubtitlesMode() // this will be a single option for multiple languages
{
	if (TextLanguage == Languages_Japanese) return; // this obviously doesn't have a retranslated version

	bool useRetranslatedSubs = Config::SubtitlesMode == "AlwaysRetranslated" || Config::SubtitlesMode == "Auto" && VoiceLanguage == Languages_Japanese;

	ExtraSubs[Languages_English] = useRetranslatedSubs ? &ExtraSubs_English_Retranslated : &ExtraSubs_English;
	SkyChase1[Languages_English] = useRetranslatedSubs ? &SkyChase1_English_Retranslated : &SkyChase1_English;
	SkyChase2[Languages_English] = useRetranslatedSubs ? &SkyChase2_English_Retranslated : &SkyChase2_English;

	ExtraSubs[Languages_French] = useRetranslatedSubs ? &ExtraSubs_French_Retranslated : &ExtraSubs_French;
	SkyChase1[Languages_French] = useRetranslatedSubs ? &SkyChase1_French_Retranslated : &SkyChase1_French;
	SkyChase2[Languages_French] = useRetranslatedSubs ? &SkyChase2_French_Retranslated : &SkyChase2_French;
}


void DisplaySubtitle(int id)
{
	SetSubtitlesMode();

	if (id == 187) // Sky Chase 1 Egg Cannon sequence
	{
		SetUpSkyChase1Subtitles();
	}
	if (id == 2025) // Sky Chase 2 Tornado transformation sequence
	{
		DisplaySkyChase2Subtitles();
	}

	if (id == 1575 && CurrentCutsceneID == 20) // for cutscene after Twinkle Park (Sonic) to prevent this subtitle overriding a cutscene one
	{
		if (WelcomeToTwinkleParkCutscene[TextLanguage] != nullptr)
		{
			DisplayHintText(WelcomeToTwinkleParkCutscene[TextLanguage]->data(), 30);
		}		
		return;
	}

	if (ExtraSubs[TextLanguage] == nullptr) return;
	if (!ExtraSubs[TextLanguage]->count(id)) return;
	
	if (ExtraSubs[TextLanguage]->at(id).Condition == Menu)
	{
		if (!Config::DisableMenuExtraSubs)
		{
			SetUpMenuSubtitle(id);
		}		
	}
	else if (ExtraSubs[TextLanguage]->at(id).Condition == Cutscene)
	{
		DisplayCutsceneSubtitle(id);
	}
	else
	{
		DisplayGameplaySubtitle(id);
	}
}


void __cdecl PlayVoice_ExtraSub(int id)
{
	if (VoicesEnabled)
	{
		CurrentVoiceNumber = id;
	}

	DisplaySubtitle(id);
}


// Sound effect subtitles

void DisplaySoundEffectSubtitle(int id)
{
	if (ExtraSubs_SE[TextLanguage] == nullptr) return;
	if (!ExtraSubs_SE[TextLanguage]->count(id)) return;
	
	Buffer[0] = ExtraSubs_SE[TextLanguage]->at(id).Text;
	DisplayHintText(Buffer, ExtraSubs_SE[TextLanguage]->at(id).Duration);
}


int __cdecl PlaySound_ExtraSub(int id, void* a2, int a3, void* a4)
{
	if (Config::DisplaySESubtitles)
	{
		DisplaySoundEffectSubtitle(id);
	}
	
	return PlaySound_Hook.Original(id, a2, a3, a4);
}


// Loading text from json files

void LoadText(const char* modPath)
{
	ExtraSubs_English = Json::ReadExtraSubs(modPath, "English", "Main", Latin);
	ExtraSubs_SE_English = Json::ReadExtraSubs(modPath, "English", "SE", Latin);
	SkyChase1_English = Json::ReadArray(modPath, "English", "SkyChase1", Latin);
	SkyChase2_English = Json::ReadArray(modPath, "English", "SkyChase2", Latin);
	WelcomeToTwinklePark_English = Json::ReadArray(modPath, "English", "TwinklePark", Latin);

	ExtraSubs_English_Retranslated = Json::ReadExtraSubs(modPath, "English (Retranslated)", "Main", Latin);
	SkyChase1_English_Retranslated = Json::ReadArray(modPath, "English (Retranslated)", "SkyChase1", Latin);
	SkyChase2_English_Retranslated = Json::ReadArray(modPath, "English (Retranslated)", "SkyChase2", Latin);

	ExtraSubs_French = Json::ReadExtraSubs(modPath, "French", "Main", Latin);
	ExtraSubs_SE_French = Json::ReadExtraSubs(modPath, "French", "SE", Latin);
	SkyChase1_French = Json::ReadArray(modPath, "French", "SkyChase1", Latin);
	SkyChase2_French = Json::ReadArray(modPath, "French", "SkyChase2", Latin);
	WelcomeToTwinklePark_French = Json::ReadArray(modPath, "French", "TwinklePark", Latin);

	ExtraSubs_French_Retranslated = Json::ReadExtraSubs(modPath, "French (Retranslated)", "Main", Latin);
	SkyChase1_French_Retranslated = Json::ReadArray(modPath, "French (Retranslated)", "SkyChase1", Latin);
	SkyChase2_French_Retranslated = Json::ReadArray(modPath, "French (Retranslated)", "SkyChase2", Latin);

	ExtraSubs_Japanese = Json::ReadExtraSubs(modPath, "Japanese", "Main", Japanese);
	ExtraSubs_SE_Japanese = Json::ReadExtraSubs(modPath, "Japanese", "SE", Japanese);
	SkyChase1_Japanese = Json::ReadArray(modPath, "Japanese", "SkyChase1", Japanese);
	SkyChase2_Japanese = Json::ReadArray(modPath, "Japanese", "SkyChase2", Japanese);
	WelcomeToTwinklePark_Japanese = Json::ReadArray(modPath, "Japanese", "TwinklePark", Japanese);

	PrintDebug("[SADX Extra Subtitles] Text has been successfully loaded from json files.\n");
}


void ExtraSubs::Init(const char* modPath)
{
	LoadText(modPath);

	WriteJump((void*)0x425710, PlayVoice_ExtraSub);
	PlaySound_Hook.Hook(PlaySound_ExtraSub);

	WriteData((char*)0x40BC9A, (char)52);		// changing the text box height for menu screens, so two lines would fit properly
	WriteData((int*)0x40BCA1, 384);				// changing the y coordinate of the text box to match menu and gameplay display methods
}


// OnFrame stuff

void DisplaySubtitleForOneFrame()
{
	DialogJimakuInit();
	DialogJimakuPut(TextBuffer);
	SubtitleDisplayFrameCount++;
}

void ClearSubtitle()
{
	SubtitleDisplayFrameCount = 0;
	EggCannonFrameCount = 0;
	SubtitleDuration = 0;
}

void DisplayEggCannonSubtitles()
{
	DialogJimakuInit();
	if (EggCannonFrameCount <= 180)
	{
		DialogJimakuPut(SkyChase1[TextLanguage]->at(0));
	}
	else if (EggCannonFrameCount <= 270)
	{
		DialogJimakuPut(SkyChase1[TextLanguage]->at(1));
	}
	else if (EggCannonFrameCount > 360 && EggCannonFrameCount <= 480)
	{
		DialogJimakuPut(SkyChase1[TextLanguage]->at(2));
	}
	else if (EggCannonFrameCount > 660 && EggCannonFrameCount <= 780)
	{
		DialogJimakuPut(SkyChase1[TextLanguage]->at(3));
	}
	else if (EggCannonFrameCount > 780)
	{
		DialogJimakuPut(SkyChase1[TextLanguage]->at(4));
	}
	EggCannonFrameCount++;
}


void ExtraSubs::OnFrame()
{
	if (SubtitleDisplayFrameCount > 0 && SubtitleDisplayFrameCount <= SubtitleDuration)
	{
		DisplaySubtitleForOneFrame();
	}

	if (EggCannonFrameCount > 0)
	{
		DisplayEggCannonSubtitles();
	}
	
	if (SubtitleDisplayFrameCount > SubtitleDuration || EggCannonFrameCount > 960)
	{
		ClearSubtitle();
	}	
}
