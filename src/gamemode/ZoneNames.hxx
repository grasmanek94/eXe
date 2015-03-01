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

namespace ZoneNames
{
	struct ZonePosition
	{
		float
			Xmin,
			Ymin,
			Zmin,
			Xmax,
			Ymax,
			Zmax;
	};

	struct ZoneData
	{
		std::string name;
		ZonePosition pos;
	};

	extern std::array<ZoneData, 365> gSAZones;
	extern std::array<int, MAX_PLAYERS> ZoneTextDraw;
	using PlayerAreaManager = std::pair < int, unsigned long > ;
	extern std::array<std::stack<int>, MAX_PLAYERS> PlayerAreas;
	extern std::unordered_map<int, std::pair<std::string, unsigned long>> ZoneAreas;
	std::string GetCurrentZone(int playerid);
};