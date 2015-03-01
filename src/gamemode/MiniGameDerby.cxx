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
namespace ZabawaDerby
{
	class Arena : public Base
	{
	public:
		struct DerbyMap
		{
			int ModelID;
			int AreaID;
			std::vector<SSpawnPoint> ArenaSpawns;
			glm::vec3 AreaMin;
			glm::vec3 AreaMax;
			std::vector<SObject> ArenaObjects;
		};

		unsigned long current_map;
		std::vector<DerbyMap> DerbyMaps;

		std::array<int, MAX_PLAYERS> PlayerSpawnPoint;

		Arena() :
			Base(L_games_ZabawaDerby_name, "/drb", eST_Zabawa, cmd_alias({"/derby"})),
			current_map(0),
			DerbyMaps(
		{
			/////map 1///////////////////////////////////////////////
			{
				495,
				0,
				std::vector<SSpawnPoint>({
					{ -427.4330, -4216.0894, 102.7602, 0.0 },
					{ -409.1801, -4123.6689, 102.7602, 90.0 },
					{ -476.3754, -4095.3752, 102.7600, 180.0 },
					{ -489.0786, -4204.6670, 102.7601, 270.0 },
					{ -449.5252, -4215.7070, 102.7602, 0.0 },
					{ -409.2461, -4150.2163, 102.7603, 90.0 },
					{ -449.0996, -4096.1602, 102.7600, 180.0 },
					{ -490.4264, -4175.9795, 102.7600, 270.0 },
					{ -476.3982, -4210.7144, 102.7571, 0.0 },
					{ -409.2839, -4176.1714, 102.7603, 90.0 },
					{ -427.0610, -4096.2554, 102.7601, 180.0 },
					{ -488.8577, -4123.3794, 102.7602, 270.0 },
					{ -409.3241, -4204.0166, 102.7603, 90.0 },
					{ -488.8786, -4150.1509, 102.7602, 270.0 },
					{ 0.0f, 0.0f, 0.0f, 0.0f }
				}),
				{ -550.0, -4280.0, 50.0 },
				{ -350.0, -4050.0, 150.0 },
				std::vector<SObject>({
					{ 3458, -426.8999900, -4176.2998000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 0
					{ 3458, -427.2000100, -4153.7002000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 1
					{ 3458, -427.3999900, -4198.7998000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 2
					{ 3458, -426.5000000, -4150.1001000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 3
					{ 3458, -426.3999900, -4123.6001000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 4
					{ 3458, -427.2000100, -4113.5000000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 5
					{ 3458, -427.0000000, -4204.2998000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 6
					{ 3458, -449.5000000, -4198.7002000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 7
					{ 3458, -449.1000100, -4153.8999000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 8
					{ 3458, -467.1000100, -4176.2998000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 9
					{ 3458, -449.1000100, -4113.6001000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 10
					{ 3458, -471.6000100, -4123.5000000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 11
					{ 3458, -471.5000000, -4150.1001000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 12
					{ 3458, -471.8999900, -4204.7998000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 13
					{ 3458, -476.6000100, -4112.7998000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 14
					{ 3458, -476.6000100, -4153.0000000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 15
					{ 3458, -476.7000100, -4193.2998000, 100.4000000, 0.0000000, 0.0000000, 270.0000000 }, //Object number 16
					{ 3458, -473.3999900, -4176.2002000, 100.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 17
					{ 7073, -487.2999900, -4106.2998000, 106.2000000, 0.0000000, 0.0000000, 320.0000000 }, //Object number 18
					{ 7073, -412.6000100, -4220.1001000, 104.7000000, 0.0000000, 0.0000000, 319.9990000 }, //Object number 19
					{ 1247, -463.8999900, -4189.3999000, 108.9000000, 0.0000000, 0.0000000, 36.7500000 }, //Object number 20
					{ 1212, -415.8999900, -4136.8999000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 21
					{ 1212, -414.7999900, -4163.2002000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 22
					{ 1212, -414.2000100, -4189.8999000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 23
					{ 1212, -437.2000100, -4213.1001000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 24
					{ 1212, -462.5000000, -4216.8999000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 25
					{ 1212, -485.0000000, -4192.7998000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 26
					{ 1212, -487.1000100, -4167.2002000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 27
					{ 1212, -486.1000100, -4136.8999000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 28
					{ 1212, -465.7000100, -4109.3999000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 29
					{ 1212, -438.5000000, -4106.8999000, 102.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 30
					{ 1240, -395.2999900, -4150.5000000, 110.1000000, 0.0000000, 0.0000000, 92.2500000 }, //Object number 31
					{ 1240, -499.7999900, -4150.7002000, 110.1000000, 0.0000000, 0.0000000, 90.9970000 }
				})},
				/////map 2///////////////////////////////////////////////
				{
					415,
					0,
					std::vector<SSpawnPoint>(
					{
						{ 3387.5012207, -1961.4083252, 54.8453412, 180.0787964 },
						{ 3361.0510254, -1980.0303955, 54.6203403, 294.0769043 },
						{ 3462.7250977, -2057.8564453, 54.6203403, 356.0668945 },
						{ 3421.4091797, -1997.2432861, 54.6203403, 114.0765381 },
						{ 3399.6987305, -2058.6416016, 54.6203403, 0.0714111 },
						{ 3425.3320312, -2043.1748047, 54.6203403, 180.0714111 },
						{ 3455.0332031, -2058.3505859, 54.6203365, 356.0668945 },
						{ 3416.2524414, -1984.9276123, 54.6203403, 114.0765381 },
						{ 3410.7949219, -2058.8908691, 54.6203403, 0.0764160 },
						{ 3393.7509766, -1961.4967041, 54.8453412, 180.0769043 },
						{ 3365.5695801, -1990.8753662, 54.6203403, 294.0765381 },
						{ 3449.1645508, -2058.7512207, 54.6203403, 356.0714111 },
						{ 3437.9885254, -2043.0209961, 54.6203403, 180.0714111 },
						{ 3419.0817871, -1991.9316406, 54.6203403, 114.0765381 },
						{ 3405.0000000, -2058.7270508, 54.6203403, 0.0714111 },
						{ 3432.4387207, -2042.9602051, 54.6203403, 180.0714111 },
						{ 3363.4782715, -1985.0949707, 54.6203403, 294.0765381 },
						{ 3381.0012207, -1961.2591553, 54.8453412, 180.0787964 },		
						{ 0.0f, 0.0f, 0.0f, 0.0f }
					}),
					{ 3312.7693, -2141.0283, 33.0 },
					{ 3502.6235, -1942.5770, 95.0 },
					std::vector<SObject>({
						{ 8557, 3479.4694824, -2050.1779785, 0.0000000 + 50, 0.0000000, 0.0000000, 0.0000000 }, //object(vgshseing27) (1)
						{ 8557, 3439.2412109, -2050.2255859, 0.0000000 + 50, 0.0000000, 0.0000000, 0.0000000 }, //object(vgshseing27) (2)
						{ 8557, 3399.1174316, -2050.2727051, 0.0000000 + 50, 0.0000000, 0.0000000, 0.0000000 }, //object(vgshseing27) (3)
						{ 8557, 3360.0925293, -2050.3056641, 8.8500099 + 50, 0.0000000, 26.0000000, 0.0000000 }, //object(vgshseing27) (4)
						{ 8557, 3393.8796387, -2028.3476562, 7.0499959 + 50, 0.0000000, 19.9951172, 270.0000000 }, //object(vgshseing27) (5)
						{ 8557, 3393.8037109, -1991.4365234, 7.0499959 + 50, 0.0000000, 339.9896240, 270.2500000 }, //object(vgshseing27) (6)
						{ 8557, 3381.5620117, -2027.9809570, 0.0000000 + 50, 0.0000000, 0.0000000, 90.0000000 }, //object(vgshseing27) (7)
						{ 8557, 3399.1325684, -2005.3372803, 0.0000000 + 50, 0.0000000, 0.0000000, 0.0000000 }, //object(vgshseing27) (8)
						{ 8557, 3408.7299805, -1988.2521973, 0.0000000 + 50, 0.0000000, 0.0000000, 114.0000000 }, //object(vgshseing27) (9)
						{ 8557, 3384.4658203, -1970.0107422, 0.0000000 + 50, 0.0000000, 0.0000000, 181.9940186 }, //object(vgshseing27) (10)
						{ 8557, 3496.9475098, -2047.2518311, 0.0000000 + 50, 0.0000000, 326.0000000, 90.0000000 }, //object(vgshseing27) (11)
						{ 8557, 3496.9526367, -2011.1873779, 11.0249958 + 50, 0.0000000, 359.9973145, 90.0000000 }, //object(vgshseing27) (12)
						{ 8557, 3474.3869629, -1992.9124756, 11.0249958 + 50, 0.0000000, 359.9945068, 178.0000000 }, //object(vgshseing27) (13)
						{ 8557, 3434.5573730, -1991.5343018, 11.0249958 + 50, 0.0000000, 359.9890137, 177.9949951 }, //object(vgshseing27) (14)
						{ 8557, 3395.3991699, -1989.8962402, 19.1999207 + 50, 0.0000000, 335.9890137, 177.9949951 }, //object(vgshseing27) (15)
						{ 8557, 3359.2583008, -1988.6492920, 20.3749256 + 50, 0.0000000, 19.9838867, 177.9895020 }, //object(vgshseing27) (16)
						{ 8557, 3335.9870605, -2005.4429932, 14.0499153 + 50, 0.0000000, 357.9786377, 265.9895020 }, //object(vgshseing27) (17)
						{ 8557, 3333.2954102, -2045.2817383, 15.5249157 + 50, 0.0000000, 357.9785156, 265.9844971 }, //object(vgshseing27) (19)
						{ 8557, 3330.4765625, -2085.5166016, 16.9499207 + 50, 0.0000000, 357.9785156, 265.9790039 }, //object(vgshseing27) (20)
						{ 8557, 3339.8457031, -2067.8867188, 17.3749828 + 50, 0.0000000, 359.4946289, 88.7475586 }, //object(vgshseing27) (22)
						{ 8557, 3335.5310059, -2067.7416992, 16.5749950 + 50, 338.0009155, 359.4549561, 88.5433350 }, //object(vgshseing27) (22)
						{ 8557, 3373.2744141, -1988.1955566, 0.0000000 + 50, 0.0000000, 0.0000000, 291.9940186 }, //object(vgshseing27) (10)
						{ 8557, 3347.1237793, -2104.4038086, 18.0999031 + 50, 0.0000000, 357.9785156, 355.9790039 }, //object(vgshseing27) (20)
						{ 8557, 3380.3623047, -2069.5659180, 12.7749157 + 50, 358.4236450, 37.9954529, 87.2045593 }, //object(vgshseing27) (20)
						{ 8557, 3378.0964355, -2103.3696289, 19.4249020 + 50, 356.3957825, 343.9412537, 84.9363403 }, //object(vgshseing27) (20)
						{ 8557, 3364.5141602, -2105.2592773, 18.5748959 + 50, 0.0000000, 355.7285156, 355.9735107 }, //object(vgshseing27) (20)
						{ 16000, 3456.2758789, -2052.6247559, 1.6999995 + 50, 69.9626465, 356.3496704, 3.6799927 }, //object(drvin_screen) (1)
						{ 16000, 3405.0395508, -2052.4704590, 1.4749997 + 50, 70.4902039, 1.4921875, 358.8366699 }, //object(drvin_screen) (2)
						{ 16000, 3432.1611328, -2048.0263672, 1.5749996 + 50, 69.9969177, 359.2662659, 180.9320068 }, //object(drvin_screen) (3)
						{ 16000, 3413.7277832, -1993.2775879, 1.5749996 + 50, 69.9938965, 359.2639160, 114.6783447 }, //object(drvin_screen) (4)
						{ 16000, 3388.7365723, -1967.3057861, 1.5749996 + 50, 69.9884033, 359.2639160, 182.4252930 }, //object(drvin_screen) (5)
						{ 16000, 3368.8613281, -1984.0828857, 1.5749996 + 50, 69.9829102, 359.2639160, 291.6724854 }, //object(drvin_screen) (6)
					})
				}
			}
		){}

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
			for (auto &map : DerbyMaps)
			{
				MakeAllDynamicObjects(map.ArenaObjects, CurrentGameVW);
				map.AreaID = StreamerLibrary::CreateDynamicCube(
					std::min(map.AreaMin.x, map.AreaMax.x), 
					std::min(map.AreaMin.y, map.AreaMax.y), 
					std::min(map.AreaMin.z, map.AreaMax.z),
					std::max(map.AreaMin.x, map.AreaMax.x),
					std::max(map.AreaMin.y, map.AreaMax.y),
					std::max(map.AreaMin.z, map.AreaMax.z),
					CurrentGameVW);
			}

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
				GivePlayerAchievement(playerid, EAM_ZabawyDerbyLosses, 1);
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
					fixSendClientMessageToAllF(Color::COLOR_INFO3, L_mm_drb_announce_winner, true, true, Player[playerid].PlayerName.c_str());
					Player[playerid].GiveExperienceKill(playerid);
					Player[playerid].GiveScore(3);
					GivePlayerAchievement(playerid, EAM_ZabawyDerbyWins, 1);
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
			if (DerbyMaps[current_map].AreaID == areaid)
			{
				CheckWinner(playerid);
			}
			return true;
		}

		void PutPlayerIntoGame(int playerid)
		{
			AddStagingPlayer(playerid);
			fixSetPlayerHealth(playerid, 10000000.0);
			PlayerSpawnPoint[playerid] = std::distance(PlayersInGame.begin(), PlayersInGame.find(playerid));
			auto i = DerbyMaps[current_map].ArenaSpawns[PlayerSpawnPoint[playerid]];
			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
			fixTogglePlayerControllable(playerid, false);
		}

		bool OnGameCommandExecute(int playerid, std::string params) override
		{
			if (CurrentGameState == 0)
			{
				SetGameState(1);
				SwitchGameTimer(GameTimer, CreateTimer<Arena>(15000, true, &Arena::ArenaTick));
				fixSendClientMessageToAll(Color::COLOR_INFO3, L_mm_drb_startcd);

				if (!Functions::is_number_unsigned(params, current_map))//valid number provided?
				{//no
					if (params[0] == 'R' || params[0] == 'r')//random
					{
						current_map = Functions::RandomGenerator->Random(0, DerbyMaps.size());
					}
					else//just go to next map
					{
						++current_map;
					}
				}

				if (current_map >= DerbyMaps.size())//but make sure we don't make an IOOB error/crash
				{
					current_map = 0;
				}
			}

			SetPlayerStaging(playerid);

			SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_drb_staging);
			
			return true;
		}

		bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) override
		{
			if (newkeys == 1 || newkeys == 9 || newkeys == 33 && oldkeys != 1 || oldkeys != 9 || oldkeys != 33)
			{
				AddVehicleComponent(Player[playerid].CurrentVehicle, 1010);
			}
			return true;
		}

		const int ARENA_MIN_PLAYERS = 2;

		void ArenaTick(int timerid, ZabawyTimerVector& vector)
		{
			switch(CurrentGameState)
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
								if (Counter == 1)
								{
									CreatePlayerVehicleSafe(i, DerbyMaps[current_map].ModelID, 0.0, 0.0, 0.0);
									SetCameraBehindPlayer(i);
								}else
								if (Counter == 2)
								{
									auto u = DerbyMaps[current_map].ArenaSpawns[PlayerSpawnPoint[i]];
									TeleportPlayer(i, u.x, u.y, u.z, u.a, true, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
								}else
								if (Counter == 3)
								{
									if (!GetPlayerVehicleID(i))
									{
										CreatePlayerVehicleSafe(i, DerbyMaps[current_map].ModelID, 0.0, 0.0, 0.0);
										SetCameraBehindPlayer(i);
									}
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
								if (!StreamerLibrary::IsPlayerInDynamicArea(i, DerbyMaps[current_map].AreaID, 1) || !GetPlayerVehicleID(i))
								{
									CheckWinner(i, true);
								}
							}						
							if (PlayersInGame.size() < ARENA_MIN_PLAYERS)
							{
								ResetGameFull(true);
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
					if (++Counter > 180)
					{
						ResetGameFull(true);
					}
					else
					{
						for (auto i : GetPlayersCopy())
						{
							if (Player[i].CurrentInactivityTime > 1750)
							{
								CheckWinner(i);
								SendClientMessage(i, Color::COLOR_ERROR, L_minigame_inactivity);
							}
						}
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

		bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override
		{
			if (CurrentGameState == 3)
			{
				if (oldstate == PLAYER_STATE_DRIVER && newstate != PLAYER_STATE_DRIVER)
				{
					CheckWinner(playerid);
					if (!PlayersInGame.size())
					{
						ResetGameFull();
					}
				}
			}
			return true;
		}

		bool Joinable() override
		{
			return (CurrentGameState == 0 || CurrentGameState == 1) && PlayersInGame.size() < (DerbyMaps[current_map].ArenaSpawns.size() - 1);
		}

		bool Staging() override
		{
			return CurrentGameState == 1;
		}

		int GetCommandRestrictions() override
		{ 
			return RESTRICTION_IN_VEHICLE_OR_ONFOOT | RESTRICTION_NOT_IN_A_GAME; 
		}
	} 
	_Arena
		;
};