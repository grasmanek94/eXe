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

static const std::array<std::string, 4> DisconnectReasons =
{
	"[Timeout]",
	"[Exit]",
	"[Kick]",
	"[Ban]"
};

static const std::array<std::string, 4> DisconnectReasonsSafe =
{
	"[Timeout]",
	"[Exit]",
	"[Kick]",
	"[Ban]"
};

enum eLoggerType
{
	LOG_DUMMY,//needed else crash? null == 0
	LOG_CHAT,
	LOG_COMMANDS,
	LOG_CONNECT,
	LOG_DISCONNECT,
	LOG_IP,
	LOG_KICK,
	LOG_BAN,
	LOG_DEATH,
	LOG_RCONCMD,
	LOG_RCONATTEMPT,
	LOG_AUDIT,
	LOG_REGISTER,
	LOG_LOGIN,
	LOG_PRIVMSG,
	LOG_MAFIA,
	LOG_OTHER,
	LOG_ERROR,
	LOG_WARNING,
	LOG_MUTE,
	LOG_CMDMUTE,
	LOG_CALLBACKS,
	LOG_ALL,
	LOG_FLUSH_ALL
};

void gtLog(eLoggerType loggerid, std::string towrite);