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
#include "VehicleExtraGetFunctions.hxx"
#pragma once

#define INVALID_PAINTJOB_ID (3)

void FixPaintjobColors(int timerid, void *param);
struct SPrivateCar
{
	int ModelID;
	std::array<int,14> Components;
	std::array<int, 2> Color;
	int Paintjob;
	SPrivateCar();
	void Save(int vehicleid);
	int Create(float x, float y, float z, float a);
	void Apply(int vehicleid);
};