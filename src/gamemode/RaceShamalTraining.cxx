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

using namespace RaceSystem;
namespace RaceShamalTraining
{
	class RaceEvent : Race
	{
	public:
		RaceEvent() :
			Race(
			"Shamal Training", //nazwa wyœcigu
			"/rst", //komenda wyœcigu
			RaceSystem::eRT_RaceFlyRivals, //typ: 
			/*
				eRT_RaceRivals,
				eRT_RaceTimeAttack,
				eRT_RaceEndurance,
				eRT_RacePlatform,
				eRT_RaceDrifting,
				eRT_RaceDrag,
				eRT_RaceFlyRivals,
				eRT_RaceFlyTimeAttack,
				eRT_RaceFlyEndurance
			*/
			0,//iloœæ okr¹¿en, 0 lub 1 brak
			15000, //iloœæ milisekund zanim sie rozpocznie
			519, //vehiclemodelid
			true,//vehgod
			2, //minimum graczy
			eRKA_None)//opcje keyów
		{
			this->StartingPositions = std::vector<Zabawy::SSpawnPoint>({
				{ 1487.3342, 1756.7030, 11.7629, 180.0 },
				{ 1467.6364, 1739.2029, 11.7629, 180.0 },
				{ 1487.3342, 1723.9163, 11.7629, 180.0 },
				{ 1467.6364, 1706.8854, 11.7629, 180.0 },
				{ 1487.3342, 1691.7825, 11.7629, 180.0 },
				{ 1467.6364, 1675.6016, 11.7629, 180.0 }
			});

			this->CheckPointsPositions = std::vector<CheckPointPos>({
				{1477.3810, 1454.2476, 11.7439, 180.7620, 40.0},
				{1477.0001, 1213.7842, 25.1284, 179.8543, 40.0},
				{1475.5031, 787.1824, 133.6241, 179.8333, 40.0},
				{1392.9637, 469.9145, 82.7003, 173.9964 , 40.0},
				{1490.2063, 155.4658, 70.9212, 213.8511 , 40.0},
				{1662.4088, -263.2031, 64.2410, 191.8726, 40.0},
				{1731.7275, -785.5974, 77.0226, 149.0257, 40.0},
				{1632.3774, -1239.5275, 96.4008, 167.495, 40.0},
				{1608.1156, -1634.7577, 57.3077, 182.782, 40.0},
				{1426.7090, -2027.2693, 101.9750, 103.77, 40.0},
				{1097.4515, -2112.5347, 114.3043, 97.560, 40.0},
				{827.9200, -2191.2393, 124.6158, 118.730, 40.0},
				{688.0899, -2476.4783, 114.2860, 188.109, 40.0},
				{814.2955, -2647.5862, 112.2624, 235.308, 40.0},
				{1017.9081, -2673.9131, 99.5419, 275.194, 40.0},
				{1401.3103, -2601.4399, 56.9579, 281.162, 40.0},
				{1869.9272, -2590.8254, 14.4967, 270.025, 40.0},
				{2110.0081, -2552.7214, 14.4676, 359.161, 40.0},
				{2029.4807, -2494.2051, 14.4684, 91.0745, 40.0},
				{1825.5063, -2495.6716, 14.5139, 89.1539, 40.0},
				{1531.8406, -2494.5938, 14.5187, 91.7075, 40.0}
			});
		}

		void StreamerInit() override
		{
			//tutaj obiekty itp robiæ
		}

		void OnPlayerReachNextCheckPoint(int playerid, unsigned short current_lap, unsigned short current_checkpoint, unsigned long long current_race_time, bool is_checkpoint_finish, bool race_finish) override
		{
			if (race_finish)
			{
				unsigned long long PositionMoney;
				unsigned long long PositionScore;
				size_t pos = FinishedPlayers.size();
				PositionMoney = 30000 / pos;
				PositionScore = 6 / pos;
				Player[playerid].GiveMoney(PositionMoney);
				Player[playerid].GiveScore(PositionScore);
				SendClientMessageF(playerid, Color::COLOR_INFO3, L_race_winner_text, false, true, RaceName.c_str(), pos, ((float)current_race_time) / 1000.0f, PositionMoney, PositionScore);
			}
		}
	} _RaceEvent;
};