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
#include "_exe24_RemoteManager.hxx"
#include "MenuManager.hxx"
#include "PrivateCar.hxx"
#include "MiniGameSystem.hxx"
#include "MiniGameRacingSystem.hxx"
#include "Mafia.hxx"

extern std::set<int> PlayersOnline;
extern std::set<int> VipsOnline;
extern std::set<int> SponsorsOnline;
extern std::set<int> ModeratorsOnline;
extern std::set<int> AdminsOnline;
extern std::set<int> ViceHeadAdminsOnline;
extern std::set<int> HeadAdminsOnline;
extern std::set<int> StaffOnline;

extern std::map<std::string, int, ci_less> PlayerNameToID;
extern std::array<int, MAX_VEHICLES + 1> VehicleDriver;
extern std::array<int, MAX_VEHICLES + 1> VehicleSpawnedBy;
extern boost::posix_time::time_duration UniversalModeTime;
extern boost::posix_time::ptime UniversalModeDate;
extern int gMySpecialNPCWhichFucksUp;
extern std::map<boost::asio::ip::address_v4, std::set<int>> IPtoPlayers;
extern std::map<unsigned int, std::set<int>> TeamPlayers;
extern std::vector<std::string> GlobalChatHistory;

struct Position
{
	float x;
	float y;
	float z;
};

struct BulletShootCounter
{
	std::array<unsigned long long, 4> bsc_ull;

	unsigned long long total_count()
	{
		return bsc_ull[0] + bsc_ull[1] + bsc_ull[2] + bsc_ull[3];
	}

	void push_new(unsigned long long newval)
	{
		bsc_ull[0] = bsc_ull[1];
		bsc_ull[1] = bsc_ull[2];
		bsc_ull[2] = bsc_ull[3];
		//std::rotate(bsc_ull, bsc_ull + 1, bsc_ull + 4);
		bsc_ull[3] = newval;
	}

	void reset()
	{
		bsc_ull[0] = 0;
		bsc_ull[1] = 0;
		bsc_ull[2] = 0;
		bsc_ull[3] = 0;
	}
};

enum eStatsTextDrawInfo
{
	eSTDI_Main,
	eSTDI_Respect,
	eSTDI_Exp,
	eSTDI_SIZE
};

struct SPlayer
{
	int Color;
	int WorldID;
	int sp_PlayerID;
	int FullColor;

	std::array<int, eSTDI_SIZE> StatsTextDraw;

	int CurrentVehicleModel;
	//if you want the vehicle the player uses currently THIS IS NOT THE RIGHT VARIABLE (CurrentVehicle)
	int PlayerVehicle;
	//if you want the vehicle spawned by the player THIS IS NOT THE RIGHT VARIABLE (PlayerVehicle)
	int CurrentVehicle;
	int CurrentRamp;
	int CurrentTutorial;
	int CurrentTutorialID;
	//int CurrentWeaponID;
	int CurrentPendingOnWeaponShot;
	unsigned long long LastShotTime;
	BulletShootCounter DiffShotTimes;
	unsigned int Team;
	bool ShownConnectMessage;
	bool IsDriver;
	bool IsPassenger;
	//std::string IP;
	std::string PlayerName;
	std::string TemporaryRecoveryCode;
	bool IsS0beitDetected;
	bool BanImmunity;
	bool IsRegistered;
	bool IsLoggedIn;
	bool Connected;
	bool DataLoaded;
	bool DoFirstSpawn;
	CMafia* Mafia;
	std::set<std::string, ci_less > InviteMafia;
	unsigned char MafiaPermissions;
	//unsigned int intip;
	unsigned long long ChatMute;
	unsigned long long CommandMute;
	unsigned long long LastDamageTime;
	Zabawy::Base* CurrentGameID;
	Zabawy::Base* StagingForGameID;
	bool LanguageChosen;
	unsigned long Language;
	unsigned long LanguageID;

	bool Exe24ModEnabled;
	unsigned long long Exe24ModVersion;
	Exe24Tool::CRemoteOptions Exe24ModOptions;
	PlayerID RakNetID;

	boost::asio::ip::address ip;
	boost::asio::ip::address_v4 ipv4;

	bool InSkinSelection;
	bool Spawned;
	bool AquiredData;
	bool RequestSkinSelection;
	bool KeepInGame;
	bool HasSpawnedOnce;
	bool AlreadyKicked;
	unsigned long long StartPlayTime;
	unsigned long long LastExpReceived;

	typedef std::unique_ptr<PlayerMenu::Display> PlayerMenuPointer;
	PlayerMenuPointer QuickMenu;

	std::vector<std::string> ChatHistory;
	std::vector<std::string> MessageHistory;
	std::vector<std::string> CommandHistory;

	void InsertMessageHistory(std::string message);
	void InsertChatHistory(std::string message);
	void InsertCommandHistory(std::string cmdtext);

	long CurrentInactivityTime;

	struct WeaponStat
	{
		unsigned int Ammo;
		unsigned int Owned;
	};
	struct WeaponData
	{
		std::array<WeaponStat, 48> weapon;
		std::array<unsigned int, 14> equipped;
	};
	WeaponData weapondata;
	std::set<int> OwnedWeapons;

	std::set<int> HasAlreadyAchievement;

	RaceSystem::SPlayerRaceData RaceData;
	//just to be sure
	//so bitset doesn't
	//fuck up everything
	__declspec(align(1)) 
	struct SAchievements//keep this 4096 bytes or fuck you. this is also used to store USER SETTINGS! Not only achievments
	{
		std::array<unsigned int,		48>		WeaponKills;
		std::array<unsigned int,		48>		WeaponDeaths;
		std::array<unsigned int,		48>		SameWeaponKills;
		std::array<unsigned int,		48>		SameWeaponDeaths;
		std::array<unsigned long long,	48>		WeaponExperience;
		std::array<unsigned int,		48>		WeaponMaxKillStreak;
		std::array<unsigned int,		48>		CurrentWeaponKillstreak;
		std::bitset<					256>	GoldCoins;
		std::array<int,					14>		TutorialState;
		int										ShowIpOnConnect;
		int										SecretPersonelPrivilidges;
		int										IsInformationArrowToggled;
		unsigned long							CurrentlySelectedLanguage;
		unsigned char							DidPlayerSelectLanguage;
		std::array<char,				407>	keepthis4096bytes;
		std::array<unsigned long long,	256>	AchievementsMain;
	};

	//keep this 4096 bytes or fuck you. this is also used to store USER SETTINGS! Not only achievments
	SAchievements Achievementdata;

	std::array<SPrivateCar, 10> PrywatnePojazdy;

	user userdata;

	struct stats
	{
		long long money;
		long long respect;
		unsigned long long kills;
		unsigned long long experience;
		unsigned long long deaths;
		unsigned long long privilidges;
		unsigned long long bounty;
		unsigned long long suicides;
		unsigned long long kicks;
		unsigned long long warns;
		unsigned long long playtime;
		unsigned long long banned;
		unsigned long long jailtime;
		unsigned long long registertime;
		std::string ban_reason;
		std::string ban_who;
		int SkinModelID;
		unsigned long long Premium;
		unsigned long long lastconnected;
		std::string MafiaName;
		bool UseMafiaSkin;
		bool UseMafiaSpawn;
		int rampid;
		float SaveX;
		float SaveY;
		float SaveZ;
		float SaveA;
		int staticcolor;
		long long bank;
		std::string registerip;
		std::string lastip;
		std::string recoverycode;
		std::string email;
		std::string verifiedmail;
		unsigned long long mailverifiedtime;
		unsigned long long verificationsent;
		unsigned long long lastmailsent;
		std::string verifiedrecoverycode;

		std::string password;
		std::string nickname;
		std::string displayname;

		unsigned long bannedip;
		unsigned long long bannedidentificator;

		void Reset();
	};

	stats statistics;

	void SetMoney(long long money);
	void GiveMoney(long long money);
	void SetScore(long long score);
	void GiveScore(long long score);
	void CheckNewLevel(unsigned long long Old, unsigned long long New);
	void GiveExperience(unsigned long long exp);

	#pragma warning(disable: 4018)

	void GiveExperience(long long exp);
	unsigned long long GiveExperienceKill(int killedid);
	unsigned long long GetExperience();
	unsigned long long GetExperience(double level);
	double GetLevel(double dexp = -1.0);
	double GetPercentToNextLevel(double dexp = -1.0);
	void UpdateFullColor();
	void PlayerConnect();
	void LoadStats(user * userpointer, bool registered = false, bool forcedata = false);
	void TransferStats();
	void PlayerDisconnect(bool saveonly = false, bool emulate_logged_in = false);
	void SendChat(std::string text);
	void SendChatIpAds(std::string text);
	void StateChange(int newstate, int oldstate);
	void CleanupPlayerVehicleNicely();

	bool SetTeam(unsigned int team);
	unsigned int GetTeam();

	SPlayer();
};

extern std::array<SPlayer, MAX_PLAYERS> Player;

int ValidPlayerID(int playerid);
int ValidPlayerID(std::string idornickname);
int ValidPlayerName(std::string name);

unsigned long long SafeAddULL(unsigned long long& value, unsigned long long toadd);
unsigned long long SafeSubULL(unsigned long long& value, unsigned long long tosubstract);