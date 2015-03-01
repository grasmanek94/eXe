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
#pragma once

namespace SentryGunSystem
{
	extern const std::string SentryInformationLabelText;

	struct ObjectInfo
	{
		int arrid;
		int modelid;
		glm::vec3 offset_pos;
		glm::vec3 offset_rot;
	};

	extern const std::vector<ObjectInfo> objects;

	class SentryGun
	{
	private:
		sentrygun dbsentry;

		std::set<int> Object_IDs;
		std::set<int> DoNotAttack;

		int AreaID;
		int LabelID;

		std::set<int> PlayersInArea;
		std::set<int> PlayersInShootingArea;

		unsigned long long next_create_time;

		float health;
		float maxhealth;
		bool destroyed;		
	public:
		//start savedata
		unsigned long ID;

		glm::vec3 Position;
		float rot_z;

		glm::vec3 AreaOffsets_a;
		glm::vec3 AreaOffsets_b;

		int WorldID;
		int InteriorID;

		unsigned long long experience;
		long long respect;

		unsigned long KilledVehicles;
		unsigned long KilledPlayers;
		unsigned long Destroyed;

		std::string owner_name;
		int owner_status;

		bool deactivated;
		//end savedata

		bool unsaved_changes;
		unsigned long long last_save_time;

		SentryGun(float x = 0.0, float y = 0.0, float z = 0.0, float angle = 0.0, float sx = 15.0, float sy = 15.0, float sz = 15.0, int worldid = 0, int interiorid = 0);

		void RefreshSentryGun();
		void AddToArea(int playerid);
		void RemoveFromArea(int playerid);
		void CheckFire(unsigned long long TimeNow);
		void RefreshArea();
		void SetPos(float x, float y, float z, float angle = 0.0, bool rebuild = true);
		void SetRange(float r, bool rebuild = true);
		void SetArea(float x, float y, float z, bool rebuild = true);
		void SetArea(float minx, float miny, float minz, float maxx, float maxy, float maxz, bool rebuild = true);
		void RefreshSentryGun(float x, float y, float z, float angle, float sx, float sy, float sz, int worldid, int interiorid);
		void OnPlayerHitSentryGun(int playerid, int weaponid);
		void Killed(int playerid);
		void KilledVehicle(int playerid);
		void AddExperience(long long exp);
		void DestroySentry(unsigned long long recreatetime);
		void RefreshSentryGunObjects();
		bool CheckLevel(unsigned long long oldlevel, unsigned long long newlevel);
		double GetLevel();
		float GetDamage();
		bool IsActive();
		bool IsDestroyed();
		void CheckReCreate(unsigned long long TimeNow);
		std::string ReturnStatsText();
		void UpdateLabel();
		void AddToDoNotAttackList(int playerid);
		void RemoveFromDoNotAttackList(int playerid);
		void DeleteAll();
		void NeedsSave();
		void SetOwner(std::string name, int type);
		void Save();
		void Load(sentrygun * sentryptr);
		//suicide
		void Delete();
	};

	extern std::map<int/*AreaID*/, SentryGun*> AreaToSentryGun;
	extern std::map<int/*ObjectID*/, SentryGun*> ObjectToSentryGun;
	extern std::set<SentryGun*> SentryGuns;

	SentryGun* CreateSentryGun(float x, float y, float z, float sx = 15.0, float sy = 15.0, float sz = 15.0, int worldid = 0, int interiorid = 0);
	SentryGun* CreateSentryGunRange(float x, float y, float z, float maxrange = 21.21/*sx=sy=sz=15.0=(21.21/(1.0/cos(45 deg)))*/, int worldid = 0, int interiorid = 0);
};