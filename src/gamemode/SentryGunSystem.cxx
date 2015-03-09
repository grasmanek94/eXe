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

namespace SentryGunSystem
{
	unsigned long gHighestSentryID = 0;

	std::set<SentryGun*> SentriesNotKillingPlayerId[MAX_PLAYERS];

	std::map<int/*AreaID*/, SentryGun*> AreaToSentryGun;
	std::map<int/*ObjectID*/, SentryGun*> ObjectToSentryGun;
	std::map<int/*dbID*/, SentryGun*> DbIdToSentryGun;

	std::map < std::string /*which owner*/, std::pair<int/*owner type, 0 - none, 1 - user, 2 - mafia*/, SentryGun*>, ci_less> Owners;

	std::pair<SentryGun*, unsigned long long> LastHitBySentry[MAX_PLAYERS];

	std::set<SentryGun*> NeedsSaving;
	std::set<SentryGun*> SentryGuns;

	const std::vector<ObjectInfo> objects =
	{
		{ 0, 3877, { 0.0, 0.0, -0.9 }, { 0.0, 0.0, 0.0 } },
		{ 1, 2889, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 } },
		{ 2, 360, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } },
		{ 3, 360, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 120.0 } },
		{ 4, 360, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 240.0 } },
	};

	SentryGun::SentryGun(float x, float y, float z, float angle, float sx, float sy, float sz, int worldid, int interiorid)
		: Position(x, y, z), rot_z(angle), WorldID(worldid), InteriorID(interiorid), AreaOffsets_a(-sx, -sy, -sz), AreaOffsets_b(sx, sy, sz), owner_name(""), owner_status(0),
		last_save_time(0)
	{
		SentryGuns.insert(this);

		AreaID = 0xFFFF;

		LabelID = INVALID_3DTEXT_ID;

		next_create_time = 0;

		experience = 0;
		respect = 0;

		KilledVehicles = 0;
		KilledPlayers = 0;
		Destroyed = 0;

		health = 100.0;
		destroyed = false;
		deactivated = false;
	}

	void SentryGun::NeedsSave()
	{
		unsaved_changes = true;
		NeedsSaving.insert(this);
	}

	void SentryGun::RefreshSentryGun(float x, float y, float z, float angle, float sx, float sy, float sz, int worldid, int interiorid)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;
		rot_z = angle;

		AreaOffsets_a.x = -sx;
		AreaOffsets_a.y = -sy;
		AreaOffsets_a.z = -sz;

		AreaOffsets_b.x = sx;
		AreaOffsets_b.y = sy;
		AreaOffsets_b.z = sz;


		WorldID = worldid;
		InteriorID = interiorid;

		RefreshSentryGun();
	}


	void SentryGun::RefreshSentryGunObjects()
	{
		while (Object_IDs.size())
		{
			StreamerLibrary::DestroyDynamicObject(*Object_IDs.begin());
			auto foundobj = ObjectToSentryGun.find(*Object_IDs.begin());
			if (foundobj != ObjectToSentryGun.end())
			{
				ObjectToSentryGun.erase(foundobj);
			}
			Object_IDs.erase(Object_IDs.begin());
		}

		for (auto object : objects)
		{
			int objectid = StreamerLibrary::CreateDynamicObject(object.modelid, Position.x + object.offset_pos.x, Position.y + object.offset_pos.y, Position.z + object.offset_pos.z, object.offset_rot.x, object.offset_rot.y, object.offset_rot.z + rot_z, WorldID, InteriorID);
			Object_IDs.insert(objectid);
			ObjectToSentryGun.insert(std::pair<int, SentryGun*>(objectid, this));
		}
	}

	void SentryGun::DeleteAll()
	{
		while (Object_IDs.size())
		{
			StreamerLibrary::DestroyDynamicObject(*Object_IDs.begin());
			auto foundobj = ObjectToSentryGun.find(*Object_IDs.begin());
			if (foundobj != ObjectToSentryGun.end())
			{
				ObjectToSentryGun.erase(foundobj);
			}
			Object_IDs.erase(Object_IDs.begin());
		}
		if (LabelID != INVALID_3DTEXT_ID)
		{
			StreamerLibrary::DestroyDynamic3DTextLabel(LabelID);
			LabelID = INVALID_3DTEXT_ID;
		}
		if (AreaID != 0xFFFF)
		{
			StreamerLibrary::DestroyDynamicArea(AreaID);
			auto found = AreaToSentryGun.find(AreaID);
			if (found != AreaToSentryGun.end())
			{
				AreaToSentryGun.erase(found);
			}
			AreaID = 0xFFFF;
		}
	}

	void SentryGun::UpdateLabel()
	{
		//#ifdef _LOCALHOST_DEBUG
		if (LabelID != INVALID_3DTEXT_ID)
		{
			StreamerLibrary::DestroyDynamic3DTextLabel(LabelID);
			LabelID = INVALID_3DTEXT_ID;
		}

		LabelID = StreamerLibrary::CreateDynamic3DTextLabel(ReturnStatsText(), -1, Position.x, Position.y, Position.z + 1.5, 60.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, WorldID, InteriorID, -1, 60.0);
		//#endif
	}

	void SentryGun::RefreshSentryGun()
	{
		DeleteAll();

		{//create
			AreaID = StreamerLibrary::CreateDynamicCube(Position.x + AreaOffsets_a.x, Position.y + AreaOffsets_a.y, Position.z + AreaOffsets_a.z, Position.x + AreaOffsets_b.x, Position.y + AreaOffsets_b.y, Position.z + AreaOffsets_b.z, WorldID, InteriorID);

			AreaToSentryGun.insert(std::pair<int, SentryGun*>(AreaID, this));

			for (auto object : objects)
			{
				int objectid = StreamerLibrary::CreateDynamicObject(object.modelid, Position.x + object.offset_pos.x, Position.y + object.offset_pos.y, Position.z + object.offset_pos.z, object.offset_rot.x, object.offset_rot.y, object.offset_rot.z + rot_z, WorldID, InteriorID);
				Object_IDs.insert(objectid);
				ObjectToSentryGun.insert(std::pair<int, SentryGun*>(objectid, this));
			}
		}

		maxhealth = 50.0 + (GetLevel() * (22.5));
		health = maxhealth;

		UpdateLabel();
	}

	void SentryGun::RefreshArea()
	{
		if (AreaID != 0xFFFF)
		{
			StreamerLibrary::DestroyDynamicArea(AreaID);
			auto found = AreaToSentryGun.find(AreaID);
			if (found != AreaToSentryGun.end())
			{
				AreaToSentryGun.erase(found);
			}
			AreaID = 0xFFFF;
		}

		AreaID = StreamerLibrary::CreateDynamicCube(Position.x + AreaOffsets_a.x, Position.y + AreaOffsets_a.y, Position.z + AreaOffsets_a.z, Position.x + AreaOffsets_b.x, Position.y + AreaOffsets_b.y, Position.z + AreaOffsets_b.z, WorldID, InteriorID);
		AreaToSentryGun.insert(std::pair<int, SentryGun*>(AreaID, this));
	}

	void SentryGun::AddToArea(int playerid)
	{
		bool do_not_add = 
			(DoNotAttack.find(playerid) != DoNotAttack.end())
			||
			(owner_status == 1 && boost::iequals(Player[playerid].PlayerName, owner_name))
			||
			(owner_status == 2 && (Player[playerid].Mafia != nullptr && boost::iequals(Player[playerid].Mafia->Name, owner_name)))
			;

		if (!do_not_add)
		{
			PlayersInShootingArea.insert(playerid);
		}
		PlayersInArea.insert(playerid);
	}

	void SentryGun::RemoveFromArea(int playerid)
	{
		PlayersInShootingArea.erase(playerid);
		PlayersInArea.erase(playerid);
	}

	void SentryGun::CheckFire(unsigned long long TimeNow)
	{
		float fHitTarget[3];
		float hp;
		float dmg;

		bool attacked = false;

		int count = 0;
		int maxcount = (int)(std::pow(GetLevel(), 0.6) + 1.0);
		for (auto playerid : PlayersInShootingArea)
		{
			if (++count == maxcount)
			{
				break;
			}
			if (Player[playerid].Spawned)
			{
				GetPlayerPos(playerid, &fHitTarget[0], &fHitTarget[1], &fHitTarget[2]);

				if (Player[playerid].CurrentVehicle)
				{
					GetVehicleHealth(Player[playerid].CurrentVehicle, &hp);
					if (hp > 25.0f)
					{
						dmg = GetDamage()*10.0;

						hp -= dmg;

						if (hp < 25.0f && hp != (-dmg))
						{
							hp = 0.0f;
							KilledVehicle(playerid);
						}
						else
						{
							LastHitBySentry[playerid].first = this;
							LastHitBySentry[playerid].second = TimeNow + 1000;
							CreateExplosionForPlayer(playerid, fHitTarget[0] + Functions::gen_random_float(-1.0, 1.0), fHitTarget[1] + Functions::gen_random_float(-1.0, 1.0), fHitTarget[2] + 1.0, 1, FLT_EPSILON);
						}
						safeSetVehicleHealth(Player[playerid].CurrentVehicle, hp);

						attacked = true;
					}
				}
				else
				{
					GetPlayerHealth(playerid, &hp);
					if (hp > 0.0f)
					{
						dmg = GetDamage();

						hp -= dmg;

						if (hp < 0.0f)
						{
							hp = 0.0f;
						}
						LastHitBySentry[playerid].first = this;
						LastHitBySentry[playerid].second = TimeNow + 1000;
						CreateExplosionForPlayer(playerid, fHitTarget[0] + Functions::gen_random_float(-3.0, 3.0), fHitTarget[1] + Functions::gen_random_float(-3.0, 3.0), fHitTarget[2] + 1.0, 12, FLT_EPSILON);
						fixSetPlayerHealth(playerid, hp);

						attacked = true;
					}
				}
			}
		}

		if (!attacked && health < maxhealth)
		{
			health += 2.0;
			if (health > maxhealth)
			{
				health = maxhealth;
			}
		}
	}

	void SentryGun::SetPos(float x, float y, float z, float angle, bool rebuild)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;
		rot_z = angle;

		if (rebuild)
		{
			RefreshSentryGun();
		}

		NeedsSave();
	}

	void SentryGun::SetRange(float r, bool rebuild)
	{
		const float sqrt2 = 1.4142135623730950488016887242097f;

		AreaOffsets_a.x = -r*sqrt2;
		AreaOffsets_a.y = -r*sqrt2;
		AreaOffsets_a.z = -r*sqrt2;
							 
		AreaOffsets_b.x =  r*sqrt2;
		AreaOffsets_b.y =  r*sqrt2;
		AreaOffsets_b.z =  r*sqrt2;

		if (rebuild)
		{
			RefreshArea();
		}

		NeedsSave();
	}

	void SentryGun::SetArea(float minx, float miny, float minz, float maxx, float maxy, float maxz, bool rebuild)
	{
		AreaOffsets_a.x = std::min(minx, maxx) - Position.x;
		AreaOffsets_a.y = std::min(miny, maxy) - Position.y;
		AreaOffsets_a.z = std::min(minx, maxz) - Position.z;

		AreaOffsets_b.x = std::max(minx, maxx) - Position.x;
		AreaOffsets_b.y = std::max(miny, maxy) - Position.y;
		AreaOffsets_b.z = std::max(minx, maxz) - Position.z;

		if (rebuild)
		{
			RefreshArea();
		}

		NeedsSave();
	}

	void SentryGun::SetArea(float x, float y, float z, bool rebuild)
	{
		AreaOffsets_a.x = -x;
		AreaOffsets_a.y = -y;
		AreaOffsets_a.z = -z;

		AreaOffsets_b.x =  x;
		AreaOffsets_b.y =  y;
		AreaOffsets_b.z =  z;

		if (rebuild)
		{
			RefreshArea();
		}

		NeedsSave();
	}

	void SentryGun::DestroySentry(unsigned long long recreatetime)
	{
		next_create_time = recreatetime;

		destroyed = true;

		++Destroyed;
		--respect;

		NeedsSave();
	}

	void SentryGun::OnPlayerHitSentryGun(int playerid, int weaponid)
	{
		if (PlayersInShootingArea.find(playerid) != PlayersInShootingArea.end())
		{
			if (health > 0.0f && IsActive() && !IsDestroyed())
			{
				health -= WeaponInfo[weaponid].sentrygundamage;
				if (health <= 0.0f)
				{
					health = 0.0f;
					Player[playerid].GiveExperience(2 * (unsigned long long)(((GetLevel() * std::log(Player[playerid].GetLevel())) / Player[playerid].GetLevel()) + 4.0));
					DestroySentry(Functions::GetTime() + (300000 - (unsigned long long)(GetLevel() * 10000)));
				}
				UpdateLabel();
			}
		}
		else if (PlayersInArea.find(playerid) != PlayersInArea.end())
		{
			deactivated = !deactivated;
			NeedsSave();
			UpdateLabel();
		}
	}

	void SentryGun::Killed(int playerid)
	{
		++KilledPlayers;
		++respect;
		AddExperience((unsigned long long)(((Player[playerid].GetLevel() * std::log(GetLevel())) / GetLevel()) + 2.0));
	}

	void SentryGun::KilledVehicle(int playerid)
	{
		++KilledVehicles;
		++respect;
		AddExperience((unsigned long long)(((Player[playerid].GetLevel() * std::log(GetLevel())) / GetLevel()) + 2.0));
	}

	void SentryGun::AddExperience(long long exp)
	{
		experience = SafeAddULL(experience, exp);
		maxhealth = 50.0 + (GetLevel() * (20.0));
		UpdateLabel();
		NeedsSave();
	}

	double SentryGun::GetLevel()
	{
		double lvl = std::pow((double)experience, 0.3) + 1.0;
		if (lvl > 21.0)
		{
			lvl = 21.0;
		}
		return lvl;
	}

	float SentryGun::GetDamage()
	{
		return 4.0 + GetLevel();
	}

	bool SentryGun::IsActive()
	{
		return !deactivated;
	}

	bool SentryGun::IsDestroyed()
	{
		return destroyed;
	}

	void SentryGun::CheckReCreate(unsigned long long TimeNow)
	{
		if (destroyed && next_create_time <= TimeNow)
		{
			destroyed = false;
			health = maxhealth;
			//RefreshSentryGunObjects();
			UpdateLabel();
		}
	}
	
	const std::string SentryInformationLabelText(
"{%06X}LVL: %I64u (%d%%) / R: %I64d / ID: %d\n\
K/D/V: % d / %d / %d / DMG: %.0f / HP: %.0f\n\
%s");

	std::string SentryGun::ReturnStatsText()
	{
		double level = GetLevel();

		unsigned long long ulllevel = level;
		unsigned long percentage = (level - (double)((unsigned long long)level))*100.0;

		float dmg = GetDamage();

		const int colors[3] = { 0xFF0000, 0xFF8000, 0x00FF00 };

		int whichcolor = 0;
		if (deactivated)
		{
			whichcolor = 2;
		}
		else if (destroyed)
		{
			whichcolor = 1;
		}

		return Functions::string_format(
			SentryInformationLabelText,
			colors[whichcolor],
			ulllevel, percentage, respect, ID,
			KilledPlayers, Destroyed, KilledVehicles,
			dmg, health,
			owner_name.c_str()
		);
	}

	void SentryGun::AddToDoNotAttackList(int playerid)
	{
		SentriesNotKillingPlayerId[playerid].insert(this);
		DoNotAttack.insert(playerid);
	}

	void SentryGun::RemoveFromDoNotAttackList(int playerid)
	{
		SentriesNotKillingPlayerId[playerid].erase(this);
		DoNotAttack.erase(playerid);
	}

	void SentryGun::SetOwner(std::string name, int type)
	{
		auto iterator = Owners.find(owner_name);
		if (iterator != Owners.end())
			Owners.erase(iterator);

		owner_name = name;
		owner_status = type;

		Owners[owner_name] = std::pair<int, SentryGun*>(type, this);

		UpdateLabel();
	}

	void SentryGun::Save()
	{
		dbsentry.id(ID);

		dbsentry.posX(Position.x);
		dbsentry.posY(Position.y);
		dbsentry.posZ(Position.z);

		dbsentry.angle(rot_z);

		dbsentry.area_a_X(AreaOffsets_a.x);
		dbsentry.area_a_Y(AreaOffsets_a.y);
		dbsentry.area_a_Z(AreaOffsets_a.z);

		dbsentry.area_b_X(AreaOffsets_b.x);
		dbsentry.area_b_Y(AreaOffsets_b.y);
		dbsentry.area_b_Z(AreaOffsets_b.z);

		dbsentry.interiorid(InteriorID);
		dbsentry.worldid(WorldID);

		dbsentry.owner_status(owner_status);
		dbsentry.owner_name(owner_name);

		dbsentry.experience(experience);
		dbsentry.respect(respect);

		dbsentry.KilledVehicles(KilledVehicles);
		dbsentry.KilledPlayers(KilledPlayers);
		dbsentry.Destroyed(Destroyed);

		dbsentry.deactivated(deactivated);

		sentrygun * tmpsentrygun = new sentrygun;
		*tmpsentrygun = dbsentry;

		CreateWorkerRequest(0, "", tmpsentrygun, DATABASE_REQUEST_OPERATION_SAVE_SENTRYGUN, DATABASE_POINTER_TYPE_SENTRYGUN, 0, 0);
	}

	void SentryGun::Load(sentrygun * sentryptr)
	{
		dbsentry = *sentryptr;

		ID = dbsentry.id();

		DbIdToSentryGun[ID] = this;

		Position.x = dbsentry.posX();
		Position.y = dbsentry.posY();
		Position.z = dbsentry.posZ();

		rot_z = dbsentry.angle();

		AreaOffsets_a.x = dbsentry.area_a_X();
		AreaOffsets_a.y = dbsentry.area_a_Y();
		AreaOffsets_a.z = dbsentry.area_a_Z();

		AreaOffsets_b.x = dbsentry.area_b_X();
		AreaOffsets_b.y = dbsentry.area_b_Y();
		AreaOffsets_b.z = dbsentry.area_b_Z();

		InteriorID = dbsentry.interiorid();
		WorldID = dbsentry.worldid();

		owner_status = dbsentry.owner_status();
		owner_name = dbsentry.owner_name();

		experience = dbsentry.experience();
		respect = dbsentry.respect();

		KilledVehicles = dbsentry.KilledVehicles();
		KilledPlayers = dbsentry.KilledPlayers();
		Destroyed = dbsentry.Destroyed();

		deactivated = dbsentry.deactivated();
	}

	void SentryGun::Delete()
	{
		dbsentry.id(ID);

		sentrygun * tmpsentrygun = new sentrygun;
		*tmpsentrygun = dbsentry;

		DeleteAll();

		for (auto playerid : DoNotAttack)
		{
			LastHitBySentry[playerid].first = nullptr;
			SentriesNotKillingPlayerId[playerid].erase(this);
			PlayersInArea.erase(playerid);
			PlayersInShootingArea.erase(playerid);
		}

		DoNotAttack.clear();

		{
			auto toerase = DbIdToSentryGun.find(ID);
			if (toerase != DbIdToSentryGun.end())
			{
				DbIdToSentryGun.erase(toerase);
			}
		}

		{
			auto toerase = Owners.find(owner_name);
			if (toerase != Owners.end())
			{
				Owners.erase(toerase);
			}
		}
		
		NeedsSaving.erase(this);
		SentryGuns.erase(this);

		CreateWorkerRequest(0, "", tmpsentrygun, DATABASE_REQUEST_OPERATION_DELETE_SENTRYGUN, DATABASE_POINTER_TYPE_SENTRYGUN, ID, 0);

		//byebye
		delete this;
	}

	SentryGun* CreateSentryGun(float x, float y, float z, float sx, float sy, float sz, int worldid, int interiorid)
	{
		sentrygun * dbgun = new sentrygun;

		dbgun->id(++gHighestSentryID);

		dbgun->posX(x);
		dbgun->posY(y);
		dbgun->posZ(z);
	
		dbgun->angle(0.0);
	
		dbgun->area_a_X(-sx);
		dbgun->area_a_Y(-sy);
		dbgun->area_a_Z(-sz);
		
		dbgun->area_b_X(sx);
		dbgun->area_b_Y(sy);
		dbgun->area_b_Z(sz);
	
		dbgun->interiorid(interiorid);
		dbgun->worldid(worldid);

		dbgun->owner_status(0);
		dbgun->owner_name("");
	
		dbgun->experience(0);
		dbgun->respect(0);
		
		dbgun->KilledVehicles(0);
		dbgun->KilledPlayers(0);
		dbgun->Destroyed(0);
	
		dbgun->deactivated(false);

		SentryGun* gun = new SentryGun(x, y, z, sx, sy, sz, worldid, interiorid);

		gun->Load(dbgun);
		gun->RefreshSentryGun();
		gun->Save();

		delete dbgun;

		return gun;
	}

	SentryGun* CreateSentryGunRange(float x, float y, float z, float maxrange, int worldid, int interiorid)
	{
		SentryGun* gun = new SentryGun(x, y, z, maxrange*cos(M_PI / 4), maxrange*cos(M_PI / 4), maxrange*cos(M_PI / 4), worldid, interiorid);
		gun->RefreshSentryGun();
		return gun;
	}

	void ProcessSentryGunsTimer(int timerid, void * param)
	{
		unsigned long long TimeNow = Functions::GetTime();
		for (auto gun : SentryGuns)
		{
			if (!gun->IsDestroyed())
			{
				if (gun->IsActive())
				{
					gun->CheckFire(TimeNow);
				}
			}
			else
			{
				gun->CheckReCreate(TimeNow);
			}
		}
	}

	void ProcessSentryGunsSaveTimer(int timerid, void * param)
	{
		while (NeedsSaving.size())
		{
			(*NeedsSaving.begin())->Save();
			NeedsSaving.erase(NeedsSaving.begin());
		}
	}

	class SentryGunProcessor : public Extension::Base
	{
	public:
		bool OnGameModeInit() override
		{
			sampgdk_SetTimerEx(1333, true, ProcessSentryGunsTimer, nullptr, this);
			sampgdk_SetTimerEx(60000, true, ProcessSentryGunsSaveTimer, nullptr, this);

			typedef odb::query<sentrygun> query;
			typedef odb::result<sentrygun> result;

			transaction t(userdb->begin());

			result r(userdb->query<sentrygun>(query::id >= 0));

			for (result::iterator i(r.begin()); i != r.end(); ++i)
			{
				SentryGun* gun = new SentryGun;
				gun->Load(&*i);
				gun->RefreshSentryGun();

				if (gHighestSentryID < gun->ID)
				{
					gHighestSentryID = gun->ID;
				}
			}

			t.commit();

			std::cout << "Loaded " << SentryGunSystem::SentryGuns.size() << " Sentry Guns" << std::endl;
			return true;
		}

		bool OnPlayerConnect(int playerid) override
		{
			LastHitBySentry[playerid].first = nullptr;
			LastHitBySentry[playerid].second = 0;
			return true;
		}

		bool OnPlayerDisconnect(int playerid, int reason) override
		{
			while (SentriesNotKillingPlayerId[playerid].size())
			{
				SentryGun* gun = *SentriesNotKillingPlayerId[playerid].begin();
				gun->RemoveFromDoNotAttackList(playerid);
			}
			return true;
		}

		bool OnPlayerDeath(int playerid, int killerid, int reason) override
		{
			if (LastHitBySentry[playerid].first != nullptr && Functions::GetTime() < LastHitBySentry[playerid].second)
			{
				LastHitBySentry[playerid].first->RemoveFromArea(playerid);
				LastHitBySentry[playerid].first->Killed(playerid);
				LastHitBySentry[playerid].first = nullptr;
				LastHitBySentry[playerid].second = 0;
			}

			return true;
		}

		bool OnPlayerEnterDynamicArea(int playerid, int areaid) override
		{
			auto vsentrygun = AreaToSentryGun.find(areaid);
			if (vsentrygun != AreaToSentryGun.end())
			{
				vsentrygun->second->AddToArea(playerid);
			}
			return true; 
		}

		bool OnPlayerLeaveDynamicArea(int playerid, int areaid) override
		{ 
			auto vsentrygun = AreaToSentryGun.find(areaid);
			if (vsentrygun != AreaToSentryGun.end())
			{
				vsentrygun->second->RemoveFromArea(playerid);
			}
			return true;
		}

		bool OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float x, float y, float z) override
		{ 
			auto sentrygun = ObjectToSentryGun.find(objectid);
			if (sentrygun != ObjectToSentryGun.end())
			{
				SentryGun * gun = sentrygun->second;
				gun->OnPlayerHitSentryGun(playerid, weaponid);
			}
			return true; 
		}
	} 
	_SentryGunProcessor
		;
};

ZCMDF(sentryturret_add, PERMISSION_GAMER, RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/sentryadd" }), "fp")
{
	if (parser.Good() == 2)
	{
		float x, y, z, a, r;
		r = parser.Get<float>(0);
		if (r > 1.0)
		{
			GetPlayerPos(playerid, &x, &y, &z);
			GetPlayerFacingAngle(playerid, &a);
			SentryGunSystem::SentryGun* gun = SentryGunSystem::CreateSentryGun(0.0, 0.0, 0.0);
			gun->SetPos(x, y, z + 1.0, a, false);
			fixSetPlayerPos(playerid, x, y, z + 3.0);
			gun->SetRange(r, false);
			int ownerid = parser.Get<ParsePlayer>(1).playerid;
			if (ownerid != INVALID_PLAYER_ID)
			{
				gun->SetOwner(Player[ownerid].PlayerName, 1);
			}
			gun->RefreshSentryGun();
			fixSendClientMessage(playerid, -1, L_sentrygun_created);
			return true;
		}	
	}
	fixSendClientMessage(playerid, -1, L_sentrygun_usage);
	return true;
}

struct ActionParameters
{
	unsigned long id;
	std::string parse_format;
	std::string usage;
	std::string success;
};

std::map<std::string, ActionParameters> ActionNameToActionID =
{
	{ "range", {
		0,
		"f",
		"<RANGE> (>1.0)",
		"RANGE SET"
	} },

	{ "exp", {
		1,
		"u",
		"<NEWEXP>",
		"EXP SET"
	} },

	{ "delete", {
		2,
		"s",
		"",
		"SentryGun DELETED"
	} },

	{ "setpos", {
		3,
		"s",
		"",
		"SentryGun POS CHANGED"
	} },

	{ "owner", {
		4,
		"p",
		"<id/nick>(NEW OWNER, 0 - NO_ONE)",
		"OWNER SET"
	} },

	{ "setarea", {
		5,
		"ffffff",
		"<min xyz> <max xyz>",
		"AREA SET"
	} },

	{ "setareaex", {
		6,
		"s",
		"/savepos + current pos",
		"AREAEX SET"
	} },

	{ "ownermafia", {
		7,
		"s",
		"<MAFIA NAME>(NEW MAFIA OWNE)",
		"MAFIA OWNER SET"
	} },
};

ZCMDF(sgm, PERMISSION_GAMER, RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/sentrygunmanagement" }), "Iws")
{
	if (parser.Good() == 3)
	{
		unsigned long id = parser.Get<unsigned long>(0);
		auto found_action = ActionNameToActionID.find(parser.Get<std::string>(1));
		std::string nextparams = parser.Get<std::string>(2);

		auto found_turret_id = SentryGunSystem::DbIdToSentryGun.find(id);
		
		if (found_turret_id != SentryGunSystem::DbIdToSentryGun.end())
		{
			SentryGunSystem::SentryGun * current_gun = found_turret_id->second;

			if (found_action != ActionNameToActionID.end())
			{
				ParseInput second_parser;
				second_parser.SetFormat(found_action->second.parse_format);
				second_parser.ExecuteManualParse(nextparams);

				if (second_parser.Good() == found_action->second.parse_format.size())
				{
					switch (found_action->second.id)
					{
						case 0:
						{
							float r = second_parser.Get<float>(0);
							if (r > 1.0)
							{
								current_gun->SetRange(r);
								fixSendClientMessage(playerid, -1, found_action->second.success);
								return true;
							}
							break;
						}
						case 1:
						{
							long long exp = second_parser.Get<long long>(0);
							current_gun->AddExperience(exp);
							fixSendClientMessage(playerid, -1, found_action->second.success);
							return true;
						}
						case 2:
						{
							current_gun->Delete();
							fixSendClientMessage(playerid, -1, found_action->second.success);
							return true;
						}
						case 3:
						{
							float x, y, z, a;
							GetPlayerPos(playerid, &x, &y, &z);
							GetPlayerFacingAngle(playerid, &a);
							current_gun->SetPos(x, y, z + 1.0, a);
							fixSetPlayerPos(playerid, x, y, z + 3.0);
							fixSendClientMessage(playerid, -1, found_action->second.success);
							return true;
						}
						case 4:
						{
							int ownerid = second_parser.Get<ParsePlayer>(0).playerid;
							if (ownerid != INVALID_PLAYER_ID)
							{
								current_gun->SetOwner(Player[ownerid].PlayerName, 1);
							}
							else
							{
								current_gun->SetOwner("", 0);
							}
							fixSendClientMessage(playerid, -1, found_action->second.success);
							return true;
						}
						case 5:
						{
							float area[6] =
							{
								second_parser.Get<float>(0), second_parser.Get<float>(1), second_parser.Get<float>(2),
								second_parser.Get<float>(3), second_parser.Get<float>(4), second_parser.Get<float>(5)
							};
							current_gun->SetArea(area[0], area[1], area[2], area[3], area[4], area[5]);
							fixSendClientMessage(playerid, -1, found_action->second.success);
							return true;
						}
						case 6:
						{
							float x, y, z;
							GetPlayerPos(playerid, &x, &y, &z);
							float area[6] =
							{
								Player[playerid].statistics.SaveX, Player[playerid].statistics.SaveY, Player[playerid].statistics.SaveZ,
								x, y, z
							};
							current_gun->SetArea(area[0], area[1], area[2], area[3], area[4], area[5]);
							fixSendClientMessage(playerid, -1, found_action->second.success);
							return true;
						}
						case 7:
						{
							std::string mafiaid = second_parser.Get<std::string>(0);
							auto mafia_found = MafiaProxy.find(mafiaid);

							if (mafia_found != MafiaProxy.end())
							{
								current_gun->SetOwner(mafiaid, 2);
								fixSendClientMessage(playerid, -1, found_action->second.success);
								return true;
							}
							break;
						}
					}
				}
				fixSendClientMessage(playerid, -1, TranslateP(playerid, L_sentrygun_manage_nexuse) + found_action->first + " " + found_action->second.usage);
				return true;
			}
			fixSendClientMessage(playerid, -1, L_sentrygun_invalid_action);
			return true;
		}
		fixSendClientMessage(playerid, -1, L_sentrygun_notfound);
		return true;
	}
	fixSendClientMessage(playerid, -1, L_sentrygun_manage_usage);
	return true;
}