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

namespace Extension
{
	std::vector<extension_filler> * Extensions;
	bool Prioritizer (extension_filler &i,extension_filler &j)
	{ 
		return (i.first>j.first); 
	}
};

std::array<bool, MAX_PLAYERS> IsNPC;

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnGameModeInit">OnGameModeInit on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnGameModeInit())
{
	static bool init = false;

	if (init)
	{
		return true;
	}

	init = true;

	for (auto &i : *Extension::Extensions)
	{
		i.second->OnGameModeInit();
	}
	return true;
}


/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerConnect">OnPlayerConnect on SA-MP Wiki</a>
*/

namespace AntiChatSpam
{
	std::array<unsigned long long, MAX_PLAYERS> LastChat;
	unsigned long long chat_time;
	std::array<std::string, MAX_PLAYERS> lasttext;
};


SAMPGDK_CALLBACK(bool, OnPlayerConnect(int playerid))
{
	AntiChatSpam::lasttext[playerid].clear();
	if (!IsPlayerNPC(playerid))
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerConnect(playerid);
		}
	}
	else
	{
#if defined _DEBUG__
		PlayersOnline.insert(playerid);
#endif
		IsNPC[playerid] = true;
		gMySpecialNPCWhichFucksUp = playerid;
		fixSetPlayerVirtualWorld(playerid, 0x70000000);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerDisconnect">OnPlayerDisconnect on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerDisconnect(int playerid, int reason))
{
	if (!IsPlayerNPC(playerid))
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerDisconnect(playerid, reason);
		}
	}
	else
	{
#if defined _DEBUG__
		PlayersOnline.erase(playerid);
#endif
		IsNPC[playerid] = false;
		gMySpecialNPCWhichFucksUp = INVALID_PLAYER_ID;
		ConnectNPC("Douchebag", "npcidle");
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerRequestClass">OnPlayerRequestClass on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerRequestClass(int playerid, int classid))
{
	if (classid > 299 || classid < 0)
	{
		//Kick(playerid);
		return false;
	}
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			if (!i.second->OnPlayerRequestClass(playerid, classid))
			{
				return false;
			}
		}
	}
	return true;
}


/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerCommandText">OnPlayerCommandText on SA-MP Wiki</a>
*/

namespace AntiCmdSpam
{
	std::array<unsigned long long, MAX_PLAYERS> LastCmd;
	unsigned long long cmd_time;
};

SAMPGDK_CALLBACK(bool, OnPlayerCommandText(int playerid, const char * cmdtext))
{
	if (!IsNPC[playerid])
	{
		AntiCmdSpam::cmd_time = Functions::GetTime();
		if ((AntiCmdSpam::cmd_time - AntiCmdSpam::LastCmd[playerid]) < 250)
		{
			return true;
		}
		AntiCmdSpam::LastCmd[playerid] = AntiCmdSpam::cmd_time;
		std::string commandtext(cmdtext);
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerCommandText(playerid, commandtext);//for logging etc
		}
		return ZCMD_OnPlayerCommandText(playerid, commandtext);
	}
	return 0;
}


/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnRconCommand">OnRconCommand on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnRconCommand(const char * cmd))
{
	for (auto &i : *Extension::Extensions)
	{
		if (i.second->OnRconCommand(std::string(cmd)))
		{
			return true;
		}
	}
	return false;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

extern void *pAMXFunctions;

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->Unload();
	}
	sampgdk::Unload();
}

std::atomic<bool> running = true;
extern std::thread * dbworkerthreadid;
BOOL WINAPI HandlerRoutine(
	_In_  DWORD dwCtrlType
	)
{
	switch (dwCtrlType)
	{
	case 0:
	case 1:
	case 2:
	case 5:
	case 6:
		std::cout << "Detected forced close, emergency save" << std::endl;//haha heh, nope, no emergency save
		fixSendClientMessageToAll(0xFF0000FF, "SERVER SHUTTING DOWN...");
		fixSendClientMessageToAll(0xFF0000FF, "SERVER SHUTTING DOWN...");
		fixSendClientMessageToAll(0xFF0000FF, "SERVER SHUTTING DOWN...");
		fixSendClientMessageToAll(0xFF0000FF, "SERVER SHUTTING DOWN...");
		fixSendClientMessageToAll(0xFF0000FF, "SERVER SHUTTING DOWN...");
		HostBan::DumpHostBans();
		//this crashes.
		/*for (auto z : PlayersOnline)
		{
			for (auto &i : *Extension::Extensions)
				i.second->OnPlayerDisconnect(z, 1);
			//Kick(z);
		}*/


		running = false;

		dbworkerthreadid->join();
		//sampgdk::Unload();//not even needed here...
	}
	return false;
}

void ** global_ppData;
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	global_ppData = ppData;
	SetConsoleCtrlHandler(HandlerRoutine,true);
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	bool load = sampgdk::Load(ppData);

	for (auto &i : *Extension::Extensions)
	{
		i.second->Load();
	}

	return load;
}


PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	sampgdk::ProcessTick();
	for (auto &i : *Extension::Extensions)
	{
		i.second->ProcessTick();
	}
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnGameModeExit">OnGameModeExit on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnGameModeExit())
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnGameModeExit();
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerSpawn">OnPlayerSpawn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerSpawn(int playerid))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			if (!i.second->OnPlayerSpawn(playerid))
			{
				return false;
			}
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerDeath">OnPlayerDeath on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerDeath(int playerid, int killerid, int reason))
{
	if ((killerid >= MAX_PLAYERS && killerid != INVALID_PLAYER_ID) || killerid < 0 || reason < 0 || reason > 255)
	{
		//Kick(playerid);
		debug_fixSendClientMessageToAll(Functions::string_format("OnPlayerDeath::BLOCK2()"));
		return false;
	}
#if defined _LOCALHOST_DEBUG
	int counter = 0;
#endif
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			if (!i.second->OnPlayerDeath(playerid, killerid, reason))
			{
#if defined _LOCALHOST_DEBUG
				debug_fixSendClientMessageToAll(Functions::string_format("OnPlayerDeath::BLOCK(%d)", counter));
#endif
				return false;
			}
#if defined _LOCALHOST_DEBUG
			++counter;
#endif
		}
	}
	debug_fixSendClientMessageToAll(Functions::string_format("OnPlayerDeath::NOT_BLOCKED()"));
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleSpawn">OnVehicleSpawn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleSpawn(int vehicleid))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehicleSpawn(vehicleid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleDeath">OnVehicleDeath on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleDeath(int vehicleid, int killerid))//killerid == the one who reported the death
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0)
	{
		if(killerid != INVALID_PLAYER_ID)
			//Kick(killerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehicleDeath(vehicleid, killerid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerText">OnPlayerText on SA-MP Wiki</a>
*/

SAMPGDK_CALLBACK(bool, OnPlayerText(int playerid, const char * text))
{
	if (!IsNPC[playerid])
	{
		AntiChatSpam::chat_time = Functions::GetTime();
		std::string currenttext(text);
		if ((AntiChatSpam::chat_time - AntiChatSpam::LastChat[playerid]) < 1000 || !AntiChatSpam::lasttext[playerid].compare(currenttext))
		{
			AntiChatSpam::LastChat[playerid] = AntiChatSpam::chat_time + 1000;
			fixSendClientMessage(playerid, -1, L_antispam);
			return false;
		}
		if (Player[playerid].ChatMute > AntiChatSpam::chat_time && text[0] != '`')
		{
			fixSendClientMessageF(playerid, -1, L_chatmute, true, true, (Player[playerid].ChatMute - AntiChatSpam::chat_time) / 1000);
			return false;
		}
		AntiChatSpam::LastChat[playerid] = AntiChatSpam::chat_time;
		AntiChatSpam::lasttext[playerid].assign(currenttext);
		for (auto &i : *Extension::Extensions)
		{
			if (!i.second->OnPlayerText(playerid, currenttext))
			{
				return false;
			}
		}
	}
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEnterVehicle">OnPlayerEnterVehicle on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEnterVehicle(int playerid, int vehicleid, bool ispassenger))
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0)
	{
		//Kick(playerid);
		return false;
	}
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerExitVehicle">OnPlayerExitVehicle on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerExitVehicle(int playerid, int vehicleid))
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0)
	{
		//Kick(playerid);
		return false;
	}
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerExitVehicle(playerid, vehicleid);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerStateChange">OnPlayerStateChange on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerStateChange(int playerid, int newstate, int oldstate))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerStateChange(playerid, newstate, oldstate);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEnterCheckpoint">OnPlayerEnterCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEnterCheckpoint(int playerid))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerEnterCheckpoint(playerid);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerLeaveCheckpoint">OnPlayerLeaveCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerLeaveCheckpoint(int playerid))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerLeaveCheckpoint(playerid);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEnterRaceCheckpoint">OnPlayerEnterRaceCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEnterRaceCheckpoint(int playerid))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerEnterRaceCheckpoint(playerid);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerLeaveRaceCheckpoint">OnPlayerLeaveRaceCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerLeaveRaceCheckpoint(int playerid))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerLeaveRaceCheckpoint(playerid);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerRequestSpawn">OnPlayerRequestSpawn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerRequestSpawn(int playerid))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			if (!i.second->OnPlayerRequestSpawn(playerid))
			{
				return false;
			}
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnObjectMoved">OnObjectMoved on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnObjectMoved(int objectid))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnObjectMoved(objectid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerObjectMoved">OnPlayerObjectMoved on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerObjectMoved(int playerid, int objectid))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerObjectMoved(playerid, objectid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerPickUpPickup">OnPlayerPickUpPickup on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerPickUpPickup(int playerid, int pickupid))
{
	if (pickupid >= 4096 || pickupid < 0)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerPickUpPickup(playerid, pickupid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleMod">OnVehicleMod on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleMod(int playerid, int vehicleid, int componentid))
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0 || componentid < 0 || componentid > 1193)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehicleMod(playerid, vehicleid, componentid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnEnterExitModShop">OnEnterExitModShop on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnEnterExitModShop(int playerid, int enterexit, int interiorid))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnEnterExitModShop(playerid, enterexit, interiorid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehiclePaintjob">OnVehiclePaintjob on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid))
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0 || paintjobid < 0)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehiclePaintjob(playerid, vehicleid, paintjobid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleRespray">OnVehicleRespray on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleRespray(int playerid, int vehicleid, int color1, int color2))
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0 || color1 < 0 || color2 < 0 || color1 > 255 || color2 > 255)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehicleRespray(playerid, vehicleid, color1, color2);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleDamageStatusUpdate">OnVehicleDamageStatusUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleDamageStatusUpdate(int vehicleid, int playerid))
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehicleDamageStatusUpdate(vehicleid, playerid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnUnoccupiedVehicleUpdate">OnUnoccupiedVehicleUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat, float new_x, float new_y, float new_z, float vel_x, float vel_y, float vel_z))
{
	if (vehicleid >= MAX_VEHICLES || vehicleid < 0 || passenger_seat < 0 || passenger_seat > 255)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, new_x, new_y, new_z, vel_x, vel_y, vel_z);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerSelectedMenuRow">OnPlayerSelectedMenuRow on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerSelectedMenuRow(int playerid, int row))
{
	if (row < 0 || row > 128)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerSelectedMenuRow(playerid, row);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerExitedMenu">OnPlayerExitedMenu on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerExitedMenu(int playerid))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerExitedMenu(playerid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerInteriorChange">OnPlayerInteriorChange on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid))
{
	if (newinteriorid < 0 || oldinteriorid < 0 || newinteriorid > 128 || oldinteriorid > 128)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerKeyStateChange">OnPlayerKeyStateChange on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	}
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnRconLoginAttempt">OnRconLoginAttempt on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnRconLoginAttempt(const char * ip, const char * password, bool success))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnRconLoginAttempt(std::string(ip), std::string(password), success);
	}
	BlockIpAddress(ip, 10000);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerUpdate">OnPlayerUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerUpdate(int playerid))
{
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			if (!i.second->OnPlayerUpdate(playerid))
			{
				return false;
			}
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerStreamIn">OnPlayerStreamIn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerStreamIn(int playerid, int forplayerid))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerStreamIn(playerid, forplayerid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerStreamOut">OnPlayerStreamOut on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerStreamOut(int playerid, int forplayerid))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerStreamOut(playerid, forplayerid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleStreamIn">OnVehicleStreamIn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleStreamIn(int vehicleid, int forplayerid))
{
	if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
	{
		//Kick(forplayerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehicleStreamIn(vehicleid, forplayerid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleStreamOut">OnVehicleStreamOut on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleStreamOut(int vehicleid, int forplayerid))
{
	if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
	{
		//Kick(forplayerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnVehicleStreamOut(vehicleid, forplayerid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnDialogResponse">OnDialogResponse on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char * inputtext))
{
	if (listitem < -1 || dialogid >= 32000 || dialogid < -32000)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnDialogResponse(playerid, dialogid, response, listitem, std::string(inputtext));
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerTakeDamage">OnPlayerTakeDamage on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart))
{
	if (issuerid < 0 || (issuerid >= MAX_PLAYERS && issuerid != INVALID_PLAYER_ID) || weaponid < 0 || bodypart < 0 || weaponid > 48)
	{
		//Kick(playerid);
		return false;
	}
	if (!IsNPC[playerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerGiveDamage">OnPlayerGiveDamage on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart))
{
	if (damagedid < 0 || (damagedid >= MAX_PLAYERS && damagedid != INVALID_PLAYER_ID) || weaponid < 0 || bodypart < 0 || weaponid > 48)
	{
		//Kick(playerid);
		return false;
	}
	if (!IsNPC[damagedid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickMap">OnPlayerClickMap on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickMap(int playerid, float fX, float fY, float fZ))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerClickMap(playerid, fX, fY, fZ);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickTextDraw">OnPlayerClickTextDraw on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickTextDraw(int playerid, int clickedid))
{
	if (clickedid < 0 || (clickedid >= MAX_TEXT_DRAWS && clickedid != INVALID_TEXT_DRAW))
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerClickTextDraw(playerid, clickedid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickPlayerTextDraw">OnPlayerClickPlayerTextDraw on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickPlayerTextDraw(int playerid, int playertextid))
{
	if (playertextid < 0 || (playertextid >= MAX_PLAYER_TEXT_DRAWS && playertextid != INVALID_TEXT_DRAW))
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerClickPlayerTextDraw(playerid, playertextid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnIncomingConnection">OnIncomingConnection on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnIncomingConnection(int playerid, const char * ip_address, int port))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnIncomingConnection(playerid, std::string(ip_address), port);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnTrailerUpdate">OnTrailerUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnTrailerUpdate(int playerid, int vehicleid))
{
	if (vehicleid < 0 || (vehicleid >= MAX_VEHICLES && vehicleid != INVALID_VEHICLE_ID))
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnTrailerUpdate(playerid, vehicleid);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickPlayer">OnPlayerClickPlayer on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickPlayer(int playerid, int clickedplayerid, int source))
{
	if (clickedplayerid < 0 || (clickedplayerid >= MAX_PLAYERS && clickedplayerid != INVALID_PLAYER_ID) || source < 0 || source > 10)
	{
		//Kick(playerid);
		return false;
	}
	if (!IsNPC[clickedplayerid])
	{
		for (auto &i : *Extension::Extensions)
		{
			i.second->OnPlayerClickPlayer(playerid, clickedplayerid, source);
		}
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEditObject">OnPlayerEditObject on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ))
{
	if (objectid < 0 || (objectid >= MAX_OBJECTS && objectid != INVALID_OBJECT_ID) || response < 0)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerEditObject(playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEditAttachedObject">OnPlayerEditAttachedObject on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEditAttachedObject(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ))
{
	if (index < 0 || response < 0 || !IsValidObjectModel(modelid))
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerSelectObject">OnPlayerSelectObject on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ))
{
	if (type < 0 || objectid < 0 || (objectid >= MAX_OBJECTS && objectid != INVALID_OBJECT_ID) || !IsValidObjectModel(modelid))
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerSelectObject(playerid, type, objectid, modelid, fX, fY, fZ);
	}
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerWeaponShot">OnPlayerWeaponShot on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ))
{
	if (weaponid < 0 || weaponid > 48 || hitid < 0)
	{
		//Kick(playerid);
		return false;
	}
	static int max = 0;
	switch (hittype)
	{
	case BULLET_HIT_TYPE_NONE:
		max = 0;
		break;
	case BULLET_HIT_TYPE_PLAYER:
		max = MAX_PLAYERS;
		break;
	case BULLET_HIT_TYPE_VEHICLE:
		max = MAX_VEHICLES;
		break;
	case BULLET_HIT_TYPE_OBJECT:
	case BULLET_HIT_TYPE_PLAYER_OBJECT:
		max = MAX_OBJECTS;
		break;
	default:
		return false;
	}
	if ((hitid >= max && hitid != 0xFFFF) || hitid < 0)
	{
		//Kick(playerid);
		return false;
	}
	for (auto &i : *Extension::Extensions)
	{
		if (!i.second->OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ))
		{
			return false;
		}
	}
	return true;
}

bool Worker_OnPlayerDataAcquired(int playerid, int result)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerDataAcquired(playerid, result);
	}
	return true;
}

bool OnPlayerCommandReceived(int playerid, std::string& command, std::string& params, int min_execute_level, int this_restrictions)
{
	for (auto &i : *Extension::Extensions)
	{
		if (!i.second->OnPlayerCommandReceived(playerid, command, params, min_execute_level, this_restrictions))
		{
			return false;
		}
	}
	return true;
}

bool OnPlayerCommandExecuted(int playerid, std::string& command, std::string& params, bool success)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerCommandExecuted(playerid, command, params, success);
	}
	return success;
}

void OnUnknownCommand(int playerid, std::string& command, std::string& params)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnUnknownCommand(playerid, command, params);
	}
}

bool StreamerLibrary::OnDynamicObjectMoved(int objectid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnDynamicObjectMoved(objectid);
	}
	return true;
}

bool StreamerLibrary::OnPlayerEditDynamicObject(int playerid, int objectid, int response, float x, float y, float z, float rx, float ry, float rz)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	}
	return true;
}

bool StreamerLibrary::OnPlayerSelectDynamicObject(int playerid, int objectid, int modelid, float x, float y, float z)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerSelectDynamicObject(playerid, objectid, modelid, x, y, z);
	}
	return true;
}

bool StreamerLibrary::OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float x, float y, float z)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerShootDynamicObject(playerid, weaponid, objectid, x, y, z);
	}
	return true;
}

bool StreamerLibrary::OnPlayerPickUpDynamicPickup(int playerid, int pickupid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerPickUpDynamicPickup(playerid, pickupid);
	}
	return true;
}

bool StreamerLibrary::OnPlayerEnterDynamicCP(int playerid, int checkpointid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerEnterDynamicCP(playerid, checkpointid);
	}
	return true;
}

bool StreamerLibrary::OnPlayerLeaveDynamicCP(int playerid, int checkpointid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerLeaveDynamicCP(playerid, checkpointid);
	}
	return true;
}

bool StreamerLibrary::OnPlayerEnterDynamicRaceCP(int playerid, int checkpointid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerEnterDynamicRaceCP(playerid, checkpointid);
	}
	return true;
}

bool StreamerLibrary::OnPlayerLeaveDynamicRaceCP(int playerid, int checkpointid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerLeaveDynamicRaceCP(playerid, checkpointid);
	}
	return true;
}

bool StreamerLibrary::OnPlayerEnterDynamicArea(int playerid, int areaid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerEnterDynamicArea(playerid, areaid);
	}
	return true;
}

bool StreamerLibrary::OnPlayerLeaveDynamicArea(int playerid, int areaid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerLeaveDynamicArea(playerid, areaid);
	}
	return true;
}

class StreamerProcessor : public Extension::Base
{
public:
	void Load()
	{
		StreamerLibrary::Load();
	}
	void Unload()
	{
		StreamerLibrary::Unload();
	}
	//int AmxLoad(AMX *amx);//nn
	//int AmxUnload(AMX *amx);//nn
	void ProcessTick()
	{
		StreamerLibrary::ProcessTick();
	}
	bool OnPlayerConnect(int playerid)
	{
		return StreamerLibrary::OnPlayerConnect(playerid);
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		return StreamerLibrary::OnPlayerDisconnect(playerid, reason);
	}
	bool OnPlayerEnterCheckpoint(int playerid)
	{
		return StreamerLibrary::OnPlayerEnterCheckpoint(playerid);
	}
	bool OnPlayerLeaveCheckpoint(int playerid)
	{
		return StreamerLibrary::OnPlayerLeaveCheckpoint(playerid);
	}
	bool OnPlayerEnterRaceCheckpoint(int playerid)
	{
		return StreamerLibrary::OnPlayerEnterRaceCheckpoint(playerid);
	}
	bool OnPlayerLeaveRaceCheckpoint(int playerid)
	{
		return StreamerLibrary::OnPlayerLeaveRaceCheckpoint(playerid);
	}
	bool OnPlayerPickUpPickup(int playerid, int pickupid)
	{
		return StreamerLibrary::OnPlayerPickUpPickup(playerid, pickupid);
	}
	bool OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
	{
		return StreamerLibrary::OnPlayerEditObject(playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	}
	bool OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float x, float y, float z)
	{
		return StreamerLibrary::OnPlayerSelectObject(playerid, type, objectid, modelid, x,y,z);
	}
	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float x, float y, float z)
	{
		return StreamerLibrary::OnPlayerWeaponShot(playerid,weaponid,hittype,hitid,x,y,z);
	}

} _StreamerProcessor;

void extOnPlayerGameBegin(int playerid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerGameBegin(playerid);
	}
}

void extOnPlayerEnterMiniGame(int playerid, Zabawy::Base* game)
{ 
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerEnterMiniGame(playerid, game);
	}
}

SAMPGDK_CALLBACK(void, OnHTTPResponse(int index, int response_code, const char * data))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnHTTPResponse(index, response_code, std::string(data));
	}
}

void extOnPlayerVirtualWorldChange(int playerid, int newVWid, int oldVWid)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnPlayerVirtualWorldChange(playerid, newVWid, oldVWid);
	}
}

void extOnMafiaLoad(int id, CMafia* mafia)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnMafiaLoad(id, mafia);
	}
}

void extOnMafiaUnload(int id, CMafia* mafia)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnMafiaUnload(id, mafia);
	}
}

SAMPGDK_CALLBACK(bool, OnClientCheckResponse(int playerid, int type, int arg, int response))
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->OnClientCheckResponse(playerid, type, arg, response);
	}
	return true;
}

/*return callbacks
OnDialogResponse true
OnPlayerClickPlayerTextDraw true
OnPlayerClickTextDraw true
OnPlayerCommandText true
OnPlayerKeyStateChange false

OnPlayerRequestClass true
OnPlayerRequestSpawn true
OnPlayerSpawn true

OnPlayerTakeDamage false
OnPlayerText false
OnPlayerUpdate false
OnRconCommand true
OnPlayerWeaponShot true
OnTrailerUpdate true
OnUnoccupiedVehicleUpdate true
OnVehicleMod true
OnVehicleRespray true*/