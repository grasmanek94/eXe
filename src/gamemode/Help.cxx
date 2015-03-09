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

std::string SecondsToDaysHoursMinutesSecondsForStats(unsigned long long input_seconds)
{
	int days = input_seconds / 86400;
	int hours = (input_seconds / 3600) % 24;
	int minutes = (input_seconds / 60) % 60;
	int seconds = input_seconds % 60;
	return Functions::string_format("%02d dni %02d godz. %02d min. %02d sek.", days, hours, minutes, seconds);
}

std::string SecondsToDaysHoursMinutesSecondsForStatsTP(int playerid, unsigned long long input_seconds)
{
	int days = input_seconds / 86400;
	int hours = (input_seconds / 3600) % 24;
	int minutes = (input_seconds / 60) % 60;
	int seconds = input_seconds % 60;
	return TranslatePF(playerid, L_help_timespan, days, hours, minutes, seconds);
}

void SecondsToDaysHoursMinutesSecondsForStats(unsigned long long input_seconds, int& out_days, int& out_hours, int& out_minutes, int& out_seconds)
{
	out_days = input_seconds / 86400;
	out_hours = (input_seconds / 3600) % 24;
	out_minutes = (input_seconds / 60) % 60;
	out_seconds = input_seconds % 60;
}

void MilliSecondsToDaysHoursMinutesSecondsForStats(unsigned long long input_milliseconds, int& out_days, int& out_hours, int& out_minutes, int& out_seconds, int& out_milliseconds)
{
	out_days = input_milliseconds / 86400000;
	out_hours = (input_milliseconds / 3600000) % 24;
	out_minutes = (input_milliseconds / 60000) % 60;
	out_seconds = (input_milliseconds / 1000) % 60;
	out_milliseconds = input_milliseconds % 1000;
}

ZCMD(stats, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({"/stat", "/statystyki", "/staty", "/statistics", "/playerstats", "/statsp" }))
{
	int statid = playerid;
	if (params.size())
	{
		statid = ValidPlayerID(params);
		if (statid == INVALID_PLAYER_ID)
		{
			fixSendClientMessage(playerid, -1, L_invalid_playerid);
			return true;
		}
	}

	std::string fmt_str[6] =
	{
		SecondsToDaysHoursMinutesSecondsForStatsTP(playerid, Functions::GetTimeSeconds() - Player[statid].StartPlayTime),
		SecondsToDaysHoursMinutesSecondsForStatsTP(playerid, Player[statid].statistics.playtime + (Functions::GetTimeSeconds() - Player[statid].StartPlayTime)),
		Functions::GetTimeStrFromMs(Player[statid].statistics.Premium),
		Functions::GetTimeStrFromMs(Player[statid].statistics.jailtime),
		Functions::GetTimeStrFromMs(Player[statid].statistics.registertime),
		Functions::GetTimeStrFromMs(Player[statid].statistics.banned)
	};

	std::string mafia("");
	if (Player[statid].Mafia != nullptr)
	{
		mafia = Player[statid].Mafia->Name;
	}

	std::string exe24modenabled_str((Player[statid].Exe24ModEnabled ? TranslateP(playerid, L_help_stats_exe24mod_on) : TranslateP(playerid, L_help_stats_exe24mod_off)));
	if (Player[playerid].statistics.privilidges >= PERMISSION_MODERATOR)
	{

		ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_help_stats_title, Player[statid].PlayerName.c_str()),
			TranslatePF(playerid, L_help_stats_infostringadmin,
			Player[statid].PlayerName.c_str(),
			Player[statid].statistics.money + Player[statid].statistics.bank,
			Player[statid].statistics.respect,
			(int)Player[statid].GetLevel(),
			((int)Player[statid].GetLevel())+1,
			(int)Player[statid].GetPercentToNextLevel(),
			Player[statid].statistics.deaths,
			Player[statid].statistics.suicides,
			Player[statid].statistics.kills,
			Player[statid].statistics.bounty,
			Player[statid].Achievementdata.GoldCoins.count(),
			GoldCoins.size(),
			Player[statid].statistics.kicks,
			Player[statid].statistics.warns,
			fmt_str[0].c_str(),
			fmt_str[1].c_str(),
			Player[statid].statistics.SkinModelID,
			mafia.c_str(),
			fmt_str[2].c_str(),
			fmt_str[3].c_str(),
			fmt_str[4].c_str(),
			fmt_str[5].c_str(),
			Player[statid].statistics.ban_reason.c_str(),
			Player[statid].statistics.ban_who.c_str(),
			exe24modenabled_str.c_str(),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_VIP) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_SPONSOR) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_MODERATOR) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_ADMIN) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_VICEHEAD) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_HEADADMIN) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_WSPOLNIK) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_GAMER) ? 0x0000FF00 : 0x00FF0000)
		), "V", "X");
	}
	else
	{
		ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_help_stats_title, Player[statid].PlayerName.c_str()),
			TranslatePF(playerid, L_help_stats_infostringuser,
			Player[statid].PlayerName.c_str(),
			Player[statid].statistics.money + Player[statid].statistics.bank,
			Player[statid].statistics.respect,
			(int)Player[statid].GetLevel(),
			((int)Player[statid].GetLevel())+1,
			(int)Player[statid].GetPercentToNextLevel(),
			Player[statid].statistics.deaths,
			Player[statid].statistics.suicides,
			Player[statid].statistics.kills,
			Player[statid].statistics.bounty,
			Player[statid].Achievementdata.GoldCoins.count(),
			GoldCoins.size(),
			Player[statid].statistics.kicks,
			fmt_str[0].c_str(),
			fmt_str[1].c_str(),
			Player[statid].statistics.SkinModelID,
			mafia.c_str(),
			fmt_str[2].c_str(),
			fmt_str[4].c_str(),
			exe24modenabled_str.c_str(),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_VIP) ? 0x0000FF00 : 0x00FF0000),
			(bIsBitEnabled(Player[statid].statistics.privilidges, PERMISSION_SPONSOR) ? 0x0000FF00 : 0x00FF0000),
			((Player[statid].statistics.privilidges >= PERMISSION_MODERATOR) ? 0x0000FF00 : 0x00FF0000)
		)
		, "V", "X");
	}
	return true;
}

ZCMD(khelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_account_title), TranslateP(playerid, L_help_account_text), "V", "X");
	return true;
}

ZCMD(zmhelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_monety_title), TranslateP(playerid, L_help_monety_text), "V", "X");
	return true;
}

ZCMD(plus, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_plus_title), TranslateP(playerid, L_help_plus_text), "V", "X");
	return true;
}

ZCMD(vip, PERMISSION_NONE, RESTRICTION_REGISTERED_AND_LOADED, cmd_alias({"/viphelp"}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	std::string mailverified_str(Player[playerid].statistics.verifiedmail.size() ? TranslateP(playerid, L_help_vip_mailverified) : TranslateP(playerid, L_help_vip_mailnotverified));
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_vip_title), TranslatePF(playerid, L_help_vip_text,
		Player[playerid].statistics.playtime/3600,
		Player[playerid].GetLevel(),
		mailverified_str.c_str(),
		(((Functions::GetTime() - Player[playerid].statistics.registertime))/(1000 * 60 * 60 * 24)),
		Player[playerid].Achievementdata.GoldCoins.count()), "V", "X");
	return true;
}
ZCMD(vhelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_vehicle_title), TranslateP(playerid, L_help_vehicle_text), "V", "X");
	return true;
}

ZCMD(chathelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_chat_title), TranslateP(playerid, L_help_chat_text), "V", "X");
	return true;
}

ZCMD(chhelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_staff_title), TranslateP(playerid, L_help_staff_text), "V", "X");
	return true;
}

ZCMD(ghelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_global_title), TranslateP(playerid, L_help_global_text), "V", "X");
	return true;
}

ZCMD(ghelp2, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_global2_title), TranslateP(playerid, L_help_global2_text), "V", "X");
	return true;
}

ZCMD(virtualworld, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	fixSendClientMessage(playerid, Color::COLOR_INFO3, TranslatePF(playerid, L_help_virtualworld, Player[playerid].WorldID));
	return true;
}

ZCMD(animid, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	fixSendClientMessage(playerid, Color::COLOR_INFO3, TranslatePF(playerid, L_help_animationid, Player[playerid].WorldID));
	return true;
}

ZCMD(ck, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_ON_FOOT, cmd_alias({}))
{
	static float x, y, z;
	switch (GetPlayerAnimationIndex(playerid))
	{
	case 1209:
	case 1210:
	case 1211:
		GetPlayerPos(playerid, &x, &y, &z);
		SetPlayerPos(playerid, x, y, z + 2.0);
		break;
	}
	return true;
}

ZCMD(ahelp, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({"/adminhelp", "/acmd", "/akomendy", "/acmds" }))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_acmd_title), TranslateP(playerid, L_help_acmd_text), "V", "X");
	return true;
}

ZCMD(ahelp2, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({"/adminhelp2", "/acmd2", "/akomendy2", "/acmds2" }))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_acmd2_title), TranslateP(playerid, L_help_acmd2_text), "V", "X");
	return true;
}
void ShowGameModeInfo(int playerid);

ZCMD3(showgminfoloc, 0, 0)
{
	ShowGameModeInfo(playerid);
	return true;
}

ZCMD(mhelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({"/mafia.h", "/mafia.help", "/mafia.pomoc", "/mafia", "/mafiahelp", "/gang", "/gangs" }))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_mafia_title), TranslateP(playerid, L_help_mafia_text), "V", "X");
	return true;
}

ZCMD(pmhelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_pm_title), TranslateP(playerid, L_help_pm_text), "V", "X");
	return true;
}

void ShowTeleportsList(int playerid)
{///tsdin
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	static const std::string teleporty =
"/8texit       /8track       /tpammo         /andromeda\n\
/barbara      /brothel      /brothel2     /bs\n\
/budynek      /budyneksf    /calligula    /cela\n\
/cementarzls  /chillad      /cjgarage     /cpn\n\
/db           /dbexit       /dd           /ddexit\n\
/denise       /dh           /driftls      /driftsf\n\
/elektrownia  /g1           /g2           /g3\n\
/g4           /gora         /hacjenda     /helena\n\
/hop          /impra        /jaysdin      /jeziorko\n\
/jump         /katie        /kladka       /koloseumls\n\
/koloseumlv   /koloseumsf   /kopalnia     /kosciol\n\
/kss          /kssexit      /libertycity  /lot\n\
/lot2         /ls           /lslot        /lv\n\
/lvlot        /miasteczko   /millie       /minidrift\n\
/mischelle    /molo         /molols       /molols2\n\
/motel        /pgr          /plaza        /plazals2\n\
/plazasf      /portls       /portsf       /portsf2\n\
/radio        /rcbattle     /rcshop       /resta\n\
/salon        /salon2       /sf           /sflot\n\
/shamal       /skatepark    /solo         /stacjals\n\
/stacjals2    /stacjalv     /stacjasf     /stateksf\n\
/stateksf2    /stateksf3    /tama         /telebank\n\
/tir		  /tune	        /tunels       /tunelv\n\
/tunesf       /vc           /vcexit       /walka\n\
/wh           /wh2          /wieza10      /wieza11\n\
/wieza12      /wieza2       /wieza3       /wieza4\n\
/wieza5       /wieza6       /wieza7       /wieza8\n\
/wieza9       /wiezalv      /wiezalv10    /wiezalv2\n\
/wiezalv3     /wiezalv4     /wiezalv5     /wiezalv6\n\
/wiezalv7     /wiezalv8     /wiezalv9     /wiezasf\n\
/wiezasf2     /wiezasf3     /wiezasf4     /wiezasf5\n\
/wiezasf6     /wiezowiec    /woc          /wojsko\n\
/wojsko2      /wooziebed    /zadupie      /zajezdnia\n\
/zakochani    /zamek		/cjlot		  /stunt\n\
/stunt-wyskok /stunt-wyskok2 /wyskok	/atlantis\n\
/sanchez";
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_teleport_title), teleporty, "V", "X");
}

ZERO_COMMAND(tphelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/teles", "/tp", "/teleport", "/t" }))
{
	ShowTeleportsList(playerid);
	return true;
}

ZCMD(opisserwera, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/opis-serwera", "/opis", "/serverinfo", "/owner", "/description" }))
{
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_serverinfo_title), TranslateP(playerid, L_help_serverinfo_text), "V", "X");
	return true;
}

ZCMD(regulamin, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({"/reg", "/rules", "/beleid", "/serverrules", "/privacy", "/regulations", "/statute", "/regimen", "/bye-laws", "/law", "/laws", "/regels"}))
{
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_rules_title), TranslateP(playerid, L_help_rules_text), "V", "X");
	return true;
}

ZCMD(domhelp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({"/dompomoc","/pomocdom"}))
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_help_house_title), TranslateP(playerid, L_help_house_text), "V", "X");
	return true;
}

ZCMD(komendy,
	PERMISSION_NONE,
	RESTRICTION_NONE,
	cmd_alias({
	"/c",
	"/cpi",
	"/cmds",
	"/commands",
	"/pomoc",
	"/help",
	"/info",
	"/credits",
	"/autor",
	"/autorzy",
	"/contact",
	"/kontakt",
	"/sugestia",
	"/cmd",
	"/kmd" })
	)
{
	GivePlayerAchievement(playerid, EAM_HELPHELPHELP, 1);
	ShowPlayerCustomDialog(playerid, "centrumpomocy", DIALOG_STYLE_LIST, TranslateP(playerid, L_help_center_title), TranslateP(playerid, L_help_center_text) + (Player[playerid].statistics.privilidges >= PERMISSION_MODERATOR ? TranslateP(playerid, L_help_center_admincmd) : ""), "V", "X");
	return true;
}

ZERO_DIALOG(centrumpomocy)
{
	static const std::vector<std::string> commands_to_run =
	{
		"/vhelp",
		"/pmhelp",
		"/khelp",
		"/tphelp",
		"/chhelp",
		"/ghelp",
		"/mhelp",
		"/zhelp",
		"/vip",
		"/zmhelp",
		"/chathelp",
		"/domhelp",
		"/opis-serwera",
		"/napisy",
		"/plus",
		"/regulamin",
		"/showgminfoloc",
		"/acmd"
	};
	if (response)
	{
		if (listitem >= 0 && listitem < commands_to_run.size())
		{
			ZCMD_CALL_COMMAND(playerid, commands_to_run[listitem], "");
		}
	}
}