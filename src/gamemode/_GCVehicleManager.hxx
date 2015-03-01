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

#ifdef VEHICLEMANAGER_ENABLE
using VEHICLEID = unsigned short;
using PLAYERID = unsigned short;

class CVehicle;

class CVehicleManager;

extern CVehicleManager* VehicleManager;

struct _GC_VehicleSpawnInfo
{
	int modelid;
	float x;
	float y;
	float z;
	float a;
	char color1;
	char color2;
	int interiorid;
	int virtualworld;
	int respawndelay;
};

class CVehicle
{
	VEHICLEID vehicleid;
	PLAYERID driver;
	std::set<PLAYERID> passengers;
	int _interiorid;
	int _virtualworld;
	std::array<int, 14>  Components;
	std::array<int, 2> Color;
	int Paintjob;
	bool created;
	glm::vec4 creationpos;
	std::string plate;
	int modelid;
	bool destroyonrespawn;
	//std::bitset<MAX_PLAYERS> _doorlocked;
	//struct NamedVehParams
	//{
	//	bool engine;
	//	bool lights;
	//	bool alarm;
	//	bool doors;
	//	bool bonnet;
	//	bool boot;
	//	bool objective;
	//};
	//union VehParams
	//{
	//	bool b_params[7];
	//	NamedVehParams n_params;
	//};
	//VehParams VehicleParameters;

	std::map<CVehicle*, VEHICLEID>::iterator VehPtrToIdItr;
	std::map<VEHICLEID, CVehicle*>::iterator IdToVehPtrItr;
	CVehicleManager* local_VehicleManager;

	friend class CVehicleManager;
	void ApplyIterators(CVehicleManager* manager, VEHICLEID _vehicleid);
public:
	CVehicle();
	VEHICLEID ID();
	bool GetPos(float& x, float& y, float& z);
	bool GetPos(float& x, float& y, float& z, float &a);
	glm::vec3 GetPos3();
	glm::vec4 GetPos4();
	bool GetPos3(glm::vec3& pos);
	bool GetPos4(glm::vec4& pos);
	bool SetPos(glm::vec3 position);
	bool SetPos(glm::vec4 position);
	bool SetPos(float x, float y, float z);
	bool SetPos(float x, float y, float z, float a);
	float GetAngle();
	bool GetAngle(float& a);
	bool SetAngle(float a);
	bool IsValid();
	float DistanceFromPoint(float x, float y, float z);
	float DistanceFromPoint(glm::vec3 position);
	bool IsStreamedIn(PLAYERID forplayerid);
	glm::vec4 GetQuat();
	bool GetQuat(glm::vec4& quat);
	bool GetQuat(float& x, float& y, float& z, float& w);
	bool Respawn();
	bool SetInterior(int interiorid);
	int GetInterior();
	bool SetVirtualWorld(int worldid);
	int GetVirtualWorld();
	bool DetachTrailer();
	bool HasTrailer();
	CVehicle* GetTrailer();
	VEHICLEID GetTrailer(VEHICLEID);
	bool AddComponent(int componentid);
	bool RemoveComponent(int componentid);
	bool ChangeColor(int color1, int color2);
	bool ChangeColor(int color);
	std::array<int, 2> GetColor();
	void GetColor(int& color1, int& color2);
	void GetColor(int& color);
	bool Repair();
	bool ChangePaintjob(int paintjobid);
	bool SetHealth(float health);
	float GetHealth();
	bool GetHealth(float &health);
	bool GetModelInfo(int infotype, float& X, float& Y, float& Z);
	bool AttachTrailer(VEHICLEID trailerid);
	bool AttachTrailer(CVehicle* trailerid);
	const int GetModel();
	bool SetNumberPlate(std::string strplate);
	const std::string GetNumberPlate();
	bool SetVelocity(float X, float Y, float Z);
	bool SetVelocity(glm::vec3 velocity);
	bool GetVelocity(float& X, float& Y, float& Z);
	bool GetVelocity(glm::vec3 &velocity);
	glm::vec3 GetVelocity();
	void GetMatrix(glm::mat4x3& Mat4x3);
	glm::mat4x3 GetMatrix();
	glm::mat4x3 PositionAtOffset(glm::vec3 offsets, glm::vec3& return_pos);
	bool Refresh(bool visualonly = false);
	static CVehicle* Create(int vehicletype, float x, float y, float z, float rotation, int color1, int color2, int interiorid, int virtualworld, int respawn_delay, bool dorefresh, bool destroyonrespawn);
	static CVehicle* Create(_GC_VehicleSpawnInfo& info, bool dorefresh, bool destroyonrespawn);
	bool Destroy();
	bool DestroyOnRespawn();
	//bool SetParamsForPlayer(int playerid, bool objective, bool doorslocked);
	//bool SetParamsEx(bool engine, bool lights, bool alarm, bool doors, bool bonnet, bool boot, bool objective);
	//bool GetParamsEx(bool * engine, bool * lights, bool * alarm, bool * doors, bool * bonnet, bool * boot, bool * objective);
	//bool GetDamageStatus(int * panels, int * doors, int * lights, int * tires);
	//bool UpdateDamageStatus(int panels, int doors, int lights, int tires);
	//bool SetAngularVelocity(float X, float Y, float Z);

	bool IsAirplane();
	bool IsHelicopter();
	bool IsBike();
	bool IsTrailer();
	bool IsRC();
	bool IsArmed();
	bool IsLand();
	bool IsAir();
	bool IsBoat();
	bool IsWater();
	bool IsCar();
	bool IsTrain();
	bool IsTrainTrailer();
};

class CVehicleManager
{
public:
	CVehicle* PlayerCurrentlyInVehicle[MAX_PLAYERS];
	CVehicle* PlayerCurrentlyDriving[MAX_PLAYERS];
	std::map<CVehicle*, VEHICLEID> VehPtrToId;
	std::map<VEHICLEID, CVehicle*> IdToVehPtr;
	std::array<CVehicle, MAX_VEHICLES + 1> Vehicles;
	CVehicleManager();
};
#endif