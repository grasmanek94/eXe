/*
	Copyright (c) 2014 - 2015 Rafa� "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa� Grasman <grasmanek94@gmail.com>
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
		Mateusz 'eider' Cicho�
*/
#pragma once
#include "GameMode.hxx"

namespace RaceSystem
{
	enum eRaceTypes
	{
		eRT_RaceRivals,
		eRT_RaceTimeAttack,
		eRT_RaceEndurance,
		eRT_RacePlatform,
		eRT_RaceDrifting,
		eRT_RaceDrag,
		eRT_RaceFlyRivals,
		eRT_RaceFlyTimeAttack,
		eRT_RaceFlyEndurance
	};

	enum eRaceExitType
	{
		eRaceExit_Exit,
		eRaceExit_Disconnect,
		eRaceExit_Death,
		eRaceExit_LeaveCar,
		eRaceExit_Finish
	};

	enum eRaceKeyAction
	{
		eRKA_None = 0,
		eRKA_Fix = 1,
		eRKA_FixBehaviourRestore = 2,
		eRKA_Restore = 4,
		eRKA_UnFlip = 8,
		eRKA_UnFlipBehaviourRestore = 16
	};

	struct CheckPointPos
	{
		double x;
		double y;
		double z;
		double a;
		double r;
	};

	struct SPlayerRaceData
	{
		int playerid;
		unsigned long long RaceStartTime;
		unsigned long long RaceCurrentTime;
		unsigned short Checkpoints;
		unsigned short CurrentLap;
		bool Finished;
		unsigned long Score;
		unsigned long SpawnPoint;
	};

	bool TimePrioritizer(SPlayerRaceData*& s1, SPlayerRaceData*& s2);
	bool ScorePrioritizer(SPlayerRaceData*& s1, SPlayerRaceData*& s2);

	class Race : public Zabawy::Base
	{
	public:
		using FinishEntry = std::pair < int, std::pair<unsigned long long, unsigned long> > ;
	private:
		std::vector<FinishEntry> FinishedPositions;
		unsigned short MaxLaps;
		unsigned short MaxLapsMinusOne;
		eRaceTypes RaceType;
		unsigned long EnabledActions;
		int CheckpointsSize;
		int CheckpointsSizeMinusOne;
		int Counter;
		bool RaceRunning;
		unsigned long StagingTime;
		int VehicleModel;
		bool VehicleGodMode;
		int CurrentGameState;
		int MinimumPlayers;
		bool LapRace;
		int normal_type;
		int finish_type;
		int real_finish_type;
		std::vector<FinishEntry>& GetPositions();
		int TIMER;
		void UpdatePlayerCheckPoint(int playerid);
		bool NitroEnabled;
	public:
		std::set<int> FinishedPlayers;
		std::string RaceName;
		std::vector<Zabawy::SSpawnPoint> StartingPositions;
		std::vector<CheckPointPos> CheckPointsPositions;

		Race(
			std::string CurrentRaceName_,
			std::string CurrentRaceCommandName_,
			eRaceTypes type = eRT_RaceRivals,
			unsigned short AmountOfLaps = 0,
			unsigned long RaceStagingTime = 15000,
			int modelid = 400,
			bool vehgod = false,
			int MinPlayers = 2,
			unsigned long EnabledKeyActions = eRKA_None,
			bool EnableNitro = false
			);

		bool OnGameModeInit() override;
		virtual void PlayerLeaveRace(int playerid, eRaceExitType exit_type);
		virtual void OnPlayerReachNextCheckPoint(int playerid, unsigned short current_lap, unsigned short current_checkpoint, unsigned long long current_race_time, bool is_checkpoint_finish, bool race_finish);
		virtual void RaceFinished(std::vector<FinishEntry>& positions, bool real_finish);
		virtual void StreamerInit();
		void ResetPlayerRaceData(int playerid);
		bool OnPlayerEnterRaceCheckpoint(int playerid) override;
		void InternalPlayerLeaveRace(int playerid, eRaceExitType exit_type);
		bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override;
		bool PlayerRequestGameExit(int playerid, int reason) override;
		bool PlayerRequestCencelStaging(int playerid, int reason) override;
		void PutPlayerIntoGame(int playerid);
		void SetGameState(int state);
		void Tick(int timerid, Zabawy::ZabawyTimerVector& vector);
		void ReachMeCountDown(int timerid, Zabawy::ZabawyTimerVector& vector);
		bool OnGameCommandExecute(int playerid, std::string params) override;
		bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) override;
		bool Joinable() override;
		bool Staging() override;
		int GetCommandRestrictions() override;
	};
};