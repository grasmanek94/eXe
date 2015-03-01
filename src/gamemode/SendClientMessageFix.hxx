/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Declaring the fixed functions to use in the mode

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

int fixSendClientMessage(int playerid, int color, std::string message, bool playSound = false, bool displayPrefix = false);
int fixSendClientMessageToAll(int color, std::string message, bool playSound = false, bool displayPrefix = false);
int fixSendPlayerMessageToAll(int senderid, std::string message);
int fixSendPlayerMessageToPlayer(int playerid, int senderid, std::string message);
int fixSetPlayerChatBubble(int playerid, std::string message, int color, float drawdistance, int expiretime);

void debug_fixSendClientMessage(int playerid, std::string message);
void debug_fixSendClientMessageToAll(std::string message);