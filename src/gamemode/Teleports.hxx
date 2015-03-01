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
#include "AntiCheat.hxx"
void ShowTeleportsList(int playerid);
void TeleportPlayer(int playerid = INVALID_PLAYER_ID, float x = 0.0f, float y = 0.0f, float z = 0.0f, float a = 0.0f, bool vehicle = false, int interiorid = 0, int virtualworldid = 0, std::string gametext = "", int gametexttime = 800, int gametextstyle = 1, float maxrx = 0.0f, float maxry = 0.0f, bool reset_weapons = false, bool update_streamer = false);