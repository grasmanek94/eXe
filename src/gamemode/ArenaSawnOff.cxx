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

namespace SawnOff
{
	class Arena : public Base
	{
	public:

		std::vector<SSpawnPoint> ArenaSpawns;

		Arena() :
			Base(L_games_ArenaSawnOff_name, "/sof"),
			ArenaSpawns({
				{ 246.9347, 1859.4136, 14.0840, 11.3835 },
				{ 241.7209, 1832.2262, 4.7109, 215.6556 },
				{ 214.5250, 1817.9489, 6.4141, 291.1698 },
				{ 256.4714, 1801.8352, 7.5259, 73.0877 },
				{ 263.7035, 1820.6194, 2.2109, 180.8519 },
				{ 296.2002, 1818.4178, 1.5412, 89.3578 },
				{ 0.0f, 0.0f, 0.0f, 0.0f }
		})
		{}

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

		bool OnPlayerDeath(int playerid, int killerid, int reason)
		{
			GivePlayerAchievement(playerid, EAM_ZabawySawnOffDeaths, 1);
			if (killerid != INVALID_PLAYER_ID)
			{
				GivePlayerAchievement(killerid, EAM_ZabawySawnOffKills, 1);
			}
			return true;
		}

		void PutPlayerIntoGame(int playerid)
		{
			fixSetPlayerHealth(playerid, 100);
			fixSetPlayerArmour(playerid, 100);

			auto i = ArenaSpawns[Functions::RandomGenerator->Random(0, ArenaSpawns.size() - 1)];
			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
			CreateTimer<Arena>(333, false, &Arena::Timer, playerid);			
		}

		void Timer(int timerid, ZabawyTimerVector& vector)
		{
			int playerid = TimerDataGet<int>(vector, 0);
			if (Player[playerid].CurrentGameID == this)
			{
				safeGivePlayerWeapon(playerid, 26, 999999);
			}
		}

		bool OnKeepInGameSpawn(int playerid)
		{
			PutPlayerIntoGame(playerid);
			return true;
		}

		bool OnGameCommandExecute(int playerid, std::string params)
		{
			AddPlayer(playerid, true, 0xFFFFFF00);
			PutPlayerIntoGame(playerid);
			return true;
		}
	} _Arena;
}