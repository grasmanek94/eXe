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
#include "Mafia.hxx"

std::array<SPlayer, MAX_PLAYERS> Player;

std::set<int> PlayersOnline;

std::set<int> VipsOnline;
std::set<int> SponsorsOnline;
std::set<int> ModeratorsOnline;
std::set<int> AdminsOnline;
std::set<int> ViceHeadAdminsOnline;
std::set<int> HeadAdminsOnline;
std::set<int> StaffOnline;

std::map<std::string, int, ci_less> PlayerNameToID;

std::map<unsigned int, std::set<int>> TeamPlayers;
std::map<boost::asio::ip::address_v4, std::set<int>> IPtoPlayers;

std::array<int, MAX_VEHICLES + 1> VehicleDriver;
std::array<int, MAX_VEHICLES + 1> VehicleSpawnedBy;

std::vector<std::string> GlobalChatHistory;

boost::posix_time::ptime UniversalModeDate = boost::posix_time::second_clock::local_time();
boost::posix_time::time_duration UniversalModeTime;

int ValidPlayerID(int playerid)
{
	if (PlayersOnline.find(playerid) == PlayersOnline.end())
		return INVALID_PLAYER_ID;
	return playerid;
}

int ValidPlayerID(std::string idornickname)
{
	int tmpid = INVALID_PLAYER_ID;
	if (!(Functions::is_number_unsigned(idornickname, tmpid) && idornickname.size() < 4))
	{
		auto data = PlayerNameToID.find(idornickname);
		if (data != PlayerNameToID.end())
		{
			tmpid = data->second;
		}
	}
	return ValidPlayerID(tmpid);
}

int ValidPlayerName(std::string name)
{
	auto data = PlayerNameToID.find(name);
	if (data != PlayerNameToID.end())
	{
		return data->second;
	}
	return INVALID_PLAYER_ID;
}

int gMySpecialNPCWhichFucksUp = INVALID_PLAYER_ID;

class WorldDataProcessor : public Extension::Base
{
public:
	WorldDataProcessor() : Base(ExecutionPriorities::WorldData)
	{
		for (size_t i = 0; i < MAX_PLAYERS; ++i)
		{
			Player[i].sp_PlayerID = i;
			if (!Player[i].QuickMenu)
				Player[i].QuickMenu = SPlayer::PlayerMenuPointer(new PlayerMenu::Display(i));
		}
		for (size_t i = 0; i < MAX_VEHICLES + 1; ++i)
		{
			VehicleDriver[i] = INVALID_PLAYER_ID;
			VehicleSpawnedBy[i] = INVALID_PLAYER_ID;
		}
	}
	bool OnPlayerConnect(int playerid) override
	{
		Player[playerid].PlayerConnect();
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		if (reason == 0 || reason == 1)
		{
			SendSpecialDeathMessage(playerid, 201, 0xDB465077, DisconnectReasonsSafe[reason]);
		}
		Player[playerid].PlayerDisconnect();
		return true;
	}
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override
	{
		if (!IsPlayerNPC(playerid))
		{
			Player[playerid].StateChange(newstate, oldstate);
		}
		return true;
	}
	bool OnPlayerSpawn(int playerid) override
	{
		Player[playerid].Spawned = true;
		Player[playerid].LastDamageTime = 0;
		return true;
	}
	bool OnPlayerDeath(int playerid, int killerid, int reason) override
	{
		if (!IsPlayerNPC(playerid))
		{
			Player[playerid].Spawned = false;
			SendDeathMessage(killerid, playerid, reason);
			++Player[playerid].statistics.deaths;

			float loss_percentage;

			if (Player[playerid].statistics.privilidges >= PERMISSION_VIP)
			{
				loss_percentage = 0.1f;
			}
			else
			{
				loss_percentage = 0.33f;
			}


			long long MoneyLost = abs((long long)(((float)Player[playerid].statistics.money)*loss_percentage));
			Player[playerid].GiveMoney(-MoneyLost);
			Player[playerid].GiveScore(-1);
			
			for (auto&i : Player[playerid].Achievementdata.CurrentWeaponKillstreak)
			{
				i = 0;
			}

			Player[playerid].Achievementdata.AchievementsMain[EAM_CurrentKillStreak] = 0;

			if (Player[playerid].Mafia != nullptr)
			{
				++Player[playerid].Mafia->deaths;
			}

			if (killerid != INVALID_PLAYER_ID)
			{
				long long base_money;
				long long money_factor;
				long long get_score;
				long long MoneyScoreAndExperienceFactor = 1;//spowrotem na 1, ju¿ po promocji '2x'

				if (Player[killerid].statistics.privilidges >= PERMISSION_VIP)
				{
					base_money = 10000;
					money_factor = 2;
					get_score = 3;
				}
				else
				{
					base_money = 7500;
					money_factor = 3;
					get_score = 2;
				}


				++Player[killerid].statistics.kills;

				Player[killerid].GiveScore(get_score * MoneyScoreAndExperienceFactor);
				Player[killerid].GiveMoney((base_money + (MoneyLost / money_factor)) * MoneyScoreAndExperienceFactor);

				if (Player[playerid].statistics.bounty)
				{
					GivePlayerAchievement(killerid, EAM_TotalBountyKills, 1);
					GivePlayerAchievement(killerid, EAM_CollectedTotalBounty, Player[playerid].statistics.bounty);
					Player[killerid].GiveMoney(Player[playerid].statistics.bounty);
					Player[playerid].statistics.bounty = 0;
				}

				unsigned long long killexp = Player[killerid].GiveExperienceKill(playerid);
				Player[killerid].GiveExperience(killexp * (MoneyScoreAndExperienceFactor - 1));

				GivePlayerAchievement(killerid, EAM_CurrentKillStreak, 1);
				if (Player[killerid].Achievementdata.AchievementsMain[EAM_CurrentKillStreak] > Player[killerid].Achievementdata.AchievementsMain[EAM_LongestKillStreak])
				{
					GivePlayerAchievement(killerid, EAM_LongestKillStreak, Player[killerid].Achievementdata.AchievementsMain[EAM_CurrentKillStreak] - Player[killerid].Achievementdata.AchievementsMain[EAM_LongestKillStreak]);
				}

				bool mafia_check_passed = true;
				if (Player[killerid].Mafia != nullptr && Player[killerid].Mafia != Player[playerid].Mafia)
				{
					mafia_check_passed = false;
					Player[killerid].Mafia->money += ((base_money + (MoneyLost / money_factor)) * MoneyScoreAndExperienceFactor) / 10;
					++Player[killerid].Mafia->score;
					++Player[killerid].Mafia->kills;
					unsigned long long mafiaxp = (unsigned long long)ceil((double)killexp / 10.0);
					SafeAddULL(Player[killerid].Mafia->experience, mafiaxp);
				}

				if (reason >= 0 && reason < 48)
				{
					if (GetPlayerWeapon(playerid) == reason)
					{
						++Player[playerid].Achievementdata.SameWeaponDeaths[reason];
						++Player[killerid].Achievementdata.SameWeaponKills[reason];
					}

					++Player[playerid].Achievementdata.WeaponDeaths[reason];
					++Player[killerid].Achievementdata.WeaponKills[reason];

					if (++Player[killerid].Achievementdata.CurrentWeaponKillstreak[reason] > Player[killerid].Achievementdata.WeaponMaxKillStreak[reason])
					{
						Player[killerid].Achievementdata.WeaponMaxKillStreak[reason] = Player[killerid].Achievementdata.CurrentWeaponKillstreak[reason];
					}

					if (mafia_check_passed)
					{
						if (Player[playerid].CurrentVehicle && IsVehicleAir(Player[playerid].CurrentVehicleModel))
						{
							unsigned long long additional_experience_vehiclekill = killexp / 3;
							killexp += additional_experience_vehiclekill;
							Player[killerid].GiveExperience(additional_experience_vehiclekill);
						}
					}

					SafeAddULL(Player[killerid].Achievementdata.WeaponExperience[reason], killexp);
				}


			}
			else
			{
				if (reason >= 0 && reason < 48)
				{
					++Player[playerid].Achievementdata.WeaponDeaths[reason];
				}
				++Player[playerid].statistics.suicides;
			}
		}
		return true;
	}
	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ) override
	{
		if (Player[playerid].IsPassenger && VehicleDriver[Player[playerid].CurrentVehicle] == INVALID_PLAYER_ID)
		{
			SetPlayerArmedWeapon(playerid, 0);
			return false;//uniewa¿niæ strza³
		}
		return true;
	}
	bool OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart) override
	{
		Player[playerid].LastDamageTime = Functions::GetTime()+7000;
		if (!(Player[playerid].IsPassenger && VehicleDriver[Player[playerid].CurrentVehicle] == INVALID_PLAYER_ID))
		{
			Player[damagedid].LastDamageTime = Player[playerid].LastDamageTime;
		}
		return true;
	}
	bool OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart) override
	{
		float hp;
		Player[playerid].LastDamageTime = Functions::GetTime()+7000;
		if (issuerid != INVALID_PLAYER_ID)
		{
			Player[issuerid].LastDamageTime = Player[playerid].LastDamageTime;
			if (bodypart == 9)//headshot
			{
				GivePlayerAchievement(issuerid, EAM_HeadShots, 1);
				if (Player[playerid].CurrentGameID == nullptr)
				{
					//instant kill on headshot
					switch (weaponid)
					{
					case 22:
					case 23:
					case 24:
					case 33:
					case 34:
						if (GetPlayerWeapon(playerid) == 38 && (GetPlayerHealth(playerid, &hp), hp) > 1.0)
						{
							fixSetPlayerHealth(playerid, 1.0f);
						}
						else
						{
							fixSetPlayerHealth(playerid, 0.0f);
						}
						break;
					}
				}
			}
		}
		return true;
	}
	bool OnVehicleSpawn(int vehicleid) override
	{
		if (VehicleSpawnedBy[vehicleid] != INVALID_PLAYER_ID)
		{
			Player[VehicleSpawnedBy[vehicleid]].CleanupPlayerVehicleNicely();
		}
		return true;
	}
	//bool OnPlayerUpdate(int playerid) override
	//{
		//static int currweaponid;
		//currweaponid = GetPlayerWeapon(playerid);
		//if (currweaponid != Player[playerid].CurrentWeaponID)
		//{
		//	//OnPlayerWeaponChange(playerid, currweaponid, Player[playerid].CurrentWeaponID)
		//	Player[playerid].CurrentWeaponID = currweaponid;
		//}
	//	return true;
	//}
} _WorldDataProcessor;

void ___OnVehicleLostHealth(int playerid, int vehicleid, float oldhealth, float newhealth)
{
	if (abs(oldhealth-newhealth) > 30)
		Player[playerid].LastDamageTime = Functions::GetTime() + 7000;
}

void InsertGolbalChatHistory(std::string message)
{
	GlobalChatHistory.push_back("{FFFFFF}[" + TextDraws::ActualTimeText + "]" + message + "\n");
	if (GlobalChatHistory.size() > 32)
		GlobalChatHistory.erase(GlobalChatHistory.begin());
}

void SPlayer::SendChat(std::string text)
{
	GivePlayerAchievement(sp_PlayerID, EAM_SpammerChat, 1);
	std::vector<std::string> vec;
	bool sendmsg = false;
	switch (text[0])
	{
	case '!':
		if (Mafia != nullptr)
		{
			GivePlayerAchievement(sp_PlayerID, EAM_SpammerMafiaChat, 1);
			text[0] = ' ';
			std::string temp(Functions::string_format("{81AFE3}!M  {FFFFFF}%-3d {%06X}%s{81AFE3}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			InsertChatHistory(temp);
			InsertGolbalChatHistory(temp);
			makesmallerstrings(temp, vec, 136);
			bool first = true;
			sendmsg = true;
			for (auto i : vec)
			{
				if (first)
				{
					first = false;
					Mafia->SendMessageToMafia(-1, i);
				}
				else
				{
					std::string sendmsg(("{000000}" + std::string(5 + PlayerName.size(), '_') + "{81AFE3}" + i));
					Player[sp_PlayerID].Mafia->SendMessageToMafia(-1, sendmsg);
				}
			}
		}
		break;
	case '@':
		if (statistics.privilidges >= PERMISSION_ADMIN)
		{
			GivePlayerAchievement(sp_PlayerID, EAM_SpammerAdminChat, 1);
			text[0] = ' ';
			std::string temp(Functions::string_format("{E8A854}@A {FFFFFF}%-3d {%06X}%s{E8A854}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			InsertChatHistory(temp);
			InsertGolbalChatHistory(temp);
			makesmallerstrings(temp, vec, 136);
			bool first = true;
			sendmsg = true;
			for (auto i : vec)
			{
				if (first)
					first = false, SendMessageToAdmins(-1, i);
				else
				{
					std::string sendmsg(("{000000}" + std::string(5 + PlayerName.size(), '_') + "{E8A854}" + i));
					SendMessageToAdmins(-1, sendmsg);
				}
			}
		}
		break;
	case '#':
		if (statistics.privilidges >= PERMISSION_VIP)
		{
			GivePlayerAchievement(sp_PlayerID, EAM_SpammerVipChat, 1);
			text[0] = ' ';
			std::string temp(Functions::string_format("{EAF20A}#V  {FFFFFF}%-3d {%06X}%s{EAF20A}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			InsertChatHistory(temp);
			InsertGolbalChatHistory(temp);
			makesmallerstrings(temp, vec, 136);
			bool first = true;
			sendmsg = true;
			for (auto i : vec)
			{
				if (first)
				{
					first = false;
					SendMessageToVips(-1, i);
				}
				else
				{
					std::string sendmsg(("{000000}" + std::string(5 + PlayerName.size(), '_') + "{EAF20A}" + i));
					SendMessageToVips(-1, sendmsg);
				}
			}
		}
		break;
	case '$':
		if (statistics.privilidges >= PERMISSION_SPONSOR)
		{
			GivePlayerAchievement(sp_PlayerID, EAM_SpammerSponsorChat, 1);
			text[0] = ' ';
			std::string temp(Functions::string_format("{E03DBA}$S  {FFFFFF}%-3d {%06X}%s{E03DBA}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			InsertChatHistory(temp);
			InsertGolbalChatHistory(temp);
			makesmallerstrings(temp, vec, 136);
			bool first = true;
			sendmsg = true;
			for (auto i : vec)
			{
				if (first)
				{
					first = false;
					SendMessageToSponsors(-1, i);
				}
				else
				{
					std::string sendmsg(("{000000}" + std::string(5 + PlayerName.size(), '_') + "{E03DBA}" + i));
					SendMessageToSponsors(-1, sendmsg);
				}
			}
		}
		break;
	case '&':
		if (statistics.privilidges >= PERMISSION_MODERATOR)
		{
			GivePlayerAchievement(sp_PlayerID, EAM_SpammerModeratorChat, 1);
			text[0] = ' ';
			std::string temp(Functions::string_format("{B1E334}&P {FFFFFF}%-3d {%06X}%s{B1E334}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			InsertChatHistory(temp);
			InsertGolbalChatHistory(temp);
			makesmallerstrings(temp, vec, 136);
			bool first = true;
			sendmsg = true;
			for (auto i : vec)
			{
				if (first)
					first = false, SendMessageToStaff(-1, i.c_str());
				else
				{
					std::string sendmsg(("{000000}" + std::string(5 + PlayerName.size(), '_') + "{B1E334}" + i));
					SendMessageToStaff(-1, sendmsg.c_str());
				}
			}
		}
		break;
	case '|':
	{
		text[0] = ' ';
		boost::replace_all(text, "•", "%");
		sendmsg = true;
		if (text.size() < 136)
		{
			fixSetPlayerChatBubble(sp_PlayerID, text.c_str(), -1, 150.0f, 5000);
			std::string temp = Functions::string_format("{FFFFFF}|G %-3d {%06X}%s{FFFFFF}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str());
			InsertChatHistory(temp);
			InsertGolbalChatHistory(temp);
			SendMessageToStaff(-1, temp.c_str());
			if (statistics.privilidges < PERMISSION_MODERATOR)
				fixSendClientMessage(sp_PlayerID, -1, temp);
		}
		else
		{
			SendClientMessage(sp_PlayerID, -1, L_local_message_too_long);
		}
		break;
	}
	case '\\':
		if (Team != 0)
		{
			text[0] = ' ';
			std::string temp(Functions::string_format("{EC9EFF}\\T {FFFFFF}%-3d {%06X}%s{EC9EFF}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			InsertChatHistory(temp);
			InsertGolbalChatHistory(temp);
			makesmallerstrings(temp, vec, 136);
			bool first = true;
			sendmsg = true;
			for (auto i : vec)
			{
				if (first)
				{
					first = false;
					for (auto team_member : TeamPlayers[Team])
						fixSendClientMessage(team_member, -1, i);
				}
				else
				{
					std::string sendmsg(("{000000}" + std::string(5 + PlayerName.size(), '_') + "{EC9EFF}" + i));
					for (auto team_member : TeamPlayers[Team])
						fixSendClientMessage(team_member, -1, sendmsg);
				}
			}
			break;
		}
	case '`':
	{
		text.erase(text.begin(), text.begin()+1);
		boost::replace_all(text, "•", "%");
		ZCMD_CALL_COMMAND(sp_PlayerID, "/odp", text);
		sendmsg = true;
		break;
	}
	}
	if (!sendmsg)
	{
		std::string temp(Functions::string_format("{FFFFFF}%-3d {%06X}%s{FFFFFF}: %s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
		boost::replace_all(temp, "•", "%");
		InsertChatHistory(temp);
		InsertGolbalChatHistory(temp);
		makesmallerstrings(temp, vec, 136);
		bool first = true;

		for (auto i : vec)
		{
			if (first)
				first = false, fixSendClientMessageToAll(-1, i, false, false);
			else
				fixSendClientMessageToAll(-1, ("{000000}" + std::string(5 + PlayerName.size(), '_') + "{FFFFFF}" + i), false, false);
		}
	}
}

void SPlayer::SendChatIpAds(std::string text)
{
	std::vector<std::string> vec;
	std::string additional_color("");
	bool sendmsg = false;
	switch (text[0])
	{
		case '!':
		if (Mafia != nullptr)
		{
			text[0] = ' ';
			std::string temp(Functions::string_format("{81AFE3}!M  {FFFFFF}%-3d {%06X}%s{81AFE3}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			additional_color = "{81AFE3}";
			makesmallerstrings(temp, vec, 136);
		}
		break;
		case '@':
		if (statistics.privilidges >= PERMISSION_ADMIN)
		{
			text[0] = ' ';
			std::string temp(Functions::string_format("{E8A854}@A {FFFFFF}%-3d {%06X}%s{E8A854}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			additional_color = "{E8A854}";
			makesmallerstrings(temp, vec, 136);
		}
		break;
		case '#':
		if (statistics.privilidges >= PERMISSION_VIP)
		{
			text[0] = ' ';
			std::string temp(Functions::string_format("{EAF20A}#V  {FFFFFF}%-3d {%06X}%s{EAF20A}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			additional_color = "{EAF20A}";
			makesmallerstrings(temp, vec, 136);
		}
		break;
		case '$':
		if (statistics.privilidges >= PERMISSION_SPONSOR)
		{
			text[0] = ' ';
			std::string temp(Functions::string_format("{E03DBA}$S  {FFFFFF}%-3d {%06X}%s{E03DBA}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			additional_color = "{E03DBA}";
			makesmallerstrings(temp, vec, 136);
		}
		break;
		case '&':
		if (statistics.privilidges >= PERMISSION_MODERATOR)
		{
			text[0] = ' ';
			std::string temp(Functions::string_format("{B1E334}&P {FFFFFF}%-3d {%06X}%s{B1E334}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			additional_color = "{B1E334}";
			makesmallerstrings(temp, vec, 136);
		}
		break;
		case '|':
		{
			sendmsg = true;
			if (text.size() >= 136)
				SendClientMessage(sp_PlayerID, -1, L_local_message_too_long);
			break;
		}
		case '\\':
		if (Team != NO_TEAM)
		{
			text[0] = ' ';
			std::string temp(Functions::string_format("{EC9EFF}\\T {FFFFFF}%-3d {%06X}%s{EC9EFF}:%s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			additional_color = "{EC9EFF}";
			makesmallerstrings(temp, vec, 136);
			break;
		}
		case '`':
		{
			text[0] = ' ';
			boost::replace_all(text, "•", "%");
			ZCMD_CALL_COMMAND(sp_PlayerID, "/odp", text);
			sendmsg = true;
			break;
		}
	}
	if (!sendmsg)
	{
		if (!additional_color.size())
		{
			std::string temp(Functions::string_format("{FFFFFF}%-3d {%06X}%s{FFFFFF}: %s", sp_PlayerID, Color, PlayerName.c_str(), text.c_str()));
			boost::replace_all(temp, "•", "%");
			additional_color = "{FFFFFF}";
			makesmallerstrings(temp, vec, 136);
		}
		bool first = true;
		for (auto i : vec)
		{
			if (first)
				first = false, fixSendClientMessage(sp_PlayerID, -1, i, false, false);
			else
				fixSendClientMessage(sp_PlayerID , - 1, ("{000000}" + std::string(5 + PlayerName.size(), '_') + additional_color + i), false, false);
		}
	}
}

/////////////////////////////SPLAYER
void SPlayer::stats::Reset()
{
	money			= 0;
	kills			= 0;
	experience		= 0;
	deaths			= 0;
	privilidges		= 0;
	bounty			= 0;
	suicides		= 0;
	respect			= 0;
	kicks			= 0;
	warns			= 0;
	playtime		= 0;
	banned			= 0;
	jailtime		= 0;
	registertime	= 0;
	ban_reason		= "";
	ban_who			= "";
	SkinModelID		= 10000;
	Premium			= 0;
	lastconnected	= 0;
	MafiaName		= "";
	UseMafiaSkin	= false;
	UseMafiaSpawn	= false;
	rampid			= 0;
	SaveX			= 0.0;
	SaveY			= 0.0;
	SaveZ			= 0.0;
	SaveA			= 0.0;
	staticcolor		= 0;
	bank			= 0;
	registerip		= "";
	lastip			= "";
	recoverycode	= "";
	email			= "";
	mailverifiedtime= 0;
	lastmailsent	= 0;
	verifiedmail	= "";
	verificationsent= 0;
	verifiedrecoverycode = "";
	password = "";
	nickname = "";
	displayname = "";
	bannedip = 0;
	bannedidentificator = 0;
};

void SPlayer::InsertMessageHistory(std::string message)
{
	MessageHistory.push_back("{FFFFFF}[" + TextDraws::ActualTimeText + "] " + message + "\n");
	if (MessageHistory.size() > 32)
		MessageHistory.erase(MessageHistory.begin());
}

void SPlayer::InsertCommandHistory(std::string cmdtext)
{
	CommandHistory.push_back("{FFFFFF}[" + TextDraws::ActualTimeText + "] " + cmdtext + "\n");
	if (CommandHistory.size() > 32)
		CommandHistory.erase(CommandHistory.begin());
}

void SPlayer::InsertChatHistory(std::string message)
{
	ChatHistory.push_back("{FFFFFF}[" + TextDraws::ActualTimeText + "] " + message + "\n");
	if (ChatHistory.size() > 32)
		ChatHistory.erase(ChatHistory.begin());
}

void SPlayer::SetMoney(long long money)
{
	statistics.money = money;
	GivePlayerMoney(sp_PlayerID, statistics.money - (long long)GetPlayerMoney(sp_PlayerID));
	#ifdef FAILSAFE_MODE
	Player[i].PlayerDisconnect(true);//save only
	#endif
}

void SPlayer::GiveMoney(long long money)
{
	statistics.money += money;
	GivePlayerMoney(sp_PlayerID, statistics.money - (long long)GetPlayerMoney(sp_PlayerID));
	#ifdef FAILSAFE_MODE
	Player[i].PlayerDisconnect(true);//save only
	#endif
}

void SPlayer::SetScore(long long score)
{
	statistics.respect = score;
	SetPlayerScore(sp_PlayerID, statistics.respect);
	#ifdef FAILSAFE_MODE
	Player[i].PlayerDisconnect(true);//save only
	#endif
}

void SPlayer::GiveScore(long long score)
{
	statistics.respect += score;
	SetPlayerScore(sp_PlayerID, statistics.respect);
	UpdatePlayerStatsTextDrawRespect(sp_PlayerID);
	#ifdef FAILSAFE_MODE
	Player[i].PlayerDisconnect(true);//save only
	#endif
}

void SPlayer::CheckNewLevel(unsigned long long Old, unsigned long long New)
{
	if (Old < New)
	{
		//1133;1052;1053;1054;1055;1056;1057;1058;1083;1084;1085;1135;1137;1138;1139;1147;1149;1150;1190
		SendClientMessage(sp_PlayerID, Color::COLOR_INFO3, L_mm_achievement_newlevel);
	}
}

void SPlayer::GiveExperience(unsigned long long exp)
{
	double Old = GetLevel();
	unsigned long long temp = statistics.experience + exp;
	if (temp < statistics.experience)
		statistics.experience = std::numeric_limits<unsigned long long>::max();
	else
		statistics.experience = temp;
	statistics.experience += exp;
	UpdatePlayerLevelTextDraw(sp_PlayerID);
	CheckNewLevel(Old, GetLevel());
	#ifdef FAILSAFE_MODE
	Player[i].PlayerDisconnect(true);//save only
	#endif
}

#pragma warning(disable: 4018)

void SPlayer::GiveExperience(long long exp)
{
	double Old = GetLevel();
	if (exp >= 0)
		statistics.experience = SafeAddULL(statistics.experience, exp);
	else
		statistics.experience = SafeSubULL(statistics.experience, -exp);
	UpdatePlayerLevelTextDraw(sp_PlayerID);
	CheckNewLevel(Old, GetLevel());
	#ifdef FAILSAFE_MODE
	Player[i].PlayerDisconnect(true);//save only
	#endif
}

unsigned long long SafeAddULL(unsigned long long& value, unsigned long long toadd)
{
	unsigned long long temp = value + toadd;
	if (temp < value)
		temp = std::numeric_limits<unsigned long long>::max();
	value = temp;
	return temp;
}

unsigned long long SafeSubULL(unsigned long long& value, unsigned long long tosubstract)
{
	unsigned long long temp = value - tosubstract;
	if (temp > value)
		temp = 0;
	value = temp;
	return temp;
}

unsigned long long SPlayer::GiveExperienceKill(int killedid)
{
	double Old = GetLevel();
	unsigned long long NowStats = statistics.experience;
	statistics.experience = SafeAddULL(statistics.experience, (unsigned long long)(((Player[killedid].GetLevel() * std::log(GetLevel())) / GetLevel()) + 2.0));
	UpdatePlayerLevelTextDraw(sp_PlayerID);
	CheckNewLevel(Old, GetLevel());
	#ifdef FAILSAFE_MODE
	Player[i].PlayerDisconnect(true);//save only
	#endif
	return statistics.experience - NowStats;
}

unsigned long long SPlayer::GetExperience()
{
	return statistics.experience;
}

double SPlayer::GetLevel(double dexp)
{
	if (dexp == -1.0)
		dexp = (double)statistics.experience;
	if (dexp < 0.0)
		dexp = 0.0;
	return std::pow(dexp, 0.3) + 1.0;
}

double SPlayer::GetPercentToNextLevel(double dexp)
{
	double f = GetLevel(dexp);
	return (f - (long)f)*100.00;
}

unsigned long long SPlayer::GetExperience(double level)
{
	return (((unsigned long long)std::pow((level - 1.0), 1.0/0.3)) + 1);
}

void SPlayer::UpdateFullColor()
{
	FullColor = (Color << 0x08) | 0x00000077;
}

void SPlayer::PlayerConnect()
{
	//zero what needs to be zero
	DataLoaded					= false;
	RequestSkinSelection		= false;
	KeepInGame					= false;
	Spawned						= false;
	IsRegistered				= false;
	IsLoggedIn					= false;
	AquiredData					= false;
	ShownConnectMessage			= false;
	HasSpawnedOnce				= false;
	IsDriver					= false;
	IsPassenger					= false;
	BanImmunity					= false;
	IsS0beitDetected			= false;

	AlreadyKicked				= false;
	Mafia						= nullptr;

	MafiaPermissions			= 0;
	PlayerVehicle				= 0;
	CurrentVehicle				= 0;
	CurrentVehicleModel			= 0;
	ChatMute					= 0;
	Team						= NO_TEAM;
	CommandMute					= 0;
	CurrentGameID				= nullptr;
	StagingForGameID			= nullptr;
	LastDamageTime				= 0;
	WorldID						= 0;
	//CurrentWeaponID				= 0;
	CurrentPendingOnWeaponShot	= 0;
	Language					= 0;
	LanguageChosen				= false;
	LanguageID					= 0;

	Exe24ModEnabled				= false;
	Exe24ModVersion				= 0;
	Exe24ModOptions.Reset();

	statistics.Reset();
	SetMoney(statistics.money);
	SetScore(statistics.respect);

	DiffShotTimes.reset();
	LastShotTime				= 0;

	CurrentInactivityTime		= 0;

	//this should work instead of ZeroMemory
	SecureZeroMemory(&weapondata, sizeof(WeaponData));
	SecureZeroMemory(&Achievementdata, sizeof(SAchievements));

	InviteMafia.clear();

	//init what needs to be inited
	PlayerName					= _GetPlayerName(sp_PlayerID);
	ipv4						= ipv4.from_string(_GetPlayerIP(sp_PlayerID));

	PlayerNameToID[PlayerName]	= sp_PlayerID;

	Connected					= true;
	InSkinSelection				= true;
	DoFirstSpawn				= true;

	StartPlayTime				= Functions::GetTimeSeconds();
	LastExpReceived				= Functions::GetTime();

	Color						= Functions::RandomGenerator->Random(0x000000, 0xFFFFFF);
	FullColor					= (Color << 0x08) | 0x00000077;

	CurrentRamp					= INVALID_OBJECT_ID;
	CurrentTutorial				= INVALID_TEXT_DRAW;

	CurrentTutorialID			= -1;
	
	TemporaryRecoveryCode		= "";

	RakNetID = pRakServer->GetPlayerIDFromIndex(sp_PlayerID);

	PlayersOnline.insert(sp_PlayerID);
	IPtoPlayers[ipv4].insert(sp_PlayerID);
	SetPlayerColor(sp_PlayerID, FullColor);

	if (LoadingData.find(PlayerName) == LoadingData.end())
	{
		LoadingData.insert(PlayerName);
		CreateWorkerRequest(sp_PlayerID, PlayerName, nullptr, DATABASE_REQUEST_OPERATION_FIND_LOAD, DATABASE_POINTER_TYPE_USER, 0, 0);
		if (IP_SUPER_WHITELIST.find(ipv4) == IP_SUPER_WHITELIST.end())
			CreateWorkerRequest(sp_PlayerID, PlayerName, nullptr, DATABASE_REQUEST_OPERATION_CHECK_BAN, DATABASE_POINTER_TYPE_BAN, ipv4.to_ulong(), 0);
		else
			BanImmunity = true;
	}
}

void SPlayer::LoadStats(user * userpointer, bool registered, bool forcedata)
{
	if (forcedata)
	{
		DataLoaded = true;
		IsRegistered = registered;
	}
	if (DataLoaded)
		return;

	#define LoadStat(name) statistics.name = userdata.name()
	if (userpointer && !forcedata)
	{
		DataLoaded = true;
		userdata = *userpointer;

		memcpy(&PrywatnePojazdy, userdata.PrivateCars(), 720);
		memcpy((void*)&weapondata, userdata.weapondata(), 440);
		OwnedWeapons.clear();
		for (size_t i = 0; i < 48; ++i)
		{
			if (weapondata.weapon[i].Owned)
				OwnedWeapons.insert(i);
		}
		memcpy((void*)&Achievementdata, userdata.achievementdata(), 4096);
		HasAlreadyAchievement.clear();
		for (auto i : CategoryAchievements)
		{
			if (Achievementdata.AchievementsMain[i] >= SingleFormatAchievements[i].first)
			{
				HasAlreadyAchievement.insert((EAchievementsMain)i);
			}
		}
		LoadStat(money);
		LoadStat(kills);
		LoadStat(experience);
		LoadStat(deaths);
		LoadStat(privilidges);
		LoadStat(bounty);
		LoadStat(suicides);
		LoadStat(respect);
		LoadStat(kicks);
		LoadStat(warns);
		LoadStat(playtime);
		LoadStat(jailtime);
		LoadStat(banned);
		LoadStat(ban_reason);
		LoadStat(ban_who);
		LoadStat(SkinModelID);
		LoadStat(registertime);
		LoadStat(Premium);
		LoadStat(lastconnected);
		LoadStat(MafiaName);
		LoadStat(UseMafiaSkin);
		LoadStat(UseMafiaSpawn);
		LoadStat(rampid);
		LoadStat(SaveX);
		LoadStat(SaveY);
		LoadStat(SaveZ);
		LoadStat(SaveA);
		LoadStat(staticcolor);
		LoadStat(bank);
		LoadStat(registerip);
		LoadStat(lastip);
		LoadStat(recoverycode);
		LoadStat(mailverifiedtime);
		LoadStat(email);
		LoadStat(lastmailsent);
		LoadStat(verifiedmail);
		LoadStat(verificationsent);
		LoadStat(verifiedrecoverycode);
		LoadStat(password);
		LoadStat(nickname);
		LoadStat(displayname);
		LoadStat(bannedip);
		LoadStat(bannedidentificator);
	}
	IsRegistered = registered;
}

void SPlayer::TransferStats()
{
	#define Transferstat(name) userdata.name(statistics.name)
	if (IsRegistered && DataLoaded)
	{
		userdata.PrivateCars((char*)&PrywatnePojazdy);
		userdata.weapondata((char*)&weapondata);
		userdata.achievementdata((char*)&Achievementdata);
		statistics.playtime += (Functions::GetTimeSeconds() - StartPlayTime);
		Transferstat(money);
		Transferstat(kills);
		Transferstat(experience);
		Transferstat(deaths);
		Transferstat(privilidges);
		Transferstat(bounty);
		Transferstat(suicides);
		Transferstat(respect);
		Transferstat(kicks);
		Transferstat(warns);
		Transferstat(playtime);
		Transferstat(banned);
		Transferstat(jailtime);
		Transferstat(ban_reason);
		Transferstat(ban_who);
		Transferstat(SkinModelID);
		Transferstat(Premium);
		Transferstat(lastconnected);
		Transferstat(registertime);
		Transferstat(MafiaName);
		Transferstat(UseMafiaSkin);
		Transferstat(UseMafiaSpawn);
		Transferstat(rampid);
		Transferstat(SaveX);
		Transferstat(SaveY);
		Transferstat(SaveZ);
		Transferstat(SaveA);
		Transferstat(staticcolor);
		Transferstat(bank);
		Transferstat(registerip);
		Transferstat(lastip);
		Transferstat(recoverycode);
		Transferstat(mailverifiedtime);
		Transferstat(email);
		Transferstat(lastmailsent);
		Transferstat(verifiedmail);
		Transferstat(verificationsent);
		Transferstat(verifiedrecoverycode);
		Transferstat(password);
		Transferstat(nickname);
		Transferstat(displayname);
		Transferstat(bannedip);
		Transferstat(bannedidentificator);
	}
}

void SPlayer::PlayerDisconnect(bool saveonly, bool emulate_logged_in)
{
	if (IsRegistered && (IsLoggedIn || emulate_logged_in) && DataLoaded)
	{
		TransferStats();
		user* tosave = new user;
		*tosave = userdata;
		CreateWorkerRequest(sp_PlayerID, PlayerName, tosave, DATABASE_REQUEST_OPERATION_SAVE, DATABASE_POINTER_TYPE_USER, 0, 0);
	}
	if (!saveonly)
	{
		MessageHistory.clear();
		CommandHistory.clear();
		ChatHistory.clear();

		auto foundip = IPtoPlayers.find(ipv4);
		if (foundip != IPtoPlayers.end())
		{
			foundip->second.erase(sp_PlayerID);
			if (!foundip->second.size())
				IPtoPlayers.erase(foundip);
		}

		Connected = false;
		DataLoaded = false;
		CleanupPlayerVehicleNicely();

		PlayerNameToID.erase(PlayerName);
		PlayersOnline.erase(sp_PlayerID);
		VipsOnline.erase(sp_PlayerID);
		SponsorsOnline.erase(sp_PlayerID);
		ModeratorsOnline.erase(sp_PlayerID);
		AdminsOnline.erase(sp_PlayerID);
		ViceHeadAdminsOnline.erase(sp_PlayerID);
		HeadAdminsOnline.erase(sp_PlayerID);
		StaffOnline.erase(sp_PlayerID);

		if (Team != NO_TEAM)
		{
			auto foundteam = TeamPlayers.find(Team);
			if (foundteam != TeamPlayers.end())
			{
				foundteam->second.erase(sp_PlayerID);
				if (!foundteam->second.size())
					TeamPlayers.erase(foundteam);
			}
		}
	}
}

void SPlayer::StateChange(int newstate, int oldstate)
{
	switch (newstate)
	{
		case PLAYER_STATE_DRIVER:
		case PLAYER_STATE_PASSENGER:
		{
			CurrentVehicle = GetPlayerVehicleID(sp_PlayerID);
			if (CurrentVehicle >= 0 && CurrentVehicle < MAX_VEHICLES)
			{
				if (newstate == PLAYER_STATE_DRIVER)
					IsDriver = true;
				else
					IsDriver = false;

				IsPassenger = !IsDriver;

				if (IsDriver)
					VehicleDriver[CurrentVehicle] = sp_PlayerID;

				CurrentVehicleModel = GetVehicleModel(CurrentVehicle);
				GameTextForPlayer(sp_PlayerID, NazwyPojazdow[CurrentVehicleModel].c_str(), 1500, 6);
			}
		}
		break;
		default:
		{
			if (CurrentVehicle)
			{
				if (IsDriver)
					VehicleDriver[CurrentVehicle] = INVALID_PLAYER_ID;
				IsDriver = false;
				IsPassenger = false;
				if (CurrentVehicleModel == 449)
					SetCameraBehindPlayer(sp_PlayerID);
				CurrentVehicle = 0;
				CurrentVehicleModel = 0;
			}
		}
		break;
	}
}

void SPlayer::CleanupPlayerVehicleNicely()
{
	if (PlayerVehicle)
	{
		VehicleSpawnedBy[PlayerVehicle] = INVALID_PLAYER_ID;
		DestroyVehicle(PlayerVehicle);
		PlayerVehicle = 0;
	}
}

bool SPlayer::SetTeam(unsigned int team)
{
	Team = team;
	return SetPlayerTeam(sp_PlayerID, team);
}

unsigned int SPlayer::GetTeam()
{
	return Team;
}

SPlayer::SPlayer()
{
	static_assert (sizeof(SAchievements) == 0x1000, "SAchievements size NOT 4096 bytes, correct this!");
}