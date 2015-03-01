/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Definition of the command execute permission levels and restrictions
		-	Prototype declarations for the mini-game systems to identify if a command is linked to a mini-game
			(probably need another way of doing that like a separate processor in the mini-game system bug meh this is ok)

	________________________________________________________________	
	Notes
		-	LoginRegister checks all the restrictions and permissions

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

enum eCommandExecuteLevel
{
	PERMISSION_NONE			= 0 << 0,
	PERMISSION_USER			= 1 << 0,//najnizszy
	PERMISSION_VIP			= 1 << 1,
	PERMISSION_SPONSOR		= 1 << 2,
	PERMISSION_MODERATOR	= 1 << 3,
	PERMISSION_ADMIN		= 1 << 4,
	PERMISSION_VICEHEAD		= 1 << 5,
	PERMISSION_HEADADMIN	= 1 << 6,
	PERMISSION_WSPOLNIK		= 1 << 7,
	PERMISSION_GAMER		= 1 << 8 //najwyzszy stopien :P
};

enum eCommandRestrictions // : unsigned long long
{
	RESTRICTION_NONE					= 0 << 0,
	RESTRICTION_NOT_IN_A_GAME			= 1 << 0,
	RESTRICTION_ONLY_IN_A_GAME			= 1 << 1,//which game? Set/Get CommandPermissionGameID
	RESTRICTION_NOT_AFTER_FIGHT			= 1 << 2,//			|
	RESTRICTION_ONLY_IN_VEHICLE			= 1 << 3,//			|
	RESTRICTION_ONLY_ON_FOOT			= 1 << 4,//			|
	RESTRICTION_ONLY_IN_AIRPLANE		= 1 << 5,//			|
	RESTRICTION_ONLY_IN_HELI			= 1 << 6,//			|
	RESTRICTION_ONLY_IN_CAR				= 1 << 7,//			|
	RESTRICTION_ONLY_LOGGED_IN			= 1 << 8,//			|
	RESTRICTION_CAN_USE_WHILE_MUTED		= 1 << 9,//			|
	RESERVED_BIT1_FOR_GAME_ID			= 1 << 10,//}-------+ specify here with		SetCommandPermissionGameID
	RESERVED_BIT2_FOR_GAME_ID			= 1 << 11,// }		|	or get it with		GetCommandPermissionGameID
	RESERVED_BIT3_FOR_GAME_ID			= 1 << 12,//  }		|
	RESERVED_BIT4_FOR_GAME_ID			= 1 << 13,//   }----|
	RESERVED_BIT5_FOR_GAME_ID			= 1 << 14,//   }----|
	RESERVED_BIT6_FOR_GAME_ID			= 1 << 15,//  }		|
	RESERVED_BIT7_FOR_GAME_ID			= 1 << 16,// }		|
	RESERVED_BIT8_FOR_GAME_ID			= 1 << 17,//}-------+
	RESTRICTION_REGISTERED_AND_LOADED	= 1 << 18,
	RESTRICTION_NOT_IN_MAFIA			= 1 << 19,
	RESTRICTION_IS_IN_MAFIA				= 1 << 20,
	RESTRICTION_IS_NOT_CHAT_MUTED		= 1 << 21,
	RESTRICTION_SPAWNED					= 1 << 22,
	RESTRICTION_USING_EXE24PLUS			= 1 << 23,
	RESTRICTION_IN_VEHICLE_OR_ONFOOT	= 1 << 24,
	RESTRICTION_ONLY_DRIVER				= 1 << 25,
	RESTRICTION_NOT_IN_STAGING			= 1 << 26
};

unsigned char GetCommandPermissionGameID(int restrictions);
int SetCommandPermissionGameID(int restriction, unsigned char GameID);