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
#include "../gtasa/CarColorGenerator.hxx"

#define INVALID_PAINTJOB_ID (3)

struct SExtraVehicleInfo
{
	std::array<int,2> Color;
	int Paintjob;
};

std::array<SExtraVehicleInfo, MAX_VEHICLES + 1> ExtraVehicleInfo;

const std::array<int, 2> GetVehicleColor(int vehicleid)
{
	return ExtraVehicleInfo[vehicleid].Color;
}

int GetVehiclePaintjob(int vehicleid)
{
	return ExtraVehicleInfo[vehicleid].Paintjob;
}

int fixChangeVehicleColor(int vehicleid, int col1, int col2)
{
	ExtraVehicleInfo[vehicleid].Color[0] = col1;
	ExtraVehicleInfo[vehicleid].Color[1] = col2;
	return ChangeVehicleColor(vehicleid, col1, col2);
}

int fixChangeVehiclePaintjob(int vehicleid, int id)
{
	ExtraVehicleInfo[vehicleid].Paintjob = id;
	return ChangeVehiclePaintjob(vehicleid, id);
}

void ReApplyShit(int vehicleid)
{
	ChangeVehicleColor(vehicleid, ExtraVehicleInfo[vehicleid].Color[0], ExtraVehicleInfo[vehicleid].Color[1]);
	ChangeVehiclePaintjob(vehicleid, ExtraVehicleInfo[vehicleid].Paintjob);
}

int fixCreateVehicle(int modelid, float x, float y, float z, float a, int col1, int col2, int respawntime)
{
	// make sure we'll use only native gta:sa colors, since sa-mp's colors sucks for some vehicles
	const std::array<int, 2> carColor = GTACarColorGenerator::GenerateColorByCarModel(modelid);

	if (col1 == -1 || col1 > 126)
		col1 = carColor[0];
	if (col2 == -1 || col2 > 126)
		col2 = carColor[1];

	int id = CreateVehicle(modelid, x, y, z, a, col1, col2, respawntime);
	if (id != 0xFFFF && id)
	{
		SetVehicleNumberPlate(id, "{0000FF}eXe24 {FF0000}DM");

		ExtraVehicleInfo[id].Color[0] = col1;
		ExtraVehicleInfo[id].Color[1] = col2;
		ExtraVehicleInfo[id].Paintjob = INVALID_PAINTJOB_ID;
	}
	return id;
}

class CVehicleExtraFunctions : public Extension::Base
{
public:
	void Load()
	{
		for (auto&vehicle : ExtraVehicleInfo)
		{
			vehicle.Color[0] = 0;
			vehicle.Color[1] = 0;
			vehicle.Paintjob = INVALID_PAINTJOB_ID;

		}
	}
	bool OnVehicleRespray(int playerid, int vehicleid, int color1, int color2)
	{
		ExtraVehicleInfo[vehicleid].Color[0] = color1;
		ExtraVehicleInfo[vehicleid].Color[1] = color2;
		return true;
	}
	bool OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid)
	{
		ExtraVehicleInfo[vehicleid].Paintjob = paintjobid;
		return true;
	}
	bool OnVehicleStreamIn(int vehicleid, int forplayerid)
	{
		ReApplyShit(vehicleid);
		return true;
	}
} _CVehicleExtraFunctions;