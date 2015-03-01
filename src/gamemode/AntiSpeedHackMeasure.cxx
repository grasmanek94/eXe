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

#ifdef _LOCALHOST_DEBUG

struct SASHMS
{
	int TD;
	int VID;
	int TIMER;
	float VMAXC;
	float VMAX;
};

std::array<SASHMS, MAX_PLAYERS> ASHMS;
std::array<float, 612> VehicleSpeeds;

void UpdateMeasurements(int playerid)
{
	if (Player[playerid].CurrentVehicle)
	{
		std::array<float, 3> VEL;
		GetVehicleVelocity(Player[playerid].CurrentVehicle, &VEL[0], &VEL[1], &VEL[2]);
		float velXY = sqrt(VEL[0] * VEL[0] + VEL[1] * VEL[1]) + FLT_EPSILON;
		float velZ = VEL[2];
		float SpeedMult = pow(1.0 - ((atan(abs(velZ) / velXY)*RAD_TO_DEG) / 90.0), 1.5);

		float N_VMAX = sqrt(velXY*velXY + velZ*velZ);
		float N_VMAXC = SpeedMult*N_VMAX;

		if (ASHMS[playerid].VMAX < N_VMAX)
		{
			ASHMS[playerid].VMAX = N_VMAX;
			VehicleSpeeds[Player[playerid].CurrentVehicleModel] = N_VMAX;
		}
		if (ASHMS[playerid].VMAXC < N_VMAXC)
			ASHMS[playerid].VMAXC = N_VMAXC;


		PlayerTextDrawSetString(playerid, ASHMS[playerid].TD, Functions::string_format(
		"\
V-MAX_______________: %4.2f~n~\
V-MAX-CORRECTED_____: %4.2f~n~\
V-CURRENT___________: %4.2f~n~\
V-CURRENT-CORRECTED_: %4.2f~n~\
V-CORRECTION________: %4.2f~n~\
V-MAX-SAVED_________: %4.2f~n~",
ASHMS[playerid].VMAX*100.0, 
ASHMS[playerid].VMAXC*100.0, 
N_VMAX*100.0, 
N_VMAXC*100.0, 
SpeedMult,
VehicleSpeeds[Player[playerid].CurrentVehicleModel]).c_str());
	}
}

class AntiSpeedHackMeasure : public Extension::Base
{
public:
	bool OnPlayerConnect(int playerid)
	{
		ASHMS[playerid].TD = CreatePlayerTextDraw(playerid, 10.000000, 220.000000, "_");
		ASHMS[playerid].VID = 400;
		ASHMS[playerid].TIMER = INVALID_TIMER_ID;
		PlayerTextDrawBackgroundColor(playerid, ASHMS[playerid].TD, 255);
		PlayerTextDrawFont(playerid, ASHMS[playerid].TD, 2);
		PlayerTextDrawLetterSize(playerid, ASHMS[playerid].TD, 0.189998, 1.399999);
		PlayerTextDrawColor(playerid, ASHMS[playerid].TD, -1);
		PlayerTextDrawSetProportional(playerid, ASHMS[playerid].TD, 1);
		PlayerTextDrawSetShadow(playerid, ASHMS[playerid].TD, 1);
		PlayerTextDrawShow(playerid, ASHMS[playerid].TD);
		return true;
	}
	bool OnPlayerSpawn(int playerid)
	{
		ZCMD_CALL_COMMAND(playerid, "/ac", "off");
		return true;
	}
	bool OnPlayerUpdate(int playerid)
	{
		UpdateMeasurements(playerid);		
		return true;
	}
	bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)
	{
		if (PRESSED(KEY_CROUCH))
		{
			ASHMS[playerid].VMAX = 0.0f;
			ASHMS[playerid].VMAXC = 0.0f;
			if (Player[playerid].CurrentVehicle)
				VehicleSpeeds[Player[playerid].CurrentVehicleModel] = 0.0f;
		}
		return true;
	}
} _AntiSpeedHackMeasure;

ZCMD3(dumpspeeds, 0, RESTRICTION_ONLY_DRIVER)
{

	std::ofstream speedssave("SH_SPEED_DATA.txt", std::ios::trunc);
	if (speedssave.good())
	{
		for (size_t i = 0; i < 212; ++i)
			VehicleSpeeds[i] = VehicleSpeeds[i + 400];

		speedssave << "std::array<float, 612> VehicleMaxSpeeds =\r\n{\r\n";
		for (auto &i : VehicleSpeeds)
			speedssave << Functions::string_format("\t%2.6f,\r\n", i);
		speedssave << "};\r\n";
	}
	return true;
}

void DelayNext(int timerid, void* param)
{
	int playerid = (int)param;
	ASHMS[playerid].TIMER = INVALID_TIMER_ID;
	CreatePlayerVehicleSafe(playerid, ASHMS[playerid].VID);
	++ASHMS[playerid].VID;
	ASHMS[playerid].VMAX = 0.0f;
	ASHMS[playerid].VMAXC = 0.0f;
}

ZCMD3(next, 0, 0)
{
	if (ASHMS[playerid].TIMER == INVALID_TIMER_ID)
	{
		TeleportPlayer(playerid, Player[playerid].statistics.SaveX, Player[playerid].statistics.SaveY, Player[playerid].statistics.SaveZ, Player[playerid].statistics.SaveA, false, 0, 0, "", 0, 0, 0.0, 0.0, false, true);
		ASHMS[playerid].TIMER = sampgdk_SetTimerEx(1000, false, DelayNext, (void*)playerid, nullptr);
	}
	return true;
}
#endif