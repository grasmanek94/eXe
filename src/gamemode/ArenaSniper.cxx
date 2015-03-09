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

namespace Sniper
{
	class Arena : public Base
	{
	public:

		std::vector<SSpawnPoint> ArenaSpawns;
		std::vector<SObject> ArenaObjects;

		Arena() :
			Base(L_games_ArenaSniper_name, "/snp"),
			ArenaSpawns({
				{ 188.1574, 1931.3578, 17.6749, 288.6865 },
				{ 153.5499, 1845.0328, 17.6406, 358.2236 },
				{ 171.7195, 1834.5513, 17.6406, 93.7678 },
				{ 211.4944, 1811.1135, 21.8672, 3.8636 },
				{ 224.4762, 1872.2844, 13.7344, 95.6477 },
				{ 0.0f, 0.0f, 0.0f, 0.0f }
		}),
		ArenaObjects({
			{ 2939, 227.1000100, 1869.7000000, 12.7000000, 0.0000000, 268.0000000, 338.0000000 },//object(ramp_bot) (1)
			{ 2939, 266.8999900, 1876.8000000, 16.1000000, 338.0140000, 2.1570000, 358.8080000 },//object(ramp_bot) (2)
			{ 843, 267.2000100, 1860.3000000, 18.5000000, 0.0000000, 0.0000000, 0.0000000 },//object(dead_tree_15) (1)
			{ 843, 232.8999900, 1914.3000000, 18.5000000, 0.0000000, 0.0000000, 0.0000000 },//object(dead_tree_15) (2)
			{ 843, 174.3000000, 1877.7000000, 21.7000000, 0.0000000, 0.0000000, 0.0000000 },//object(dead_tree_15) (3)
			{ 622, 150.1000100, 1888.5000000, 17.3000000, 0.0000000, 0.0000000, 0.0000000 },//object(veg_palm03) (1)
			{ 622, 239.8999900, 1813.7000000, 16.6000000, 0.0000000, 0.0000000, 0.0000000 },//object(veg_palm03) (2)
			{ 648, 246.5000000, 1907.6000000, 19.1000000, 0.0000000, 0.0000000, 0.0000000 },//object(veg_palm01) (1)
			{ 681, 245.1000100, 1873.6000000, 19.2000000, 0.0000000, 0.0000000, 0.0000000 },//object(sm_des_josh_sm2) (1)
			{ 681, 178.2000000, 1890.8000000, 19.3000000, 0.0000000, 0.0000000, 0.0000000 },//object(sm_des_josh_sm2) (2)
			{ 703, 214.6000100, 1927.1000000, 33.0000000, 0.0000000, 0.0000000, 0.0000000 },//object(sm_veg_tree7_big) (1)
			{ 703, 177.2000000, 1859.5000000, 19.3000000, 0.0000000, 0.0000000, 0.0000000 },//object(sm_veg_tree7_big) (2)
			{ 647, 267.5000000, 1881.6000000, 5.7000000, 0.0000000, 0.0000000, 0.0000000 },//object(new_bushsm) (1)
			{ 647, 268.1000100, 1893.9000000, 34.7000000, 0.0000000, 0.0000000, 0.0000000 },//object(new_bushsm) (2)
			{ 762, 195.6210900, 1915.5087900, 38.0027300, 0.0000000, 0.0000000, 0.0000000 },//object(new_bushtest) (1)
			{ 3269, 221.2000000, 1874.6000000, 16.6000000, 0.0000000, 0.0000000, 0.0000000 },//object(bonyrd_block1_) (1)
			{ 3270, 124.3000000, 1826.3000000, 16.6000000, 0.0000000, 0.0000000, 0.0000000 },//object(bonyrd_block2_) (1)
			{ 3363, 169.0000000, 1899.1000000, 17.4000000, 0.0000000, 0.0000000, 90.0000000 },//object(des_ruin1_) (1)
			{ 6052, 241.5000000, 1888.5000000, 20.6000000, 0.0000000, 0.0000000, 0.0000000 },//object(artcurve_law) (1)
			{ 8875, 168.6000100, 1882.8000000, 25.8000000, 0.0000000, 0.0000000, 0.0000000 },//object(vgsecnstrct14) (1)
			{ 3567, 195.3000000, 1898.9000000, 17.5000000, 0.0000000, 0.0000000, 0.0000000 },//object(lasnfltrail) (2)
			{ 2669, 150.3999900, 1881.6000000, 18.4000000, 0.0000000, 0.0000000, 0.0000000 },//object(cj_chris_crate) (1)
			{ 2669, 255.3000000, 1878.5000000, 20.6000000, 0.0000000, 0.0000000, 0.0000000 },//object(cj_chris_crate) (2)
			{ 13489, 236.6000100, 1924.5000000, 19.3000000, 0.0000000, 0.0000000, 0.0000000 },//object(sw_fueldrum04) (1)
			{ 1454, 225.0000000, 1861.2000000, 20.4000000, 0.0000000, 0.0000000, 0.0000000 },//object(dyn_h_bale2) (1)
			{ 12918, 177.7000000, 1872.1000000, 19.0000000, 0.0000000, 0.0000000, 270.0000000 }//object(sw_haypile05) (1)
				})
		{
			EnableHeadShotsFromAllWeapons();
		}

		bool OnGameModeInit() override
		{
			MakeAllDynamicObjects(ArenaObjects, CurrentGameVW);
			return true;
		}

		bool PlayerRequestGameExit(int playerid, int reason) override
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

		void PutPlayerIntoGame(int playerid)
		{
			auto i = ArenaSpawns[Functions::RandomGenerator->Random(0, ArenaSpawns.size() - 1)];
			TeleportPlayer(playerid, i.x, i.y, i.z, i.a, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
			CreateTimer<Arena>(333, false, &Arena::Timer, playerid);
		}

		void Timer(int timerid, ZabawyTimerVector& vector)
		{
			int playerid = TimerDataGet<int>(vector, 0);
			if (Player[playerid].CurrentGameID == this)
			{
				safeGivePlayerWeapon(playerid, 34, 999999);
			}
		}

		bool OnKeepInGameSpawn(int playerid) override
		{
			PutPlayerIntoGame(playerid);
			return true;
		}

		bool OnPlayerDeath(int playerid, int killerid, int reason) override
		{
			GivePlayerAchievement(playerid, EAM_ZabawySniperDeaths, 1);
			if (killerid != INVALID_PLAYER_ID)
			{
				GivePlayerAchievement(killerid, EAM_ZabawySniperKills, 1);
			}
			return true;
		}

		bool OnGameCommandExecute(int playerid, std::string params) override
		{
			AddPlayer(playerid, true, 0xFFFFFF00);
			PutPlayerIntoGame(playerid);
			return true;
		}
	} _Arena;
};