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
#pragma once
#include "GameMode.hxx"
#include "CommandPermissions.hxx"
#include "0cmdpp.hxx"

namespace Zabawy
{
	enum PlayerRequestExitType
	{
		PlayerRequestExitTypeExit,
		PlayerRequestExitTypeDeath,
		PlayerRequestExitTypeDisconnect
	};

	struct SPos
	{
		float x;
		float y;
		float z;
	};

	struct SPosRot
	{
		float x;
		float y;
		float z;
		float rx;
		float ry;
		float rz;
	};

	struct SObject
	{
		int ModelID;
		float x;
		float y;
		float z;
		float rx;
		float ry;
		float rz;
	};

	struct SSpawnPoint
	{
		float x;
		float y;
		float z;
		float a;
	};

	
	typedef boost::variant<char, unsigned char, short, unsigned short, int,unsigned int, long, unsigned long, long long, unsigned long long, float, double, std::string> ZabawyTimerData;
	typedef std::vector<ZabawyTimerData> ZabawyTimerVector;
	typedef std::function<void(int timerid, ZabawyTimerVector& vector)> ZabawyVirtualTimerCall;	

	struct ZabawyTimerInternalResolver
	{
		bool repeat;
		ZabawyTimerVector data_vector;
		ZabawyVirtualTimerCall function;

		ZabawyTimerInternalResolver()
		{}

		ZabawyTimerInternalResolver(bool _repeat, ZabawyTimerVector _data_vector, ZabawyVirtualTimerCall _function)
			: repeat(_repeat), data_vector(_data_vector), function(_function)
		{}
	};

	typedef std::pair<int/*timerid*/, ZabawyTimerInternalResolver> ZabawyTimerInternalResolverMapItem;


	extern std::map<int/*timerid*/, ZabawyTimerInternalResolver> ZabawyTimers;

	void NeedUpdate();

	static const int ZabawyBaseVirtualWorld = 0x5000;

	void AddGameToList(std::string text);

	class Base;
	extern std::vector<Base*> * Games;
	extern std::map<std::string, Base*, ci_less> * CmdToGame;

	bool IsPlayerStaging(int playerid);
	bool PlayerStageCheckWithMessage(int playerid, unsigned char gameid);
	void ZabawyBaseTimerLauncher(int timerid, void * param);

	enum eSystemTypes
	{
		eST_Arena,
		eST_Zabawa,
		eST_Special,
		eST_Race
	};

	class Base
	{
	public:
		size_t CurrentGameID;
		int  CurrentGameVW;
		int TYPE;
		language_string_ids CurrentGameName;
		std::string CurrentGameCommandName;

		bool HeadShotsEnabled;
		bool enableallweaponheadshot;
		bool OneHitOneKill;

		std::set<int> PlayersInGame;
		std::set<int> HeadShotWeapons;

		Base(language_string_ids CurrentGameName_, std::string CurrentGameCommandName_, eSystemTypes type = eST_Arena, cmd_alias aliasses = cmd_alias({}));
		void PostInitOnLoad();

		//game
		virtual bool PlayerRequestGameExit(int playerid, int reason) = 0;
		virtual bool PlayerRequestCencelStaging(int playerid, int reason) { return true; }
		virtual bool OnGameCommandExecute(int playerid, std::string params) = 0;
		virtual bool Joinable() { return true; }
		virtual bool Staging() { return false; }
		virtual bool OnKeepInGameSpawn(int playerid) { return true; }
		virtual bool IsExe24PlusGame() { return false; }
		virtual int GetCommandRestrictions() { return RESTRICTION_IN_VEHICLE_OR_ONFOOT | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT; }

		//SA-MP
		virtual bool OnPlayerConnect(int playerid) { return true; }
		virtual bool OnPlayerDeath(int playerid, int killerid, int reason) { return true; }
		virtual bool OnGameModeInit() { return true; }
		virtual void ProcessTick() { }
		virtual bool OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float x, float y, float z) { return true; }
		virtual bool OnPlayerLeaveDynamicArea(int playerid, int areaid) { return true; }
		virtual bool OnPlayerStateChange(int playerid, int newstate, int oldstate) { return true; }
		virtual bool OnDynamicObjectMoved(int objectid) { return true; }
		virtual bool OnPlayerPickUpDynamicPickup(int playerid, int pickupid) { return true; }
		virtual bool OnPlayerEnterRaceCheckpoint(int playerid) { return true; }
		virtual bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) { return true; }
		//Dtor
		virtual ~Base() {}

		void PlayerExitGame(int playerid);
		
		void AddStagingPlayer(int playerid, int newcolor = 0);
		void SetPlayerStaging(int playerid);

		void AddPlayer(int playerid, bool keep_in_game_after_death = false, int newcolor = 0);
		void RemoveAllPlayers(bool remove_only_staging = false);

		unsigned char GetDisplayColor();

		bool DestroyTimer(int timerid);

		template<typename T> T TimerDataGet(ZabawyTimerVector& vector_data, size_t index = 0)
		{
			return boost::get<T>(vector_data[index]);
		}

		template <class T> int CreateTimer(int interval, bool repeat, void(T::*func)(int, ZabawyTimerVector&), ZabawyTimerVector vector)
		{
			return ZabawyTimers.insert(
				ZabawyTimerInternalResolverMapItem(
					sampgdk_SetTimerEx(interval, repeat, ZabawyBaseTimerLauncher, nullptr, this), 
					ZabawyTimerInternalResolver(
						repeat, 
						vector,
						std::bind(func, (T*)this, std::placeholders::_1, std::placeholders::_2)
					)
				)
			).first->first;		
		}

		template <class T> int CreateTimer(int interval, bool repeat, void(T::*func)(int, ZabawyTimerVector&), ZabawyTimerData data)
		{
			return ZabawyTimers.insert(
				ZabawyTimerInternalResolverMapItem(
					sampgdk_SetTimerEx(interval, repeat, ZabawyBaseTimerLauncher, nullptr, this), 
					ZabawyTimerInternalResolver(
						repeat, 
						ZabawyTimerVector({ data }),
						std::bind(func, (T*)this, std::placeholders::_1, std::placeholders::_2)
					)
				)
			).first->first;		
		}

		template <class T> int CreateTimer(int interval, bool repeat, void(T::*func)(int, ZabawyTimerVector&))
		{
			return ZabawyTimers.insert(
				ZabawyTimerInternalResolverMapItem(
					sampgdk_SetTimerEx(interval, repeat, ZabawyBaseTimerLauncher, nullptr, this), 
					ZabawyTimerInternalResolver(
						repeat, 
						ZabawyTimerVector(), 
						std::bind(func, (T*)this, std::placeholders::_1, std::placeholders::_2)
					)
				)
			).first->first;
		}

		const std::set<int> GetPlayersCopy();

		void DestroyTimerValidate(int& timerid);
		void SwitchGameTimer(int& timerid, int newtimerid);

		void EnableHeadShotsFromAllWeapons(bool enable = true);
		void EnableHeadShotsFromWeapon(int weaponid, bool enable = true);
	};

	void MakeAllDynamicObjects(std::vector<SObject>& objects, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 200.0f, float drawdistance = 0.0f);

	template <size_t SIZE> void BalanceTeams(const std::set<int>& Players, std::array<std::set<int>, SIZE>& TeamsArray, std::set<unsigned int>& ActiveTeams)
	{
		for (auto& cta : TeamsArray)
			cta.clear();

		ActiveTeams.clear();

		size_t current = 0;

		for (auto playerid : Players)
		{
			TeamsArray[current].insert(playerid);
			Player[playerid].SetTeam(current);
			if (++current == SIZE)
				current = 0;
		}

		for (size_t index = 0; index < SIZE; ++index)
		{
			if (TeamsArray[index].size())
				ActiveTeams.insert((unsigned int)index);
		}
	}
};