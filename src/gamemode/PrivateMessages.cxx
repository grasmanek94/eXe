/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the Private Message System

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies
		Extension
		WorldData
		ZeroCommand
		ZeroDialog
		Language

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

struct SPlayerPMSettings
{
	bool Enabled;
	bool Muted;
	int LastSender;
	std::set<int> BlockedList;
	std::set<int> BlockedBy;
};

std::array<SPlayerPMSettings, MAX_PLAYERS> PlayerPMSettings;

class PrivateMessageManager : public Extension::Base
{
public:
	bool OnPlayerConnect(int playerid) override
	{
		PlayerPMSettings[playerid].Enabled = true;
		PlayerPMSettings[playerid].Muted = false;
		PlayerPMSettings[playerid].LastSender = INVALID_PLAYER_ID;
		PlayerPMSettings[playerid].BlockedList.clear();
		PlayerPMSettings[playerid].BlockedBy.clear();
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		if (PlayerPMSettings[playerid].LastSender != INVALID_PLAYER_ID)
		{
			PlayerPMSettings[PlayerPMSettings[playerid].LastSender].LastSender = INVALID_PLAYER_ID;
			PlayerPMSettings[playerid].LastSender = INVALID_PLAYER_ID;
		}
		for (auto blocker : PlayerPMSettings[playerid].BlockedBy)
		{
			PlayerPMSettings[blocker].BlockedList.erase(playerid);
		}
		return true;
	}
} _PrivateMessageManager;

void SendPM(int playerid, int receiverid, std::string msg)
{
	if (playerid == receiverid)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_SendPM_ownID);
		return;
	}
	if (!PlayerPMSettings[playerid].Enabled)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_SendPM_pmAreOff);
		return;
	}
	if (PlayerPMSettings[receiverid].Muted || !PlayerPMSettings[receiverid].Enabled || PlayerPMSettings[playerid].BlockedList.find(receiverid) != PlayerPMSettings[playerid].BlockedList.end() || PlayerPMSettings[receiverid].BlockedList.find(playerid) != PlayerPMSettings[receiverid].BlockedList.end())
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_SendPM_receiverIsBlockedInfo);
		return;
	}
	if (msg.size() > 87)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_SendPM_tooLong);
		return;
	}

	PlayerPMSettings[playerid].LastSender = receiverid;
	std::string toplayer(Functions::string_format("›› %-3d {%06X}%s{FFFFFF}: {%06X}%s", receiverid, Player[receiverid].Color, Player[receiverid].PlayerName.c_str(), Color::ToChat(Color::COLOR_PRIVATEMSG_OUT), msg.c_str()));
	fixSendClientMessage(playerid, Color::COLOR_PRIVATEMSG_OUT, toplayer);

	if (!has_string_ip_ad(msg))
	{
		PlayerPlaySound(receiverid, 1057, 0.0, 0.0, 0.0);
		PlayerPMSettings[receiverid].LastSender = playerid;
		GivePlayerAchievement(playerid, EAM_SpammerPW, 1);
		std::string toreceiver(Functions::string_format("‹‹ %-3d {%06X}%s{FFFFFF}: {%06X}%s", playerid, Player[playerid].Color, Player[playerid].PlayerName.c_str(), Color::ToChat(Color::COLOR_PRIVATEMSG_IN), msg.c_str()));
		fixSendClientMessage(receiverid, Color::COLOR_PRIVATEMSG_IN, toreceiver);
		Player[receiverid].InsertMessageHistory(toreceiver);
		Player[playerid].InsertMessageHistory(toplayer);
		gtLog(LOG_PRIVMSG, Functions::string_format("[%d][%s]->[%d][%s]: %s", playerid, Player[playerid].PlayerName.c_str(), receiverid, Player[receiverid].PlayerName.c_str(), msg.c_str()));
	}
}

ZCMD(pm, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/pw" }))
{
	if (PlayerPMSettings[playerid].Muted)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_pm_wasMutedByAdmin);
		return true;
	}
	std::string::size_type pos = params.find(' ');
	if (params.npos != pos) 
	{
		std::string msg(params.substr(pos + 1));
		std::string id(params.substr(0, pos));
		if (id.size() && msg.size())
		{
			int receiverid = ValidPlayerID(id);
			if (receiverid != INVALID_PLAYER_ID)
			{
				SendPM(playerid, receiverid, msg);
				return true;
			}
			else
			{
				SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
				return true;
			}
		}
	}
	else if (params.size())
	{
		if (boost::iequals(params, "Z"))
		{
			PlayerPMSettings[playerid].Enabled = true;
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_pm_turnOn);
			return true;
		}
		else if (boost::iequals(params, "W"))
		{
			PlayerPMSettings[playerid].Enabled = false;
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_pm_turnOff);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_pm_usage, false, true);
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_pm_usage2);
	return true;
}

ZCMD(reply, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/odp" }))
{
	if (params.size())
	{
		int receiverid = ValidPlayerID(PlayerPMSettings[playerid].LastSender);
		if (receiverid != INVALID_PLAYER_ID)
		{
			SendPM(playerid, receiverid, params);
			return true;
		}
		else
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_reply_cannotFoundLastID);
			return true;
		}
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_reply_cannotBeEmpty);
	return true;
}

ZCMD(pmblock, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({"/blockpm", "/ignorepm", "/pmoff", "/pmignore" }))
{
	if (params.size())
	{
		int id = ValidPlayerID(params);
		if (id != INVALID_PLAYER_ID && id != playerid)
		{
			PlayerPMSettings[playerid].BlockedList.insert(id);
			PlayerPMSettings[id].BlockedBy.insert(playerid);
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_pmblock_playerBlockedInfo);
			return true;
		}
		else
		{
			SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_pmblock_usage);
	return true;
}

ZCMD(pmunblock, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/unblockpm", "/unignorepm", "/pmon", "/pmunignore" }))
{
	if (params.size())
	{
		int id = ValidPlayerID(params);
		if (id != INVALID_PLAYER_ID && id != playerid)
		{
			PlayerPMSettings[playerid].BlockedList.erase(id);
			PlayerPMSettings[id].BlockedBy.erase(playerid);
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_pmunblock_playerBlocked);
			return true;
		}
		else
		{
			SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_pmunblock_usage);
	return true;
}