/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the mini-statistics TextDraw (top bar)

	________________________________________________________________	
	Notes
		-	This extension manages the information and the associated
			TextDraw which displays the players location, the www address,
			his/her id, killd, deaths, k/d, vehicle and mafia
		-	This extension knows when to call the textdraw update efficiently,
			it should not be needed to call the update ouside this source file

	________________________________________________________________
	Dependencies
		Extension
		WorldData

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

//{BLUE}ID: {GREEN}NICK {RED}K D K/D {YELLOW}MAFIA {WHITE}LOCATION
//const std::string main_stats_format("~b~~h~~h~[%d]~g~~h~~h~%-34s~r~~h~K: %-5I64u  D: %-5I64u  K/D: %.3f  ~y~~h~MAFIA: %-24s  ~w~%62s  ~g~%25s_");
const std::string main_stats_format("~b~~h~~h~[%d]~g~~h~~h~%-34s~r~~h~K: %-5I64u  D: %-5I64u  K/D: %.3f  ~y~~h~MAFIA: %-24s~g~%91s_");
const std::string respect_stats_format("RESPECT%8I64d");
const std::string exp_stats_format("LVL%4u%6I64u/%I64u XP");

class PlayerStatsTextDrawProcessor : public Extension::Base
{
public:
	PlayerStatsTextDrawProcessor() : Base(-1) {}

	int GlobalBackgroundTextDraw;

	bool OnGameModeInit() override
	{
		GlobalBackgroundTextDraw = TextDrawCreate(0.000000, 439.000000, "BACKGROUND_BOX");
		TextDrawBackgroundColor(GlobalBackgroundTextDraw, 255);
		TextDrawFont(GlobalBackgroundTextDraw, 1);
		TextDrawLetterSize(GlobalBackgroundTextDraw, 0.000000, 3.099998);
		TextDrawColor(GlobalBackgroundTextDraw, -1);
		TextDrawSetOutline(GlobalBackgroundTextDraw, 0);
		TextDrawSetProportional(GlobalBackgroundTextDraw, 1);
		TextDrawSetShadow(GlobalBackgroundTextDraw, 1);
		TextDrawUseBox(GlobalBackgroundTextDraw, 1);
		TextDrawBoxColor(GlobalBackgroundTextDraw, 102);
		TextDrawTextSize(GlobalBackgroundTextDraw, 640.000000, 0.000000);
		TextDrawSetSelectable(GlobalBackgroundTextDraw, 0);
		return true;
	}
	bool OnPlayerConnect(int playerid) override
	{
		Player[playerid].StatsTextDraw[eSTDI_Main] = PlayerTextDrawCreate(playerid, 0.000000, 438.000000, "_");
		PlayerTextDrawBackgroundColor(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], 255);
		PlayerTextDrawFont(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], 1);
		PlayerTextDrawLetterSize(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], 0.179998, 0.899999);
		PlayerTextDrawColor(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], -1);
		PlayerTextDrawSetOutline(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], 0);
		PlayerTextDrawSetProportional(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], 1);
		PlayerTextDrawSetShadow(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], 1);
		PlayerTextDrawSetSelectable(playerid, Player[playerid].StatsTextDraw[eSTDI_Main], 0);

		Player[playerid].StatsTextDraw[eSTDI_Respect] = PlayerTextDrawCreate(playerid, 553.000000, 97.000000, "RESPECT      0");
		PlayerTextDrawAlignment(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], 2);
		PlayerTextDrawBackgroundColor(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], 255);
		PlayerTextDrawFont(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], 3);
		PlayerTextDrawLetterSize(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], 0.389999, 1.500000);
		PlayerTextDrawColor(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], -1676273153);
		PlayerTextDrawSetOutline(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], 1);
		PlayerTextDrawSetProportional(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], 1);
		PlayerTextDrawSetSelectable(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], 0);

		Player[playerid].StatsTextDraw[eSTDI_Exp] = PlayerTextDrawCreate(playerid, 553.000000, 111.000000, "LVL   0     0/0 XP");
		PlayerTextDrawAlignment(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 2);
		PlayerTextDrawBackgroundColor(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 255);
		PlayerTextDrawFont(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 3);
		PlayerTextDrawLetterSize(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 0.260000, 1.099999);
		PlayerTextDrawColor(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 7697919);
		PlayerTextDrawSetOutline(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 1);
		PlayerTextDrawSetProportional(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 1);
		PlayerTextDrawSetSelectable(playerid, Player[playerid].StatsTextDraw[eSTDI_Exp], 0);

		return true;
	}
	void OnPlayerGameBegin(int playerid) override
	{	
		TextDrawShowForPlayer(playerid, GlobalBackgroundTextDraw);

		for (auto textdraw : Player[playerid].StatsTextDraw)
			PlayerTextDrawShow(playerid, textdraw);

		UpdatePlayerStatsTextDraw(playerid);
		UpdatePlayerStatsTextDrawRespect(playerid);
		UpdatePlayerLevelTextDraw(playerid);
	}
	bool OnPlayerDeath(int playerid, int killerid, int reason) override
	{
		UpdatePlayerStatsTextDraw(playerid);
		if (killerid != INVALID_PLAYER_ID)
			UpdatePlayerStatsTextDraw(killerid);
		return true;
	}
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override
	{
		if (newstate == PLAYER_STATE_DRIVER || 
			oldstate == PLAYER_STATE_DRIVER ||
			newstate == PLAYER_STATE_PASSENGER ||
			oldstate == PLAYER_STATE_PASSENGER)
		{
			UpdatePlayerStatsTextDraw(playerid);
		}
		return true;
	}
} _PlayerStatsTextDrawProcessor;

void UpdatePlayerStatsTextDraw(int playerid)
{
	float kd;
	if (Player[playerid].statistics.deaths == 0)
		kd = 0.0;
	else
		kd = (float)Player[playerid].statistics.kills / (float)Player[playerid].statistics.deaths;
	
	PlayerTextDrawSetString(
		playerid, 
		Player[playerid].StatsTextDraw[eSTDI_Main], 
		Functions::string_format(
			main_stats_format,

			playerid,
			Player[playerid].PlayerName.c_str(),
			Player[playerid].statistics.kills,
			Player[playerid].statistics.deaths,
			kd,
			(Player[playerid].Mafia == nullptr) ? "-" : Player[playerid].Mafia->Name.c_str(),
			//("[ " + ZoneNames::GetCurrentZone(playerid) + " ]").c_str(),
			(Player[playerid].CurrentVehicleModel > 0) ? ("[ " + NazwyPojazdow[Player[playerid].CurrentVehicleModel] + " ]").c_str() : "_"
		).c_str()
	);
}

void UpdatePlayerStatsTextDrawRespect(int playerid)
{
	PlayerTextDrawSetString(playerid, Player[playerid].StatsTextDraw[eSTDI_Respect], Functions::string_format(respect_stats_format, Player[playerid].statistics.respect).c_str());
}

void UpdatePlayerLevelTextDraw(int playerid)
{
	double level = Player[playerid].GetLevel();
	double next_level = (double)((unsigned long)level + 1);
	PlayerTextDrawSetString(
		playerid,
		Player[playerid].StatsTextDraw[eSTDI_Exp],
		Functions::string_format(
			exp_stats_format,

			(unsigned long)level,
			Player[playerid].GetExperience(),
			Player[playerid].GetExperience(next_level)
		).c_str()
	);
}