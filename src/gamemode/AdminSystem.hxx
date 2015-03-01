/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Declaring administration functions of the servers
		-	Enables the ColorIPFixer namespace to be used externally

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

void aKick(int kickid, int kreason = 2, bool allip = false, int kickerid = INVALID_PLAYER_ID, std::string reason = "", std::string kickername = ":AntiCheat:", bool silent = false);
void aBan(int banid, int breason = 3, int playerid = INVALID_PLAYER_ID, unsigned long long time = 0, std::string reason = "", std::string banername = ":AntiCheat:", bool silent = false);
size_t SendMessageToStaff(int color, std::string message, bool important = false, int sound = 1133);
size_t SendMessageToVips(int color, std::string message, bool important = false, int sound = 1133);
size_t SendMessageToSponsors(int color, std::string message, bool important = false, int sound = 1133);
size_t SendMessageToModerators(int color, std::string message, bool important = false, int sound = 1133);
size_t SendMessageToAdmins(int color, std::string message, bool important = false, int sound = 1133);
size_t SendMessageToViceHeadAdmins(int color, std::string message, bool important = false, int sound = 1133);
size_t SendMessageToHeadAdmins(int color, std::string message, bool important = false, int sound = 1133);
void RearrangePermissions(int playerid);

namespace ColorIPFixer
{
	int HSVtoRGBcvScalar(int H, int S, int V);
};