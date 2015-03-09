/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementing fixed versions for some erroneous sa-mp functios (which crash on bad input)
		-	Implementing debug-only messages

	________________________________________________________________	
	Notes
		-	Debug-only messages do not show in dedicated debug compilations

	________________________________________________________________
	Dependencies

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

int fixSendClientMessage(int playerid, int color, std::string message, int playSound, bool displayPrefix)
{
	boost::algorithm::replace_all(message, "%", "%%");
	ProcessStringWithBTag(color, message);

	if (displayPrefix)
	{
		message.insert(0, "››› ");
	}

	if (playSound)
	{
		PlayerPlaySound(playerid, color, 0, 0, 0);
	}

	if (message.size() > 144)
	{
		message.resize(144);
	}

	return sampgdk_SendClientMessage(playerid, color, message.c_str());
}

int fixSendClientMessageToAll(int color, std::string message, int playSound, bool displayPrefix)
{
	boost::algorithm::replace_all(message, "%", "%%");
	ProcessStringWithBTag(color, message);

	if (displayPrefix)
	{
		message.insert(0, "››› ");
	}

	if (playSound)
	{
		for (auto playerid : PlayersOnline)
		{
			PlayerPlaySound(playerid, playSound, 0, 0, 0);
		}
	}

	if (message.size() > 144)
	{
		message.resize(144);
	}

	return sampgdk_SendClientMessageToAll(color, message.c_str());
}

int fixSendPlayerMessageToAll(int senderid, std::string message)
{
	boost::algorithm::replace_all(message, "%", "%%");

	if (message.size() > 144)
	{
		message.resize(144);
	}

	return sampgdk_SendPlayerMessageToAll(senderid, message.c_str());
}

int fixSendPlayerMessageToPlayer(int playerid, int senderid, std::string message)
{
	boost::algorithm::replace_all(message, "%", "%%");

	if (message.size() > 144)
	{
		message.resize(144);
	}

	return sampgdk_SendPlayerMessageToPlayer(playerid, senderid, message.c_str());
}

int fixSetPlayerChatBubble(int playerid, std::string message, int color, float drawdistance, int expiretime)
{
	boost::algorithm::replace_all(message, "%", "%%");

	if (message.size() > 144)
	{
		message.resize(144);
	}

	return sampgdk_SetPlayerChatBubble(playerid, message.c_str(), color, drawdistance, expiretime);
}

void debug_fixSendClientMessage(int playerid, std::string message)
{
#ifdef _LOCALHOST_DEBUG
	boost::algorithm::replace_all(message, "%", "%%");

	if (message.size() > 144)
	{
		message.resize(144);
	}

	sampgdk_SendClientMessage(playerid, -1, ("{FFEE00}[DEBUG]{FFFFFF}" + message).c_str());
#endif
}

void debug_fixSendClientMessageToAll(std::string message)
{
#ifdef _LOCALHOST_DEBUG
	boost::algorithm::replace_all(message, "%", "%%");

	if (message.size() > 144)
	{
		message.resize(144);
	}

	sampgdk_SendClientMessageToAll(-1, ("{FFEE00}[DEBUG]{FFFFFF}" + message).c_str());
#endif
}