/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the the Command Spy system
		-	Implementation of the IP-ON-Join technology
		-	Implementation of history commands (chat, pm, commands, etc.)

	________________________________________________________________	
	Notes
		-	Mods & Admins only

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

std::array<std::set<int>, MAX_PLAYERS> PlayerEyeCmdAdmins;
std::array<std::set<int>, MAX_PLAYERS> IsWatchingPlayers;
std::set<int> WatchingAllCommands;
std::set<int> WatchingIncommingIPs;

class AdminCmdEyeProcessor : public Extension::Base
{
public:
	bool OnPlayerConnect(int playerid)
	{
		std::string msg(Functions::string_format("[JOIN] [%3d] " + Player[playerid].PlayerName + " " + Player[playerid].ipv4.to_string(), playerid));
		
		for (auto i : WatchingIncommingIPs)
			fixSendClientMessage(i, Color::COLOR_JOININFO, msg, false, false);

		return true;
	}
	/*
		We cleanup all the "watching lists" here to make sure the stupid bug doesn't happen again...
		I mean the bug that everyone could see which commands all other people use :x
	*/
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		std::string msg(Functions::string_format("[LEAVE] [%3d] " + Player[playerid].PlayerName + " " + Player[playerid].ipv4.to_string(), playerid));

		for (auto i : WatchingIncommingIPs)
			fixSendClientMessage(i, Color::COLOR_LEAVEINFO, msg, false, false);

		/*
			So, first: We make sure all admins are not watching the leaving playerid anymore
		*/
		for (auto admin : PlayerEyeCmdAdmins[playerid])
			IsWatchingPlayers[admin].erase(playerid);

		/*
			Then.. We make sure that any players this player is watching are not watched anymore by this player
		*/
		for (auto player : IsWatchingPlayers[playerid])
			PlayerEyeCmdAdmins[player].erase(playerid);

		/*
			And finally clear all watching targets for the current player
		*/
		PlayerEyeCmdAdmins[playerid].clear();
		IsWatchingPlayers[playerid].clear();

		/*
			This player won't watch those anyway while disconnected...
		*/
		WatchingIncommingIPs.erase(playerid);
		WatchingAllCommands.erase(playerid);

		return true;
	}
	/*
		Here the Command Spy takes over, sure does a good job but it doesn't work for mini-games command because those command really don't exist
		Maybe need to fix that in a future version, would require editing the mini-game system, not this
	*/
	void OnPlayerCommandExecuted(int playerid, std::string& command, std::string& params, bool success)
	{
		std::string totalcommand = command + " " + params;
		Player[playerid].InsertCommandHistory(totalcommand);

		totalcommand.insert(0, Functions::string_format("[%-3d]" + Player[playerid].PlayerName + "-> ", playerid));

		for (auto admin : PlayerEyeCmdAdmins[playerid])
			fixSendClientMessage(admin, Color::COLOR_INFO3, totalcommand);

		for (auto admin : WatchingAllCommands)
		{
			if (admin != playerid)
				fixSendClientMessage(admin, Color::COLOR_INFO3, totalcommand);
		}
	}
	/*
		Just some user settings loading when the player joins the game
	*/
	void OnPlayerGameBegin(int playerid)
	{ 
		if (Player[playerid].Achievementdata.ShowIpOnConnect && Player[playerid].statistics.privilidges >= PERMISSION_MODERATOR)
		{
			WatchingIncommingIPs.insert(playerid);
		}
	}
} _AdminCmdEyeProcessor;

ZCMDF(eyecmd, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({"/cmdeye", "/eye"}), "p")
{
	if (parser.Good())
	{
		if (WatchingAllCommands.find(playerid) == WatchingAllCommands.end())
		{
			int eyeid = parser.Get<ParsePlayer>(0).playerid;
			if (eyeid != INVALID_PLAYER_ID && eyeid != playerid)
			{
				if (PlayerEyeCmdAdmins[eyeid].find(playerid) == PlayerEyeCmdAdmins[eyeid].end())
				{
					PlayerEyeCmdAdmins[eyeid].insert(playerid);
					IsWatchingPlayers[playerid].insert(eyeid);
					SendClientMessage(playerid, Color::COLOR_INFO2, L_eyecmd_player_enabled);
				}
				else
				{
					PlayerEyeCmdAdmins[eyeid].erase(playerid);
					IsWatchingPlayers[playerid].erase(eyeid);
					SendClientMessage(playerid, Color::COLOR_INFO2, L_eyecmd_player_disabled);
				}
				return true;
			}
			SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_INFO3, L_eyecmd_player_watching_everyone);
		return true;
	}
	SendClientMessage(playerid, Color::COLOR_ERROR, L_eyecmd_player_usage);
	return true;
}

ZCMD(eyecmdall, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/cmdeyeall", "/eyeall" }))
{
	if (WatchingAllCommands.find(playerid) == WatchingAllCommands.end())
	{	
		for (auto i : IsWatchingPlayers[playerid])
			PlayerEyeCmdAdmins[i].erase(playerid);
		IsWatchingPlayers[playerid].clear();
		WatchingAllCommands.insert(playerid);
		SendClientMessage(playerid, Color::COLOR_INFO2, L_eyecmdall_enabled);
	}
	else
	{
		WatchingAllCommands.erase(playerid);
		SendClientMessage(playerid, Color::COLOR_INFO2, L_eyecmdall_disabled);
	}
	return true;
}

ZCMD3(eyeip, PERMISSION_MODERATOR, RESTRICTION_NONE)
{
	if (Player[playerid].Achievementdata.ShowIpOnConnect)
	{
		Player[playerid].Achievementdata.ShowIpOnConnect = 0;
		SendClientMessage(playerid, Color::COLOR_INFO2, L_eyeip_disabled);
		WatchingIncommingIPs.erase(playerid);
	}
	else
	{
		Player[playerid].Achievementdata.ShowIpOnConnect = 1;
		SendClientMessage(playerid, Color::COLOR_INFO2, L_eyeip_enabled);
		WatchingIncommingIPs.insert(playerid);
	}
	return true;
}

ZCMD(agmh, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/globalchathistory", "/historiaczatu", "/glastchat" }))
{
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_global_chat_history), boost::algorithm::join(GlobalChatHistory, ""), "V", "X");
	return true;
}

ZCMDF(apmh, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/playerchathistory", "/historiaczatugracza", "/lastchat" }), "p")
{
	if (parser.Good())
	{
		int id = parser.Get<ParsePlayer>(0).playerid;
		if (id != INVALID_PLAYER_ID)
		{
			if (Player[id].ChatHistory.size())
			{
				ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_chat_history) + " - " + Player[id].PlayerName, boost::algorithm::join(Player[id].ChatHistory, ""), "V", "X");
				return true;
			}
			SendClientMessage(playerid, Color::COLOR_ERROR, L_no_history);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	SendClientMessage(playerid, Color::COLOR_ERROR, L_lastchat_usage);
	return true;
}

ZCMDF(apph, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/playerpmhistory", "/historiapwgracza", "/lastpm" }), "p")
{
	if (parser.Good())
	{
		int id = parser.Get<ParsePlayer>(0).playerid;
		if (id != INVALID_PLAYER_ID)
		{
			if (Player[id].MessageHistory.size())
			{
				ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_PM_history) + " - " + Player[id].PlayerName, boost::algorithm::join(Player[id].MessageHistory, ""), "V", "X");
				return true;
			}
			SendClientMessage(playerid, Color::COLOR_ERROR, L_no_history);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	SendClientMessage(playerid, Color::COLOR_ERROR, L_lastpm_usage);
	return true;
}

ZCMDF(apch, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/playercmdhistory", "/historiakomendgracza", "/lastcmd" }), "p")
{
	if (parser.Good())
	{
		int id = parser.Get<ParsePlayer>(0).playerid;
		if (id != INVALID_PLAYER_ID)
		{
			if (Player[id].CommandHistory.size())
			{
				ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_CMD_history) + " - " + Player[id].PlayerName, boost::algorithm::join(Player[id].CommandHistory, ""), "V", "X");
				return true;
			}
			SendClientMessage(playerid, Color::COLOR_ERROR, L_no_history);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	SendClientMessage(playerid, Color::COLOR_ERROR, L_lastcmd_usage);
	return true;
}

ZCMD3(players, PERMISSION_MODERATOR, RESTRICTION_NONE)
{
	std::string toshow;
	int switcher = 0;
	for (auto ip : IPtoPlayers)
	{
		int hue = (int)(((float)(ip.first.to_ulong())) / ((float)0xFFFFFFFF)*255.0);
		int xcolor = ColorIPFixer::HSVtoRGBcvScalar(hue, 255, 255);
		for (auto player : ip.second)
		{
			toshow.append(Functions::string_format("{%06X}[" + ip.first.to_string() + "]{%06X}[%-3d]" + Player[player].PlayerName + " ", xcolor, Player[player].Color, player));
			if (++switcher % 7 == 0)
				toshow.append("\n");
		}
	}
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, "Online", toshow, "V", "X");
	return true;
}

ZCMD3(ips, PERMISSION_MODERATOR, RESTRICTION_NONE)
{
	std::string toshow(" ");
	for (auto ip : IPtoPlayers)
	{
		if (ip.second.size() > 1)
		{
			toshow.append("\n" + ip.first.to_string() + ": ");
			for (auto player : ip.second)
			{
				toshow.append( " [" + std::to_string(player) + "] " + Player[player].PlayerName + " ");
			}
		}
	}
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, "Multi-IP", toshow, "V", "X");
	return true;
}