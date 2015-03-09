/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implement of handling reported cheats by the core AntiCheat

	________________________________________________________________	
	Notes
		-	This extension disconnects players when cheats are suspected

	________________________________________________________________
	Dependencies
		Extension
		ZeroCommand
		AntiCheat
		MessageManager
		AdminSystem
		AchievementManagers

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"
#include <valarray>

template<size_t detections, unsigned long long ms_time_between_detections, unsigned long long ms_oldest_detection> struct DetectionCounter
{
	std::valarray<unsigned long long> bsc_ull;
	size_t maxarr;
	size_t detection_count;

	DetectionCounter()
		: bsc_ull(detections), maxarr(detections - 1)
	{}

	bool dodetect(unsigned long long timenow)
	{
		if ((timenow - bsc_ull[0]) <= ms_oldest_detection)
		{
			++detection_count;
			return true;
		}
		return false;
	}

	bool add_detect(unsigned long long timenow)
	{
		if ((timenow - bsc_ull[maxarr]) >= ms_time_between_detections)
		{
			bsc_ull = bsc_ull.shift(1);
			bsc_ull[maxarr] = timenow;
		}
		return dodetect(timenow);
	}

	void reset()
	{
		for (auto &i : bsc_ull)
			i = 0;
		detection_count = 0;
	}
};

std::array<DetectionCounter<3, 111, 1000>, MAX_PLAYERS> PlayerSpeedCheck;
std::array<DetectionCounter<5, 50, 500>, MAX_PLAYERS> PlayerAirbreakCheck;
std::array<unsigned long long, MAX_PLAYERS> Reported;

unsigned long long TimeNow;

class Resettor : public Extension::Base
{
public:
	bool OnPlayerConnect(int playerid) override
	{
		PlayerSpeedCheck[playerid].reset();
		PlayerAirbreakCheck[playerid].reset();
		return true;
	}
} _Resettor;

bool AntyCheatEnabled = true;
void CheatHandler(int playerid, int type, int extraint, float extrafloat, int extraint2)
{
	if (!AntyCheatEnabled)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, "ANTICHEAT OFF");
		return;
	}
	TimeNow = Functions::GetTime();
	switch (type)
	{
		case CHECK_JETPACK:
		{
			aKick(playerid, 2, false, INVALID_PLAYER_ID, "Jetpack Hack");
			break;
		}
		case CHECK_WEAPON:
		{
			aKick(playerid, 2, false, INVALID_PLAYER_ID, "Weapon Hack");
			break;
		}
		case CHECK_SPEED:
		{
			debug_fixSendClientMessage(playerid, "Speed");
			if (PlayerSpeedCheck[playerid].add_detect(TimeNow))
			{
				aKick(playerid, 2, false, INVALID_PLAYER_ID, "Speed Hack");
			}
			else
			{
				GivePlayerAchievement(playerid, EAM_Czitek, 1);
			}
			break;
		}
		case CHECK_IPFLOOD:
		{
			aKick(playerid, 5, true, INVALID_PLAYER_ID, "IP flood", ":AntiCheat:", true);
			break;
		}
		case CHECK_PING:
		{
			if (Player[playerid].Connected && (Functions::GetTimeSeconds() - Player[playerid].StartPlayTime) > 5)
			{
				aKick(playerid, 2, false, INVALID_PLAYER_ID, "PING > 1000");
			}
			break;
		}
		case CHECK_INACTIVITY:
		{
			aKick(playerid, 2, false, INVALID_PLAYER_ID, "AFK");
			break;
		}
		case CHECK_TELEPORT:
		{
			if (TimeNow - Reported[playerid] < 60000)
			{
				aKick(playerid, 2, false, INVALID_PLAYER_ID, "Teleport Hack");
			}
			Reported[playerid] = TimeNow;
			break;
		}
		case CHECK_AIRBREAK:
		{
			if (PlayerAirbreakCheck[playerid].add_detect(TimeNow))
			{
				aKick(playerid, 2, false, INVALID_PLAYER_ID, "Airbreak Hack");
			}
			else
			{
				GivePlayerAchievement(playerid, EAM_Czitek, 1);
			}
			break;
		}
		case CHECK_FASTCONNECT:
		{
			aKick(playerid, 5, true, INVALID_PLAYER_ID, "IP connect flood", ":AntiCheat:", true);
			break;
		}
		case CHECK_REMOTECONTROL:
		{
			aKick(playerid, 2, false, INVALID_PLAYER_ID, "Remote Control");
			break;
		}
		case CHECK_MASSCARTELEPORT:
		{
			aKick(playerid, 2, false, INVALID_PLAYER_ID, "Mass Vehicle Teleport");
			break;
		}
		case CHECK_CARJACKHACK:
		{
			aKick(playerid, 2, false, INVALID_PLAYER_ID, "Carjack Hack");
			break;
		}
		case CHECK_FLYHACK:
		{
			aKick(playerid, 2, false, INVALID_PLAYER_ID, "Fly Hack");
			break;
		}
	}
}

ZCMD3(ac, PERMISSION_GAMER, RESTRICTION_NONE)
{
	AntyCheatEnabled = !(_stricmp(params.c_str(), "off") == 0);
	return true;
}