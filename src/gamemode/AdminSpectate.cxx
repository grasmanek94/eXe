/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the spectate system
		Update #2:
		-	Destroying your life
		Update #3:
		-	Cutting your fucking brain in half
		-	Making you skip your sleep

	________________________________________________________________	
	Notes
		-	Fix any bugs and never open this file again (done??)
		-	Please make sure you don't create a infinite loop in here
			if you decide to open the file anyway... 

	________________________________________________________________
	Dependencies
		Extension
		WorldData	
		Language

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

std::array<std::set<int>, MAX_PLAYERS>	PlayerSpectators;
std::array<int, MAX_PLAYERS>			PlayerCurrentlySpectating;

bool PlayerSpactatable(int playerid)
{
	static int state;
	state = GetPlayerState(playerid);
	return (state == PLAYER_STATE_ONFOOT || state == PLAYER_STATE_DRIVER || state == PLAYER_STATE_PASSENGER);
}

bool DisablePlayerSpectate(int playerid, bool doexit = true)
{
	if (PlayerCurrentlySpectating[playerid] == INVALID_PLAYER_ID)
	{
		return false;
	}

	if (doexit)
	{
		fixTogglePlayerSpectating(playerid, false);
	}

	PlayerSpectators[PlayerCurrentlySpectating[playerid]].erase(playerid);
	PlayerCurrentlySpectating[playerid] = INVALID_PLAYER_ID;
	return true;
}

bool PlayerSpectateOtherPlayer(int playerid, int specid, int ignoreid = INVALID_PLAYER_ID)
{
	if (playerid == specid || PlayerCurrentlySpectating[playerid] == specid || !PlayerSpactatable(specid) || specid == ignoreid)
	{
		return false;
	}

	fixSetPlayerVirtualWorld(playerid, Player[specid].WorldID);
	SetPlayerInterior(playerid, GetPlayerInterior(specid));

	fixTogglePlayerSpectating(playerid, true);

	if (Player[specid].CurrentVehicle)
	{
		PlayerSpectateVehicle(playerid, Player[specid].CurrentVehicle, SPECTATE_MODE_NORMAL);
	}
	else
	{
		PlayerSpectatePlayer(playerid, specid, SPECTATE_MODE_NORMAL);
	}

	if (PlayerCurrentlySpectating[playerid] != INVALID_PLAYER_ID)
	{
		PlayerSpectators[PlayerCurrentlySpectating[playerid]].erase(playerid);
	}

	PlayerSpectators[specid].insert(playerid);
	PlayerCurrentlySpectating[playerid] = specid;
	return true;
}

bool SpectateNextPlayer(int playerid, int ignoreid = INVALID_PLAYER_ID)
{
	auto current = PlayersOnline.find(PlayerCurrentlySpectating[playerid]);

	for (auto it = current; it != PlayersOnline.end(); ++it)
	{
		if (PlayerSpectateOtherPlayer(playerid, *it, ignoreid))
		{
			return true;
		}
	}

	for (auto it = PlayersOnline.begin(); it != current; ++it)
	{
		if (PlayerSpectateOtherPlayer(playerid, *it, ignoreid))
		{
			return true;
		}
	}

	return false;
}

bool SpectatePreviousPlayer(int playerid, int ignoreid = INVALID_PLAYER_ID)
{	
	auto rcurrent = find(PlayersOnline.rbegin(), PlayersOnline.rend(), PlayerCurrentlySpectating[playerid]);

	for (auto it = rcurrent; it != PlayersOnline.rend(); ++it)
	{
		if (PlayerSpectateOtherPlayer(playerid, *it, ignoreid))
		{
			return true;
		}
	}

	for (auto it = PlayersOnline.rbegin(); it != rcurrent; ++it)
	{
		if (PlayerSpectateOtherPlayer(playerid, *it, ignoreid))
		{
			return true;
		}
	}

	return false;
}

void DoSpectateCleanup(int playerid)
{
	//don't use 'ranged for' cuz container is being modified
	while (PlayerSpectators[playerid].size())
	{
		static int spectator;
		spectator = *PlayerSpectators[playerid].begin();
		if (!SpectateNextPlayer(spectator, playerid))
		{
			DisablePlayerSpectate(spectator, false);
		}
	}
}

class AdminSpectateProcessor : public Extension::Base
{
public:
	bool OnPlayerConnect(int playerid) override
	{
		PlayerCurrentlySpectating[playerid] = INVALID_PLAYER_ID;
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		//guaranteed here PlayersOnline doesn't contain playerid << NOTE
		//because PlayersOnline is erased in WorldData.cxx/SPlayer class which has a higher execution priority (first)

		DoSpectateCleanup(playerid);

		if (PlayerCurrentlySpectating[playerid] != INVALID_PLAYER_ID)
		{
			PlayerSpectators[PlayerCurrentlySpectating[playerid]].erase(playerid);
		}

		return true;
	}
	/*
		Do some state checking here and decide if we need to take any action on the spectators (if there are any)
	*/
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override
	{ 
		static int states = 0;
		if (PlayerSpectators[playerid].size())
		{			
			switch (newstate)
			{
				case PLAYER_STATE_DRIVER:
				case PLAYER_STATE_PASSENGER:
					if (oldstate == PLAYER_STATE_ONFOOT)
						states = 1;
					break;
				case PLAYER_STATE_ONFOOT:
					if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER)
						states = 2;
					break;
				default:
					states = 0;
					break;
			}
			switch (states)
			{
				case 0:
					DoSpectateCleanup(playerid);
					break;
				case 2:
					for (auto i : PlayerSpectators[playerid])
					{
						PlayerSpectatePlayer(i, playerid, SPECTATE_MODE_NORMAL);
					}
					break;
				case 1:
					for (auto i : PlayerSpectators[playerid])
					{
						PlayerSpectateVehicle(i, Player[playerid].CurrentVehicle, SPECTATE_MODE_NORMAL);
					}
					break;
			}
		}
		return true;
	}
	bool OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid) override
	{
		for (auto i : PlayerSpectators[playerid])
		{
			SetPlayerInterior(i, newinteriorid);
		}
		return true;
	}
	void OnPlayerVirtualWorldChange(int playerid, int newVWid, int oldVWid) override
	{
		for (auto i : PlayerSpectators[playerid])
		{
			fixSetPlayerVirtualWorld(i, newVWid);
		}
	}
	bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) override
	{
		if (PlayerCurrentlySpectating[playerid] != INVALID_PLAYER_ID)
		{
			if (PRESSED(KEY_FIRE))
			{
				SpectateNextPlayer(playerid);
			}
			if (PRESSED(KEY_HANDBRAKE))
			{
				SpectatePreviousPlayer(playerid);
			}
		}
		return true;
	}
} _AdminSpectateProcessor;

ZCMD(spec, PERMISSION_MODERATOR, RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/spectate", "/podglad", "/view", "/obserwacja" }))
{
	if (!params.size())
	{
		SendClientMessage(playerid, Color::COLOR_ERROR, L_spectate_usage);
		return true;
	}
	int specid = ValidPlayerID(params);
	if (specid == INVALID_PLAYER_ID)
	{
		SendClientMessage(playerid, Color::COLOR_ERROR, L_player_not_connected);
		return true;
	}
	if (!PlayerSpactatable(specid))
	{
		SendClientMessage(playerid, Color::COLOR_ERROR, L_player_not_spawned);
		return true;
	}
	if (!PlayerSpectateOtherPlayer(playerid, specid))
	{
		SendClientMessage(playerid, Color::COLOR_ERROR, L_spectate_generic_error);
		return true;
	}
	return true;
}

ZCMD(specoff, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/endspectate", "/endspec", "/nospec" }))
{
	if (!DisablePlayerSpectate(playerid))
	{
		SendClientMessage(playerid, Color::COLOR_ERROR, L_specoff_need_spectate_first);
		return true;
	}
	return true;
}