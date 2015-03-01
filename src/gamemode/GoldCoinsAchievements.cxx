/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the Secret Golden Coins system
		-	Storing all locations of the coins and making the objects on the map
		-	Handling shot coins (OnPlayerShootObject) and setting the correct information in the users' stats

	________________________________________________________________	
	Notes
		-	You leak it, I kill you.
		-	I promise,
		-	Seriously...
		-	I will find you,
		-	And do it,
		-	Even if I have to go to - fucking - Siberia

		-	Maximum of 256 Golden Coins supported (so don't exceed this array size, it just won't work)

	________________________________________________________________
	Dependencies
		Extension
		Streamer
		ZeroCommand
		PlayerTeleport
		MessageManager
		WorldData

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"
//3053 | 0, 0, -1, 50.0, 50.0
std::vector<Zabawy::SPos> GoldCoins =
{
	{ 1117.634521f, -2037.145386f, 77.950000f },
	#include "GoldCoinsAchievements.inline" //not gonna give them all to you, sorry :')
	{ 1121.915894f, -1693.905762f, 18.746875f }
};

std::map<int, size_t> ObjectIDToIndex;

class GoldCoinsAchievmentsProcessor : public Extension::Base
{
public:
	bool OnGameModeInit()
	{
		for (size_t i = 0; i < GoldCoins.size(); ++i)
		{
			ObjectIDToIndex[StreamerLibrary::CreateDynamicObject(3053, GoldCoins[i].x, GoldCoins[i].y, GoldCoins[i].z, 0.0, 0.0, 0.0, 0, 0, -1, 35.0, 35.0)] = i;
		}
		return true;
	}
	bool OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float x, float y, float z)
	{
		auto index = ObjectIDToIndex.find(objectid);
		if (index != ObjectIDToIndex.end())
		{
			if (!Player[playerid].Achievementdata.GoldCoins[index->second])
			{
				Player[playerid].Achievementdata.GoldCoins[index->second] = true;
				Player[playerid].GiveMoney(Player[playerid].Achievementdata.GoldCoins.count() * 1000);
				Player[playerid].GiveExperienceKill(playerid);
				Player[playerid].GiveScore((long long)sqrt(Player[playerid].Achievementdata.GoldCoins.count()));

				if (Player[playerid].Achievementdata.GoldCoins.count() == GoldCoins.size())
				{
					Player[playerid].GiveMoney(2500000);
					Player[playerid].GiveScore(1000);
				}
				fixSendClientMessage(playerid, Color::COLOR_INFO3, L_goldcoin_foundnew, Player[playerid].Achievementdata.GoldCoins.count(), GoldCoins.size());
			}			
		}
		return true;
	}
} _GoldCoinsAchievmentsProcessor;

ZERO_COMMAND3(zmtp, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1114.2549, -2037.0563, 78.2060, 271.2055, false, 0, 0, "", 0, 0, 0.3, 0.3);
	SetCameraBehindPlayer(playerid);
	GameTextForPlayer(playerid, TranslateP(playerid, L_goldcoin_helpmsg).c_str(), 5000, 3);
	return true;
}