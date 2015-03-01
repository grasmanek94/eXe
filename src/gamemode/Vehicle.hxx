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

enum eVehicleModelInfo
{
	VEHICLE_AIRPLANE = 1 << 0,
	VEHICLE_HELICOPTER = 1 << 1,
	VEHICLE_BIKE = 1 << 2,
	VEHICLE_KABRIOLET = 1 << 3,
	VEHICLE_INDUSTRIA = 1 << 4,
	VEHICLE_LOWRIDER = 1 << 5,
	VEHICLE_OFFROAD = 1 << 6,
	VEHICLE_PUBLICSERVICE = 1 << 7,
	VEHICLE_SALOON = 1 << 8,
	VEHICLE_SPORT = 1 << 9,
	VEHICLE_COMBI = 1 << 10,
	VEHICLE_BOAT = 1 << 11,
	VEHICLE_TRAILER = 1 << 12,
	VEHICLE_UNIQUE = 1 << 13,
	VEHICLE_TRAIN = 1 << 14,
	VEHICLE_TRAINTRAILER = 1 << 15,
	VEHICLE_REMOTECONTROL = 1 << 16,
	VEHICLE_ARMED = 1 << 17,
	VEHICLE_CAR = 1 << 18,
	VEHICLE_LAND = 1 << 19,
	VEHICLE_AIR = 1 << 20,
	VEHICLE_WATER = 1 << 21
};

extern int VehicleModelInfo[612];

inline bool IsVehicleAirplane(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_AIRPLANE);
}

inline bool IsVehicleHelicopter(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_HELICOPTER);
}

inline bool IsVehicleBike(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_BIKE);
}

inline bool IsVehicleTrailer(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_TRAILER);
}

inline bool IsVehicleRC(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_REMOTECONTROL);
}

inline bool IsVehicleArmed(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_ARMED);
}

inline bool IsVehicleLand(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_LAND);
}

inline bool IsVehicleAir(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_AIR);
}

inline bool IsVehicleBoat(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_BOAT);
}

inline bool IsVehicleWater(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_WATER);
}

inline bool IsVehicleCar(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_CAR);
}

inline bool IsVehicleTrain(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_TRAIN);
}

inline bool IsVehicleTrainTrailer(int VehicleModel) {
	return
		(VehicleModelInfo[(VehicleModel)] & VEHICLE_TRAINTRAILER);
}

void CreatePlayerVehicleSafe(int playerid, int modelid, float ax = 0.0, float ay = 0.0, float az = 1.5);

struct VehicleSpawnInfo
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
	int vehicleid;
};

int MakeSpawnInfoVehicle(VehicleSpawnInfo& info);
void MakeAllSpawnInfoVehicles(std::vector<VehicleSpawnInfo>& vehicles);