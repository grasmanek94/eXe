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

void FixPaintjobColors(int timerid, void *param)
{
	fixChangeVehicleColor((int)param, GetVehicleColor((int)param)[0], GetVehicleColor((int)param)[1]);
}

ZCMDF(vsave, PERMISSION_NONE, RESTRICTION_ONLY_IN_VEHICLE | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_LOGGED_IN, cmd_alias({ "/vzapisz", "/zapiszpojazd", "/privcar", "/savecar" }), "d")
{
	if (parser.Good())
	{
		long slot = parser.Get<long>();
		if (slot >= 0 && slot < 10)
		{
			if (IsVehicleTrain(Player[playerid].CurrentVehicleModel) || IsVehicleTrainTrailer(Player[playerid].CurrentVehicleModel))
			{
				fixSendClientMessage(playerid, -1, L_savecar_cannot_save);
				return true;
			}
			Player[playerid].PrywatnePojazdy[slot].Save(Player[playerid].CurrentVehicle);
			SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_savecar_success);
			return true;
		}
	}
	fixSendClientMessage(playerid, -1, L_savecar_usage_1);
	fixSendClientMessage(playerid, -1, L_savecar_usage_2);
	return true;
}

////////////////////////////PRIVATECAR
SPrivateCar::SPrivateCar()
{
	ModelID = 0;
}

void SPrivateCar::Save(int vehicleid)
{
	for (int i = 0; i < 14; ++i)
	{
		Components[i] = GetVehicleComponentInSlot(vehicleid, i);
	}
	Paintjob = GetVehiclePaintjob(vehicleid);
	Color[0] = GetVehicleColor(vehicleid)[0];
	Color[1] = GetVehicleColor(vehicleid)[1];
	ModelID = GetVehicleModel(vehicleid);
}

int SPrivateCar::Create(float x, float y, float z, float a)
{
	int vehicleid = fixCreateVehicle(ModelID, x, y, z, a, Color[0], Color[1], 84600);
	for (int i = 0; i < 14; ++i)
	{
		AddVehicleComponent(vehicleid, Components[i]);
	}
	fixChangeVehicleColor(vehicleid, Color[0], Color[1]);
	fixChangeVehiclePaintjob(vehicleid, Paintjob);
	sampgdk_SetTimerEx(333, false, FixPaintjobColors, (void*)vehicleid, nullptr);
	return vehicleid;
}

void SPrivateCar::Apply(int vehicleid)
{
	for (int i = 0; i < 14; ++i)
	{
		AddVehicleComponent(vehicleid, Components[i]);
	}
	fixChangeVehicleColor(vehicleid, Color[0], Color[1]);
	fixChangeVehiclePaintjob(vehicleid, Paintjob);
	sampgdk_SetTimerEx(333, false, FixPaintjobColors, (void*)vehicleid, nullptr);
}