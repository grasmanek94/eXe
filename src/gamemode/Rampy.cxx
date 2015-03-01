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

static const std::vector<std::pair<int,glm::vec4>> ramptypes = 
{ 
	//Zfix, Dist, ZRotFix, Xoffsetfix
	{ 1503, glm::vec4(-0.75, 17.0, -90.0, 0.0) },
	{ 1660, glm::vec4(-1.50, 17.0, -90.0, 0.0) },
	{ 1245, glm::vec4(-0.33, 17.0, -180.0, 0.0) },
	{ 1631, glm::vec4(-0.50, 20.0, -90.0, -1.0) },
	{ 1632, glm::vec4(-0.75, 20.0, -90.0, 0.0) },
	{ 1655, glm::vec4(-0.75, 20.0, -90.0, 0.0) }
};

void RemoveRamp(int timerid, void* param)
{
	if (Player[(int)param].CurrentRamp != INVALID_OBJECT_ID)
	{
		StreamerLibrary::DestroyDynamicObject(Player[(int)param].CurrentRamp);
		Player[(int)param].CurrentRamp = INVALID_OBJECT_ID;
	}
}

class Rampy : public Extension::Base
{
public:
	Rampy():Base(){}

	glm::vec3 return_pos;
	int model;
	float speedu;
	float vx, vy, vz;
	glm::vec2 rot;
	glm::vec3 pos;

	bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)
	{
		if (
			Player[playerid].CurrentVehicle && PRESSED(KEY_ACTION) && 
			Player[playerid].IsDriver && //driver trick ;P
			Player[playerid].CurrentRamp == INVALID_OBJECT_ID && 
			Player[playerid].statistics.rampid > 0 && 
			Player[playerid].statistics.rampid < (ramptypes.size()+1) &&
			Player[playerid].CurrentGameID == nullptr &&
			!IsVehicleAir(Player[playerid].CurrentVehicleModel) &&
			!IsVehicleRC(Player[playerid].CurrentVehicleModel)
			)
		{
				GivePlayerAchievement(playerid, EAM_Ramper, 1);
				GetVehicleVelocity(Player[playerid].CurrentVehicle, &vx, &vy, &vz);
				speedu = (sqrt(vx * vx + vy * vy + vz * vz)*51.05f) + 0.5f;
				pos = PositionFromVehicleOffset(Player[playerid].CurrentVehicle, glm::vec3(ramptypes[Player[playerid].statistics.rampid - 1].second.w, ramptypes[Player[playerid].statistics.rampid - 1].second.y + pow(speedu, 0.2) * ((float)GetPlayerPing(playerid)) / 50.0, 0.1), return_pos)[mp_POS];
				GetPitchYawBetweenCoords(pos, return_pos, rot);
				Player[playerid].CurrentRamp = StreamerLibrary::CreateDynamicObject(ramptypes[Player[playerid].statistics.rampid - 1].first, return_pos.x, return_pos.y, return_pos.z + ramptypes[Player[playerid].statistics.rampid - 1].second.x, rot.y, 0.0, rot.x + ramptypes[Player[playerid].statistics.rampid - 1].second.z, -1, -1, playerid);
				sampgdk_SetTimerEx(1500, false, RemoveRamp, (void*)playerid, this);
		}

		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		RemoveRamp(0, (void*)playerid);
		return true;
	}
} _Rampy;

ZCMD(ramp, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/setramp", "/rampid", "/rampon", "/rampoff", "/rampy", "/rampa" }))
{
	int rampid;
	if (Functions::is_number_unsigned(params, rampid))
	{	
		if (rampid >= 0 && rampid < (ramptypes.size() + 1))
		{
			Player[playerid].statistics.rampid = rampid;
			fixSendClientMessage(playerid, -1, L_ramp_succes);
			return true;
		}
	}
	fixSendClientMessage(playerid, -1, L_ramp_usage);
	return true;
}