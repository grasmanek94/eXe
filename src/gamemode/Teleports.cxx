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

#pragma warning(disable:4305)

void TeleportPlayer(int playerid, float x, float y, float z, float a, bool vehicle, int interiorid, int virtualworldid, std::string gametext, int gametexttime, int gametextstyle, float maxrx, float maxry, bool reset_weapons, bool update_streamer)
{
	if (PlayersOnline.find(playerid) != PlayersOnline.end())
	{
		if (reset_weapons)
		{
			safeResetPlayerWeapons(playerid);
		}

		float adder[2] = { 0.0f, 0.0f };

		if (maxrx != 0.0)
		{
			adder[0] = Functions::gen_random_float(-maxrx, +maxrx);
		}

		if (maxry != 0.0)
		{
			adder[1] = Functions::gen_random_float(-maxry, +maxry);
		}

		SetPlayerInterior(playerid, interiorid);
		fixSetPlayerVirtualWorld(playerid, virtualworldid);

		if (gametext.size())
		{
			GameTextForPlayer(playerid, gametext.c_str(), gametexttime, gametextstyle);
		}

		if (vehicle && Player[playerid].CurrentVehicle && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		{
			fixSetVehiclePos(Player[playerid].CurrentVehicle, x + adder[0], y + adder[1], z);
			SetVehicleZAngle(Player[playerid].CurrentVehicle, a);
			LinkVehicleToInterior(Player[playerid].CurrentVehicle, interiorid);
			SetVehicleVirtualWorld(Player[playerid].CurrentVehicle, virtualworldid);
		}
		else
		{
			fixSetPlayerPos(playerid, x + adder[0], y + adder[1], z);
			SetPlayerFacingAngle(playerid, a);
		}

		GivePlayerAchievement(playerid, EAM_Teleporter, 1);

		if (update_streamer)
		{
			StreamerLibrary::Streamer_UpdateEx(playerid, x, y, z, virtualworldid, interiorid);
		}
	}
}

ZERO_COMMAND3(wiezowiec, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1544.9739, -1353.2709, 329.4744, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(walka, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1570.9393, -1244.2360, 277.8774, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(cjlot, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 424.1675, 2514.2297, 16.1913, 90.9332, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1666.4323, -1246.4912, 233.3750, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza3, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1455.5492, -1065.0872, 213.3828, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza4, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1513.9174, -1080.9136, 181.2031, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza5, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1385.4972, -1208.3534, 202.4453, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza6, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1424.2277, -1204.6274, 195.0469, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza7, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1474.2690, -1353.9392, 163.7188, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza8, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1670.5518, -1343.3254, 158.4766, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza9, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1421.7285, -1475.6760, 125.3672, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza10, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1488.2866, -1266.2251, 113.7795, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza11, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1683.0547, -1391.3328, 84.6563, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wieza12, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1816.2559, -1297.4911, 131.7332, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(hacjenda, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1293.4426, -787.4669, 96.4554, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(skatepark, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1876.0181, -1385.7559, 13.5567, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(ls, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2494.2742, -1671.2235, 13.3359, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(koloseumls, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2788.3767, -1855.6229, 9.8355, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZCMD(tunels, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT, cmd_alias({ "/lowrider" }))
{
	TeleportPlayer(playerid, 2644.7351, -2003.8347, 13.2563, 180.2385, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(portls, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2575.8342, -2372.8958, 13.9129, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(lslot, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1953.5204, -2290.1130, 13.5469, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(stacjals, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1707.4773, -1949.4626, 14.1172, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(stacjals2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2016.2495, -1964.8615, 16.6941, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(plaza, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 306.0038, -1847.1047, 3.5841, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(plazals2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2895.5266, -2051.2097, 3.5659, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(jeziorko, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -818.6106, -2051.5264, 25.4066, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(driftls, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -757.4308, -1628.5690, 96.5873, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(zamek, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1150.3993, -2038.2489, 69.0078, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(molols, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 838.1479, -2012.3489, 12.8672, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(molols2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 370.0555, -1968.9962, 7.6719, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(PGR, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 68.4253, -224.2566, 1.5781, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(zadupie, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1383.3280, -1507.3010, 102.2328, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(gora, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1418.2213, -948.6544, 201.0938, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(sflot, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1538.8635, -422.9142, 5.8516, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(sf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2261.2009, 564.2894, 34.7200, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(koloseumsf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2166.4258, -413.4754, 35.0404, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(stacjasf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1964.9642, 148.5759, 27.2633, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(salon, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1987.7372, 288.7828, 34.5681, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(salon2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1637.7758, 1223.5553, 7.2194, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(plazasf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2896.8655, 144.7969, 4.9552, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezasf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2054.1460, 446.7105, 139.9188, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezasf2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1942.8779, 538.8998, 209.0944, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezasf3, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1810.8959, 531.6208, 235.0648, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezasf4, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1754.5250, 790.7621, 167.8330, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezasf5, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1753.9846, 885.1415, 295.8750, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezasf6, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1917.7532, 665.3909, 145.3181, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(budyneksf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2439.9031, 362.6330, 88.5163, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(driftsf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1808.4343, 1301.3114, 59.9110, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(kosciol, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2692.2734, -237.8270, 6.9716, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(radio, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2447.7688, -619.6808, 132.7393, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(tunesf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2699.7595, 217.2415, 3.9903, 89.7891, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(chillad, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2321.1321, -1634.2689, 483.8788, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(zakochani, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2647.1643, 1408.9735, 906.2734, 0.0, false, 3, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(stateksf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2417.2703, 1546.2479, 32.0360, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(stateksf2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1421.1108, 1485.9211, 11.9851, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(stateksf3, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1424.4974, 500.2057, 18.4061, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(portsf, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2227.4998, 2325.6189, 7.7236, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(portsf2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1569.1106, 105.1680, 3.7313, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(g1, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2261.3674, 1398.6605, 42.4787, 267.9729, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(cementarzls, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 823.7956, -1103.0261, 25.7891, 267.9729, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(g2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2008.1486, 1732.1975, 18.9339, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(g3, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2074.0437, 2416.8750, 49.5234, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(g4, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1700.6284, 1194.1071, 34.7891, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(budynek, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1440.5228, 2155.8462, 29.8675, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(minidrift, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1121.7252, 2200.9453, 16.7188, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(zajezdnia, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -219.1885, 2601.7380, 62.8800, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2061.0576, 2435.3770, 165.7939, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2201.3179, 2190.3064, 104.0552, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv3, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2211.9817, 2091.0667, 100.5501, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv4, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1962.2118, 1911.7910, 131.1139, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv5, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1935.8817, 1629.5906, 72.4344, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv6, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1899.2327, 1424.6434, 72.7827, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv7, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1862.9833, 1302.3127, 55.5497, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv8, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1968.1450, 1224.4768, 63.0629, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv9, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2198.5457, 999.7937, 86.1142, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wiezalv10, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2006.2198, 1006.7678, 39.2672, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wojsko, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 351.3806, 1786.0936, 17.9556, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wojsko2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 72.7191, 1917.2032, 17.8172, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(lvlot, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1686.3107, 1609.5485, 10.8203, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(tir, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -586.4269, -487.4195, 25.5234, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(telebank, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2186.7747, 1991.9857, 10.8203, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(dh, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -287.3147, 1526.4092, 75.5359, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(hop, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -904.9630, 1919.9769, 123.0391, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(tama, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -912.1113, 2005.2953, 60.4852, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(elektrownia, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -957.1490, 1948.5901, 9.0000, 0.0, false, 17, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(miasteczko, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -393.5246, 2280.4822, 40.7083, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(kladka, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -679.5541, 2311.9834, 133.4530, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(impra, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 489.0615, -14.6790, 1000.6797, 0.0, false, 17, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(tunelv, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2387.0808, 1016.9999, 10.5459, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(molo, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2290.3882, 611.6700, 10.5462, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(stacjalv, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2851.4250, 1290.9858, 11.1149, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(koloseumlv, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1132.2198, 1673.0586, 5.5250, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(kopalnia, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 667.0722, 898.3034, -40.8398, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(lv, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2140.6675, 993.1867, 10.5248, 0.0, true, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(libertycity, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -737.0225, 489.1295, 1371.9766, 0.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(cela, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 198.6262, 161.7780, 1003.0300, 0.0, false, 3, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(andromeda, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 315.8185, 984.2496, 1959.0851, 0.0, false, 9, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(shamal, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1.5527, 32.4773, 1199.5938, 0.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(dd, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1405.4443, 946.1092, 1030.0840, 0.0, false, 15, 0, "", 0, 0, 2.0, 2.0, true);
	fixSendClientMessage(playerid, 0x33CCFFAA, "*** Aby opuœciæ Destruction Derby wpisz /ddexit");
	return true;
}
ZERO_COMMAND3(8track, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1403.8608, -254.5934, 1043.6169, 0.0, false, 7, 0, "", 0, 0, 2.0, 2.0, true);
	fixSendClientMessage(playerid, 0x33CCFFAA, "*** Aby opuœciæ 8-Track wpisz /8texit");
	return true;
}
ZERO_COMMAND3(db, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1433.8196, -653.9620, 1051.5610, 0.0, false, 4, 0, "", 0, 0, 2.0, 2.0, true);
	fixSendClientMessage(playerid, 0x33CCFFAA, "*** Aby opuœciæ Dirt-Bike wpisz /dbexit");
	return true;
}
ZERO_COMMAND3(kss, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1475.9512, 1640.5054, 1052.5313, 0.0, false, 14, 0, "", 0, 0, 2.0, 2.0, true);
	fixSendClientMessage(playerid, 0x33CCFFAA, "*** Aby opuœciæ Kickstart Stadium wpisz /kssexit");
	return true;
}
ZERO_COMMAND3(vc, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1396.3193, 86.3535, 1032.4810, 0.0, false, 1, 0, "", 0, 0, 2.0, 2.0, true);
	fixSendClientMessage(playerid, 0x33CCFFAA, "*** Aby opuœciæ Vice Stadium wpisz /vcexit");
	return true;
}
ZERO_COMMAND3(jump, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2504.1304, -707.0590, 279.7344, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(lot, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1827.1473, 7.2074, 1061.1436, 0.0, false, 14, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(lot2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -1855.5687, 41.2632, 1061.1436, 0.0, false, 14, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(tpammo, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 302.2929, -143.1391, 1004.0625, 0.0, false, 7, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(motel, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 446.6230, 509.3190, 1001.4195, 0.0, false, 12, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(rcshop, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2239.5710, 130.0224, 1035.4141, 0.0, false, 6, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(cpn, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 666.2331, -572.6985, 16.3359, 0.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(cjgarage, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2048.6060, 162.0934, 28.8359, 0.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(calligula, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2172.0037, 1620.7543, 999.9792, 0.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(Wooziebed, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -2158.7200, 641.2880, 1052.3817, 0.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(jaysdin, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 460.1000, -88.4285, 999.5547, 0.0, false, 4, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(woc, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 451.6645, -18.1390, 1001.1328, 0.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(tsdin, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 681.4750, -451.1510, -25.6172, 235.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(denise, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 244.4120, 305.0330, 999.1484, 235.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(katie, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 271.8850, 306.6320, 999.1484, 90.0, false, 2, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(helena, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 291.2830, 310.0320, 999.1484, 90.0, false, 3, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(mischelle, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 302.1810, 300.7230, 999.1484, 0.0, false, 4, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(barbara, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 322.1980, 303.5611, 999.1484, 0.0, false, 5, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(millie, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 346.8700, 309.2590, 999.1557, 180.0, false, 6, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wh, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1412.6399, -1.7875, 1000.9244, 180.0, false, 1, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(wh2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 1302.5199, -1.7875, 1001.0283, 180.0, false, 18, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(resta, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2324.4199, -1147.5400, 1050.7101, 180.0, false, 12, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(rcbattle, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, -972.4957, 1060.9830, 1345.6693, 180.0, false, 10, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(bs, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 2543.6599, -1303.6299, 1025.0703, 180.0, false, 2, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(brothel, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 747.6089, 1438.7130, 1102.9531, 180.0, false, 6, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(brothel2, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	TeleportPlayer(playerid, 942.1720, -17.0070, 1000.9297, 180.0, false, 3, 0, "", 0, 0, 2.0, 2.0);
	return true;
}
ZERO_COMMAND3(solo, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)
{
	fixSetPlayerPos(playerid, 1466.8445, -1085.1541, 213.3828);
	SetPlayerInterior(playerid, 0);
	SetPlayerFacingAngle(playerid, 220);
	return true;
}
ZERO_COMMAND3(kill, PERMISSION_NONE, RESTRICTION_NONE)
{
	SetPlayerInterior(playerid, 0);
	fixSetPlayerHealth(playerid, 0.0);
	return true;
}
ZCMD(respawn, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_ONLY_ON_FOOT, cmd_alias({ "/vcexit", "/ddexit", "/8texit", "/dbexit", "/kssexit" }))
{
	SetPlayerInterior(playerid, 0);
	SpawnPlayer(playerid);
	return true;
}