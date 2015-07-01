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

std::array<int, MAX_PLAYERS> PlayerSpeedo;
std::set<int> UpdateSpeedoTextdrawList;

float GetVehicleSpeed_EndUser(float VelocityX, float VelocityY, float VelocityZ)
{
	return (VelocityX*VelocityX + VelocityY*VelocityY + VelocityZ*VelocityZ)*(427833329.0 / 2305550.0);
}

void UpdateSpeedoTds(int timerid, void * param)
{
	float vx, vy, vz, vh;

	const std::array<std::string, 7> star_maker =
	{{
		"~g~~h~",
		"~g~",
		"~y~~h~",
		"~y~",
		"~r~~h~",
		"~r~",
		""
	}};

	for (auto i : UpdateSpeedoTextdrawList)
	{
		GetVehicleVelocity(Player[i].CurrentVehicle, &vx, &vy, &vz);
		GetVehicleHealth(Player[i].CurrentVehicle, &vh);

		float speed = GetVehicleSpeed_EndUser(vx, vy, vz);

		float percentage = vh / 10.0;
		if (percentage > 100.0)
		{
			percentage = 100.0;
		}

		unsigned char star_count = 6 - (unsigned char)floor(vh / (1000.0f / 6.0f));
		if (star_count > 5)
		{
			star_count = 5;
		}

		unsigned char star_count_speed = (unsigned char)floor(speed / (400.0f / 6.0f));
		if (star_count_speed > 5)
		{
			star_count_speed = 5;
		}

		PlayerTextDrawSetString(i, PlayerSpeedo[i], Functions::string_format("~w~[%s%3.0f KM/H~w~][%s%3.0f%%~w~]", star_maker[star_count_speed].c_str(), speed, star_maker[star_count].c_str(), percentage).c_str());
	}
}

class SpeedoProcessor : public Extension::Base
{
public:
	bool OnGameModeInit() override
	{
		sampgdk_SetTimerEx(222, true, UpdateSpeedoTds, nullptr, this);
		return true;
	}
	bool OnPlayerConnect(int playerid)
	{
		PlayerSpeedo[playerid] = PlayerTextDrawCreate(playerid, 642.000000, 437.000000, "_");
		PlayerTextDrawAlignment(playerid, PlayerSpeedo[playerid], 3);
		PlayerTextDrawBackgroundColor(playerid, PlayerSpeedo[playerid], 255);
		PlayerTextDrawFont(playerid, PlayerSpeedo[playerid], 1);
		PlayerTextDrawLetterSize(playerid, PlayerSpeedo[playerid], 0.189998, 1.000000);
		PlayerTextDrawColor(playerid, PlayerSpeedo[playerid], -1);
		PlayerTextDrawSetOutline(playerid, PlayerSpeedo[playerid], 0);
		PlayerTextDrawSetProportional(playerid, PlayerSpeedo[playerid], 1);
		PlayerTextDrawSetShadow(playerid, PlayerSpeedo[playerid], 1);
		PlayerTextDrawSetSelectable(playerid, PlayerSpeedo[playerid], 0);
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		PlayerTextDrawHide(playerid, PlayerSpeedo[playerid]);
		UpdateSpeedoTextdrawList.erase(playerid);
		return true;
	}
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate)
	{
		switch (newstate)
		{
			case PLAYER_STATE_DRIVER:
			case PLAYER_STATE_PASSENGER:
				PlayerTextDrawShow(playerid, PlayerSpeedo[playerid]);
				UpdateSpeedoTextdrawList.insert(playerid);
				break;
			default:
				PlayerTextDrawHide(playerid, PlayerSpeedo[playerid]);
				UpdateSpeedoTextdrawList.erase(playerid);
				break;
		}
		return true;
	}
} _SpeedoProcessor;