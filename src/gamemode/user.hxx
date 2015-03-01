/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	This file is the base for the DataBase system, it defines
			all database structures

	________________________________________________________________
	Dependencies
		ODB

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include <string>
#include <limits>
#include <odb/core.hxx>
#ifdef WIN32
#include <Windows.h>
#else
inline void* SecureZeroMemory(void* ptr, size_t ctn)
{
	return memset_s(ptr, 0, ctn);
}
#endif

#undef max

#define acmf(variable_type,variable_name) \
	public:\
		const variable_type& variable_name () const \
		{ \
			return variable_name ## _; \
		} \
		void variable_name (variable_type variable_name) \
		{ \
			variable_name ## _ = variable_name; \
		}\
	private:\
		variable_type variable_name ## _

#define accessor(variable_type, variable_name) \
	public:\
		const variable_type& variable_name () const \
		{ \
			return variable_name ## _; \
		}\
		void variable_name(variable_type variable_name) \
		{ \
			variable_name ## _ = variable_name; \
		}

#define ResetValue(name) name ## _(0)
#define ResetValueStr(name) name ## _("")

#pragma db object
class user
{
public:
	accessor(std::string, nickname);
	acmf(std::string, displayname);
	acmf(std::string, password);
	acmf(long long, money);
	acmf(long long, score);//unused
	acmf(unsigned long long, kills);
	acmf(unsigned long long, experience);
	acmf(unsigned long long, deaths);
	acmf(unsigned long long, privilidges);
	acmf(unsigned long long, bounty);
	acmf(unsigned long long, suicides);
	acmf(long long, respect);
	acmf(unsigned long long, kicks);
	acmf(unsigned long long, warns);
	acmf(unsigned long long, playtime);

	acmf(unsigned long long, banned);
	acmf(std::string, ban_reason);
	acmf(std::string, ban_who);
	acmf(unsigned long, bannedip);
	acmf(unsigned long long, bannedidentificator);
	acmf(unsigned long long, jailtime);
	acmf(unsigned long long, registertime);
	acmf(std::string, email);
	acmf(int, SkinModelID);
	acmf(unsigned long long, Premium);
	acmf(unsigned long long, lastconnected);
	acmf(std::string, MafiaName);
	acmf(bool, UseMafiaSkin);
	acmf(bool, UseMafiaSpawn);
	acmf(int, rampid);
	acmf(float, SaveX);
	acmf(float, SaveY);
	acmf(float, SaveZ);
	acmf(float, SaveA);
	acmf(int, staticcolor);
	acmf(long long, bank);
	acmf(std::string, registerip);
	acmf(std::string, lastip);
	acmf(std::string, recoverycode);
	acmf(unsigned long long, mailverifiedtime);
	acmf(unsigned long long, lastmailsent);
	acmf(std::string, verifiedmail);
	acmf(unsigned long long, verificationsent);
	acmf(std::string, verifiedrecoverycode);
public:
	const char& PrivateCars(int a) const
	{
		return PrivateCars_[a];
	}
	char* PrivateCars()
	{
		return PrivateCars_;
	}
	void PrivateCars(char* PrivateCars)
	{
		memcpy(PrivateCars_, PrivateCars, 720);
	}

	const char& weapondata(int a) const
	{
		return weapondata_[a];
	}
	char* weapondata()
	{
		return weapondata_;
	}
	void weapondata(char* weapondata)
	{
		memcpy(weapondata_, weapondata, 440);
	}

	const char& achievementdata(int a) const
	{
		return achievementdata_[a];
	}
	char* achievementdata()
	{
		return achievementdata_;
	}
	void achievementdata(char* Achievementdata)
	{
		memcpy(achievementdata_, Achievementdata, 4096);
	}
private:
	friend class odb::access;
#pragma db type("BLOB")
	char PrivateCars_[720];
#pragma db type("BLOB")
	char achievementdata_[4096];
#pragma db type("BLOB")
	char weapondata_[440];

#pragma db unique id index
	std::string nickname_;

public:
	user():
		nickname_(""),
		displayname_(""),
		password_(""),
		ResetValue(money),
		ResetValue(score),//unused
		ResetValue(kills),
		ResetValue(experience),
		ResetValue(deaths),
		ResetValue(privilidges),
		ResetValue(bounty),
		ResetValue(suicides),
		ResetValue(respect),
		ResetValue(kicks),
		ResetValue(warns),
		ResetValue(playtime),
		ResetValue(banned),
		ResetValueStr(ban_reason),
		ResetValueStr(ban_who),
		ResetValue(jailtime),
		ResetValue(registertime),
		ResetValueStr(email),
		ResetValue(SkinModelID),
		ResetValue(Premium),
		ResetValue(lastconnected),
		ResetValueStr(MafiaName),
		UseMafiaSkin_(false),
		UseMafiaSpawn_(false),
		ResetValue(rampid),
		ResetValue(SaveX),
		ResetValue(SaveY),
		ResetValue(SaveZ),
		ResetValue(SaveA),
		ResetValue(staticcolor),
		ResetValue(bank),
		ResetValueStr(registerip),
		ResetValueStr(lastip),
		ResetValueStr(recoverycode),
		ResetValue(mailverifiedtime),
		ResetValue(lastmailsent),
		ResetValueStr(verifiedmail),
		ResetValue(verificationsent),
		ResetValueStr(verifiedrecoverycode)
	{
#ifdef SAMPGDK_AMALGAMATION
		SecureZeroMemory(PrivateCars_, 720);
		SecureZeroMemory(achievementdata_, 4096);
		SecureZeroMemory(weapondata_, 440);
#endif
	}
	user
		(
		std::string inickname,
		std::string ipassword
		) :
		nickname_(inickname),
		displayname_(inickname),
		password_(ipassword),
		ResetValue(money),
		ResetValue(score),//unused
		ResetValue(kills),
		ResetValue(experience),
		ResetValue(deaths),
		ResetValue(privilidges),
		ResetValue(bounty),
		ResetValue(suicides),
		ResetValue(respect),
		ResetValue(kicks),
		ResetValue(warns),
		ResetValue(playtime),
		ResetValue(banned),
		ResetValueStr(ban_reason),
		ResetValueStr(ban_who),
		ResetValue(jailtime),
		ResetValue(registertime),
		ResetValueStr(email),
		ResetValue(SkinModelID),
		ResetValue(Premium),
		ResetValue(lastconnected),
		ResetValueStr(MafiaName),
		UseMafiaSkin_(false),
		UseMafiaSpawn_(false),
		ResetValue(rampid),
		ResetValue(SaveX),
		ResetValue(SaveY),
		ResetValue(SaveZ),
		ResetValue(SaveA),
		ResetValue(staticcolor),
		ResetValue(bank),
		ResetValueStr(registerip),
		ResetValueStr(lastip),
		ResetValueStr(recoverycode),
		ResetValue(mailverifiedtime),
		ResetValue(lastmailsent),
		ResetValueStr(verifiedmail),
		ResetValue(verificationsent),
		ResetValueStr(verifiedrecoverycode)
	{
		#ifdef SAMPGDK_AMALGAMATION
		SecureZeroMemory(PrivateCars_, 720);
		SecureZeroMemory(achievementdata_, 4096);
		SecureZeroMemory(weapondata_, 440);
		#endif
	}
};

#pragma db object
class ban
{
public:
	accessor(unsigned long, ip);
	acmf(unsigned long long, bannedtime);
	acmf(unsigned long long, bannedidentificator);
	acmf(std::string, banneduser);
	acmf(std::string, whobannedthisuser);
	acmf(std::string, banreason);
private:
	friend class odb::access;

#pragma db unique id index
	unsigned long ip_;

public:
	ban()
		:
		ip_(0),
		bannedtime_(0),
		banneduser_(""),
		whobannedthisuser_(""),
		banreason_(""),
		bannedidentificator_(0)
	{}
	ban
		(
		unsigned long ip,
		unsigned long long bannedtime,
		unsigned long long bannedidentificator,
		std::string banneduser,
		std::string whobannedthisuser,
		std::string banreason
		) :
		ip_(ip),
		bannedtime_(bannedtime),
		banneduser_(banneduser),
		whobannedthisuser_(whobannedthisuser),
		banreason_(banreason),
		bannedidentificator_(bannedidentificator)
	{}
};

#pragma db object
class mafia
{
public:
	accessor(std::string, name);
	acmf(std::string, displayname);
	acmf(std::string, members);
	acmf(std::string, base_identifier);
	acmf(unsigned long long, last_load);
	acmf(unsigned long long, action_permissions);
	acmf(unsigned int, color);
	acmf(long long, money);
	acmf(long long, score);
	acmf(int, skin);
	acmf(float, SpawnX);
	acmf(float, SpawnY);
	acmf(float, SpawnZ);
	acmf(float, SpawnA);
	acmf(unsigned long, kills);
	acmf(unsigned long, deaths);
	acmf(unsigned long long, experience);
	acmf(unsigned long long, creation_date);
	acmf(unsigned long long, playtime);
	acmf(std::string, tag);
public:
	const char& PrivateCars(int a) const
	{
		return PrivateCars_[a];
	}
	char* PrivateCars()
	{
		return PrivateCars_;
	}
	void PrivateCars(char* PrivateCars)
	{
		memcpy(PrivateCars_, PrivateCars, 144);
	}
private:
#pragma db type("BLOB")
	char PrivateCars_[144];

	friend class odb::access;

#pragma db unique id index
	std::string name_;

public:
	mafia():
		name_(""),
		members_(""),
		displayname_(""),
		base_identifier_(""),
		last_load_(0),
		action_permissions_(std::numeric_limits<unsigned long long>::max()),
		color_(0xFF3BC1),
		score_(0),
		money_(0),
		skin_(0),
		SpawnX_(0.0f),
		SpawnY_(0.0f),
		SpawnZ_(5.0f),
		SpawnA_(0.0f),
		kills_(0),
		deaths_(0),
		experience_(0),
		creation_date_(0),
		playtime_(0),
		tag_("")
	{}
	mafia(std::string iname) : 
		name_(iname), 
		members_(""), 
		displayname_(""),
		base_identifier_(""), 
		last_load_(0), 
		action_permissions_(std::numeric_limits<unsigned long long>::max()), 
		color_(0xFF3BC1),
		score_(0), 
		money_(0),
		skin_(0),
		SpawnX_(0.0f),
		SpawnY_(0.0f),
		SpawnZ_(5.0f),
		SpawnA_(0.0f),
		kills_(0),
		deaths_(0),
		experience_(0),
		creation_date_(0),
		playtime_(0),
		tag_("")
	{
#ifdef SAMPGDK_AMALGAMATION
		SecureZeroMemory(PrivateCars_, 144);
#endif	
	}
};

#pragma db member(mafia::members_) type("VARCHAR(4096)")

#pragma db object
class house
{
public:
	accessor(unsigned long, id);

	acmf(float, world_posX);
	acmf(float, world_posY);
	acmf(float, world_posZ);
	acmf(float, world_angle);

	acmf(float, interior_posX);
	acmf(float, interior_posY);
	acmf(float, interior_posZ);
	acmf(float, interior_angle);

	acmf(unsigned long long, rented_to_date);	
	acmf(unsigned long long, rent_cost_cash_per_day);
	acmf(unsigned long long, rent_cost_respect_per_day);

	acmf(int, interiorid);
	acmf(bool, rented_to_mafia);

	acmf(std::string, rented_to_nickname);

	acmf(bool, locked);
private:
	friend class odb::access;

	#pragma db id auto      
	unsigned long id_;      

public:
	house():
		world_posX_(0.0f),
		world_posY_(0.0f),
		world_posZ_(3.0f),
		world_angle_(0.0f),

		interior_posX_(0.0f),
		interior_posY_(0.0f),
		interior_posZ_(3.0f),
		interior_angle_(0.0f),

		rented_to_date_(0),
		rent_cost_cash_per_day_(0),
		rent_cost_respect_per_day_(0),

		interiorid_(0),
		rented_to_mafia_(false),

		rented_to_nickname_(""),

		locked_(false)
	{}
};


#pragma db object
class sentrygun
{
public:
	accessor(unsigned long, id);

	acmf(float, posX);
	acmf(float, posY);
	acmf(float, posZ);
	acmf(float, angle);

	acmf(float, area_a_X);
	acmf(float, area_a_Y);
	acmf(float, area_a_Z);

	acmf(float, area_b_X);
	acmf(float, area_b_Y);
	acmf(float, area_b_Z);

	acmf(int, interiorid);
	acmf(int, worldid);

	acmf(int, owner_status);
	acmf(std::string, owner_name);

	acmf(unsigned long long, experience);
	acmf(long long, respect);

	acmf(unsigned long, KilledVehicles);
	acmf(unsigned long, KilledPlayers);
	acmf(unsigned long, Destroyed);

	acmf(bool, deactivated);
private:
	friend class odb::access;

#pragma db id auto      
	unsigned long id_;

public:
	sentrygun() :
		posX_(0.0f),
		posY_(0.0f),
		posZ_(3.0f),
		angle_(0.0f),

		interiorid_(0),
		worldid_(0),

		owner_status_(0),
		owner_name_(""),

		experience_(0),
		respect_(0),

		KilledVehicles_(0),
		KilledPlayers_(0),
		Destroyed_(0),

		 deactivated_(false)
	{}
};
#undef acmf
#undef accessor