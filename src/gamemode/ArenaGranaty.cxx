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
namespace Granaty
{
	class Arena : public Base
	{
	public:
		std::vector<SObject> ArenaObjects;

		std::vector<SSpawnPoint> ArenaSpawns;

		Arena() :
			Base(L_games_ArenaGranaty_name, "/agr"),
			ArenaSpawns({
				{ 1693.7280, 1752.8513, 1347.1156+0.3, 314.6133 },
				{ 1706.7876, 1774.4434, 1350.5059+0.3, 1.9271 },
				{ 1713.9756, 1774.8323, 1353.9396+0.3, 261.3460 },
				{ 1725.7637, 1784.6355, 1347.1156+0.3, 29.8139 },
				{ 1734.0049, 1800.2188, 1350.5059+0.3, 50.1806 },
				{ 1738.5220, 1840.9150, 1347.1156+0.3, 357.5402 },
				{ 1716.9697, 1835.5125, 1347.1156+0.3, 349.7068 },
				{ 1702.8394, 1834.3491, 1351.1027+0.3, 271.7095 },
				{ 1750.8569, 1819.5400, 1358.4078+0.3, 85.2744 },
				{ 1751.8481, 1790.6299, 1354.4078+0.3, 89.9744 },
				{ 1726.0737, 1743.6982, 1354.4078+0.3, 356.6002 },
				{ 1690.7275, 1742.2007, 1358.4078+0.3, 324.0134 },
				{ 1685.2544, 1767.2917, 1354.4078+0.3, 281.7130 },
				{ 1685.6304, 1823.4993, 1354.4078+0.3, 267.6129 },
				{ 1711.3989, 1866.2039, 1358.4078+0.3, 183.0121 },
				{ 1707.0630, 1799.3848, 1355.1027+0.3, 211.8390 },
				{ 1728.2085, 1813.5559, 1355.1019+0.3, 311.7933 },
				{ 0.0f, 0.0f, 0.0f, 0.0f }
			}),
			ArenaObjects({
				{ 4571,  1718.8999000, 1804.3999000, 1282.1000100, 0.0000000, 0.0000000, 0.0000000 }, //object(stolenbuilds09) (1)
				{ 4571,  1718.8999000, 1843.2000000, 1282.1000100, 0.0000000, 0.0000000, 0.0000000 }, //object(stolenbuilds09) (2)
				{ 4571,  1718.8999000, 1765.8000000, 1282.1000100, 0.0000000, 0.0000000, 0.0000000 }, //object(stolenbuilds09) (3)
				{ 12859, 1698.2998000, 1832.8999000, 1346.1000100, 0.0000000, 0.0000000, 0.0000000 }, //object(sw_cont03) (1)
				{ 7040,  1715.1006000, 1829.2002000, 1349.5000000, 0.0000000, 0.0000000, 0.0000000 }, //object(vgnplcehldbox01) (1)
				{ 8078,  1728.8999000, 1817.5000000, 1350.1000100, 0.0000000, 0.0000000, 0.0000000 }, //object(vgsfrates07) (1)
				{ 8078,  1747.7998000, 1816.8999000, 1350.1000100, 0.0000000, 0.0000000, 0.0000000 }, //object(vgsfrates07) (2)
				{ 12859, 1710.7998000, 1793.8000000, 1346.1000100, 0.0000000, 0.0000000, 0.0000000 }, //object(sw_cont03) (2)
				{ 7621,  1721.5000000, 1872.3000000, 1351.3999900, 0.0000000, 0.0000000, 0.0000000 }, //object(vegasnfrates06) (1)
				{ 7621,  1687.7002000, 1872.6001000, 1351.3999900, 0.0000000, 0.0000000, 0.0000000 }, //object(vegasnfrates06) (2)
				{ 7621,  1679.2002000, 1841.0000000, 1351.3999900, 0.0000000, 0.0000000, 90.2500000 }, //object(vegasnfrates06) (3)
				{ 7621,  1679.3999000, 1786.3000000, 1351.3999900, 0.0000000, 0.0000000, 90.2470000 }, //object(vegasnfrates06) (4)
				{ 7621,  1679.5000000, 1731.3999000, 1351.3999900, 0.0000000, 0.0000000, 90.2470000 }, //object(vegasnfrates06) (5)
				{ 7621,  1710.1001000, 1736.6001000, 1351.3999900, 0.0000000, 0.0000000, 180.2420000 }, //object(vegasnfrates06) (6)
				{ 7621,  1764.7002000, 1736.2000000, 1351.3999900, 0.0000000, 0.0000000, 180.2420000 }, //object(vegasnfrates06) (7)
				{ 7621,  1764.7002000, 1736.8000000, 1351.3999900, 0.0000000, 0.0000000, 180.2420000 }, //object(vegasnfrates06) (8)
				{ 7621,  1758.3999000, 1773.1001000, 1351.3999900, 0.0000000, 0.0000000, 270.2420000 }, //object(vegasnfrates06) (9)
				{ 7621,  1758.2002000, 1827.8000000, 1351.3999900, 0.0000000, 0.0000000, 270.2310000 }, //object(vegasnfrates06) (10)
				{ 7621,  1759.0000000, 1882.0000000, 1351.3999900, 0.0000000, 0.0000000, 270.2360000 }, //object(vegasnfrates06) (11)
				{ 7040,  1734.7002000, 1781.6001000, 1349.5000000, 0.0000000, 0.0000000, 0.0000000 }, //object(vgnplcehldbox01) (2)
				{ 7040,  1712.3999000, 1774.6001000, 1349.5000000, 0.0000000, 0.0000000, 0.0000000 }, //object(vgnplcehldbox01) (3)
				{ 7025,  1728.6001000, 1796.3999000, 1349.5000000, 0.0000000, 0.0000000, 52.0000000 }, //object(plantbox1) (1)
				{ 5262,  1737.3999000, 1839.0000000, 1349.1000100, 0.0000000, 0.0000000, 269.0000000 }, //object(las2dkwar04) (1)
				{ 3633,  1718.2002000, 1814.6001000, 1346.6000100, 0.0000000, 0.0000000, 0.0000000 }, //object(imoildrum4_las) (1)
				{ 3633,  1703.7998000, 1818.6001000, 1346.6000100, 0.0000000, 0.0000000, 0.0000000 }, //object(imoildrum4_las) (2)
				{ 3633,  1691.6001000, 1807.1001000, 1346.6000100, 0.0000000, 0.0000000, 0.0000000 }, //object(imoildrum4_las) (3)
				{ 3633,  1697.1001000, 1796.8999000, 1346.6000100, 0.0000000, 0.0000000, 0.0000000 }, //object(imoildrum4_las) (4)
				{ 3633,  1691.7002000, 1788.8000000, 1346.6000100, 0.0000000, 0.0000000, 0.0000000 }, //object(imoildrum4_las) (5)
				{ 3633,  1739.2998000, 1819.3999000, 1346.6000100, 0.0000000, 0.0000000, 0.0000000 }, //object(imoildrum4_las) (6)
				{ 3633,  1737.8999000, 1811.6001000, 1346.6000100, 0.0000000, 0.0000000, 0.0000000 }, //object(imoildrum4_las) (7)
				{ 8886,  1697.8999000, 1768.3000000, 1349.5000000, 0.0000000, 0.0000000, 0.0000000 } //object(vgsefrght04) (1)
			})
		{}

		bool OnGameModeInit()
		{
			MakeAllDynamicObjects(ArenaObjects, CurrentGameVW);
			return true;
		}

		void Timer(int timerid, ZabawyTimerVector& vector)
		{
			int playerid = TimerDataGet<int>(vector, 0);
			if (Player[playerid].CurrentGameID == this)
			{
				safeGivePlayerWeapon(playerid, 16, 999999);
				fixSetPlayerHealth(playerid, 100);
				fixSetPlayerArmour(playerid, 100);
				fixTogglePlayerControllable(playerid, true);
			}
		}

		void PutPlayerIntoGame(int playerid)
		{
			fixSetPlayerHealth(playerid, 10000);
			fixSetPlayerArmour(playerid, 10000);
			fixTogglePlayerControllable(playerid, false);

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
			AddPlayer(playerid, true, 0xFFFFFF00);
			PutPlayerIntoGame(playerid);
			return true;
		}

		bool OnPlayerDeath(int playerid, int killerid, int reason)
		{
			GivePlayerAchievement(playerid, EAM_ZabawyGranatyDeaths, 1);
			if (killerid != INVALID_PLAYER_ID)
			{
				GivePlayerAchievement(killerid, EAM_ZabawyGranatyKills, 1);
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