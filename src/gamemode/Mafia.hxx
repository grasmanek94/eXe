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
//don't even bother to understand what's going on here, girls are easier to understand..
#include "GameMode.hxx"
#include "PrivateCar.hxx"
#pragma once

typedef std::pair<std::string, unsigned char> MafiaMemberData;
typedef std::map<std::string, unsigned char, ci_less> MafiaMemberDataMap;

bool LoadMafia(std::string mafianame, int issuerid = INVALID_PLAYER_ID, std::string issuername = "");
bool ParseMembersAndPopulate(std::string members, MafiaMemberDataMap& memberdata);

std::string GetMafiaMembersString(MafiaMemberDataMap& memberdata);
//max players online == max loaded mafias == one loaded mafia per player
const int MAX_MAFIAS = MAX_PLAYERS;
const int INVALID_MAFIA_ID = INVALID_PLAYER_ID;

const std::array<language_string_ids,4> PermissionNames =
{
	L_mafia_rank_worker,
	L_mafia_rank_vicepresident,
	L_mafia_rank_president,
	L_mafia_rank_owner
};

enum MafiaPermissions
{
	MAFIA_MEMBER,
	MAFIA_WICEPREZES,
	MAFIA_PREZES,
	MAFIA_WLASCICIEL
};

namespace Mafia
{
	extern std::array<int,MAX_PLAYERS> TagTextLabel;
};

//two bits per permission level 00 01 10 11... 0,1,2,3
enum MAFIA_ACTIONS
{
	MAFIA_ACTION_INVITE						= (0), 
	MAFIA_ACTION_KICK						= (2), 
	MAFIA_ACTION_SAVECAR					= (4), 
	MAFIA_ACTION_SPAWNCAR					= (6), 
	MAFIA_ACTION_CHANGE_MEMBER_LEVEL		= (8), 
	MAFIA_ACTION_CHANGE_ACTION_PERMISSIONS	= (10),
	MAFIA_ACTION_CHANGE_COLOR				= (12),
	MAFIA_ACTION_DISTRIBUTE_MONEY			= (14),
	MAFIA_ACTION_GIVE_MONEY					= (16),
	MAFIA_ACTION_DISTRIBUTE_RESPECT			= (18),
	MAFIA_ACTION_GIVE_RESPECT				= (20),
	MAFIA_ACTION_DELETE						= (22),
	MAFIA_ACTION_SKIN						= (24),
	MAFIA_ACTION_SPAWN						= (26),
	MAFIA_ACTION_TAG						= (28)
};

//Special mafia base declaration, very ugly but has to be here
//This actually resolves the erroneous circular dependency the two classes have on each other
namespace MafiaBase
{
	class System;
};

class CMafia
{
public:
	bool unsaved_data;
	std::string Name;
	std::string RawMembers;

	std::set<int> OnlineMembers;
	std::set<int> OnlineWicePrezes;
	std::set<int> OnlinePrezes;
	std::set<int> OnlineWlasciciel;

	//nickname, permissions
	MafiaMemberDataMap AllMembers;

	std::set<std::string, ci_less> Members;
	std::set<std::string, ci_less> WicePrezes;
	std::set<std::string, ci_less> Prezes;
	std::set<std::string, ci_less> Wlasciciel;

	std::array<SPrivateCar,2> PrivateVehicle;

	mafia dbmafia;
	int MafiaID;

	std::bitset<64> bActionPermissions;
	unsigned int color;
	long long money;
	long long score;

	int skin;
	float SpawnX;
	float SpawnY;
	float SpawnZ;
	float SpawnA;

	unsigned long kills;
	unsigned long deaths;
	unsigned long long experience;

	unsigned long long creation_date;
	unsigned long long playtime;

	unsigned long long last_load;

	std::string tag;

	bool taken;
	bool deleting;

	MafiaBase::System* Base;

	CMafia();
	bool Valid();
	bool LoadData(mafia* pointer);
	void PutIntoMembers(std::string nickname, unsigned char level);
	void RemoveFromMembers(std::string nickname);
	void SendMessageToMafia(int color, std::string text);
	void PutIntoOnlineMembers(int playerid, unsigned char level);
	void UpdateColor();
	void UpdateColor(int playerid);
	void RemoveFromOnlineMembers(int playerid, bool reloadonly = false);
	void RefreshLabel(int playerid);
	bool OnConnect(int playerid);
	void RemoveLabel(int playerid);
	bool OnDisconnect(int playerid);
	void Save();
	bool RemoveMember(std::string nickname, bool IN_SELF = false);
	void RemoveMafia();
	void ApplySkin(int skinid);
	void ApplyTag(std::string _tag);
	bool AddMember(int playerid, unsigned char level);
	bool ChangeMemberLevel(std::string nickname, unsigned char level);
	void SetActionPermission(MAFIA_ACTIONS action, unsigned char level);
	unsigned char GetActionPermission(MAFIA_ACTIONS action);
	bool IsPlayerAllowedAction(int playerid, MAFIA_ACTIONS action);
	bool CheckAllowedWithMessage(int playerid, MAFIA_ACTIONS action);

	void SaveStats();
	void SendStatsToWeb(bool IN_MAFIA = false);

	void PrepareForSave();
};

extern std::map < std::string, CMafia*, ci_less > PlayerNameToMafia;
extern std::map < std::string, CMafia*, ci_less > MafiaProxy;
extern std::set < std::string, ci_less  > LoadingMafias;
extern std::set < std::string, ci_less  > LoadedMafias;
extern std::array<CMafia, MAX_MAFIAS> Mafias;