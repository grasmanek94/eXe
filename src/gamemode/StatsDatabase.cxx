/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	This file ensures that whenever a players leaves, his/her
			statistics will get uploaded to the websites' scoreboard

	________________________________________________________________	
	Notes
		-	This extension uses a threaded HTTP SA-MP function
		-	We do nothing with the response from the WWW server

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


//format linku:
//http://exe24.info/top/addstats.php?nick=Adolf&k=222&d=20&r=-20&e=50&c=30&t=1945
//													kills deaths resp  exp coins timeplay		

class StatsSendProcessor : public Extension::Base
{
public:
	bool OnPlayerDisconnect(int playerid, int reason)
	{
#ifdef _LOCALHOST_DEBUG
		if (false)
#else
		if (Player[playerid].IsRegistered && Player[playerid].IsLoggedIn)
#endif
		
		{
			
			std::string mafia_name("");
			if (Player[playerid].Mafia != nullptr)
			{
				mafia_name.assign(Player[playerid].Mafia->Name);
				Player[playerid].Mafia->SendStatsToWeb();
			}

			std::string weapons("");
			for (auto i : Player[playerid].OwnedWeapons)
				weapons.append(std::to_string(i) + ",");
			weapons[weapons.size() - 1] = 0;//remove last ','

			
			std::string post(Functions::string_format("nick=%s&k=%I64u&d=%I64u&r=%I64d&e=%I64u&c=%d&t=%I64u&w=%s&skin=%d&money=%I64d&vip=%d&suicides=%I64u&m=%s", Player[playerid].PlayerName.c_str(), Player[playerid].statistics.kills,
				Player[playerid].statistics.deaths, Player[playerid].statistics.respect, Player[playerid].statistics.experience, Player[playerid].Achievementdata.GoldCoins.count(),
				Player[playerid].statistics.playtime, weapons.c_str(), Player[playerid].statistics.SkinModelID, Player[playerid].statistics.bank + Player[playerid].statistics.money, 
				(int)bIsBitEnabled(Player[playerid].statistics.privilidges,PERMISSION_VIP), Player[playerid].statistics.suicides,mafia_name.c_str()));
			//printf("\nPOST: %s\n", post.c_str());
			sampgdk_HTTP(0, HTTP_POST, "exe24.info/top/addstats.php", post.c_str());//fire&forget
		}
		return true;
	}
} _StatsSendProcessor;