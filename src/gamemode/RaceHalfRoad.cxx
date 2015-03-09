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
namespace RaceHalfRoad
{
	class RaceEvent : Race
	{
	public:
		RaceEvent() :
			Race(
			"HalfRoad", //nazwa wyœcigu
			"/rhr", //komenda wyœcigu
			RaceSystem::eRT_RaceRivals, //typ: 
			/*
				eRT_RaceRivals,
				eRT_RaceFlyRivals
			*/
			0,//iloœæ okr¹¿en, 0 lub 1 brak
			15000, //iloœæ milisekund zanim sie rozpocznie
			495, //vehiclemodelid
			true,//vehgod
			2, //minimum graczy
			eRKA_UnFlip | eRKA_Restore | eRKA_Fix | eRKA_FixBehaviourRestore,//opcje keyów
			true)//nitro
		{
			this->StartingPositions = std::vector<Zabawy::SSpawnPoint>({
				{ 1424.5406, -696.6898, 88.1806, 80.2321}, // GZRACESTART1
				{ 1430.6125, -693.1121, 88.2383, 74.3894}, // GZRACESTART2
				{ 1433.6322, -699.2589, 88.5410, 70.3512}, // GZRACESTART3
				{ 1439.6790, -697.0070, 88.9905, 63.1734}, // GZRACESTART4
				{ 1441.7667, -703.6551, 89.5451, 57.0485}, // GZRACESTART5
				{ 1448.3979, -703.1201, 90.1182, 50.2620}, // GZRACESTART6
				{ 1448.4686, -709.3580, 90.6697, 42.9028}, // GZRACESTART7
				{ 1454.5341, -709.7535, 91.1656, 37.1550} // GZRACESTART8
			});

			this->CheckPointsPositions = std::vector<CheckPointPos>({
				{ 1398.4769, -691.8784, 88.8042, 0.0, 17.0 },
				{ 1239.3776, -736.5811, 95.9113, 0.0, 17.0 },
				{ 1150.4215, -763.9886, 108.3839, 0.0, 17.0 },
				{ 997.2650, -788.8035, 100.7155, 0.0, 17.0 },
				{ 953.4701, -792.6670, 106.8273, 0.0, 17.0 },
				{ 946.2162, -766.8880, 108.7741, 0.0, 17.0 },
				{ 916.9291, -757.5082, 102.7494, 0.0, 17.0 },
				{ 801.7996, -798.1728, 67.2122, 0.0, 17.0 },
				{ 705.7554, -824.5963, 70.5994, 0.0, 17.0 },
				{ 672.4678, -890.4114, 66.6535, 0.0, 17.0 },
				{ 693.7328, -918.3465, 75.7364, 0.0, 17.0 },
				{ 701.9349, -937.4578, 70.6857, 0.0, 17.0 },
				{ 723.2077, -928.4841, 65.8366, 0.0, 17.0 },
				{ 713.6073, -949.8376, 61.5816, 0.0, 17.0 },
				{ 745.3408, -937.7924, 55.6868, 0.0, 17.0 },
				{ 772.5351, -945.8861, 53.0466, 0.0, 17.0 },
				{ 826.7550, -972.1267, 37.7829, 0.0, 17.0 },
				{ 913.1808, -977.9420, 38.3383, 0.0, 17.0 },
				{ 1082.9254, -953.8527, 42.9141, 0.0, 17.0 },
				{ 1161.2996, -965.0505, 42.7254, 0.0, 17.0 },
				{ 1161.8253, -1059.1819, 30.2518, 0.0, 17.0 },
				{ 1181.6777, -1149.9318, 24.0287, 0.0, 17.0 },
				{ 1215.9036, -1161.5396, 23.5939, 0.0, 17.0 },
				{ 1228.1359, -1280.3385, 13.7522, 0.0, 17.0 },
				{ 1342.2906, -1295.1555, 13.9528, 0.0, 17.0 },
				{ 1368.3396, -1399.3185, 13.7345, 0.0, 17.0 },
				{ 1468.9346, -1440.6827, 13.7314, 0.0, 17.0 },
				{ 1671.4456, -1441.1313, 13.7333, 0.0, 17.0 },
				{ 1789.4357, -1433.6252, 13.9470, 0.0, 17.0 },
				{ 1813.7561, -1406.0305, 13.7744, 0.0, 17.0 },
				{ 1825.1919, -1342.1606, 14.7709, 0.0, 17.0 },
				{ 1865.2573, -1259.9160, 13.7368, 0.0, 17.0 },
				{ 1924.7183, -1237.4954, 17.6722, 0.0, 17.0 },
				{ 1971.5516, -1235.7595, 20.4133, 0.0, 17.0 },
				{ 2027.7963, -1223.7333, 22.6008, 0.0, 17.0 },
				{ 2057.2561, -1182.5139, 24.1683, 0.0, 17.0 },
				{ 2079.3064, -1106.8788, 25.2001, 0.0, 17.0 },
				{ 2091.6689, -1079.9556, 26.1044, 0.0, 17.0 },
				{ 2088.2659, -1052.6600, 30.0519, 0.0, 17.0 },
				{ 2018.3439, -1029.6393, 35.8360, 0.0, 17.0 },
				{ 1949.4404, -1009.9416, 33.9151, 0.0, 17.0 },
				{ 1914.1102, -954.2202, 58.0995, 0.0, 17.0 },
				{ 1909.4438, -908.4158, 75.3877, 0.0, 17.0 },
				{ 1915.7582, -839.2515, 109.8884, 0.0, 17.0 },
				{ 1921.7408, -784.4191, 119.0766, 0.0, 17.0 },
				{ 1916.7522, -741.5148, 116.4145, 0.0, 17.0 },
				{ 1879.6189, -700.3897, 105.6564, 0.0, 17.0 },
				{ 1838.5656, -684.2194, 93.9368, 0.0, 17.0 },
				{ 1777.0659, -667.8768, 85.1994, 0.0, 17.0 },
				{ 1616.7660, -622.4818, 70.3444, 0.0, 17.0 },
				{ 1594.9043, -707.9219, 66.6288, 0.0, 17.0 },
				{ 1540.2788, -741.0584, 77.6354, 0.0, 17.0 },
				{ 1476.8602, -766.7219, 91.5481, 0.0, 17.0 },
				{ 1454.8643, -714.3951, 91.6892, 0.0, 17.0 }
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