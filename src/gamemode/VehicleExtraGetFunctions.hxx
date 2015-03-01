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
#pragma once

int fixCreateVehicle(int modelid, float x, float y, float z, float a, int col1, int col2, int respawntime = 999999);
int fixChangeVehiclePaintjob(int vehicleid, int id);
int fixChangeVehicleColor(int vehicleid, int col1, int col2);
int GetVehiclePaintjob(int vehicleid);
const std::array<int, 2> GetVehicleColor(int vehicleid);