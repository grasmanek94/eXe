/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Management of player achievements
		-	Displaying achievement information about a online user
		
	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies
		Extension
		WorldData
		MessageManager
		ZeroDialog
		ZeroCommand
		Language

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

/*
	This function is used to assign (or add a value to) a achievement to a players' record
	If the achievement criteria have been met (second if) we also notify the player the he/she has a new achievement
*/
bool GivePlayerAchievement(int playerid, EAchievementsMain which, long long howmuch)
{
	if (howmuch >= 0)
	{
		SafeAddULL(Player[playerid].Achievementdata.AchievementsMain[which], howmuch);
		if (Player[playerid].Achievementdata.AchievementsMain[which] >= SingleFormatAchievements[which].first &&  Player[playerid].HasAlreadyAchievement.find(which) == Player[playerid].HasAlreadyAchievement.end() && SingleFormatAchievements[which].first != 0)
		{
			Player[playerid].HasAlreadyAchievement.insert(which);
			Player[playerid].GiveExperienceKill(playerid);
			SendClientMessage(playerid, Color::COLOR_INFO3, L_stats_new_achievement);
			return true;
		}
	}
	else
	{
		SafeSubULL(Player[playerid].Achievementdata.AchievementsMain[which], -howmuch);
	}
	return false;
}

/*
	We need to keep track of which player is viewing which player, no need to initialize to anything
*/
int viewing[MAX_PLAYERS];
void ShowMainAchievmentsListChooser(int playerid)
{
	ShowPlayerCustomDialog(playerid, "chooseviewosg", DIALOG_STYLE_LIST, TranslatePF(playerid, L_stats_achievement_title, Player[viewing[playerid]].PlayerName.c_str()), TranslateP(playerid, L_stats_achievement_info), "V", "X");
}

ZCMDF(osg, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/osiagniecia", "/osi¹gniêcia", "/achiwy", "/achievments", "/achievements", "/achievmenty", "/achiewmenty", "/achiewements", "/achiewments", "/osiagi", "/osi¹gi" }), "p")
{
	if (parser.Good())
		viewing[playerid] = parser.Get<ParsePlayer>(0).playerid;
	else
		viewing[playerid] = playerid;
	if (viewing[playerid] == INVALID_PLAYER_ID)
		viewing[playerid] = playerid;
	ShowMainAchievmentsListChooser(playerid);
	return true;
}

class osgprocessor : public Extension::Base
{
public:
	bool OnPlayerClickPlayer(int playerid, int clickedplayerid, int source)
	{
		viewing[playerid] = clickedplayerid;
		ShowMainAchievmentsListChooser(playerid);
		return true;
	}
} _osgprocessor;

/*
	This dialog contains all display logic for the command /osg
	
	First we check which kind of achievements and/or stats the selecting player wants to view
	When we have determined that by the case jump we create a list of the relevant information,
	then display it as a new dialog. The new dialog if responded succesfully will redirect back to
	this main dialog.
*/
ZERO_DIALOG(chooseviewosg)
{
	if (response)
	{
		std::string showstring("");
		switch (listitem)
		{
		case 0://osiagniecia
#ifdef _DEBUG___x
			if (CategoryAchievements.size())
			{
				for (auto i : CategoryAchievements)
#else
			if (Player[viewing[playerid]].HasAlreadyAchievement.size())
			{
				for (auto i : Player[viewing[playerid]].HasAlreadyAchievement)
#endif
				{
					showstring.append(Functions::string_format(TranslateP(playerid, SingleFormatAchievements[i].second) + "\n", Player[viewing[playerid]].Achievementdata.AchievementsMain[i]));
				}
			}
			else
			{
				showstring.assign(TranslateP(playerid,L_stats_txt_none));
			}
			
			showstring.append(Functions::string_format(TranslateP(playerid, L_stats_amount_not_achieved), (CategoryAchievements.size() - Player[viewing[playerid]].HasAlreadyAchievement.size())));
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_achievement_title, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 1://statystyki
			ZCMD_CALL_COMMAND(playerid, "/stats", Player[viewing[playerid]].PlayerName);
			break;
		case 2://dodatkowe statystyki(1)
			if (CategoryStats.size())
			{
				for (auto i : CategoryStats)
				{
					showstring.append(Functions::string_format(TranslateP(playerid, SingleFormatAchievements[i].second) + "\n", Player[viewing[playerid]].Achievementdata.AchievementsMain[i]));
				}
			}
			else
			{
				showstring.assign(TranslateP(playerid, L_stats_additional_none));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_additional_info_1, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 3://dodatkowe statystyki (2)
			if (CategoryStats2.size())
			{
				for (auto i : CategoryStats2)
				{
					showstring.append(Functions::string_format(TranslateP(playerid, SingleFormatAchievements[i].second) + "\n", Player[viewing[playerid]].Achievementdata.AchievementsMain[i]));
				}
			}
			else
			{
				showstring.assign(TranslateP(playerid, L_stats_additional_none));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_additional_info_2, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 4://Zabójstwa z broni
			for (auto i : WeaponInfo)
			{
				if (i.CanKillWithIt)
					showstring.append(Functions::string_format(i.weaponName + ": %d\n", Player[viewing[playerid]].Achievementdata.WeaponKills[i.weaponid]));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_killsfromweapons, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 5://Œmierci z broni
			for (auto i : WeaponInfo)
			{
				if (i.CanKillWithIt)
					showstring.append(Functions::string_format(i.weaponName + ": %d\n", Player[viewing[playerid]].Achievementdata.WeaponDeaths[i.weaponid]));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_killedbyweapons, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 6://Killstreak z broni
			for (auto i : WeaponInfo)
			{
				if (i.CanKillWithIt)
					showstring.append(Functions::string_format(i.weaponName + ": %d\n", Player[viewing[playerid]].Achievementdata.WeaponMaxKillStreak[i.weaponid]));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_biggestkillstreaks, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 7://Zabójstwa przeciwników z t¹ sam¹ broni¹
			for (auto i : WeaponInfo)
			{
				if (i.CanKillWithIt)
					showstring.append(Functions::string_format(i.weaponName + ": %d\n", Player[viewing[playerid]].Achievementdata.SameWeaponKills[i.weaponid]));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_killswithsameweapons, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 8://Œmierci przez przeciwników z t¹ sam¹ broni¹
			for (auto i : WeaponInfo)
			{
				if (i.CanKillWithIt)
					showstring.append(Functions::string_format(i.weaponName + ": %d\n", Player[viewing[playerid]].Achievementdata.SameWeaponDeaths[i.weaponid]));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_deathswithsameweapons, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		case 9://Poziomy broni
			for (auto i : WeaponInfo)
			{
				if (i.CanKillWithIt)
					showstring.append(Functions::string_format(i.weaponName + ": %.0f (%.0f%%)\n", Player[viewing[playerid]].GetLevel((double)Player[viewing[playerid]].Achievementdata.WeaponExperience[i.weaponid]), Player[viewing[playerid]].GetPercentToNextLevel((double)Player[viewing[playerid]].Achievementdata.WeaponExperience[i.weaponid])));
			}
			ShowPlayerCustomDialog(playerid, "osgviewer", DIALOG_STYLE_MSGBOX, TranslatePF(playerid, L_stats_weaponlevels, Player[viewing[playerid]].PlayerName.c_str()), showstring, "<", "X");
			break;
		default:
			ShowMainAchievmentsListChooser(playerid);
			break;
		}
	}
}

ZERO_DIALOG(osgviewer)
{
	if (response)
		ShowMainAchievmentsListChooser(playerid);
}