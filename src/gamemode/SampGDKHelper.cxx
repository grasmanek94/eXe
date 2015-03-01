/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Providing simple returning function variants for the reference-only variant functions

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

char tempname[32];
std::string _GetPlayerName(int playerid)
{
	GetPlayerName(playerid, tempname, 32);
	return tempname;
}
std::string _GetPlayerIP(int playerid)
{
	GetPlayerIp(playerid, tempname, 32);
	return tempname;
}