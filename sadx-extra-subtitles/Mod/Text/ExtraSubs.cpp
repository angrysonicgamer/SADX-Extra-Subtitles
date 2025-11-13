#include "pch.h"
#include "ExtraSubs.h"
#include "LoadedSubtitles.h"
#include "Encoding.h"
#include "Mod/Global/MyMod.h"
#include "Mod/Config/Config.h"
#include "Mod/Messages/DebugMessage.h"
#include "FunctionHook.h"


FunctionHook<int, int, void*, int, void*> PlaySound_Hook(0x423D70);

const char* Buffer[] = { nullptr, nullptr };
const char* TextBuffer = nullptr;
int SubtitleDisplayFrameCount = 0;
int SubtitleDuration = 0;
int EggCannonFrameCount = 0;


// Subtitle data

static LoadedSubtitles Subtitles[5];

static std::vector<const char*>* SkyChase1[]
{
	&Subtitles[Languages_Japanese].SkyChase1,
	&Subtitles[Languages_English].SkyChase1,
	&Subtitles[Languages_French].SkyChase1,
	&Subtitles[Languages_Spanish].SkyChase1,
	&Subtitles[Languages_German].SkyChase1
};

static std::vector<const char*>* SkyChase2[]
{
	&Subtitles[Languages_Japanese].SkyChase2,
	&Subtitles[Languages_English].SkyChase2,
	&Subtitles[Languages_French].SkyChase2,
	&Subtitles[Languages_Spanish].SkyChase2,
	&Subtitles[Languages_German].SkyChase2
};

static std::vector<const char*>* WelcomeToTwinkleParkCutscene[]
{
	&Subtitles[Languages_Japanese].TwinklePark,
	&Subtitles[Languages_English].TwinklePark,
	&Subtitles[Languages_French].TwinklePark,
	&Subtitles[Languages_Spanish].TwinklePark,
	&Subtitles[Languages_German].TwinklePark
};

static std::map<int, SubtitleData>* MainSet[]
{
	&Subtitles[Languages_Japanese].Main,
	&Subtitles[Languages_English].Main,
	&Subtitles[Languages_French].Main,
	&Subtitles[Languages_Spanish].Main,
	&Subtitles[Languages_German].Main
};

static std::map<int, SubtitleData>* SoundEffectSubs[]
{
	&Subtitles[Languages_Japanese].SE,
	&Subtitles[Languages_English].SE,
	&Subtitles[Languages_French].SE,
	&Subtitles[Languages_Spanish].SE,
	&Subtitles[Languages_German].SE
};


// Subtitles for normal voices

void DisplayGameplaySubtitle(int id)
{
	Buffer[0] = MainSet[TextLanguage]->at(id).Text;
	DisplayHintText(Buffer, MainSet[TextLanguage]->at(id).Duration);
}

void SetUpMenuSubtitle(int id)
{
	TextBuffer = MainSet[TextLanguage]->at(id).Text;
	SubtitleDisplayFrameCount = 1;
	SubtitleDuration = MainSet[TextLanguage]->at(id).Duration;
}

void DisplayCutsceneSubtitle(int id) // for post-Egg Walker cutscene specifically
{
	if (VoiceLanguage == Languages_English && (id == 822 || id == 824)) return; // voice language can only be English or Japanese
	
	EV_Msg(MainSet[TextLanguage]->at(id).Text);

	if (id == 823)
	{
		EV_Wait(MainSet[TextLanguage]->at(id).Duration);
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

	for (int lang = Languages_English; lang <= Languages_German; lang++)
	{
		MainSet[lang] = useRetranslatedSubs && !Subtitles[lang].MainRetranslated.empty() ? &Subtitles[lang].MainRetranslated : &Subtitles[lang].Main;
		SkyChase1[lang] = useRetranslatedSubs && !Subtitles[lang].SkyChase1Retranslated.empty() ? &Subtitles[lang].SkyChase1Retranslated : &Subtitles[lang].SkyChase1;
		SkyChase2[lang] = useRetranslatedSubs && !Subtitles[lang].SkyChase2Retranslated.empty() ? &Subtitles[lang].SkyChase2Retranslated : &Subtitles[lang].SkyChase2;
	}
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

	if (!MainSet[TextLanguage]->count(id)) return;
	
	if (MainSet[TextLanguage]->at(id).Condition == Menu)
	{
		if (!Config::DisableMenuExtraSubs)
		{
			SetUpMenuSubtitle(id);
		}		
	}
	else if (MainSet[TextLanguage]->at(id).Condition == Cutscene)
	{
		DisplayCutsceneSubtitle(id);
	}
	else
	{
		DisplayGameplaySubtitle(id);
	}
}


void PlayVoice_ExtraSub(int id)
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
	if (!SoundEffectSubs[TextLanguage]->count(id)) return;
	
	Buffer[0] = SoundEffectSubs[TextLanguage]->at(id).Text;
	DisplayHintText(Buffer, SoundEffectSubs[TextLanguage]->at(id).Duration);
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

void LoadText()
{
	DebugMessage::Loading();
	
	Subtitles[Languages_Japanese].Load("Japanese", ShiftJIS);
	
	// the game uses Japanese encoding for English text
	Subtitles[Languages_English].Load("English", ShiftJIS);
	Subtitles[Languages_English].LoadRetranslated("English (Retranslated)", ShiftJIS);
	
	Subtitles[Languages_French].Load("French", Windows1252);
	Subtitles[Languages_French].LoadRetranslated("French (Retranslated)", Windows1252);

	// German and Spanish are not currently supported, waiting for contributions
	
	//Subtitles[Languages_Spanish].Load("Spanish", Windows1252);
	//Subtitles[Languages_Spanish].LoadRetranslated("Spanish (Retranslated)", Windows1252);

	//Subtitles[Languages_German].Load("German", Windows1252);
	//Subtitles[Languages_German].LoadRetranslated("German (Retranslated)", Windows1252);

	DebugMessage::Finished();
}


void ExtraSubs::Init()
{
	LoadText();

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
