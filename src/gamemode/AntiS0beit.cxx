/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

struct SSobeitAC
{
	bool checking;
	int stage;
	int counter;
	bool spawned;
	unsigned long long check_started;
	int ASTimers;
};

std::array<SSobeitAC, MAX_PLAYERS> SobeitACCheck;

void PlayerAllowedIntoGame(int playerid, bool IN_SPAWN = false)
{
	SobeitACCheck[playerid].stage = 0;
	SobeitACCheck[playerid].checking = false;
	fixSetPlayerVirtualWorld(playerid, 0);
	fixTogglePlayerControllable(playerid, true);
	SendSpecialDeathMessage(playerid, 200, 0x00EDE177, "[Connect]");
	if (!IN_SPAWN)
	{
		SpawnPlayer(playerid);
	}
}

void SetPlayerASWorld(int playerid)
{
	int worldid = 0x6F330000 + playerid;
	if (Player[playerid].WorldID != worldid)
	{
		fixSetPlayerVirtualWorld(playerid, worldid);
	}
}

void ASKickFuckingLagger(int timerid, void* param)
{
	int playerid = (int)param;
	SobeitACCheck[playerid].ASTimers = INVALID_TIMER_ID;
	Kick(playerid);
}

class CAntiSobeitProcessor : public Extension::Base
{
public:
	CAntiSobeitProcessor() : Base(ExecutionPriorities::AntiS0beit) {}
	bool OnPlayerConnect(int playerid) override
	{
		SecureZeroMemory(&SobeitACCheck[playerid], sizeof(SSobeitAC));
		SobeitACCheck[playerid].checking = true;
		SobeitACCheck[playerid].stage = 1;
		SobeitACCheck[playerid].ASTimers = INVALID_TIMER_ID;
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		if (SobeitACCheck[playerid].ASTimers != INVALID_TIMER_ID)
		{
			sampgdk_KillTimerEx(SobeitACCheck[playerid].ASTimers, this);
			SobeitACCheck[playerid].ASTimers = INVALID_TIMER_ID;
		}
		return true;
	}
	bool OnPlayerSpawn(int playerid) override
	{	
		if (SobeitACCheck[playerid].checking)
		{
			SetPlayerASWorld(playerid);
			SobeitACCheck[playerid].spawned = true;
			TogglePlayerControllable(playerid, false);
#ifndef _LOCALHOST_DEBUG
			if (Player[playerid].BanImmunity)
			{
				PlayerAllowedIntoGame(playerid, true);
			}
#endif
		}
		return !SobeitACCheck[playerid].checking;
	}
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override
	{
		if (SobeitACCheck[playerid].checking)
		{
			SetPlayerASWorld(playerid);
			if (SobeitACCheck[playerid].stage == 2 &&
				oldstate == PLAYER_STATE_ONFOOT &&
				newstate == PLAYER_STATE_DRIVER)
			{
				Player[playerid].CleanupPlayerVehicleNicely();
				SobeitACCheck[playerid].stage = 3;
			}
			else if (SobeitACCheck[playerid].stage == 3 &&
				oldstate == PLAYER_STATE_DRIVER &&
				newstate == PLAYER_STATE_ONFOOT)
			{
				SobeitACCheck[playerid].stage = 4;
				SobeitACCheck[playerid].check_started = Functions::GetTime();
				TogglePlayerControllable(playerid, true);
			}
			return false;
		}
		return true;
	}
	bool OnPlayerUpdate(int playerid) override
	{
		if (SobeitACCheck[playerid].checking && SobeitACCheck[playerid].spawned)
		{
			int weapon, ammo;
			if (SobeitACCheck[playerid].stage == 1)
			{
				SetPlayerASWorld(playerid);
				CreatePlayerVehicleSafe(playerid, 457, 0.05, -0.43, 500.71);
				SobeitACCheck[playerid].stage = 2;
				SobeitACCheck[playerid].ASTimers = sampgdk_SetTimerEx(3000, false, ASKickFuckingLagger, (void*)playerid, this);
			}
			else if (SobeitACCheck[playerid].stage == 4)
			{			
				if (GetPlayerWeaponData(playerid, 1, &weapon, &ammo) && ++SobeitACCheck[playerid].counter && weapon != 0 && ammo != 0)
				{
					//detected sobeit
					SobeitACCheck[playerid].stage = 0;
					SendClientMessage(playerid, 0xFF0000FF, L_cheats_detected);
					aKick(playerid, 5, false, INVALID_PLAYER_ID, "Hacks detected", ":AntiCheat:", true);//silent kick
				}
				else if (SobeitACCheck[playerid].counter > 21 && (Functions::GetTime() - SobeitACCheck[playerid].check_started) > 100)
				{
					if (SobeitACCheck[playerid].ASTimers != INVALID_TIMER_ID)
					{
						sampgdk_KillTimerEx(SobeitACCheck[playerid].ASTimers, this);
						SobeitACCheck[playerid].ASTimers = INVALID_TIMER_ID;
					}
					PlayerAllowedIntoGame(playerid);
				}
			}
			return false;
		}
		return true;
	}
	bool OnPlayerCommandReceived(int playerid, std::string& command, std::string& params, int min_execute_level, int this_restrictions) override
	{
		return !SobeitACCheck[playerid].checking;
	}
	bool OnPlayerDeath(int playerid, int killerid, int reason) override
	{ 
		return !SobeitACCheck[playerid].checking;
	}
	bool OnPlayerText(int playerid, std::string text) override
	{ 
		return !SobeitACCheck[playerid].checking;
	}
} 
_CAntiSobeitProcessor
;