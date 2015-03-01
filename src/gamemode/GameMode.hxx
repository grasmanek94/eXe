/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		This file is the core of everything and generates the 
		precompiled header to be included in all other files related
		to the GameMode.
	________________________________________________________________	
	Notes
		-	30-01-2015: As of now I came to the realization we need
			some standard rules as to when or when not use ordered/
			unordered containes, so the following rules will apply:
				- STATIC containers (containers inited ONCE, pure lookup containers [find, contains]) should use `unordered`
				- DYNAMIC containers (lots of inserts/ereases) should use `ordered`

	________________________________________________________________
	Dependencies

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#pragma once

#ifdef _WIN32
#pragma warning(disable:4996 4800 4305 4244 4127 4512)
#else
#include <string.h>
inline void* SecureZeroMemory(void* ptr, size_t ctn)
{
	return memset(ptr, 0, ctn);
}

#define _DEBUG
#define _LOCALHOST_DEBUG
#define SAMPGAMEMODE_EXPORTS
#define SAMPGDK_AMALGAMATION
#define SAMPGDK_LINUX
#define SAMPGDK_STATIC
#define DATABASE_SQLITE
#define SQLITE_ENABLE_UNLOCK_NOTIFY
#define LIBODB_SQLITE_STATIC_LIB
#define LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
#define LIBODB_MYSQL_STATIC_LIB
#define LIBODB_STATIC_LIB
#define BOOST_ALL_NO_LIB
#define BOOST_ALL_NO_LIBS
#define SAMPGDK_PLUGIN_ONLY
#define BOOST_CHRONO_HEADER_ONLY
#define SAMPGDK_CPP_WRAPPERS

#endif

#include "../sampgdk/sampgdk.h"
using namespace sampgdk;

const unsigned long MUSIC_OFF = (1098);
const unsigned long NO_SOUND = MUSIC_OFF;

#undef MAX_PLAYERS
static const int MAX_PLAYERS = 500;

#ifdef _WIN32
#undef GetTickCount
#undef SelectObject
#undef SetTimer
#undef KillTimer
#undef CreateMenu
#undef DestroyMenu
#endif

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <chrono>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifdef WIN32
#include <codecvt>
#include <concurrent_queue.h>
#else
#include <locale>
#include <tbb/concurrent_queue.h>
#endif

#ifndef LIBODB_SQLITE_STATIC_LIB
#define LIBODB_SQLITE_STATIC_LIB (1)
#endif
#ifndef LIBODB_MYSQL_STATIC_LIB
#define LIBODB_MYSQL_STATIC_LIB (1)
#endif
#ifndef LIBODB_STATIC_LIB
#define LIBODB_STATIC_LIB (1)
#endif

#include <odb/database.hxx>

#if defined(DATABASE_MYSQL)
#	include <odb/mysql/database.hxx>
	using namespace odb::mysql;
#	ifdef _DEBUG
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\Debug\\libmysql.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\Debug\\clientlib.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\Debug\\mysqlclient.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\dbug\\Debug\\dbug.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\extra\\yassl\\Debug\\yassl.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\extra\\yassl\\taocrypt\\Debug\\taocrypt.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\authentication_win\\Debug\\auth_win_client.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\mysys\\Debug\\mysys.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\mysys_ssl\\Debug\\mysys_ssl.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\regex\\Debug\\regex.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\strings\\Debug\\strings.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\vio\\Debug\\vio.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\zlib\\Debug\\zlib.lib")
#	else
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\Release\\libmysql.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\Release\\clientlib.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\Release\\mysqlclient.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\dbug\\Release\\dbug.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\extra\\yassl\\Release\\yassl.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\extra\\yassl\\taocrypt\\Release\\taocrypt.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\libmysql\\authentication_win\\Release\\auth_win_client.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\mysys\\Release\\mysys.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\mysys_ssl\\Release\\mysys_ssl.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\regex\\Release\\regex.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\strings\\Release\\strings.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\vio\\Release\\vio.lib")
#		pragma comment(lib, ".\\lib\\mysql-src\\zlib\\Release\\zlib.lib")
#	endif
#elif defined(DATABASE_SQLITE)
#	include <odb/connection.hxx>
#	include <odb/transaction.hxx>
#	include <odb/schema-catalog.hxx>
#	include <odb/sqlite/database.hxx>
#	include <odb/sqlite/error.hxx>
#	include <odb/sqlite/exceptions.hxx>
	using namespace odb::sqlite;
#endif

//make sure to recompile user.hxx under correct database when changing to/from sqlite3/mysql
#include "user-odb.hxx"

#include "../../boost_extract/boost/utility.hpp"
#include "../../boost_extract/boost/lexical_cast.hpp"
#include "../../boost_extract/boost/any.hpp"
#include "../../boost_extract/boost/algorithm/string.hpp"
#include "../../boost_extract/boost/date_time.hpp"
#include "../../boost_extract/boost/random.hpp"
#include "../../boost_extract/boost/filesystem.hpp"
#include "../../boost_extract/boost/variant.hpp"
#include "../../boost_extract/boost/algorithm/string/join.hpp"
#include "../../boost_extract/boost/asio.hpp"
#include "../../boost_extract/boost/range/algorithm/remove_if.hpp"
#include "../../boost_extract/boost/regex.hpp"

extern std::set<int> PlayersOnline;
bool SendSpecialDeathMessage(int playerid, int iconid, int color, std::string killername);

template<class T> inline void bResetBits(T& var)
{
	var = 0;
}

template<class T> inline void bSetBitTrue(T& var, unsigned long long bit)
{
	var |= bit;
}

template<class T> inline void bSetBitFalse(T& var, unsigned long long bit)
{
	var &= (~bit);
}

template<class T> inline void bToggleBit(T& var, unsigned long long bit)
{
	var ^= bit;
}

template<class T> inline bool bIsBitEnabled(T& var, unsigned long long bit)
{
	return var & bit;
}

inline void makesmallerstrings(std::string data, std::vector<std::string>& smaller_strings, size_t max_substr_size = 100)
{
	for (unsigned i = 0; i < data.length(); i += max_substr_size)
		smaller_strings.push_back(data.substr(i, max_substr_size));
}

template <typename T> T clamp(const T& n, const T& lower, const T& upper) 
{
	return std::max(lower, std::min(n, upper));
}

unsigned int GetMSB(register unsigned int x);

void DelayKick(int playerid, int reason);
void SpawnPlayerAnyway(int playerid);

static const int INVALID_TIMER_ID = NULL;//first timer id is always 1, so 0 is invalid
const auto M_PI = 3.14159265358979323846;

/************************************************************************/
/* Comparator for case-insensitive comparison in STL assos. containers  */
/************************************************************************/
struct ci_less : std::binary_function<std::string, std::string, bool>
{
	// case-independent (ci) compare_less binary function
	//struct nocase_compare : public std::binary_function<unsigned char, unsigned char, bool>
	//{
	//	bool operator() (const unsigned char& c1, const unsigned char& c2) const {
	//		return tolower(c1) < tolower(c2);
	//	}
	//};
	bool operator() (const std::string & s1, const std::string & s2) const 
	{
		//return std::lexicographical_compare
		//	(s1.begin(), s1.end(),   // source range
		//	s2.begin(), s2.end(),   // dest range
		//	nocase_compare());  // comparison
		return _stricmp(s1.c_str(), s2.c_str()) < 0;//!= ?
	}
};

//Abandon all hope ye who enter beyond this point

//everything was written on days between 23:00-06:00, with extra booze and coffee, on sane but crazy minds, which eventually produced this insane unmaintanable code
extern unsigned long long LowPrecisionTimerValue;

#include "LangMain.hxx"

#include "Version.hxx"

#include "../streamer/StreamerLib.hxx" 	 
#include "MapAndreas.hxx"
#include "whirlpool.hxx"

#include "SendClientMessageFix.hxx"
#include "TextProcessing.hxx"
#include "Vehicles.hxx"
#include "NativeSAVehicleLocations.hxx" /* Here? For relevance? Also, your ctylish .hxx, and .cxx when I pasted the file contence I forgot... sooo, meh. LOVE YOU! */
#include "ThreadedLogging.hxx"
#include "Worker.hxx"
#include "extension.hxx"	 
#include "0cmdpp.hxx" 
#include "functions.hxx"
#include "Colors.hxx"
#include "DialogProcessor.hxx"
#include "ZoneNames.hxx"
#include "Vehicle.hxx"
#include "SampGDKHelper.hxx"
#include "LoginRegister.hxx"
#include "CommandPermissions.hxx"
#include "WorldData.hxx"
#include "MenuManager.hxx"
#include "Teleports.hxx"
#include "AntiCheat.hxx"
#include "AdminSystem.hxx"
#include "Mafia.hxx"
#include "VehicleExtraGetFunctions.hxx"
#include "PrivateCar.hxx"
#include "MiniGameSystem.hxx"
#include "MiniGameRacingSystem.hxx"
#include "VehicleCalculationFunctions.hxx"
#include "CommandParamsParser.hxx"
#include "WeaponInfo.hxx"
#include "AchievementsManagers.hxx"
#include "MessageManager.hxx"
#include "Odliczanie.hxx"
#include "SpeedoMeter.hxx"
#include "PlayerTeleport.hxx"
#include "email.hxx"
#include "GoldCoinsAchievements.hxx"
#include "PlayerStatsTextDraw.hxx"
#include "TutorialSystem.hxx"
#include "SentryGunSystem.hxx"
#include "HostBan.hxx"
#include "FilteringEngine.hxx"
#include "_GCVehicleManager.hxx"
#include "LanguageTextDraw.hxx"
#include "GateSystem.hxx"
#include "MafiaBase.hxx"

#include "GetPacket.hxx"
#include "BitStream.h"
#include "Structs.h"
#include "PacketEnumerations.h"
#include "_exe24_RemoteManager.hxx"

#define PlayerTextDrawCreate CreatePlayerTextDraw

namespace TextDraws
{
	extern int ActualTime;
	extern std::string ActualTimeText;
}

extern unsigned long long Exe24ModCurrentVersion;

extern std::set<boost::asio::ip::address_v4> IP_SUPER_WHITELIST;
extern std::string ConsoleTextName;

namespace ExecutionPriorities
{
	//execution order is from DOWN to UP
	enum EEP
	{//TO HERE
		NEGATIVE = -1,	   //     |
		DEFAULT = 0,	   //    |||
		GameMode,		   //   |||||
		WorldData,		   // |||||||||
		LoginRegister,	   //    |||
		AntiS0beit,		   //    |||
		AntiCheat,		   //    |||
		ThreadedLogging,   //    |||
		LottoSystem		   //    |||
	};//FROM HERE		   //    |||
};