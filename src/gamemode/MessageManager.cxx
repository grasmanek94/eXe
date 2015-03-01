/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of Announcement, Message and PlayerMessage 
			system
	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies
		Extension
		Language
		LanguageTextDraw

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

//imma gonna do sum' swap magic in here for efficiency, don't puke your brain out --Gamer--
struct SGlobalAnnouncementData
{
	std::array<std::array<std::string, 2>, LANGUAGES_SIZE> Lines;
	unsigned long long LastMessage;
	bool UpperIsShifed;
	bool StringStateA;
	bool StringStateB;
	size_t CustomMessageCount;

	SGlobalAnnouncementData()
	{
		UpperIsShifed = true;
		StringStateA = true;
		StringStateB = false;
		LastMessage = 0;
		CustomMessageCount = 0;

	}

	std::string Combine(size_t language_id)
	{
		return Lines[language_id][StringStateA] + "~n~" + Lines[language_id][StringStateB];
	}

	void PrepareNewMessage(unsigned long long CurrentTime)
	{
		LastMessage = CurrentTime;

		CustomMessageCount = 2;
		
		if (!UpperIsShifed)
		{
			StringStateA ^= 1;
			StringStateB ^= 1;
		}

	}

	void Shuffle(unsigned long long CurrentTime)
	{
		if (CustomMessageCount)
		{
			UpperIsShifed = true;

			LastMessage = CurrentTime;

			StringStateA ^= 1;
			StringStateB ^= 1;

			for (size_t language_id = 0; language_id < LANGUAGES_SIZE; ++language_id)
				Lines[language_id][StringStateA].assign("_");
			--CustomMessageCount;
		}
	}

	/*
		!!!!!!!!!!!!!
		!!IMPORTANT!! 
		!!!!!!!!!!!!!
		>>>>>>>>>>>>>use PrepareNewMessage before this function
		!!!!!!!!!!!!!
		!!IMPORTANT!! 
		!!!!!!!!!!!!!
	*/
	void AddMessage(size_t language_id, const std::string& message)
	{
		Lines[language_id][StringStateA].assign(message);
		UpperIsShifed = false;
	}

	void AddMessage(const std::string& message, unsigned long long CurrentTime)
	{
		PrepareNewMessage(CurrentTime);
		for (size_t language_id = 0; language_id < LANGUAGES_SIZE; ++language_id)
			Lines[language_id][StringStateA].assign(message);
		UpperIsShifed = false;
	}

	void AddMessage(language_string_ids message, unsigned long long CurrentTime)
	{
		PrepareNewMessage(CurrentTime);
		for (size_t language_id = 0; language_id < LANGUAGES_SIZE; ++language_id)
			Lines[language_id][StringStateA].assign(Translate(language_id, message));
		UpperIsShifed = false;
	}
};

std::array<int, MAX_PLAYERS> PlayerMessage;
std::array<int, MAX_PLAYERS> PlayerColorState;

CLanguageTextDraw GlobalServerMessage;
CLanguageTextDraw GlobalAnnouncementMessage;
SGlobalAnnouncementData GlobalAnnouncementData;

int GlobalServerMessageColorState = 1;

static const unsigned long s_color_state[2] = { 0x19E0D3AA, 0x41E019AA };

void CleanGlobalAnnouncementMessage(int timerid, void* param)
{
	unsigned long long TimeNow = Functions::GetTime();
	if (TimeNow - GlobalAnnouncementData.LastMessage > 4000)
	{
		GlobalAnnouncementData.Shuffle(TimeNow);
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
			if (PlayersUsingLanguage[i].size())
				GlobalAnnouncementMessage.SetString(i, GlobalAnnouncementData.Combine(i));
	}
}

class MessageManager : public Extension::Base
{
public:
	MessageManager() : Base() {}
	bool OnGameModeInit()
	{
		GlobalServerMessage.Create(638.000000, 438.000000, L_empty_underscore, 3, 255, 1, 0.18, 0.90, -1, 1, 0, 0, 0);
		GlobalAnnouncementMessage.Create(320.000000, 103.000000, L_empty_underscore, 2, 255, 1, 0.210000, 1.100000, -1, 1, 0, 0, 0);
		sampgdk_SetTimerEx(2000, true, CleanGlobalAnnouncementMessage, nullptr, nullptr);
		return true;
	}
	bool OnPlayerConnect(int playerid)
	{
		PlayerMessage[playerid] = CreatePlayerTextDraw(playerid, 638.000000, 429.000000, "_");
		PlayerTextDrawAlignment(playerid, PlayerMessage[playerid], 3);
		PlayerTextDrawBackgroundColor(playerid, PlayerMessage[playerid], 255);
		PlayerTextDrawFont(playerid, PlayerMessage[playerid], 1);
		PlayerTextDrawLetterSize(playerid, PlayerMessage[playerid], 0.180000, 0.899999);
		PlayerTextDrawColor(playerid, PlayerMessage[playerid], -1);
		PlayerTextDrawSetOutline(playerid, PlayerMessage[playerid], 0);
		PlayerTextDrawSetProportional(playerid, PlayerMessage[playerid], 1);
		PlayerTextDrawSetShadow(playerid, PlayerMessage[playerid], 1);
		PlayerTextDrawShow(playerid, PlayerMessage[playerid]);
		GlobalServerMessage.ShowForPlayer(playerid);
		GlobalAnnouncementMessage.ShowForPlayer(playerid);
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		GlobalServerMessage.HideForPlayer(playerid);
		GlobalAnnouncementMessage.HideForPlayer(playerid);
		return true;
	}
} _MessageManager;

//        .==.        .==.          
//       //`^\\      //^`\\         
//      // ^ ^\(\__/)/^ ^^\\        
//     //^ ^^ ^/6  6\ ^^ ^ \\       
//    //^ ^^ ^/( .. )\^ ^ ^ \\      
//   // ^^ ^/\| v""v |/\^ ^ ^\\     
//  // ^^/\/ /  `~~`  \ \/\^ ^\\    
//  -----------------------------
/// HERE BE DRAGONS

namespace Message
{
	//language critical anti bug stuff
	void RefreshTextDrawsOnLanguageChange(int playerid, bool alreadyset)
	{
		if (!alreadyset)
		{
			GlobalAnnouncementMessage.HideForPlayer(playerid);
		}
		else
		{
			GlobalAnnouncementMessage.ShowForPlayer(playerid);
		}
	}

	//announcement
	void AddAnnouncementMessage(std::string message)
	{
		GlobalAnnouncementData.AddMessage(message, Functions::GetTime());
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
			if (PlayersUsingLanguage[i].size())
				GlobalAnnouncementMessage.SetString(i, GlobalAnnouncementData.Combine(i));
	}

	void AddAnnouncementMessage(language_string_ids string_id)
	{
		GlobalAnnouncementData.AddMessage(string_id, Functions::GetTime());
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
			if (PlayersUsingLanguage[i].size())
				GlobalAnnouncementMessage.SetString(i, GlobalAnnouncementData.Combine(i));
	}

	void AddAnnouncementMessage2(language_string_ids string_id, language_string_ids string_id_2)
	{
		GlobalAnnouncementData.PrepareNewMessage(Functions::GetTime());
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
		{
			if (PlayersUsingLanguage[i].size())
			{
				GlobalAnnouncementData.AddMessage(i, Translate(i, string_id) + Translate(i, string_id_2));
				GlobalAnnouncementMessage.SetString(i, GlobalAnnouncementData.Combine(i));
			}
		}
	}

	void AddAnnouncementMessageF(language_string_ids string_id, ...)
	{
		GlobalAnnouncementData.PrepareNewMessage(Functions::GetTime());
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
		{
			if (PlayersUsingLanguage[i].size())
			{
				std::string fmt(Translate(i, string_id));
				{
					int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
					std::string str;
					std::unique_ptr<char[]> formatted;
					va_list ap;
					while (1) {
						formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
						strcpy_s(&formatted[0], n, fmt.c_str());
						va_start(ap, string_id);
						final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
						va_end(ap);
						if (final_n < 0 || final_n >= n)
							n += abs(final_n - n + 1);
						else
							break;
					}
					fmt.assign(formatted.get());
				}
				GlobalAnnouncementData.AddMessage(i, fmt);
				GlobalAnnouncementMessage.SetString(i, GlobalAnnouncementData.Combine(i));
			}
		}
	}
};