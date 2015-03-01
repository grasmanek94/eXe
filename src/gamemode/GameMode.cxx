/*
	Copyright (c) 2014 - 2015 Rafał "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafał Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies
		Extension
		Message
		Database
	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoń
*/
//57 ms retrieve one acc with 1,000,000 accounts (single)
//8 ms save one acc with 1,000,000 accounts (batch)
//175 ms save one acc with 1,000,000 accounts (single)
//On RAM disk server everything 245 times faster

#include "GameMode.hxx"
#include "../auto_version.h"

auto MakeVersion = [&](unsigned long long year, unsigned long long month, unsigned long long day, unsigned long long hour, unsigned long long minute){ return minute + (hour * 100) + (day * 10000) + (month * 1000000) + (year * 100000000); };
unsigned long long Exe24ModCurrentVersion = MakeVersion(2015, 1, 13, 11, 27);

//IP's that can join the game even if (host)banned
std::set<boost::asio::ip::address_v4> IP_SUPER_WHITELIST =
{
	#ifndef _LOCALHOST_DEBUG
	boost::asio::ip::address_v4::from_string("127.0.0.1"),		//localhost
	//boost::asio::ip::address_v4::from_string("123.45.67.89"),	//another ip
	#include "ipwhitelist.inline"
	#endif
};

static const std::vector<char> charset = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
static const unsigned char PASSWD_CHARS = 31;
std::string rcon_password(PASSWD_CHARS, '0');

unsigned long long LowPrecisionTimerValue = Functions::GetTime();

namespace TextDraws
{
	int ActualTime;
	int WWW;
	std::string ActualTimeText;
}

void SetRandomPassword()
{
	for (size_t i = 0; i < PASSWD_CHARS; ++i)
	{
		rcon_password[i] = //charset[rand() % charset.size()];
			(rand() % 253) + 1;
	}
	SendRconCommand(("rcon_password " + rcon_password).c_str());
}

unsigned int GetMSB(register unsigned int x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return(x & ~(x >> 1));
}

static void SAMPGDK_CALL GamemodeUpdate(int timerid, void *param)
{
	static int LastHour = -1;
	static int LastSecond = -1;

	UniversalModeDate = boost::posix_time::second_clock::local_time();
	UniversalModeTime = UniversalModeDate.time_of_day();

	if (LastHour != UniversalModeTime.hours())
	{
		LastHour = UniversalModeTime.hours();
		SetWorldTime(LastHour);
	}

	if (LastSecond != UniversalModeTime.seconds())
	{
		LastSecond = UniversalModeTime.seconds();
		TextDraws::ActualTimeText = Functions::string_format("%02d:%02d:%02d", UniversalModeTime.hours(), UniversalModeTime.minutes(), UniversalModeTime.seconds());
		TextDrawSetString(TextDraws::ActualTime, TextDraws::ActualTimeText.c_str());
		SendRconCommand(("worldtime " + TextDraws::ActualTimeText).c_str());
		SetRandomPassword();
	}

	LowPrecisionTimerValue = Functions::GetTime();
}

static void SAMPGDK_CALL PlayerExpUpdate(int timerid, void* param)
{
	unsigned long long TimeNow = Functions::GetTime();
#ifdef _DEBUG___x
	unsigned long long TimeToGiveExp = TimeNow - 30*1000;
#else
	unsigned long long TimeToGiveExp = TimeNow - 30*60*1000;
#endif
	static double level;
	for (auto i : PlayersOnline)
	{
		if (Player[i].LastExpReceived < TimeToGiveExp)//longer than 30 mins ago
		{
			Player[i].LastExpReceived = TimeNow;
			GivePlayerAchievement(i, EAM_SepiknaExp, Player[i].GiveExperienceKill(i));
			level = Player[i].GetLevel();
			Player[i].GiveScore((long long)(4.0*log(log(level + 2.0))));
			Player[i].GiveMoney(((long long)level) * 1750);
			
			SendClientMessage(i, Color::COLOR_ERROR, L_timed_score_add);
		}
		#ifndef FAILSAFE_MODE
		Player[i].PlayerDisconnect(true);//save only
		#endif
	}
}

bool SendSpecialDeathMessage(int playerid, int iconid, int color, std::string killername)
{
	if (!IsPlayerNPC(gMySpecialNPCWhichFucksUp) || gMySpecialNPCWhichFucksUp == INVALID_PLAYER_ID)
	{
		SendDeathMessage(playerid, INVALID_PLAYER_ID, iconid);
		return false;
	}
	else
	{
		SetPlayerColor(gMySpecialNPCWhichFucksUp, color);
		SetPlayerName(gMySpecialNPCWhichFucksUp, killername.c_str());
		SendDeathMessage(playerid, gMySpecialNPCWhichFucksUp, iconid);
		return true;
	}
}

class _SkinSelection : public PlayerMenu::Function
{
public:
	void ItemSelected(int playerid, std::string item)
	{
		Player[playerid].RequestSkinSelection = true;
	}
} SkinSelection;

std::string ConsoleTextName;

const std::vector<int> WhiteFemales =
{
	31, 38, 39, 41, 53, 55, 56, 64, 75, 77, 85, 87, 
	88, 89, 90, 92, 93, 130, 138, 145, 151, 152, 157, 
	172, 178, 191, 192, 194, 196, 197, 198, 199, 201, 
	205, 231, 232, 237, 243, 246, 251, 257
};

const std::vector<int> BlackFemales = 
{
	9, 11, 13, 40, 63, 65, 76, 91, 139, 148, 190, 
	195, 207, 211, 215, 218, 219, 238, 244, 245, 256
};

const std::vector<int> OtherFemales =
{
	12, 10, 54, 69, 129, 131, 140, 141, 150, 169,
	193, 214, 216, 224, 225, 226, 233, 263, 298
};

const std::vector<int> WhiteMales =
{
	1, 2, 3, 23, 26, 27, 29, 32, 33, 34, 35, 37,
	45, 46, 50, 52, 72, 73, 78, 81, 94, 95, 96, 97,
	99, 101, 111, 112, 113, 119, 120, 121, 122, 124,
	125, 126, 127, 128, 132, 133, 135, 137, 146, 147,
	153, 154, 155, 158, 159, 160, 161, 162, 165, 167,
	170, 171, 177, 179, 181, 187, 188, 189, 200, 202,
	206, 209, 212, 213, 217, 220, 221, 222, 227, 228,
	230, 234, 235, 236, 239, 240, 241, 242, 247, 248,
	250, 252, 254, 255, 258, 259, 261, 264, 268, 290,
	291, 295
};

const std::vector<int> BlackMales =
{
	0, 5, 6, 7, 14, 15, 16, 17, 18, 19, 21, 22,
	24, 25, 28, 51, 66, 67, 79, 80, 82, 83, 84,
	86, 98, 102, 103, 104, 105, 106, 107, 134,
	136, 142, 143, 144, 149, 156, 166, 168, 176,
	180, 182, 183, 249, 260, 262, 269, 270, 271,
	293, 296, 297
};

const std::vector<int> OtherMales =
{
	20, 30, 36, 42, 43, 4, 8, 44, 47, 48, 49,
	57, 58, 59, 60, 62, 68, 100, 108, 109, 110,
	114, 115, 116, 117, 118, 123, 173, 174, 175,
	184, 185, 186, 203, 204, 208, 210, 223, 229,
	272, 273, 289, 292, 294, 299
};

const std::vector<int> PublicServicePeds =
{
	61, 70, 71, 163, 164, 253, 265, 266, 267, 274,
	275, 276, 277, 278, 279, 280, 281, 282, 283,
	284, 285, 286, 287, 288
};

std::array<int, 300> ClassIdToModel;

size_t AddVectorClasses(const std::vector<int>& vec)
{
	static std::set<int> AddedPeds;
	static size_t current_classid = 0;
	for (auto ped : vec)
	{
		safeAddPlayerClass(ped, 1125.71201, -2036.8372, 69.8805, 270.0, 0, 0, 0, 0, 0, 0);
		AddedPeds.insert(ped);
		ClassIdToModel[current_classid] = ped;
		++current_classid;
	}
	return AddedPeds.size();
}

class GameMode : public Extension::Base
{
public:
	GameMode() : Base(ExecutionPriorities::GameMode) {}
	PlayerMenu::ActionData Data;
	bool OnPlayerConnect(int playerid) override
	{
		Player[playerid].QuickMenu->AddItem("4. SKIN", Data, &SkinSelection);
		TextDrawShowForPlayer(playerid, TextDraws::ActualTime);
		TextDrawShowForPlayer(playerid, TextDraws::WWW);
		return true;
	}
	bool OnGameModeInit() override//jeden z wielu
	{
		gtLog(LOG_OTHER, "Server is starting");

		if (!userdb)
		{
#if defined(DATABASE_MYSQL)
			userdb = new database("odbodbodb","odbodbodb","odbodbodb","127.0.0.1",3306);
#elif defined(DATABASE_SQLITE)
			userdb = new database("users.sqlite3", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
#endif
		}
		if (userdb)
		{
			SetupDatabase();

			/*unsigned long long t1, t2;
			std::vector<std::string> Names;
			std::vector<user*> Users;
			Names.reserve(50000);
			for (size_t i = 0; i < 50000; ++i)
			{
				Names.push_back(Functions::string_format("XXXXXX%I64uXXXXXX", i));
				Users.push_back(new user(Names[i], "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));
			}
			t1 = Functions::GetTime();
			for (size_t i = 0; i < 50000; ++i)
			{
				odb::transaction t(userdb->begin());
				userdb->persist(Users[i]);
				t.commit();
			}
			t2 = Functions::GetTime();
			std::cout << (t2 - t1) << "ms" << std::endl;
			t1 = Functions::GetTime();
			for (size_t i = 0; i < 50000; ++i)
			{
				odb::transaction t(userdb->begin());
				userdb->load<user>(Names[i]);
				t.commit();
			}
			t2 = Functions::GetTime();
			std::cout << (t2 - t1) << "ms" << std::endl;*/

			//itak nie używamy narazie, jak się będzie używać to odkomentować
			//MapAndreas->Init(MAP_ANDREAS_MODE_FULL);

			ShowPlayerMarkers(1);
			UsePlayerPedAnims();
			ShowNameTags(1);
			EnableStuntBonusForAll(false);
			DisableInteriorEnterExits();

			SendRconCommand("gamemodetext PL POLSKi NL ENG ENGLiSH X DM");
			SendRconCommand("mapname PL POLSKi NL ENG ENGLiSH X DM");
#ifndef _LOCALHOST_DEBUG
			SendRconCommand("hostname [PL|NL|ENG] •Gamer_Z• eXtreme Party DM 24/7");
#else
			SendRconCommand("hostname [DBG][PL|NL|ENG] •Gamer_Z• eXtreme Party DM 24/7");
#endif
			SendRconCommand("sleep 1");
			SendRconCommand("lanmode 0");
			SendRconCommand("rcon 0");

			sampgdk::logprintf("    Copyright(c) 2014 - 2015 Rafał \"Gamer_Z\" Grasman  ");
			sampgdk::logprintf("        * See LICENSE.md in source directory for license");
			sampgdk::logprintf("        * Written by Rafał Grasman <grasmanek94@gmail.com>");
			sampgdk::logprintf("        * Development Start Month - Year: October - 2014");
			sampgdk::logprintf("        * This GameMode is open-source and available at:");
			sampgdk::logprintf("                      http://github.com/grasmanek94/eXe/");

			TextDraws::ActualTime = TextDrawCreate(547.000000, 24.000000, "00:00:00");
			TextDrawBackgroundColor(TextDraws::ActualTime, 255);
			TextDrawFont(TextDraws::ActualTime, 3);
			TextDrawLetterSize(TextDraws::ActualTime, 0.360000, 1.399999);
			TextDrawColor(TextDraws::ActualTime, -1);
			TextDrawSetOutline(TextDraws::ActualTime, 1);
			TextDrawSetProportional(TextDraws::ActualTime, 1);
			TextDrawSetSelectable(TextDraws::ActualTime, 0);

			TextDraws::WWW = TextDrawCreate(499.000000, 4.000000, "~b~~h~~h~www.exe24.info");
			TextDrawBackgroundColor(TextDraws::WWW, 255);
			TextDrawFont(TextDraws::WWW, 3);
			TextDrawLetterSize(TextDraws::WWW, 0.360000, 1.399999);
			TextDrawColor(TextDraws::WWW, -1);
			TextDrawSetOutline(TextDraws::WWW, 1);
			TextDrawSetProportional(TextDraws::WWW, 1);
			TextDrawSetSelectable(TextDraws::WWW, 0);

			sampgdk_SetTimerEx(500, true, GamemodeUpdate, nullptr, nullptr);

			sampgdk_SetTimerEx(5 * 60 * 1000, true, PlayerExpUpdate, nullptr, nullptr);
			
			ConnectNPC("MessageNPC", "npcidle");

			DatabaseWorker_Activate();
			std::string additional("");

			sampgdk::logprintf(" ");

			const int TFS_TO_GIT_REVISION = 547;

			#ifdef TFS_BUILD_VERSION
				#if TFS_BUILD_VERSION > 0
					additional.assign(Functions::string_format(" [TFSCS:%d]", 
						TFS_TO_GIT_REVISION + TFS_BUILD_VERSION
					));//tak bedziemy wiedziec jaka wersje debugować czasami
					SendRconCommand(("weburl " + StaticVersionDeterminator.GetWebUrl() + "/?" + std::to_string(TFS_TO_GIT_REVISION + TFS_BUILD_VERSION) + "|" + StaticVersionDeterminator.GetModeVersion()).c_str());

					sampgdk::logprintf(("        * Version: <TFS[" + std::to_string(TFS_TO_GIT_REVISION + TFS_BUILD_VERSION) + "]> <COMPILE DATE[" + StaticVersionDeterminator.GetModeVersion() + "]>").c_str());
				#else
					sampgdk::logprintf(("        * Version: " + StaticVersionDeterminator.GetModeVersion()).c_str());
					SendRconCommand(("weburl " + StaticVersionDeterminator.GetWebUrl()).c_str());
				#endif
			#else
				sampgdk::logprintf(("        * Version: " + StaticVersionDeterminator.GetModeVersion()).c_str());
				SendRconCommand(("weburl " + StaticVersionDeterminator.GetWebUrl()).c_str());
			#endif

			sampgdk::logprintf(" ");

			AddVectorClasses(PublicServicePeds);
			AddVectorClasses(WhiteMales);
			AddVectorClasses(BlackMales);
			AddVectorClasses(OtherMales);
			AddVectorClasses(OtherFemales);
			AddVectorClasses(BlackFemales);

			size_t added_total = 
			AddVectorClasses(WhiteFemales);

			std::cout << "Added " << added_total << " classes" << std::endl;

			ConsoleTextName = ("*Gamer_Z* eXtreme Party " + additional);
#ifdef WIN32
			SetConsoleTitle(ConsoleTextName.c_str());
#endif
		}
		return true;
	}
	bool OnRconCommand(std::string cmd) override
	{
		if (cmd.compare("cmds") == 0)
		{
			for (auto i : (*SAMPGDK::ZeroCMD::Internal::command_map))
			{
				std::cout << i.first << std::endl;
			}
			return true;
		}
		if (cmd.compare("kickall") == 0)
		{
			for (auto i : PlayersOnline)
			{
				Kick(i);
			}
			return true;
		}
		return false;
	}
	void SetupPlayerForClassSelection(int playerid)
	{
		SetPlayerCameraPos(playerid, 2635.6624, 2326.5103, 30.9272);
		fixSetPlayerPos(playerid, 2635.6624, 2332.5103, 30.9272);
		SetPlayerFacingAngle(playerid, 180.0);
		SetPlayerCameraLookAt(playerid, 2635.6624, 2332.5103, 30.9272, 0);
		switch (Functions::RandomGenerator->Random(0, 6))
		{
			case 0:ApplyAnimation(playerid, "DANCING", "DAN_LOOP_A", 4.1, 1, 1, 1, 1, 1, false); break;
			case 1:ApplyAnimation(playerid, "DANCING", "DNCE_M_A", 4.1, 1, 1, 1, 1, 1, false); break;
			case 2:ApplyAnimation(playerid, "DANCING", "DNCE_M_B", 4.1, 1, 1, 1, 1, 1, false); break;
			case 3:ApplyAnimation(playerid, "DANCING", "DNCE_M_C", 4.1, 1, 1, 1, 1, 1, false); break;
			case 4:ApplyAnimation(playerid, "DANCING", "DNCE_M_D", 4.1, 1, 1, 1, 1, 1, false); break;
			case 5:ApplyAnimation(playerid, "DANCING", "DNCE_M_E", 4.1, 1, 1, 1, 1, 1, false); break;
		}
	}
	bool OnPlayerRequestClass(int playerid, int classid) override
	{
		if (Player[playerid].HasSpawnedOnce)
		{
			Player[playerid].RequestSkinSelection = true;
		}
		if (
			(Player[playerid].RequestSkinSelection && !Player[playerid].InSkinSelection) || 
			Player[playerid].InSkinSelection && classid >= 0 && classid < 300)
		{
			Player[playerid].InSkinSelection = true;
			Player[playerid].RequestSkinSelection = false;
			SetupPlayerForClassSelection(playerid);
			if (TestIfAllowed(playerid))
			{
				Player[playerid].statistics.SkinModelID = ClassIdToModel[classid];
			}
			PlayerPlaySound(playerid, 1185, 1958.3783, 1343.1572, 15.3746);
		}
		return true;
	}
	bool OnPlayerRequestSpawn(int playerid) override
	{
		ClearAnimations(playerid, false);
		PlayerPlaySound(playerid, MUSIC_OFF, 0.0, 0.0, 0.0);
		return true;
	}
	bool OnPlayerSpawn(int playerid) override
	{
		Player[playerid].HasSpawnedOnce = true;
		if (!Player[playerid].KeepInGame)
		{
			SetPlayerColor(playerid, Player[playerid].FullColor);
			Player[playerid].SetTeam(NO_TEAM);
			fixSetPlayerHealth(playerid, 100.0f);
			fixSetPlayerArmour(playerid, 0.0f);
			Player[playerid].CurrentGameID = nullptr;
			SetPlayerRandomSpawn(playerid);
			Player[playerid].InSkinSelection = false;
		}
		return true;
	}
	bool OnPlayerDeath(int playerid, int killerid, int reason) override
	{
		if (Player[playerid].RequestSkinSelection && Player[playerid].CurrentGameID == nullptr)
		{
			Player[playerid].RequestSkinSelection = false;
			Player[playerid].InSkinSelection = true;
			ForceClassSelection(playerid);
		}
		return true;
	}
	bool OnPlayerText(int playerid, std::string text) override
	{
		if (!has_string_ip_ad(text))
		{
			Player[playerid].SendChat(text);
		}
		else
		{
			Player[playerid].SendChatIpAds(text);
		}
		return false;
	}
	bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) override
	{
		if (Player[playerid].CurrentVehicle && Player[playerid].CurrentGameID == nullptr)
		{
			if (newkeys == 1 || newkeys == 9 || newkeys == 33 && oldkeys != 1 || oldkeys != 9 || oldkeys != 33)
			{
				AddVehicleComponent(Player[playerid].CurrentVehicle, 1010);
			}
			if (PRESSED(KEY_SUBMISSION))
			{
				if (IsVehicleAir(Player[playerid].CurrentVehicleModel))//NOPE NOPE NOPE for aircraft
				{
					ZCMD_CALL_COMMAND(playerid, "/fix", "");//do fix instead
				}
				else
				{
					float a;
					GetVehicleZAngle(Player[playerid].CurrentVehicle, &a);
					SetVehicleZAngle(Player[playerid].CurrentVehicle, a);
					GivePlayerAchievement(playerid, EAM_LubieDachowac, 1);
				}
			}
		}
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		TextDrawHideForPlayer(playerid, TextDraws::ActualTime);
		return true;
	}
	void OnUnknownCommand(int playerid, std::string& command, std::string& params) override
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, TranslateP(playerid, L_unknown_command) + command);
	}
	void SetPlayerRandomSpawn(int playerid)
	{
		static const std::vector<std::vector<double>> gRandomPlayerSpawns =
		{			{ 835.9832, -2040.8696, 12.8746 },			{ 808.9097, -1640.6599, 13.2320 },			{ 1034.7283, -1572.5370, 13.1137 },			{ 1220.0891, -1541.7964, 13.2739 },			{ 1290.8767, -1525.5438, 13.2787 },			{ 1453.2981, -1440.7292, 13.1153 },			{ 1603.3427, -1440.4199, 13.1099 },			{ 1771.7617, -1448.3945, 13.1024 },			{ 1824.0323, -1364.3969, 14.9852 },			{ 1881.5808, -1380.3021, 13.2993 },			{ 2002.1196, -1445.0854, 13.2891 },			{ 2124.2432, -1460.1768, 23.5472 },			{ 2280.5190, -1742.0107, 13.2742 },			{ 2355.1726, -1716.8459, 13.2833 },			{ 2480.1873, -1675.9375, 13.0657 },			{ 2593.9573, -1611.3037, 19.3390 },			{ 2794.8374, -1826.8971, 9.5922 },			{ 2658.3149, -1802.6813, 9.0302 },			{ 2769.3926, -1953.3779, 13.0869 },			{ 2860.2893, -1949.9376, 10.6654 },			{ 2936.9783, -1434.6172, 10.5896 },			{ 2903.7603, -1162.7054, 10.8680 },			{ 2626.0745, -1150.1124, 51.3488 },			{ 2321.5945, -1152.4860, 26.5320 },			{ 2222.4155, -1145.1254, 25.5276 },			{ 2129.9531, -1102.7698, 25.0453 },			{ 1954.2205, -1019.1243, 33.3019 },			{ 1525.7822, -777.7990, 79.3773 },			{ 2223.6367, -1342.6824, 23.7109 },			{ 2154.5298, -1184.1504, 23.5543 },			{ 1783.4949, -1079.9431, 23.6975 },			{ 1465.6814, -1029.4684, 23.4661 },			{ 1168.4393, -1039.6223, 31.4683 },			{ 932.9001, -1102.7867, 24.0487 },			{ 595.1119, -1237.0991, 17.7156 },			{ 299.9835, -1394.2766, 13.6817 },			{ 162.4236, -1523.4275, 11.9031 },			{ -83.1952, -1586.1154, 2.3377 },			{ -28.1936, -2512.6289, 36.3192 },			{ 27.3586, -2636.3762, 40.1421 },			{ -1565.7466, -2749.9426, 48.2647 },			{ -1899.3795, -1732.3804, 21.4770 },			{ -2074.3101, -2288.2686, 30.2712 },			{ -2197.5828, -2498.8723, 30.3449 },			{ -2394.7595, -2203.7644, 33.0161 },			{ -1996.6060, -1530.9789, 84.2609 },			{ -1988.5687, -1005.6031, 31.7544 },			{ -1953.6749, -974.6401, 35.6180 },			{ -2508.9592, -614.2473, 132.2896 },			{ -2657.0054, -136.5914, 3.7736 },			{ -2707.0383, 349.3176, 4.1410 },			{ -2573.6052, 467.9834, 14.1880 },			{ -2567.5322, 621.3440, 14.1800 },			{ -2669.0618, 586.5140, 14.1806 },			{ -2260.2927, 569.0488, 34.7427 },			{ -1941.5566, 576.8063, 34.8989 },			{ -1915.2806, 276.1676, 40.7740 },			{ -2013.6412, 163.5000, 27.4113 },			{ -2046.8640, -85.0480, 34.8921 },			{ -1760.0094, -117.0390, 3.3095 },			{ -1688.2534, -537.0994, 13.8755 },			{ -1375.3220, -97.0212, 13.8755 },			{ -1078.2452, -628.4633, 50.2321 },			{ -1039.4380, -597.1907, 31.7349 },			{ -760.8032, -101.7740, 65.4889 },			{ -532.8148, -86.0667, 62.4951 },			{ -1588.1505, 653.4904, 6.9146 },			{ -1509.0715, 916.1671, 6.9151 },			{ -1635.1089, 1264.8854, 6.9052 },			{ -1749.3591, 1401.4553, 6.9146 },			{ -1935.5795, 1333.8914, 6.9156 },			{ -2629.1841, 1373.6503, 6.8235 },			{ -2671.7529, 1222.1407, 55.1569 },			{ -2742.1230, 1248.5093, 11.4928 },			{ -2613.0579, 2261.2288, 7.9361 },			{ -2498.9246, 2422.0564, 16.3247 },			{ -2255.3962, 2299.1296, 4.5419 },			{ -2273.4336, 2672.8301, 73.2427 },			{ -1487.4755, 1967.1013, 47.8201 },			{ -1342.5981, 2056.9915, 52.4999 },			{ -1496.5046, 2601.0544, 55.4141 },			{ -1407.1927, 2643.3774, 55.4146 },			{ -1303.1541, 2676.0491, 49.7894 },			{ -837.1122, 2747.9397, 45.5782 },			{ -630.3387, 2693.9792, 72.1021 },			{ -339.0306, 2765.2732, 93.0022 },			{ -253.9672, 2700.8645, 62.2639 },			{ -69.4298, 2485.0637, 16.2114 },			{ 402.0906, 2499.2285, 16.2114 },			{ 586.3649, 1639.0103, 6.7222 },			{ 713.1967, 1933.7886, 5.2664 },			{ 1326.0210, 2719.0166, 10.4719 },			{ 1614.8529, 2727.4019, 10.4807 },			{ 2040.2161, 2737.1509, 10.4720 },			{ 2219.7639, 2773.6199, 10.4458 },			{ 2381.0195, 2472.6990, 10.5474 },			{ 2287.4573, 2426.6123, 10.5474 },			{ 2109.1953, 2086.1748, 10.5473 },			{ 2030.9346, 1916.8591, 12.0577 },			{ 2121.6294, 1782.9945, 10.5434 },			{ 2463.2026, 1770.1571, 10.5475 },			{ 2581.8044, 1688.4564, 10.5474 },			{ 2491.9976, 1536.3737, 10.4043 },			{ 2522.8625, 1271.2175, 10.4711 },			{ 2546.9771, 766.5089, 10.5073 },			{ 1506.5858, 909.8279, 10.5510 },			{ 1487.6464, 1041.6090, 10.5474 },			{ 1347.2545, 1274.6627, 10.5473 },			{ 1098.6781, 1648.8695, 11.5852 },			{ 1060.7599, 1766.9026, 10.5474 },			{ 1085.6619, 1074.1145, 10.5623 },			{ 1495.6951, 725.1281, 10.5474 },			{ 1626.9703, 613.8116, 7.5083 },			{ 2003.8318, 1544.3654, 13.3152 },			{ 1610.0649, 1827.9717, 10.5474 },			{ 2855.8701, 2411.2073, 10.5474 },			{ 2168.0635, 997.7371, 10.5437 },			{ 2338.2305, 156.8099, 26.1349 },			{ 2285.8228, -36.1071, 26.2113 },
			{ 0.0f, 0.0f, 2.0f }
		};
		size_t rand = Functions::RandomGenerator->Random(0, gRandomPlayerSpawns.size() - 1);
		TeleportPlayer(playerid, gRandomPlayerSpawns[rand][0], gRandomPlayerSpawns[rand][1], gRandomPlayerSpawns[rand][2], 45.0, false, 0, 0, "", 0, 0, 2.0, 2.0);
	}
} _GameMode;