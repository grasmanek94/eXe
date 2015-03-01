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
#include "MiniGameSystem.hxx"
#include "Mafia.hxx"

namespace Extension
{
	class Base;
	typedef std::pair<long long, Base*> extension_filler;
	extern std::vector<extension_filler> * Extensions;

	bool Prioritizer (extension_filler &i,extension_filler &j);

	class Base
	{
		
	public:
		//max priority == 0xFFFF (most important), minimal == 0x0000 (least important)
		Base(long long priority = 0) 
		{
			if (Extensions == nullptr)
			{
				Extensions = new std::vector < extension_filler > ;
			}

			Extensions->push_back(extension_filler(priority,this));
			std::sort(Extensions->begin(),Extensions->end(),Prioritizer);
		}

		//SA-MP
		virtual bool OnGameModeInit() { return true; }
		virtual bool OnGameModeExit() { return true; }
		virtual bool OnPlayerConnect(int playerid) { return true; }
		virtual bool OnPlayerDisconnect(int playerid, int reason) { return true; }
		virtual bool OnPlayerSpawn(int playerid) { return true; }
		virtual bool OnPlayerDeath(int playerid, int killerid, int reason) { return true; }
		virtual bool OnVehicleSpawn(int vehicleid) { return true; }
		virtual bool OnVehicleDeath(int vehicleid, int killerid) { return true; }
		virtual bool OnPlayerText(int playerid, std::string text) { return true; }
		virtual bool OnPlayerCommandText(int playerid, std::string cmdtext) { return true; }
		virtual bool OnPlayerRequestClass(int playerid, int classid) { return true; }
		virtual bool OnPlayerEnterVehicle(int playerid, int vehicleid, bool ispassenger) { return true; }
		virtual bool OnPlayerExitVehicle(int playerid, int vehicleid) { return true; }
		virtual bool OnPlayerStateChange(int playerid, int newstate, int oldstate) { return true; }
		virtual bool OnPlayerEnterCheckpoint(int playerid) { return true; }
		virtual bool OnPlayerLeaveCheckpoint(int playerid) { return true; }
		virtual bool OnPlayerEnterRaceCheckpoint(int playerid) { return true; }
		virtual bool OnPlayerLeaveRaceCheckpoint(int playerid) { return true; }
		virtual bool OnRconCommand(std::string cmd) { return false; }
		virtual bool OnPlayerRequestSpawn(int playerid) { return true; }
		virtual bool OnObjectMoved(int objectid) { return true; }
		virtual bool OnPlayerObjectMoved(int playerid, int objectid) { return true; }
		virtual bool OnPlayerPickUpPickup(int playerid, int pickupid) { return true; }
		virtual bool OnVehicleMod(int playerid, int vehicleid, int componentid) { return true; }
		virtual bool OnEnterExitModShop(int playerid, int enterexit, int interiorid) { return true; }
		virtual bool OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid) { return true; }
		virtual bool OnVehicleRespray(int playerid, int vehicleid, int color1, int color2) { return true; }
		virtual bool OnVehicleDamageStatusUpdate(int vehicleid, int playerid) { return true; }
		virtual bool OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat, float new_x, float new_y, float new_z, float vel_x, float vel_y, float vel_z) { return true; }
		virtual bool OnPlayerSelectedMenuRow(int playerid, int row) { return true; }
		virtual bool OnPlayerExitedMenu(int playerid) { return true; }
		virtual bool OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid) { return true; }
		virtual bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) { return true; }
		virtual bool OnRconLoginAttempt(std::string ip, std::string password, bool success) { return true; }
		virtual bool OnPlayerUpdate(int playerid) { return true; }
		virtual bool OnPlayerStreamIn(int playerid, int forplayerid) { return true; }
		virtual bool OnPlayerStreamOut(int playerid, int forplayerid) { return true; }
		virtual bool OnVehicleStreamIn(int vehicleid, int forplayerid) { return true; }
		virtual bool OnVehicleStreamOut(int vehicleid, int forplayerid) { return true; }
		virtual bool OnDialogResponse(int playerid, int dialogid, int response, int listitem, std::string inputtext) { return true; }
		virtual bool OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart) { return true; }
		virtual bool OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart) { return true; }
		virtual bool OnPlayerClickMap(int playerid, float fX, float fY, float fZ) { return true; }
		virtual bool OnPlayerClickTextDraw(int playerid, int clickedid) { return true; }
		virtual bool OnPlayerClickPlayerTextDraw(int playerid, int playertextid) { return true; }
		virtual bool OnIncomingConnection(int playerid, std::string ip_address, int port) { return true; }
		virtual bool OnTrailerUpdate(int playerid, int vehicleid) { return true; }
		virtual bool OnPlayerClickPlayer(int playerid, int clickedplayerid, int source) { return true; }
		virtual bool OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ) { return true; }
		virtual bool OnPlayerEditAttachedObject(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ) { return true; }
		virtual bool OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ) { return true; }
		virtual bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ) { return true; }
		virtual void OnHTTPResponse(int index, int response_code, std::string data) { }
		virtual void OnClientCheckResponse(int playerid, int type, int arg, int response) { }

		//Plugin
		virtual void Load() {}
		virtual void Unload() {}
		virtual void ProcessTick() {}

		//ZeroCMD
		virtual bool OnPlayerCommandReceived(int playerid, std::string& command, std::string& params, int min_execute_level, int this_restrictions) { return true; }
		virtual void OnPlayerCommandExecuted(int playerid, std::string& command, std::string& params, bool success) {}
		virtual void OnUnknownCommand(int playerid, std::string& command, std::string& params) {}

		//Streamer
		virtual bool OnDynamicObjectMoved(int objectid) { return true; }
		virtual bool OnPlayerEditDynamicObject(int playerid, int objectid, int response, float x, float y, float z, float rx, float ry, float rz) { return true; }
		virtual bool OnPlayerSelectDynamicObject(int playerid, int objectid, int modelid, float x, float y, float z) { return true; }
		virtual bool OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float x, float y, float z) { return true; }
		virtual bool OnPlayerPickUpDynamicPickup(int playerid, int pickupid) { return true; }
		virtual bool OnPlayerEnterDynamicCP(int playerid, int checkpointid) { return true; }
		virtual bool OnPlayerLeaveDynamicCP(int playerid, int checkpointid) { return true; }
		virtual bool OnPlayerEnterDynamicRaceCP(int playerid, int checkpointid) { return true; }
		virtual bool OnPlayerLeaveDynamicRaceCP(int playerid, int checkpointid) { return true; }
		virtual bool OnPlayerEnterDynamicArea(int playerid, int areaid) { return true; }
		virtual bool OnPlayerLeaveDynamicArea(int playerid, int areaid) { return true; }

		//account
		virtual bool OnPlayerDataAcquired(int playerid, int result) { return true; }

		//
		virtual void DatabaseOperationReport(WorkerTransportData * data) { }

		//
		virtual void OnPlayerGameBegin(int playerid) { }

		//Zabawy
		virtual void OnPlayerEnterMiniGame(int playerid, Zabawy::Base* game) { }

		//
		virtual void OnPlayerVirtualWorldChange(int playerid, int newVWid, int oldVWid) { }

		//Mafia
		virtual void OnMafiaLoad(int id, CMafia* mafia) { }
		virtual void OnMafiaUnload(int id, CMafia* mafia) { }

		//Dtor
		virtual ~Base() {}
	};
};

namespace AntiCmdSpam
{
	extern std::array<unsigned long long, MAX_PLAYERS> LastCmd;
	extern unsigned long long cmd_time;
};

namespace AntiChatSpam
{
	extern std::array<unsigned long long, MAX_PLAYERS> LastChat;
	extern unsigned long long chat_time;
	extern std::array<std::string, MAX_PLAYERS> lasttext;
};

void extOnPlayerEnterMiniGame(int playerid, Zabawy::Base* game);
void extOnMafiaLoad(int id, CMafia* mafia);
void extOnMafiaUnload(int id, CMafia* mafia);
void extOnPlayerGameBegin(int playerid);

extern void ** global_ppData;