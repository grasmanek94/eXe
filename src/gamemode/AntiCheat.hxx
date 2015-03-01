/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014
		
	________________________________________________________________
	Purpose of this file
		-	Definition of public replacement functions for default sa-mp
			functions which need to interact with the anticheat

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

static const unsigned int CHECK_JETPACK = (1 << 0);					
static const unsigned int CHECK_WEAPON = (1 << 1);					
static const unsigned int CHECK_SPEED = (1 << 2);					
static const unsigned int CHECK_HEALTHARMOUR = (1 << 3);			
static const unsigned int CHECK_IPFLOOD = (1 << 4);					
static const unsigned int CHECK_PING = (1 << 5);					
static const unsigned int CHECK_SPOOFKILL = (1 << 6);				
static const unsigned int CHECK_SPAWNKILL = (1 << 7);				
static const unsigned int CHECK_INACTIVITY = (1 << 8);				
static const unsigned int CHECK_TELEPORT = (1 << 9);				
static const unsigned int CHECK_AIRBREAK = (1 << 10);				
static const unsigned int CHECK_BACK_FROM_INACTIVITY = (1 << 11);	
static const unsigned int CHECK_SPECTATE = (1 << 12);				
static const unsigned int CHECK_FASTCONNECT = (1 << 13);			
static const unsigned int CHECK_REMOTECONTROL = (1 << 14);			
static const unsigned int CHECK_MASSCARTELEPORT = (1 << 15);		
static const unsigned int CHECK_CARJACKHACK = (1 << 16);			
static const unsigned int CHECK_GAMESPEEDHACK = (1 << 17);			
static const unsigned int CHECK_FLYHACK = (1 << 18);				
static const unsigned int CHECK_VEHICLE_HEAL = (1 << 19);			
static const unsigned int CHECK_VEHICLE_GODMODE = (1 << 20);		
static const unsigned int CHECK_WEAPON_AMMO = (1 << 21);			
static const unsigned int CHECK_OBJECTEDIT_CRASH = (1 << 22);

//todo: anti lessdamagemod
/////////////////////////////////////////////////////////////////////////////////////

int safeSetSpawnInfo(int playerid, int team, int skin, float spawnX, float spawnY, float spawnZ, float angle, int weapon1, int weapon1ammo, int weapon2, int weapon2ammo, int weapon3, int weapon3ammo);
int safeAddPlayerClass(int modelid, float spawnX, float spawnY, float spawnZ, float angle, int weapon1, int weapon1ammo, int weapon2, int weapon2ammo, int weapon3, int weapon3ammo);
int safeAddPlayerClassEx(int teamid, int modelid, float spawnX, float spawnY, float spawnZ, float angle, int weapon1, int weapon1ammo, int weapon2, int weapon2ammo, int weapon3, int weapon3ammo);
int safeGivePlayerWeapon(int playerid, int weaponid, int ammo);
int safeResetPlayerWeapons(int playerid);
int fixTogglePlayerControllable(int playerid, bool toggle);
int fixPutPlayerInVehicle(int playerid, int vehicleid, int seat);
int fixSetPlayerPos(int playerid, float fX, float fY, float fZ);
int fixSetPlayerPosFindZ(int playerid, float fX, float fY, float fZ);
int fixSetVehiclePos(int vehicleid, float fX, float fY, float fZ);
int fixTogglePlayerSpectating(int playerid, bool toggle);
void CheckSet(int ToCheck, int playerid = INVALID_PLAYER_ID, bool check = true);
void SetWeaponAllowed(int playerid, int weaponid, bool allowed = true);
int safeSetPlayerSpecialAction(int playerid, int action);
int fixSetPlayerHealth(int playerid, float health);
int fixSetPlayerArmour(int playerid, float armour);
int safeRepairVehicle(int vehicleid);
int safeSetVehicleHealth(int vehicleid, float health);
void ___OnVehicleLostHealth(int playerid, int vehicleid, float oldhealth, float newhealth);
int fixSetPlayerVirtualWorld(int playerid, int worldid);
void extOnPlayerVirtualWorldChange(int playerid, int newVWid, int oldVWid);
bool IsValidObjectModel(int model);
void CheckSet(int ToCheck, int playerid, bool check);

extern std::set<int> SafeHealAreas;
extern std::set<int> SafeSwimmingAreas;
extern std::set<int> SafeSpeedHackAreas;

inline void AddSafeSpeedHackArea(float x1, float y1, float x2, float y2)
{ 
	SafeSpeedHackAreas.insert(StreamerLibrary::CreateDynamicRectangle(std::min(x1, x2), std::min(y1, y2), std::max(x1, x2), std::max(y1, y2)));
};
