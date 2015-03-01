/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the CountDown System

	________________________________________________________________	
	Notes
		-	

	________________________________________________________________
	Dependencies
		Extension
		WorldData
		Streamer
		ZeroCommand
		MessageManager
		Language
		LanguageTextDraw

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

namespace CountDown
{
	CLanguageTextDraw textdraw_odliczanie;
	int counting_timerid = INVALID_TIMER_ID;
	int current_countdown_count = 0;
	bool counting_down = false;
	void* timer_owner;

	std::map<int, int> CountDownPlayers;

	void StopPlayerCountDown(int playerid)
	{
		auto it = CountDownPlayers.find(playerid);
		if (it != CountDownPlayers.end())
		{
			StreamerLibrary::DestroyDynamic3DTextLabel(it->second);
			if (current_countdown_count < 4)
			{
				fixTogglePlayerControllable(playerid, it->second);
			}
			CountDownPlayers.erase(it);
		}
		if (!CountDownPlayers.size())
		{
			if (counting_timerid != INVALID_TIMER_ID)
			{
				sampgdk_KillTimerEx(counting_timerid, timer_owner);
				counting_timerid = INVALID_TIMER_ID;
			}
			textdraw_odliczanie.Hide();
		}
	}

	class OdliczanieProcessor : public Extension::Base
	{
	public:
		bool OnGameModeInit() override
		{
			timer_owner = this;
			textdraw_odliczanie.Create(2.0, 425.0, L_countdown_counter, 0, 0xFF, 1, 0.19, 1.0, -1, 1, 1, 0, 0);
			return true;
		}
		bool OnPlayerDisconnect(int playerid, int reason) override
		{
			StopPlayerCountDown(playerid);
			return true;
		}
		void OnPlayerEnterMiniGame(int playerid, Zabawy::Base* game) override
		{
			StopPlayerCountDown(playerid);
		}
	} _OdliczanieProcessor;

	void InitPlayerCountDown(int playerid)
	{
		PlayerPlaySound(playerid, 1053, 0.0, 0.0, 0.0);
		fixTogglePlayerControllable(playerid, false);
		float vx, vy, vz;
		if (Player[playerid].CurrentVehicle)
		{
			GetVehiclePos(Player[playerid].CurrentVehicle, &vx, &vy, &vz);
			SetVehiclePos(Player[playerid].CurrentVehicle, vx, vy, vz + 0.03);
		}
		else
		{
			GetPlayerPos(playerid, &vx, &vy, &vz);
			SetPlayerPos(playerid, vx, vy, vz + 0.03);
		}
		GameTextForPlayer(playerid, "3", 750, 6);
	}

	void UpdateCountdownTDs(bool show = false)
	{
		textdraw_odliczanie.SetStringF(L_countdown_counter, current_countdown_count);
		if (show)
		{
			textdraw_odliczanie.Show();
		}
	}

	void CountdownTimer(int timerid, void * param)
	{
		--current_countdown_count;
		UpdateCountdownTDs();

		switch (current_countdown_count)
		{
		case 3:
			for (auto& i : CountDownPlayers)
			{
				InitPlayerCountDown(i.first);
			}
			break;

		case 2:
			for (auto& i : CountDownPlayers)
			{
				PlayerPlaySound(i.first, 1052, 0.0, 0.0, 0.0);
				GameTextForPlayer(i.first, "2", 750, 6);
			}
			break;
		case 1:
			for (auto& i : CountDownPlayers)
			{
				PlayerPlaySound(i.first, 1052, 0.0, 0.0, 0.0);
				GameTextForPlayer(i.first, "1", 750, 6);
			}
			break;
		case 0:
			while (CountDownPlayers.size())
			{
				PlayerPlaySound(CountDownPlayers.begin()->first, 3200, 0.0, 0.0, 0.0);
				fixTogglePlayerControllable(CountDownPlayers.begin()->first, true);
				GameTextForPlayer(CountDownPlayers.begin()->first, "GO!", 750, 6);
				StopPlayerCountDown(CountDownPlayers.begin()->first);
			}
			break;
		}
	}

	ZCMD(odlicz, PERMISSION_NONE, RESTRICTION_NOT_IN_STAGING | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_SPAWNED, cmd_alias({ "/odliczanie", "/cd", "/countdown", "/count" }))
	{
		if (_stricmp(params.c_str(), "stop") == 0)
		{
			StopPlayerCountDown(playerid);
			SendClientMessage(playerid, Color::COLOR_INFO3, L_countdown_removed);
		}
		else
		{
			if (counting_timerid == INVALID_TIMER_ID)
			{
				current_countdown_count = 10;
				counting_timerid = sampgdk_SetTimerEx(1000, true, CountdownTimer, nullptr, timer_owner);
				UpdateCountdownTDs(true);
			}
			if (CountDownPlayers.find(playerid) == CountDownPlayers.end())
			{
				if (current_countdown_count < 4)
				{
					InitPlayerCountDown(playerid);
				}
				SendClientMessage(playerid, Color::COLOR_INFO3, L_countdown_joined);
				CountDownPlayers[playerid] = StreamerLibrary::CreateDynamic3DTextLabel("{00FF00}[---- / CD ----]", 0, 0.0, 0.0, 0.25, 100.0, playerid, INVALID_VEHICLE_ID, 1, 0, 0);
			}
		}
		return true;
	}
};