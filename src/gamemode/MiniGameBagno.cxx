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

namespace ZabawaBagno
{
	class Arena : public Base
	{
	public:
		std::vector<SObject> ArenaObjects;

		std::vector<SSpawnPoint> ArenaSpawns;

		Arena() :
			Base(L_games_ZabawaBagno_name, "/bag", eST_Zabawa),
			ArenaSpawns({
				{ -841.600891, -1927.452148, 15.468193 - 1.5 },
				{ -814.541687, -1892.961670, 13.060829 - 1.5 },
				{ -846.222107, -1994.340454, 22.958942 - 1.5 },
				{ -749.042114, -2079.430664, 13.991266 - 1.5 },
				{ -628.578857, -2075.217529, 35.179409 - 1.5 },
				{ -540.343140, -1990.068604, 50.301140 - 1.5 },
				{ -516.616699, -1854.756592, 20.246778 - 1.5 },
				{ -697.769592, -1980.905762, 21.750698 - 1.5 },
				{ -568.873901, -1955.021484, 43.900959 - 1.5 },
				{ -464.418518, -1959.853271, 28.210926 - 1.5 },
				{ -486.706085, -1862.618652, 15.474884 - 1.5 },
				{ -734.702698, -1849.344604, 17.479612 - 1.5 },
				{ -738.713440, -1990.025879, 13.385991 - 1.5 },
				{ -524.805786, -2050.407227, 63.765320 - 1.5 },
				{ -705.351501, -1894.697632, 12.595118 - 1.5 },
				{ -847.870605, -1960.490234, 18.614094 - 1.5 },
				{ -570.706299, -2026.760132, 51.745140 - 1.5 },
				{ -486.559357, -1996.343750, 57.442078 - 1.5 },
				{ -506.606323, -1933.665039, 36.658501 - 1.5 },
				{ -591.338684, -1893.240112, 10.630549 - 1.5 },
				{ 0.0f, 0.0f, 0.0f, 0.0f }
		}),
		ArenaObjects({
			{ 4007, -963.605164, -1937.068970, 65.855614, 90.2407954373, 1.71887338539, 0.000000 }, //object
			{ 4007, -489.001953, -1815.440430, 19.879099, 90.2407954373, 1.71887338539, -112.586149447 }, //object (1)
			{ 4007, -438.279358, -1895.857788, 0.106758, 90.2407954373, 1.71887338539, -160.714948013 }, //object (2)
			{ 4007, -439.646423, -1992.075928, 16.209236, 102.272909135, 1.71887338539, -186.498220681 }, //object (3)
			{ 4007, -463.182526, -2053.518799, 44.026093, 102.272909135, 1.71887338539, -200.249322356 }, //object (4)
			{ 4007, -528.099365, -2108.442871, 78.079132, 102.272909135, 1.71887338539, -238.064651426 }, //object (5)
			{ 4007, -628.638245, -2125.580322, 41.676231, 68.7548781199, 1.71887338539, -268.144649192 }, //object (6)
			{ 4007, -733.517395, -2132.998291, 16.663509, 93.6785422081, 1.71887338539, -268.144649192 }, //object (7)
			{ 4007, -808.726440, -2109.112305, 27.604811, 93.6785422081, 1.71887338539, -288.770957929 }, //object (8)
			{ 4007, -463.622223, -2053.119141, 70.004158, 79.927555125, 1.71887338539, -405.653316812 }, //object (9)
			{ 3268, -779.750061, -1979.642212, -1.789421, 0.000000, 0.000000, 0.000000 }, //object (10)
			{ 3277, -854.198669, -1970.104126, 15.025349, 0.000000, 0.000000, 0.000000 }, //object (11)
			{ 3277, -831.871704, -1915.648804, 10.867263, 0.000000, 0.000000, 0.000000 }, //object (12)
			{ 3277, -639.953064, -1985.103271, 44.831829, 0.000000, 0.000000, 0.000000 }, //object (13)
			{ 3277, -540.987427, -1895.017822, 7.102386, 0.000000, 0.000000, -206.265207318 }, //object (14)
			{ 3277, -515.012878, -1930.948242, 32.223057, 0.000000, 0.000000, -206.265207318 }, //object (15)
			{ 3277, -545.354492, -1966.806763, 49.544674, 0.000000, 0.000000, -206.265207318 }, //object (16)
			{ 3277, -720.785156, -2080.249756, 12.014405, 0.000000, 0.000000, -206.265207318 }, //object (17)
			{ 3279, -781.741699, -1980.231934, 5.599025, 0.000000, 0.000000, 0.000000 }, //object (18)
			{ 3279, -810.935547, -2038.830811, 20.661579, 0.000000, 0.000000, 0.000000 }, //object (19)
			{ 3279, -778.680969, -1877.685669, 8.584713, 0.000000, 0.000000, 0.000000 }, //object (20)
			{ 3279, -646.852905, -1862.885376, 15.956116, 0.000000, 0.000000, 0.000000 }, //object (21)
			{ 3279, -543.628174, -1958.481079, 48.097321, 0.000000, 0.000000, 0.000000 }, //object (22)
			{ 3279, -562.550720, -2064.846924, 52.614410, 0.000000, 0.000000, 0.000000 }, //object (23)
			{ 3279, -657.847290, -1993.664063, 23.982594, 0.000000, 0.000000, -156.417649958 }, //object (24)
			{ 3271, -663.555786, -2029.437500, 23.830029, -0.859436692696, -5.15662015618, 0.000000 }, //object (25)
			{ 3271, -755.497681, -1955.637085, -2.959515, -30.9397209371, -5.15662015618, -119.461642989 }, //object (26)
			{ 3271, -734.051758, -1966.656616, -4.063202, 40.3935245567, -5.15662015618, -119.461642989 }, //object (27)
			{ 3271, -727.239868, -1944.277466, -3.902932, 11.1726770051, 61.0199478857, -45.5500874171 }, //object (28)
			{ 2780, -830.641296, -1915.575684, 11.367262, 0.000000, 0.000000, 0.000000 }, //object (29)
			{ 2780, -726.320374, -1968.481201, 5.027439, 0.000000, 0.000000, 0.000000 }, //object (30)
			{ 2780, -744.244202, -1949.726074, 3.399573, 0.000000, 0.000000, 0.000000 }, //object (31)
			{ 2780, -762.271912, -1941.920898, 4.774159, 0.000000, 0.000000, 0.000000 }, //object (32)
			{ 2780, -753.387939, -1946.647095, 3.082401, 0.000000, 0.000000, 0.000000 }, //object (33)
			{ 2780, -735.986023, -1960.507080, 1.026917, 0.000000, 0.000000, 0.000000 }, //object (34)
			{ 2780, -464.611023, -1952.010498, 18.907621, 0.000000, 0.000000, 0.000000 }, //object (35)
			{ 2780, -515.017456, -1929.761108, 31.893953, 0.000000, 0.000000, 0.000000 }, //object (36)
			{ 2780, -666.377319, -2017.241333, 23.662821, 0.000000, 0.000000, 0.000000 }, //object (37)
			{ 3267, -782.188660, -1980.561157, 21.237595, 0.000000, 0.000000, 52.4255809587 }, //object (38)
			{ 3267, -735.966309, -1990.376465, 7.685651, 0.000000, 0.000000, 33.5180310152 }, //object (39)
			{ 3267, -740.267700, -1993.380737, 7.867673, 0.000000, 0.000000, 31.7991576298 }, //object (40)
			{ 3267, -706.874390, -1899.300537, 8.205574, 0.000000, 0.000000, 31.7991576298 }, //object (41)
			{ 990, -795.936218, -2041.829956, 13.182087, 0.000000, 0.000000, -51.566144266 }, //object (42)
			{ 7033, -577.003845, -1891.814941, 9.576965, 0.000000, 0.000000, -90.2407954373 }, //object (43)
			{ 4199, -684.769653, -1852.676270, 10.887839, 0.000000, 0.000000, 0.000000 }, //object (44)
			{ 1267, -657.463135, -1975.346802, 36.374992, 0.000000, 0.000000, 84.2247385885 }, //object (45)
			{ 1260, -518.100464, -1921.137085, 25.973856, 0.000000, 0.000000, 0.000000 }, //object (46)
			{ 3625, -848.636292, -1944.032593, 13.115816, 0.000000, 0.000000, -99.694599057 }, //object (47)
			{ 3625, -844.686035, -1945.933716, 13.060738, 0.000000, 0.000000, -189.076473464 }, //object (48)
			{ 3625, -846.398621, -1949.447998, 13.041664, 0.000000, 0.000000, -277.59833822 } }) //object (49)
		{}

		int AreaID;
		int CurrentGameState;
		int Counter;
		int GameTimer;

		void SetGameState(int state)
		{
			if (CurrentGameState != state)
			{
				CurrentGameState = state;
				NeedUpdate();
			}
		}

		void ResetGame()
		{
			DestroyTimerValidate(GameTimer);
			SetGameState(0);
		}

		void ResetGameFull(bool PlayersAlreadyInGame = false)
		{
			if (PlayersAlreadyInGame)
			{
				while (PlayersInGame.size())
				{
					CheckWinner(*PlayersInGame.begin(), true);
				}
			}
			else
			{
				while (PlayersInGame.size())
				{
					PlayerExitGame(*PlayersInGame.begin());
				}
			}

			if (CurrentGameState)
			{
				ResetGame();
			}
		}

		bool OnGameModeInit() override
		{
			MakeAllDynamicObjects(ArenaObjects, CurrentGameVW);
			AreaID = StreamerLibrary::CreateDynamicRectangle(-989.814514, -2198.492920, -386.414612, -1698.104370, CurrentGameVW);

			SetGameState(0);
			Counter = 0;
			GameTimer = INVALID_TIMER_ID;

			return true;
		}

		void CheckWinner(int playerid, bool nowinner = false)
		{
			PlayerExitGame(playerid);

			if (CurrentGameState == 3 && PlayersInGame.size() > 1)
			{
				GivePlayerAchievement(playerid, EAM_ZabawyBAGLosses, 1);
			}

			if (PlayersInGame.size() > 1)
			{
				return;
			}

			if (PlayersInGame.size() == 1)
			{
				playerid = *PlayersInGame.begin();

				if (!nowinner)
				{
					fixSendClientMessageToAllF(Color::COLOR_INFO3, L_mm_bag_announce_winner, true, true, Player[playerid].PlayerName.c_str());
					Player[playerid].GiveExperienceKill(playerid);
					Player[playerid].GiveScore(3);
					GivePlayerAchievement(playerid, EAM_ZabawyBAGWins, 1);
				}

				PlayerExitGame(playerid);
				
			}
			ResetGame();
		}

		bool PlayerRequestGameExit(int playerid, int reason) override
		{
			switch (reason)
			{
			case PlayerRequestExitTypeDeath:
				CheckWinner(playerid);
				break;
			case PlayerRequestExitTypeDisconnect:
				CheckWinner(playerid);
				break;
			case PlayerRequestExitTypeExit:
				CheckWinner(playerid);
				break;
			}
			return true;
		}

		bool PlayerRequestCencelStaging(int playerid, int reason) override
		{
			PlayerExitGame(playerid);
			return true;
		}

		bool OnPlayerLeaveDynamicArea(int playerid, int areaid) override
		{
			if (AreaID == areaid)
			{
				CheckWinner(playerid);
			}
			return true;
		}

		bool OnPlayerDeath(int playerid, int killerid, int reason) override
		{
			CheckWinner(playerid);
			GivePlayerAchievement(playerid, EAM_ZabawyBAGDeaths, 1);
			if (killerid != INVALID_PLAYER_ID)
			{
				GivePlayerAchievement(killerid, EAM_ZabawyBAGKills, 1);
			}
			return true;
		}

		void PutPlayerIntoGame(int playerid)
		{
			AddStagingPlayer(playerid, 0xFFFFFF00);
			auto i = ArenaSpawns[Functions::RandomGenerator->Random(0, ArenaSpawns.size() - 1)];
			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
			fixTogglePlayerControllable(playerid, false);
		}

		bool OnGameCommandExecute(int playerid, std::string params) override
		{
			if (CurrentGameState == 0)
			{
				SetGameState(1);
				SwitchGameTimer(GameTimer, CreateTimer<Arena>(15000, true, &Arena::ArenaTick));
				fixSendClientMessageToAll(Color::COLOR_INFO3, L_mm_bag_startcd);
			}

			SetPlayerStaging(playerid);

			SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_bag_staging);

			return true;
		}

		const int ARENA_MIN_PLAYERS = 2;

		void ArenaTick(int timerid, ZabawyTimerVector& vector)
		{
			switch (CurrentGameState)
			{
				case 1:
				{
					Counter = 0;
					if (PlayersInGame.size() < ARENA_MIN_PLAYERS)
					{
						ResetGameFull();
					}
					else
					{
						SetGameState(2);

						for (auto i : GetPlayersCopy())
						{
							PutPlayerIntoGame(i);
						}

						SwitchGameTimer(GameTimer, CreateTimer<Arena>(1000, true, &Arena::ArenaTick));
					}
				}
				break;

				case 2:
				{
					if (PlayersInGame.size() < ARENA_MIN_PLAYERS)
					{
						ResetGameFull();
					}
					else
					{
						++Counter;
						if (Counter < 6)
						{
							std::string CountDown(Functions::string_format("%d", 6 - Counter));
							for (auto i : PlayersInGame)
							{
								if (Counter == 5)
								{
									safeGivePlayerWeapon(i, 24, 999999);
									safeGivePlayerWeapon(i, 31, 999999);
									safeGivePlayerWeapon(i, 34, 999999);
									fixSetPlayerHealth(i, 100);
									fixSetPlayerArmour(i, 0);
									SetCameraBehindPlayer(i);
								}
								GameTextForPlayer(i, CountDown.c_str(), 1000, 6);
							}
						}
						else if (Counter == 6)
						{
							for (auto i : GetPlayersCopy())
							{
								GameTextForPlayer(i, "GO!", 500, 6);
								fixTogglePlayerControllable(i, true);
								if (!StreamerLibrary::IsPlayerInDynamicArea(i, AreaID, 1))
								{
									CheckWinner(i, true);//PlayersInGame.erase
								}
							}
							if (PlayersInGame.size() < ARENA_MIN_PLAYERS)
							{
								ResetGameFull();
							}
							else
							{
								SetGameState(3);
							}
						}
					}
				}
				break;

				case 3:
				{
					++Counter;

					for (auto i : GetPlayersCopy())
					{
						if (Player[i].CurrentInactivityTime > 1750)
						{
							CheckWinner(i);
							SendClientMessage(i, Color::COLOR_ERROR, L_minigame_inactivity);
						}
					}
				
					if (Counter > 180)
					{
						ResetGameFull(true);
					}
				}
				break;

				default:
				{
					ResetGameFull();
				}
				break;
			}
		}

		bool Joinable() override
		{
			return (CurrentGameState == 0 || CurrentGameState == 1) && PlayersInGame.size() < (ArenaSpawns.size() - 1);
		}

		bool Staging() override
		{
			return CurrentGameState == 1;
		}
	}
	_Arena;
};