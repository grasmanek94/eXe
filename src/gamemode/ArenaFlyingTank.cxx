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

#if 0
using namespace Zabawy;
namespace FlyingTank
{
	class Arena : public Base
	{
	public:
		std::vector<SSpawnPoint> ArenaSpawns;
		std::vector<SObject> ArenaObjects;
		Arena() :
			Base(L_games_ArenaFlyingTank_name, "/lsc"),
			ArenaSpawns({
				{ 932.2000100, 877.9000200, 13.5000000, 96.0000000 },
				{ 925.2000100, 934.0000000, 17.3000000, 95.9990000 },
				{ 906.2999900, 1006.0000000, 17.3000000, 95.9990000 },
				{ 863.5000000, 1078.4000000, 33.0000000, 137.9990000 },
				{ 774.9000200, 1115.9000000, 33.0000000, 183.9940000 },
				{ 695.4000200, 1089.0000000, 33.0000000, 209.9930000 },
				{ 617.5000000, 1077.2000000, 33.0000000, 185.9930000 },
				{ 523.2000100, 1050.2000000, 33.0000000, 209.9880000 },
				{ 413.8999900, 979.5000000, 38.0000000, 251.9870000 },
				{ 406.2999900, 868.2999900, 38.0000000, 291.9820000 },
				{ 469.2999900, 771.7999900, 38.0000000, 333.9780000 },
				{ 588.9000200, 723.9000200, 38.0000000, 351.9730000 },
				{ 712.0000000, 708.9000200, 38.0000000, 17.9690000 },
				{ 829.0999800, 749.2999900, 38.0000000, 41.9680000 },
				{ 895.5000000, 814.7999900, 19.2000000, 65.9680000 },
				{ 0.0, 0.0, 0.0, 0.0 }
			}),
			ArenaObjects({
				{5002, 963.4000200, 897.2999900, 107.6000000, 0.0000000, 266.0000000, 0.0000000 }, //Object number 0
				{ 5002, 941.4000200, 1006.7000000, 107.6000000, 0.0000000, 265.9950000, 18.0000000 }, //Object number 1
				{ 5002, 888.4000200, 1105.6000000, 107.6000000, 0.0000000, 265.9950000, 33.9960000 }, //Object number 2
				{ 5002, 797.9000200, 1162.6000000, 107.6000000, 0.0000000, 265.9950000, 69.9920000 }, //Object number 3
				{ 5002, 704.0000000, 1156.3000000, 107.6000000, 0.0000000, 265.9950000, 101.9880000 }, //Object number 4
				{ 5002, 597.5999800, 1128.6000000, 107.6000000, 0.0000000, 265.9950000, 105.9860000 }, //Object number 5
				{ 5002, 491.7999900, 1090.5000000, 107.6000000, 0.0000000, 265.9950000, 111.9850000 }, //Object number 6
				{ 5002, 407.7000100, 1020.1000000, 107.6000000, 0.0000000, 265.9950000, 139.9840000 }, //Object number 7
				{ 5002, 345.2000100, 922.2999900, 107.6000000, 0.0000000, 265.9950000, 151.9820000 }, //Object number 8
				{ 5002, 355.5000000, 822.5999800, 107.6000000, 0.0000000, 265.9950000, 203.9790000 }, //Object number 9
				{ 5002, 428.7999900, 739.5000000, 107.6000000, 0.0000000, 265.9950000, 231.9780000 }, //Object number 10
				{ 5002, 513.5000000, 686.2999900, 107.6000000, 0.0000000, 265.9950000, 239.9760000 }, //Object number 11
				{ 5002, 622.4000200, 664.0999800, 107.6000000, 0.0000000, 265.9950000, 269.9740000 }, //Object number 12
				{ 5002, 737.5999800, 676.4000200, 107.6000000, 0.0000000, 265.9950000, 279.9730000 }, //Object number 13
				{ 5002, 843.0999800, 721.7000100, 107.6000000, 0.0000000, 265.9950000, 301.9700000 }, //Object number 14
				{ 5002, 931.9000200, 795.2000100, 107.6000000, 0.0000000, 265.9950000, 313.9650000 }, //Object number 15
				{ 5002, 876.9000200, 902.4000200, 200.3400000, 0.0000000, 180.0000000, 0.0000000 }, //Object number 16
				{ 5002, 874.9000200, 1021.6000000, 200.3300000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 17
				{ 5002, 805.4000200, 1118.0000000, 200.3200000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 18
				{ 5002, 671.7999900, 998.4000200, 200.3100000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 19
				{ 5002, 599.7999900, 1109.1000000, 200.3000000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 20
				{ 5002, 855.2999900, 796.2000100, 200.2000000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 21
				{ 5002, 670.9000200, 881.2999900, 200.2100000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 22
				{ 5002, 726.2999900, 762.0999800, 200.2200000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 23
				{ 5002, 668.0999800, 729.5000000, 200.2300000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 24
				{ 5002, 473.8999900, 1041.2000000, 200.2400000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 25
				{ 5002, 463.2000100, 925.0000000, 200.2900000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 26
				{ 5002, 459.0000000, 802.2999900, 200.2800000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 27
				{ 5002, 546.5000000, 762.0999800, 200.2500000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 28
				{ 5002, 459.5000000, 746.7000100, 200.2600000, 0.0000000, 179.9950000, 0.0000000 }, //Object number 29
				{ 5002, 436.7000100, 917.4000200, 200.2700000, 0.0000000, 179.9950000, 0.0000000 } //Object number 30
			})
		{}

		bool IsExe24PlusGame() 
		{ 
			return true; 
		}

		bool OnGameModeInit()
		{
			MakeAllDynamicObjects(ArenaObjects, CurrentGameVW);
			return true;
		}

		int GetCommandRestrictions() 
		{ 
			return RESTRICTION_IN_VEHICLE_OR_ONFOOT | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_USING_EXE24PLUS;
		}

		void Timer(int timerid, ZabawyTimerVector& vector)
		{
			int playerid = TimerDataGet<int>(vector, 0);
			if (Player[playerid].CurrentGameID == this)
			{
				CreatePlayerVehicleSafe(playerid, 432, 0.0, 0.0, 1.0);
				fixTogglePlayerControllable(playerid, true);
			}
		}

		void PutPlayerIntoGame(int playerid)
		{
			fixSetPlayerHealth(playerid, 100);
			fixSetPlayerArmour(playerid, 100);

			TogglePlayerFly(playerid, true);
			CheckSet(CHECK_SPEED, playerid, false);
			CheckSet(CHECK_FLYHACK, playerid, false);
			CheckSet(CHECK_AIRBREAK, playerid, false);
			CheckSet(CHECK_TELEPORT, playerid, false);

			auto i = ArenaSpawns[Functions::RandomGenerator->Random(0, ArenaSpawns.size() - 1)];
			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);

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
			//compensate for loss
			if (killerid == INVALID_PLAYER_ID)
			{
				Player[playerid].GiveScore(1);
				--Player[playerid].statistics.deaths;
				--Player[playerid].statistics.suicides;
			}
			return true;
		}

		bool PlayerRequestGameExit(int playerid, int reason)
		{
			switch (reason)
			{
			case PlayerRequestExitTypeDisconnect:
				TogglePlayerFly(playerid, false);
				CheckSet(CHECK_SPEED, playerid, true);
				CheckSet(CHECK_FLYHACK, playerid, true);
				CheckSet(CHECK_AIRBREAK, playerid, true);
				CheckSet(CHECK_TELEPORT, playerid, true);
				PlayerExitGame(playerid);
				break;
			case PlayerRequestExitTypeExit:
				TogglePlayerFly(playerid, false);
				CheckSet(CHECK_SPEED, playerid, true);
				CheckSet(CHECK_FLYHACK, playerid, true);
				CheckSet(CHECK_AIRBREAK, playerid, true);
				CheckSet(CHECK_TELEPORT, playerid, true);
				PlayerExitGame(playerid);
				break;
			}
			return true;
		}
	} 
	_Arena//nie wiem czy zakomentowaæ aby wy³¹czyæ t¹ arenê..
		;
};
#endif