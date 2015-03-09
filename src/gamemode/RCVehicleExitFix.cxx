/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Make exiting RC vehicles possible

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies
		Extension
		WorldData

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

class RCVehicleExitFixProcessor : public Extension::Base
{
public:
	bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) override
	{
		if (newkeys & KEY_SECONDARY_ATTACK)
		{
			int model = GetVehicleModel(Player[playerid].CurrentVehicle);
			if (IsVehicleRC(model))
			{
				static float x, y, z;
				GetVehiclePos(Player[playerid].CurrentVehicle, &x, &y, &z);
				fixSetPlayerPos(playerid, x, y, z + 1.5);
				RemovePlayerFromVehicle(playerid);
			}
		}
		return true;
	}
} _RCVehicleExitFixProcessor;