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

std::string PlayerPasswordHash(int playerid, std::string inputtext);
std::string PlayerPasswordHash(user * player, std::string inputtext);
std::string CreateAuthenticationCodeForPlayer(int playerid);
bool CheckCommandAllowed(int playerid, int this_restrictions, bool showmessage = true);
bool TestIfAllowed(int playerid);