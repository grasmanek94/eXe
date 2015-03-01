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

using namespace Zabawy;
namespace OneDe
{
	class Arena : public Base
	{
	public:
		std::vector<SSpawnPoint> ArenaSpawns;

		Arena() :
			Base(L_games_ArenaOneDe_name, "/ode"),
			ArenaSpawns({
				{ 233.4788, 142.3173, 1003.0234, 360.0000 },
				{ 210.5396, 150.4807, 1003.0234, 182.6518 },
				{ 191.6824, 157.3362, 1003.0234, 276.9661 },
				{ 221.9678, 188.0765, 1003.0313, 267.9028 },
				{ 246.0294, 185.4713, 1008.1719, 353.1070 },
				{ 268.0099, 185.1627, 1008.1719, 357.1570 },
				{ 299.1904, 188.1266, 1007.1719, 107.4281 },
				{ 299.1859, 191.2426, 1007.1794, 88.9178 },
				{ 293.3754, 172.6627, 1007.1719, 59.1509 },
				{ 257.6570, 185.4240, 1008.1719, 2.5300 },
				{ 211.6821, 170.7424, 1003.0234, 280.1460 },
				{ 212.3764, 187.1927, 1003.0313, 166.0916 },
				{ 223.2558, 185.9305, 1003.0313, 175.8752 },
				{ 213.2214, 142.4191, 1003.0234, 262.0193 },
				{ 229.9153, 141.4262, 1003.0234, 315.2864 },
				{ 271.7841, 173.5594, 1007.6719, 357.2501 },
				{ 301.0344, 185.8741, 1007.1719, 93.1309 },
				{ 273.2322, 186.9588, 1007.1719, 241.6757 },
				{ 268.7167, 185.5842, 1008.1719, 16.7470 },
				{ 0.0, 0.0, 0.0, 0.0 }
			})
		{
			EnableHeadShotsFromAllWeapons();
		}

		void Timer(int timerid, ZabawyTimerVector& vector)
		{
			int playerid = TimerDataGet<int>(vector, 0);
			if (Player[playerid].CurrentGameID == this)
			{
				safeGivePlayerWeapon(playerid, 24, 999999);
				fixSetPlayerHealth(playerid, 33);
				fixSetPlayerArmour(playerid, 0);
			}
		}

		void PutPlayerIntoGame(int playerid)
		{
			auto i = ArenaSpawns[Functions::RandomGenerator->Random(0, ArenaSpawns.size() - 1)];
			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 3, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
			CreateTimer<Arena>(333, false, &Arena::Timer, playerid);
		}

		bool OnKeepInGameSpawn(int playerid) 
		{ 
			PutPlayerIntoGame(playerid);
			return true; 
		}

		bool OnGameCommandExecute(int playerid, std::string params)
		{
			AddPlayer(playerid, true);
			PutPlayerIntoGame(playerid);
			return true;
		}

		bool OnPlayerDeath(int playerid, int killerid, int reason)
		{
			GivePlayerAchievement(playerid, EAM_ZabawyOneDeDeaths, 1);
			if (killerid != INVALID_PLAYER_ID)
			{
				GivePlayerAchievement(killerid, EAM_ZabawyOneDeKills, 1);
			}
			return true;
		}

		bool PlayerRequestGameExit(int playerid, int reason)
		{
			switch (reason)
			{
			case PlayerRequestExitTypeDisconnect:
				PlayerExitGame(playerid);
				break;
			case PlayerRequestExitTypeExit:
				PlayerExitGame(playerid);
				break;
			}
			return true;
		}
	} _Arena;
};