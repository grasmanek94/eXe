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

std::array<std::map<int, unsigned long long>, MAX_PLAYERS> TeleportRequests;
std::array<std::map<int, unsigned long long>, MAX_PLAYERS> TeleportInvites;

namespace PlayerTeleport
{
	static float x, y, z, a;
	static int interior, virtualworld;
};

using namespace PlayerTeleport;

class PlayerTeleportProcessor : public Extension::Base
{
public:
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		TeleportRequests[playerid].clear();
		TeleportInvites[playerid].clear();
		return true;
	}
} _PlayerTeleportProcessor;

int AcceptPlayerTeleportInvitationToTarget(int& playerid, int& target, unsigned long long& TimeNow, bool vehicle)
{
	//did target request to be teleported to playerid?
	auto teleport_request = TeleportInvites[playerid].find(target);

	if (teleport_request != TeleportInvites[playerid].end())
	{//yes?
		if (teleport_request->second > TimeNow)//pop the request
		{

			GetPlayerPos(target, &x, &y, &z);
			GetPlayerFacingAngle(target, &a);
			interior = GetPlayerInterior(target);
			virtualworld = GetPlayerVirtualWorld(target);

			TeleportPlayer(playerid, x, y, z, a, vehicle, interior, virtualworld, "", 0, 0, 0.7, 0.7, false, true);
			SendClientMessageF(target, Color::COLOR_INFO3, L_mm_APTITT, true, true, playerid, Player[playerid].PlayerName.c_str());

			TeleportInvites[playerid].erase(teleport_request);

			return 2;
		}
		return 1;
	}
	return 0;
}

int RequestTeleportToPlayer(int& playerid, int& target, unsigned long long& TimeToTeleport, unsigned long long& TimeNow)
{
	//did playerid request to be teleported to target?
	auto teleport_request = TeleportRequests[target].find(playerid);

	if (teleport_request != TeleportRequests[target].end())
	{//yes?
		TimeToTeleport = teleport_request->second;
	}
	if (TimeToTeleport < TimeNow)//push the request
	{
		SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_RTTP_a);
		SendClientMessageF(target, Color::COLOR_INFO3, L_mm_RTTP_b, true, true, playerid, Player[playerid].PlayerName.c_str(), playerid);
		TeleportRequests[target][playerid] = TimeNow + 30000;
		return 1;
	}
	return 0;
}

ZCMDF(tpto, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/tpdo", "/idzdo", "/ganaar", "/goto" }), "ps")
{
	unsigned long long TimeNow = Functions::GetTime();
	unsigned long long TimeToTeleport = 0;
	if (parser.Good())
	{
		int target = parser.Get<ParsePlayer>(0).playerid;
		if (target == INVALID_PLAYER_ID || target == playerid)
		{
			SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_invalid_playerid);
			return true;
		}
		if (Player[target].CurrentGameID != nullptr)
		{
			SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_invalid_playerid);
			return true;
		}
		if (AcceptPlayerTeleportInvitationToTarget(playerid, target, TimeNow, parser.Good() == 2) == 2)
			return true;
		if (RequestTeleportToPlayer(playerid, target, TimeToTeleport, TimeNow))
			return true;
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_tpto_cannot_tp);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_tpto_usage, true, true, playerid);
	return true;
}

int AcceptPlayerTeleportRequestToMe(int& playerid, int& target, unsigned long long& TimeNow, bool vehicle)
{
	//did target request to be teleported to playerid?
	auto teleport_request = TeleportRequests[playerid].find(target);

	if (teleport_request != TeleportRequests[playerid].end())
	{//yes?
		if (teleport_request->second > TimeNow)//pop the request
		{

			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerFacingAngle(playerid, &a);
			interior = GetPlayerInterior(playerid);
			virtualworld = GetPlayerVirtualWorld(playerid);

			TeleportPlayer(target, x, y, z, a, vehicle, interior, virtualworld, "", 0, 0, 0.7, 0.7, false, true);
			SendClientMessageF(target, Color::COLOR_INFO3, L_mm_APTRTM, true, true, playerid, Player[playerid].PlayerName.c_str());

			TeleportRequests[playerid].erase(teleport_request);

			return 2;
		}
		return 1;
	}
	return 0;
}

int InviteTeleportPlayerToMe(int& playerid, int& target, unsigned long long& TimeToTeleport, unsigned long long& TimeNow)
{
	//did playerid request to be teleported to target?
	auto teleport_request = TeleportInvites[target].find(playerid);

	if (teleport_request != TeleportInvites[target].end())
	{//yes?
		TimeToTeleport = teleport_request->second;
	}
	if (TimeToTeleport < TimeNow)//push the request
	{
		SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_ITPTM_a);
		SendClientMessageF(target, Color::COLOR_INFO3, L_mm_ITPTM_b, true, true, playerid, Player[playerid].PlayerName.c_str(), playerid);
		TeleportInvites[target][playerid] = TimeNow + 30000;
		return 1;
	}
	return 0;
}

ZCMDF(tphere, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/tptu" }), "ps")
{
	unsigned long long TimeNow = Functions::GetTime();
	unsigned long long TimeToTeleport = 0;
	if (parser.Good())
	{
		int target = parser.Get<ParsePlayer>(0).playerid;
		if (target == INVALID_PLAYER_ID || target == playerid)
		{
			SendClientMessage(playerid, Color::COLOR_ERROR, L_mm_invalid_playerid);
			return true;
		}	
		if (!CheckCommandAllowed(target, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_IN_VEHICLE_OR_ONFOOT, false))
		{
			SendClientMessage(playerid, Color::COLOR_ERROR, L_mm_cannot_tphere);
			return true;
		}
		if (AcceptPlayerTeleportRequestToMe(playerid, target, TimeNow, parser.Good() == 2) == 2)
			return true;
		if (InviteTeleportPlayerToMe(playerid, target, TimeToTeleport, TimeNow) != 0)
			return true;
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_tpto_cannot_tp);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_tphere_usage, true, true, playerid);
	return true;
}