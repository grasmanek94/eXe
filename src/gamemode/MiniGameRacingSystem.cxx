/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	This file implements the racing system 
			by extending the mini game system

	________________________________________________________________	
	Notes
		-	Creating races was never easier before

	________________________________________________________________
	Dependencies
		Extension
		WorldData
		MiniGameSystem
		Language

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

namespace RaceSystem
{
	bool TimePrioritizer(SPlayerRaceData*& s1, SPlayerRaceData*& s2)
	{
		return (
			s1->CurrentLap > s2->CurrentLap
			||
			(s1->CurrentLap == s2->CurrentLap && s1->Checkpoints > s2->Checkpoints)
			||
			(s1->Checkpoints == s2->Checkpoints && s1->RaceCurrentTime < s2->RaceCurrentTime)
			);
	}

	bool ScorePrioritizer(SPlayerRaceData*& s1, SPlayerRaceData*& s2)
	{
		return
			(
			s1->Score < s2->Score
			||
			(s1->Score == s2->Score && s1->CurrentLap < s2->CurrentLap)
			||
			(s1->CurrentLap == s2->CurrentLap && s1->Checkpoints < s2->Checkpoints)
			||
			(s1->Checkpoints == s2->Checkpoints && s1->RaceCurrentTime > s2->RaceCurrentTime)
			);
	}

	std::vector<Race::FinishEntry>& Race::GetPositions()
	{
		std::vector<SPlayerRaceData*> PlayerPositions;
		FinishedPositions.clear();
		if (TIMER != INVALID_TIMER_ID)
		{
			DestroyTimer(TIMER);
			TIMER = INVALID_TIMER_ID;
		}
		if (FinishedPlayers.size())
		{
			for (auto i : FinishedPlayers)
				PlayerPositions.push_back(&Player[i].RaceData);

			if (RaceType == eRT_RaceDrifting)
				std::sort(PlayerPositions.begin(), PlayerPositions.end(), ScorePrioritizer);
			else
				std::sort(PlayerPositions.begin(), PlayerPositions.end(), TimePrioritizer);

			for (auto i : PlayerPositions)
				FinishedPositions.push_back(std::pair<int, std::pair<unsigned long long, unsigned long>>(i->playerid, std::pair<unsigned long long, unsigned long>(i->RaceCurrentTime, i->Score)));
		}
		FinishedPlayers.clear();
		return FinishedPositions;
	}

	Race::Race(
		std::string CurrentRaceName_,
		std::string CurrentRaceCommandName_,
		eRaceTypes type,
		unsigned short AmountOfLaps,
		unsigned long RaceStagingTime,
		int modelid,
		bool vehgod,
		int MinPlayers,
		unsigned long EnabledKeyActions,
		bool EnableNitro
		)
		: Base(L_race_gametxt, CurrentRaceCommandName_, Zabawy::eSystemTypes::eST_Race),
		MaxLaps(AmountOfLaps),
		RaceType(type),
		StagingTime(RaceStagingTime),
		VehicleModel(modelid),
		VehicleGodMode(vehgod),
		MinimumPlayers(MinPlayers),
		RaceName(CurrentRaceName_),
		EnabledActions(EnabledKeyActions),
		NitroEnabled(EnableNitro)
	{
		for (size_t playerid = 0; playerid < MAX_PLAYERS; ++playerid)
			Player[playerid].RaceData.playerid = playerid;//playeridceptionlololololol
		CurrentGameState = 0;
		Counter = 0;
		TIMER = INVALID_TIMER_ID;
	}

	void Race::UpdatePlayerCheckPoint(int playerid)
	{
		size_t current_cp = Player[playerid].RaceData.Checkpoints;
		size_t next_cp = (current_cp < CheckpointsSizeMinusOne) ? (current_cp + 1) : 0;
		size_t type_cp = (next_cp == 0) ? ((Player[playerid].RaceData.CurrentLap == MaxLapsMinusOne && LapRace) ? real_finish_type : finish_type) : normal_type;
		auto& pos_ccp = CheckPointsPositions[current_cp];
		auto& pos_ncp = CheckPointsPositions[next_cp];
		SetPlayerRaceCheckpoint(playerid, type_cp, pos_ccp.x, pos_ccp.y, pos_ccp.z, pos_ncp.x, pos_ncp.y, pos_ncp.z, pos_ccp.r);
	}

	bool Race::OnGameModeInit()
	{
		LapRace = MaxLaps > 1;
		MaxLapsMinusOne = LapRace ? MaxLaps - 1 : MaxLaps;

		switch (RaceType)
		{
		case eRT_RaceFlyRivals:
		case eRT_RaceFlyTimeAttack:
		case eRT_RaceFlyEndurance:
			normal_type = 3;
			finish_type = LapRace ? 3 : 4;
			real_finish_type = 4;
			break;
		default:
			normal_type = 0;
			finish_type = LapRace ? 0 : 1;
			real_finish_type = 1;
			break;
		}

		CheckpointsSize = CheckPointsPositions.size();
		CheckpointsSizeMinusOne = CheckpointsSize - 1;

		RaceRunning = false;

		StreamerInit();
		return true;
	}

	void Race::PlayerLeaveRace(int playerid, eRaceExitType exit_type){}
	void Race::OnPlayerReachNextCheckPoint(int playerid, unsigned short current_lap, unsigned short current_checkpoint, unsigned long long current_race_time, bool is_checkpoint_finish, bool race_finish){}
	void Race::RaceFinished(std::vector<Race::FinishEntry>& positions, bool real_finish) {}

	void Race::ResetPlayerRaceData(int playerid)
	{
		Player[playerid].RaceData.Checkpoints = 0;
		Player[playerid].RaceData.CurrentLap = 0;
		Player[playerid].RaceData.Finished = false;
		Player[playerid].RaceData.RaceCurrentTime = 0;
		Player[playerid].RaceData.RaceStartTime = 0;
		Player[playerid].RaceData.Score = 0;
	}

	void Race::ReachMeCountDown(int timerid, Zabawy::ZabawyTimerVector& vector)
	{
		if (CurrentGameState == 4)
		{
			std::string CountDown(Functions::string_format("%d", --Counter));
			if (Counter == 0)
			{
				for (auto i : GetPlayersCopy())
				{
					InternalPlayerLeaveRace(i, eRaceExit_LeaveCar);
					PlayerExitGame(i);
				}
				RaceRunning = false;
				RaceFinished(GetPositions(), true);
				DestroyTimer(timerid);
				TIMER = INVALID_TIMER_ID;
				SetGameState(0);
			}
			else
			{
				for (auto i : PlayersInGame)
				{
					GameTextForPlayer(i, CountDown.c_str(), 1000, 6);
				}
			}
		}
		else
		{
			DestroyTimer(timerid);
			TIMER = INVALID_TIMER_ID;
		}
	}

	bool Race::OnPlayerEnterRaceCheckpoint(int playerid)
	{
		if (RaceRunning && !Player[playerid].RaceData.Finished)
		{
			Player[playerid].RaceData.RaceCurrentTime = Functions::GetTime() - Player[playerid].RaceData.RaceStartTime;

			bool checkpoint_finish = (++Player[playerid].RaceData.Checkpoints == CheckpointsSize);

			if (checkpoint_finish)
			{
				Player[playerid].RaceData.Finished = LapRace ? (++Player[playerid].RaceData.CurrentLap == MaxLaps) : true;

				Player[playerid].RaceData.Checkpoints = 0;

				if (Player[playerid].RaceData.Finished)
				{
					FinishedPlayers.insert(playerid);
					RaceRunning = PlayersInGame.size() > 1;
					if (RaceRunning && TIMER == INVALID_TIMER_ID)
					{
						SetGameState(4);
						Counter = 30;
						TIMER = CreateTimer<Race>(1000, true, &Race::ReachMeCountDown);
					}
				}
			}

			OnPlayerReachNextCheckPoint(
				playerid,
				Player[playerid].RaceData.CurrentLap,
				Player[playerid].RaceData.Checkpoints,
				Player[playerid].RaceData.RaceCurrentTime,
				checkpoint_finish,
				Player[playerid].RaceData.Finished
				);

			if (!Player[playerid].RaceData.Finished)
				UpdatePlayerCheckPoint(playerid);
			else
				DisablePlayerRaceCheckpoint(playerid);

			if (!RaceRunning)
			{
				RaceFinished(GetPositions(), true);
				for (auto i : GetPlayersCopy())
					PlayerExitGame(i);
				SetGameState(0);
			}
			if (Player[playerid].RaceData.Finished)
				PlayerExitGame(playerid);
		}
		return true;
	}

	void Race::InternalPlayerLeaveRace(int playerid, eRaceExitType exit_type)
	{
		PlayerLeaveRace(playerid, exit_type);
		if (PlayersInGame.size() < 2)
		{
			RaceRunning = false;
			RaceFinished(GetPositions(), false);
			SetGameState(0);
		}
	}

	void Race::StreamerInit(){}

	bool Race::OnPlayerStateChange(int playerid, int newstate, int oldstate)
	{
		if (RaceRunning)
		{
			if (oldstate == PLAYER_STATE_DRIVER && newstate != PLAYER_STATE_DRIVER)
			{
				InternalPlayerLeaveRace(playerid, eRaceExit_LeaveCar);
				PlayerExitGame(playerid);
			}
		}
		return true;
	}

	bool Race::PlayerRequestGameExit(int playerid, int reason)
	{
		switch (reason)
		{
		case Zabawy::PlayerRequestExitTypeDeath:
			InternalPlayerLeaveRace(playerid, eRaceExit_Death);
			PlayerExitGame(playerid);
			break;
		case Zabawy::PlayerRequestExitTypeDisconnect:
			InternalPlayerLeaveRace(playerid, eRaceExit_Disconnect);
			PlayerExitGame(playerid);
			break;
		case Zabawy::PlayerRequestExitTypeExit:
			InternalPlayerLeaveRace(playerid, eRaceExit_Exit);
			PlayerExitGame(playerid);
			break;
		}
		return true;
	}

	bool Race::PlayerRequestCencelStaging(int playerid, int reason)
	{
		PlayerExitGame(playerid);
		return true;
	}

	void Race::PutPlayerIntoGame(int playerid)
	{
		AddStagingPlayer(playerid);

		fixSetPlayerHealth(playerid, 10000000.0);
		Player[playerid].RaceData.SpawnPoint = std::distance(PlayersInGame.begin(), PlayersInGame.find(playerid));
		auto i = StartingPositions[Player[playerid].RaceData.SpawnPoint];
		TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
		fixTogglePlayerControllable(playerid, false);
	}

	void Race::SetGameState(int state)
	{
		if (CurrentGameState != state)
		{
			CurrentGameState = state;
			Zabawy::NeedUpdate();
		}
	}

	void Race::Tick(int timerid, Zabawy::ZabawyTimerVector& vector)
	{
		switch (CurrentGameState)
		{
			case 1:
			{
				Counter = 0;
				if (PlayersInGame.size() < MinimumPlayers)
				{
					SetGameState(0);

					fixSendClientMessageToAllF(Color::COLOR_ERROR, L_race_not_enough_players, true, true, RaceName.c_str());

					while (PlayersInGame.size())
					{
						PlayerExitGame(*PlayersInGame.begin());
					}

					TIMER = INVALID_TIMER_ID;
					DestroyTimer(timerid);
				}
				else
				{
					SetGameState(2);

					for (auto i : GetPlayersCopy())
						PutPlayerIntoGame(i);

					TIMER = CreateTimer<Race>(1000, true, &Race::Tick);

					DestroyTimer(timerid);
				}
			}
			break;

			case 2:
			{
				if (PlayersInGame.size() < MinimumPlayers)
				{
					SetGameState(0);

					fixSendClientMessageToAllF(Color::COLOR_ERROR, L_race_not_enough_players, true, true, RaceName.c_str());

					while (PlayersInGame.size())
					{
						PlayerExitGame(*PlayersInGame.begin());
					}

					TIMER = INVALID_TIMER_ID;
					DestroyTimer(timerid);
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
								ResetPlayerRaceData(i);
								CreatePlayerVehicleSafe(i, VehicleModel, 0.0, 0.0, 0.0);
								if (VehicleGodMode)
									safeSetVehicleHealth(Player[i].PlayerVehicle, 1000000.0);
								SetCameraBehindPlayer(i);
							}
							else if (Counter == 2)
							{
								auto u = StartingPositions[Player[i].RaceData.SpawnPoint];
								TeleportPlayer(i, u.x, u.y, u.z, u.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
							}
							else if (Counter == 3)
							{
								if (!GetPlayerVehicleID(i))
								{
									CreatePlayerVehicleSafe(i, VehicleModel, 0.0, 0.0, 0.0);
									if (VehicleGodMode)
										safeSetVehicleHealth(Player[i].PlayerVehicle, 1000000.0);
									SetCameraBehindPlayer(i);
								}
							}
							else if (Counter == 5)
							{
								UpdatePlayerCheckPoint(i);
							}
							GameTextForPlayer(i, CountDown.c_str(), 1000, 6);
						}
					}
					else if (Counter == 6)
					{
						RaceRunning = true;
						unsigned long long TimeNow = Functions::GetTime();
						for (auto i : GetPlayersCopy())
						{
							fixTogglePlayerControllable(i, true);
							auto u = StartingPositions[Player[i].RaceData.SpawnPoint];
							if (IsPlayerInRangeOfPoint(i, 3.0, u.x, u.y, u.z) && GetPlayerVehicleID(i))
							{
								GameTextForPlayer(i, "GO!", 500, 6);
								Player[i].RaceData.RaceStartTime = TimeNow;
							}
							else
							{
								InternalPlayerLeaveRace(i, eRaceExit_LeaveCar);
								PlayerExitGame(i);
							}
						}
						SetGameState(3);
						if (PlayersInGame.size() < MinimumPlayers)
						{
							for (auto i : GetPlayersCopy())
							{
								InternalPlayerLeaveRace(i, eRaceExit_Exit);
								PlayerExitGame(i);
							}
							RaceFinished(GetPositions(), false);
							SetGameState(0);
						}
						TIMER = INVALID_TIMER_ID;
						DestroyTimer(timerid);
					}
				}
			}
			break;

			default:
			{
				TIMER = INVALID_TIMER_ID;
				DestroyTimer(timerid);
				SetGameState(0);
			}
			break;
		}
	}

	bool Race::OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)
	{
		if (RaceRunning)
		{
			debug_fixSendClientMessage(playerid, Functions::string_format("%s %d %d", __FUNCSIG__, newkeys, oldkeys));
			if (bIsBitEnabled(EnabledActions, eRKA_UnFlip) && PRESSED(KEY_SUBMISSION))
			{
				if (!bIsBitEnabled(EnabledActions, eRKA_UnFlipBehaviourRestore))
				{
					float a;
					GetVehicleZAngle(Player[playerid].CurrentVehicle, &a);
					SetVehicleZAngle(Player[playerid].CurrentVehicle, a);
					GivePlayerAchievement(playerid, EAM_LubieDachowac, 1);
				}
				else
				{
					auto u = CheckPointsPositions[(Player[playerid].RaceData.Checkpoints == 0) ? CheckpointsSizeMinusOne : (Player[playerid].RaceData.Checkpoints - 1)];
					fixSetVehiclePos(Player[playerid].CurrentVehicle, u.x, u.y, u.z);
					SetVehicleZAngle(Player[playerid].CurrentVehicle, u.a);//next cp angle
				}
			}
			else if (bIsBitEnabled(EnabledActions, eRKA_Fix) && PRESSED(KEY_YES))
			{
				safeRepairVehicle(Player[playerid].CurrentVehicle);
				if (VehicleGodMode)
					safeSetVehicleHealth(Player[playerid].CurrentVehicle, 100000.0);
				if (bIsBitEnabled(EnabledActions, eRKA_FixBehaviourRestore))
				{
					auto u = CheckPointsPositions[(Player[playerid].RaceData.Checkpoints == 0) ? CheckpointsSizeMinusOne : (Player[playerid].RaceData.Checkpoints - 1)];
					fixSetVehiclePos(Player[playerid].CurrentVehicle, u.x, u.y, u.z);
					SetVehicleZAngle(Player[playerid].CurrentVehicle, u.a);//next cp angle
				}
			}
			else if (bIsBitEnabled(EnabledActions, eRKA_Restore) && PRESSED(KEY_CROUCH))
			{
				auto u = CheckPointsPositions[(Player[playerid].RaceData.Checkpoints == 0) ? CheckpointsSizeMinusOne : (Player[playerid].RaceData.Checkpoints - 1)];
				fixSetVehiclePos(Player[playerid].CurrentVehicle, u.x, u.y, u.z);
				SetVehicleZAngle(Player[playerid].CurrentVehicle, u.a);//next cp angle
			}
			else if (NitroEnabled && (newkeys == 1 || newkeys == 9 || newkeys == 33 && oldkeys != 1 || oldkeys != 9 || oldkeys != 33))
			{
				AddVehicleComponent(Player[playerid].CurrentVehicle, 1010);
			}
		}
		return true;
	}

	bool Race::OnGameCommandExecute(int playerid, std::string params)
	{
		if (CurrentGameState == 0)
		{
			SetGameState(1);
			if (TIMER != INVALID_TIMER_ID)
			{
				DestroyTimer(TIMER);
				TIMER = INVALID_TIMER_ID;
			}
			TIMER = CreateTimer<Race>(StagingTime, true, &Race::Tick);
			fixSendClientMessageToAllF(Color::COLOR_INFO3, L_race_staging, false, false, RaceName.c_str(), CurrentGameCommandName.c_str());
		}

		SetPlayerStaging(playerid);

		SendClientMessageF(playerid, Color::COLOR_INFO3, L_race_joined_success, true, true, RaceName.c_str());

		return true;
	}

	bool Race::Joinable()
	{
		return (CurrentGameState == 0 || CurrentGameState == 1) && PlayersInGame.size() < (StartingPositions.size());
	}

	bool Race::Staging()
	{
		return CurrentGameState == 1;
	}

	int Race::GetCommandRestrictions()
	{
		return RESTRICTION_IN_VEHICLE_OR_ONFOOT | RESTRICTION_NOT_IN_A_GAME;
	}
};