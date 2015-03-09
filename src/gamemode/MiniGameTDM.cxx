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

namespace ZabawaTDM
{
	class Arena : public Base
	{
	public:

		std::vector<SObject> ArenaObjects;

		static const size_t													ZABAWA_MAX_TEAMS = 2;
		std::array<std::set<int>, ZABAWA_MAX_TEAMS>							PlayersInTeam;
		std::set<unsigned int>												ActiveTeams;
		std::array<std::pair<int, language_string_ids>, ZABAWA_MAX_TEAMS>	TeamInfo;
		std::array<std::vector<SSpawnPoint>, ZABAWA_MAX_TEAMS>				ArenaSpawns;

		Arena() :
			Base(L_games_ZabawaTDM_name, "/tdm", eST_Zabawa, cmd_alias({"/wg"})),
		ArenaSpawns({{
			{
				{ 73.9193, 1936.9520, 17.6406, 4.3867 }, // #1
				{ 65.3205, 1937.1730, 17.6406, 357.1800 }, // #1
				{ 56.7282, 1937.8009, 17.6406, 357.1800 }, // #1
				{ 49.1715, 1937.6099, 17.6406, 87.1800 }, // #1
				{ 40.9936, 1938.3157, 17.6406, 357.1800 }, // #1
				{ 32.4945, 1938.8628, 17.6406, 357.1800 }, // #1
				{ 22.0260, 1939.7169, 17.6406, 357.1800 }, // #1
				{ 12.1865, 1940.4569, 17.6406, 357.1800 }, // #1
				{ 5.5747, 1940.6964, 17.6406, 357.1800 }, // #1
				{ 10.5868, 1949.1926, 17.6406, 357.1800 }, // #1
				{ 27.2152, 1948.8746, 17.6406, 351.8533 }, // #1
				{ 0.0f, 0.0f, 0.0f, 0.0f }
			},
			{
				{ 75.4366, 2040.3713, 17.6602, 157.5848 }, // #2
				{ 49.4671, 2044.6812, 17.6991, 161.6582 }, // #2
				{ 57.5081, 2045.2589, 17.7036, 190.7985 }, // #2
				{ 40.0514, 2048.8677, 17.7400, 190.1718 }, // #2
				{ 30.4032, 2046.1571, 17.7135, 181.3983 }, // #2
				{ 21.3521, 2048.2942, 17.7361, 169.4915 }, // #2
				{ 9.8482, 2043.7567, 17.7055, 173.2516 },  // #2
				{ 9.2036, 2050.0300, 17.7719, 183.9048 },  // #2
				{ 66.0341, 2032.9832, 17.6473, 182.0248 }, // #2
				{ 15.3379, 2042.4777, 17.8121, 271.3492 }, // #2
				{ 0.0f, 0.0f, 0.0f, 0.0f }
			}
		}}),
		TeamInfo({{
			{ 0xFF000000, L_mm_tdm_team_red },
			{ 0x0000FF00, L_mm_tdm_team_blue }
		}}),
		ArenaObjects({
			{ 7368, 38.942177, 1991.703857, 19.734196, 0.0000, 0.0000, 0.0000 },
			{ 7368, 44.061363, 1992.137207, 19.734196, 0.0000, 0.0000, 180.0000 },
			{ 12918, 69.275696, 1981.110107, 16.633276, 0.0000, 0.0000, 0.0000 },
			{ 3279, 26.967194, 1960.815430, 16.716242, 0.0000, 0.0000, 270.0000 },
			{ 3279, 57.332413, 1961.425659, 16.716242, 0.0000, 0.0000, 270.0000 },
			{ 3279, 26.967115, 2025.010742, 16.716242, 0.0000, 0.0000, 90.0000 },
			{ 3279, 57.374405, 2024.747681, 16.716242, 0.0000, 0.0000, 90.0001 },
			{ 12918, 13.308914, 2006.776489, 16.633274, 0.0000, 0.0000, 180.0000 },
			{ 3267, 58.753075, 1960.143311, 32.354809, 0.0000, 0.0000, 0.0000 },
			{ 3267, 28.540918, 1959.530151, 32.354809, 0.0000, 0.0000, 0.0000 },
			{ 3267, 25.460674, 2026.424561, 32.354809, 0.0000, 0.0000, 180.0000 },
			{ 3267, 55.787415, 2026.024536, 32.354809, 0.0000, 0.0000, 180.0000 },
			{ 3472, 79.864372, 2051.908691, 17.178991, 0.0000, 0.0000, 0.0000 },
			{ 3472, 79.752060, 1931.828369, 17.042696, 0.0000, 0.0000, 0.0000 },
			{ 3472, 3.115371, 1931.978882, 17.009785, 0.0000, 0.0000, 0.0000 },
			{ 3472, 3.302231, 2052.021484, 17.194735, 0.0000, 0.0000, 0.0000 },
			{ 3472, 80.084175, 1991.684204, 17.009785, 0.0000, 0.0000, 0.0000 },
			{ 3472, 80.225052, 2022.649414, 17.043865, 0.0000, 0.0000, 0.0000 },
			{ 3472, 80.066620, 1960.798950, 17.019413, 0.0000, 0.0000, 0.0000 },
			{ 3472, 3.134658, 1992.173096, 17.009785, 0.0000, 0.0000, 0.0000 },
			{ 3472, 3.082081, 1961.465698, 17.009785, 0.0000, 0.0000, 0.0000 },
			{ 3472, 2.762817, 2023.024170, 17.009785, 0.0000, 0.0000, 0.0000 },
			{ 3472, 28.736477, 1931.762817, 17.009785, 0.0000, 0.0000, 0.0000 },
			{ 3472, 52.713989, 1931.653931, 17.009785, 0.0000, 0.0000, 0.0000 },
			{ 3472, 30.258869, 2052.275146, 17.141142, 0.0000, 0.0000, 0.0000 },
			{ 3472, 54.122177, 2052.126465, 17.140638, 0.0000, 0.0000, 0.0000 },
			{ 744, 34.811787, 1969.356934, 16.526525, 0.0000, 0.0000, 56.2500 },
			{ 744, 52.619724, 2015.698486, 16.433199, 0.0000, 0.0000, 258.7500 },
			{ 749, 48.803566, 2014.860596, 16.575542, 0.0000, 0.0000, 247.5000 },
			{ 749, 38.879734, 1970.133667, 16.650541, 0.0000, 0.0000, 56.2501 },
			{ 758, 38.173332, 1974.824707, 17.031679, 0.0000, 0.0000, 236.2501 },
			{ 758, 52.329636, 2010.352539, 17.031679, 0.0000, 0.0000, 78.7500 },
			{ 879, 45.007744, 1973.220581, 18.053257, 0.0000, 0.0000, 123.7499 },
			{ 879, 43.212532, 2013.025146, 17.953259, 0.0000, 0.0000, 315.0000 },
			{ 897, 70.614746, 1964.400269, 22.770828, 0.0000, 0.0000, 292.5000 },
			{ 897, 13.888275, 2020.323486, 22.777500, 0.0000, 0.0000, 326.2500 },
			{ 16061, 42.288391, 1990.442627, 16.578051, 0.0000, 0.0000, 270.0000 },
			{ 726, 9.014233, 1979.721191, 16.638145, 0.0000, 0.0000, 0.0000 },
			{ 726, 69.602982, 2009.398071, 16.638145, 0.0000, 0.0000, 0.0000 },
			{ 8883, 55.484329, 1951.330933, 20.068405, 0.0000, 0.0000, 270.0000 },
			{ 8883, 28.648163, 2034.469360, 20.068405, 0.0000, 0.0000, 90.0001 },
			{ 5269, 67.320427, 2026.141968, 18.956345, 0.0000, 0.0000, 270.0000 },
			{ 5269, 17.492527, 1959.496948, 18.949673, 0.0000, 0.0000, 270.0000 },
			{ 5262, 12.701080, 1964.522827, 19.598286, 0.0000, 0.0000, 90.0000 },
			{ 5262, 72.074181, 2021.144897, 19.604956, 0.0000, 0.0000, 270.0000 },
			{ 2669, 15.021866, 2042.588257, 18.029753, 0.0000, 0.0000, 90.0000 },
			{ 2669, 70.852097, 1943.577881, 17.980898, 0.0000, 0.0000, 270.0000 },
			{ 3865, 35.021069, 2017.503052, 18.574547, 0.0000, 0.0000, 315.0000 },
			{ 3865, 26.719017, 2008.863159, 18.574547, 0.0000, 0.0000, 315.0000 },
			{ 3865, 36.255558, 2008.092041, 18.581219, 0.0000, 0.0000, 45.0000 },
			{ 3865, 38.649044, 1996.600342, 18.574547, 0.0000, 0.0000, 348.7500 },
			{ 3865, 24.112431, 1998.195557, 18.581219, 0.0000, 0.0000, 11.2500 },
			{ 3865, 16.290718, 2002.878906, 18.574547, 0.0000, 0.0000, 292.5000 },
			{ 3865, 10.529343, 1994.410522, 18.574547, 0.0000, 0.0000, 0.0000 },
			{ 3865, 52.611649, 1968.740967, 18.574551, 0.0000, 0.0000, 146.2500 },
			{ 3865, 58.991135, 1978.441772, 18.574547, 0.0000, 0.0000, 146.2500 },
			{ 3865, 68.374321, 1985.031006, 18.574547, 0.0000, 0.0000, 112.5000 },
			{ 3865, 49.310524, 1977.907227, 18.574551, 0.0000, 0.0000, 236.2500 },
			{ 3865, 41.225037, 1985.827759, 18.574547, 0.0000, 0.0000, 213.7500 },
			{ 3865, 74.868393, 1994.058472, 18.574547, 0.0000, 0.0000, 180.0000 },
			{ 3865, 57.069687, 1989.895142, 18.574547, 0.0000, 0.0000, 213.7500 },
			{ 3865, 48.280991, 1999.008057, 18.574547, 0.0000, 0.0000, 236.2500 },
			{ 3865, 59.773262, 2000.061890, 18.574547, 0.0000, 0.0000, 123.7499 },
			{ 3865, 15.172699, 1985.066650, 18.581217, 0.0000, 0.0000, 56.2500 },
			{ 3865, 35.270596, 1980.508667, 18.574547, 0.0000, 0.0000, 258.7500 },
			{ 3865, 25.889591, 1987.425781, 18.581219, 0.0000, 0.0000, 22.5000 },
			{ 2780, 64.844254, 1970.237549, 16.640625, 0.0000, 0.0000, 0.0000 },
			{ 2780, 21.985703, 2017.436401, 16.640625, 0.0000, 0.0000, 0.0000 },
			{ 2780, 70.804741, 2041.348633, 16.669035, 0.0000, 0.0000, 0.0000 },
			{ 2780, 14.983780, 1944.633545, 16.640625, 0.0000, 0.0000, 0.0000 },
			{ 2780, 63.530266, 1993.852783, 16.640625, 0.0000, 0.0000, 0.0000 },
			{ 2780, 15.503651, 1993.323608, 16.647297, 0.0000, 0.0000, 0.0000 },
			{ 2780, 41.710445, 1991.050049, 17.351694, 0.0000, 0.0000, 0.0000 },
			{ 1676, 44.635094, 2028.367798, 18.221493, 0.0000, 0.0000, 0.0000 },
			{ 1676, 39.369240, 1957.464844, 18.221493, 0.0000, 0.0000, 0.0000 },
			{ 1676, 42.382835, 1971.465942, 18.221493, 0.0000, 0.0000, 0.0000 },
			{ 1676, 45.204582, 2015.256714, 18.221493, 0.0000, 0.0000, 0.0000 },
			{ 1676, 7.650703, 2006.158936, 18.221493, 0.0000, 0.0000, 0.0000 },
			{ 1676, 76.310616, 1981.709961, 18.221493, 0.0000, 0.0000, 0.0000 },
			{ 1676, 14.487993, 1963.417236, 21.749733, 0.0000, 0.0000, 0.0000 },
			{ 1676, 70.454132, 2022.641113, 21.756401, 0.0000, 0.0000, 0.0000 }
		})
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

		std::array<CLanguageTextDraw, ZABAWA_MAX_TEAMS> TeamTD;

		bool OnGameModeInit() override
		{
			TeamTD[0].Create(320.0, 146.0, L_mm_tdm_teamselector_red, 2, 255, 3, 0.8, 4.2, -1, 0, 1, 1, 0);
			TeamTD[1].Create(320.0, 146.0, L_mm_tdm_teamselector_blue, 2, 255, 3, 0.8, 4.2, -1, 0, 1, 1, 0);

			MakeAllDynamicObjects(ArenaObjects, CurrentGameVW);
			AreaID = StreamerLibrary::CreateDynamicCube(0.0, 1925.0000, -100.0, 83.0000, 2055.0000, 1000.0, CurrentGameVW);

			SetGameState(0);
			Counter = 0;
			GameTimer = INVALID_TIMER_ID;

			return true;
		}

		void CheckWinner(int playerid, bool nowinner = false)
		{
			PlayerExitGame(playerid);

			if (Player[playerid].GetTeam() < ZABAWA_MAX_TEAMS)
			{
				PlayersInTeam[Player[playerid].GetTeam()].erase(playerid);
				TeamTD[Player[playerid].GetTeam()].HideForPlayer(playerid);
				if (!nowinner)
				{
					GivePlayerAchievement(playerid, EAM_ZabawyTDMLosses, 1);
				}
				if (!PlayersInTeam[Player[playerid].GetTeam()].size())
				{
					ActiveTeams.erase(Player[playerid].GetTeam());
					if (ActiveTeams.size() <= 1)
					{
						if (ActiveTeams.size() == 1)
						{
							if (!nowinner)
							{
								//winner team
								Message::AddAnnouncementMessage2(L_mm_tdm_announce_winner, TeamInfo[*ActiveTeams.begin()].second);
							}
							ActiveTeams.clear();
						}
						//cleanup code
						for (auto i : GetPlayersCopy())
						{
							if (!nowinner)
							{
								GivePlayerAchievement(i, EAM_ZabawyTDMWins, 1);
								Player[i].GiveExperienceKill(i);
								Player[i].GiveScore(3);
							}
							PlayerExitGame(i);
						}
					}
				}
			}

			if (!PlayersInGame.size())
			{
				ResetGame();
			}
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
			GivePlayerAchievement(playerid, EAM_ZabawyTDMDeaths, 1);
			if (killerid != INVALID_PLAYER_ID)
			{
				GivePlayerAchievement(killerid, EAM_ZabawyTDMKills, 1);
			}
			return true;
		}

		void PutPlayerIntoGame(int playerid)
		{
			AddStagingPlayer(playerid, TeamInfo[Player[playerid].GetTeam()].first);

			unsigned int teamid = Player[playerid].GetTeam();
			auto i = ArenaSpawns[teamid][Functions::RandomGenerator->Random(0, ArenaSpawns[teamid].size() - 1)];

			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 5.0, 5.0, true);
			
			TeamTD[teamid].ShowForPlayer(playerid);

			fixTogglePlayerControllable(playerid, false);
		}

		bool OnGameCommandExecute(int playerid, std::string params) override
		{
			if (CurrentGameState == 0)
			{
				SetGameState(1);
				SwitchGameTimer(GameTimer, CreateTimer<Arena>(15000, true, &Arena::ArenaTick));
				fixSendClientMessageToAll(Color::COLOR_INFO3, L_mm_tdm_startcd);
			}

			SetPlayerStaging(playerid);

			SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_tdm_staging);

			return true;
		}

		const int ARENA_MIN_PLAYERS = ZABAWA_MAX_TEAMS;

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

						BalanceTeams(GetPlayersCopy(), PlayersInTeam, ActiveTeams);

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
									safeGivePlayerWeapon(i, 32, 999999);
									safeGivePlayerWeapon(i, 24, 999999);
									safeGivePlayerWeapon(i, 26, 999999);
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
								GameTextForPlayer(i, "START!", 500, 6);
								fixTogglePlayerControllable(i, true);
								if (!StreamerLibrary::IsPlayerInDynamicArea(i, AreaID, 1))
								{
									CheckWinner(i, true);
								}
							}
							for (auto &TeamTextDraw : TeamTD)
							{
								TeamTextDraw.Hide();
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
			return (CurrentGameState == 0 || CurrentGameState == 1);
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
	_Arena;
};