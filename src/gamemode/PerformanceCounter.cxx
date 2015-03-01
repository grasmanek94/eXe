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

namespace PerformanceCounter
{
	const size_t measure_ticks = 64;

	struct svr_fps_counter
	{
		std::chrono::high_resolution_clock::time_point TIMES[measure_ticks];

		void tick()
		{
			std::rotate(TIMES, TIMES + 1, TIMES + measure_ticks);
			TIMES[measure_ticks - 1] = std::chrono::high_resolution_clock::now();
		}

		unsigned long long GetAvgFPS()//executed once / ~1 sec
		{
			unsigned long long ticktime = std::chrono::duration_cast<std::chrono::nanoseconds>((
				(TIMES[63] - TIMES[62]) +
				(TIMES[61] - TIMES[60]) +
				(TIMES[59] - TIMES[58]) +
				(TIMES[57] - TIMES[56]) +
				(TIMES[55] - TIMES[54]) +
				(TIMES[53] - TIMES[52]) +
				(TIMES[51] - TIMES[50]) +
				(TIMES[49] - TIMES[48]) +
				(TIMES[47] - TIMES[46]) +
				(TIMES[45] - TIMES[44]) +
				(TIMES[43] - TIMES[42]) +
				(TIMES[41] - TIMES[40]) +
				(TIMES[39] - TIMES[38]) +
				(TIMES[37] - TIMES[36]) +
				(TIMES[35] - TIMES[34]) +
				(TIMES[33] - TIMES[32]) +
				(TIMES[31] - TIMES[30]) +
				(TIMES[29] - TIMES[28]) +
				(TIMES[27] - TIMES[26]) +
				(TIMES[25] - TIMES[24]) +
				(TIMES[23] - TIMES[22]) +
				(TIMES[21] - TIMES[20]) +
				(TIMES[19] - TIMES[18]) +
				(TIMES[17] - TIMES[16]) +
				(TIMES[15] - TIMES[14]) +
				(TIMES[13] - TIMES[12]) +
				(TIMES[11] - TIMES[10]) +
				(TIMES[9] - TIMES[8]) +
				(TIMES[7] - TIMES[6]) +
				(TIMES[5] - TIMES[4]) +
				(TIMES[3] - TIMES[2]) +
				(TIMES[1] - TIMES[0])
				) / measure_ticks).count();
			
			if (!ticktime)
				return 0;

			return 1000000000 / ticktime;
		}

		unsigned long long GetLastFPS()//executed once / ~1 sec
		{
			unsigned long long ticktime = std::chrono::duration_cast<std::chrono::nanoseconds>((
				(TIMES[15] - TIMES[14])
				) / 2).count();

			if (!ticktime)
				return 0;

			return 1000000000 / ticktime;
		}
	};

	svr_fps_counter COUNTER;
}

size_t SVR_PC_Global_AVG_FPS;

size_t GetServerAverageFPS()
{
	return SVR_PC_Global_AVG_FPS;
}

size_t GetServerLastFPS()
{
	return (size_t)PerformanceCounter::COUNTER.GetLastFPS();
}

int SVR_AVGLSTFPSTD = INVALID_TEXT_DRAW;
std::set<int> PlayersWatchingSvrFpsStats;

void PrintFPS(int timerid, void* param)
{
	SVR_PC_Global_AVG_FPS = PerformanceCounter::COUNTER.GetAvgFPS();
	std::string addition(" AFPS(" + std::to_string(SVR_PC_Global_AVG_FPS) + ") LFPS(" + std::to_string(PerformanceCounter::COUNTER.GetLastFPS()) + ")");
#ifdef WIN32
	SetConsoleTitle((ConsoleTextName + addition).c_str());
#endif
	if (PlayersWatchingSvrFpsStats.size())
		TextDrawSetString(SVR_AVGLSTFPSTD, addition.c_str());
}

typedef LONG NTSTATUS;

typedef NTSTATUS(NTAPI* pSetTimerResolution)(ULONG RequestedResolution, BOOLEAN Set, PULONG ActualResolution);
typedef NTSTATUS(NTAPI* pQueryTimerResolution)(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);

ZCMD3(svrfps, PERMISSION_GAMER, 0)
{
	if (PlayersWatchingSvrFpsStats.find(playerid) == PlayersWatchingSvrFpsStats.end())
	{
		PlayersWatchingSvrFpsStats.insert(playerid);
		TextDrawShowForPlayer(playerid, SVR_AVGLSTFPSTD);
	}
	else
	{
		PlayersWatchingSvrFpsStats.erase(playerid);
		TextDrawHideForPlayer(playerid, SVR_AVGLSTFPSTD);
	}
	return true;
}

class CPerformanceCounter : public Extension::Base
{
public:
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		PlayersWatchingSvrFpsStats.erase(playerid);
		return true;
	}
	bool OnGameModeInit()
	{
		#ifdef WIN32
		ULONG MIN;
		ULONG TEMP;
		ULONG TEMP2;

		pQueryTimerResolution QueryTimerResolution;
		pSetTimerResolution SetTimerResolution;

		const HINSTANCE hLibrary = LoadLibrary("NTDLL.dll");
		if (hLibrary == NULL)
		{
			printf("Failed to load NTDLL.dll (%d)\n", GetLastError());
		}
		else
		{
			QueryTimerResolution = (pQueryTimerResolution)GetProcAddress(hLibrary, "NtQueryTimerResolution");
			if (QueryTimerResolution == nullptr)
			{
				printf("NtQueryTimerResolution is null (%d)\n", GetLastError());
			}
			else
			{
				SetTimerResolution = (pSetTimerResolution)GetProcAddress(hLibrary, "NtSetTimerResolution");
				if (SetTimerResolution == nullptr)
				{
					printf("NtSetTimerResolution is null (%d)\n", GetLastError());
				}
				else
				{
					if (0 == QueryTimerResolution(&MIN, &TEMP, &TEMP2))
					{
						if (0 != SetTimerResolution(2000, TRUE, &TEMP))
						{
							printf("SetTimerResolution error (%d)\n", GetLastError());
						}
					}
					else
					{
						printf("QueryTimerResolution error (%d)\n", GetLastError());
					}
				}
			}
		}

		if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
			std::cout << "SetPriorityClass failed" << std::endl;
		if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL))
			std::cout << "SetThreadPriority failed" << std::endl;

		SendRconCommand("sleep 0");//we are doing our own custom sleep so make sure sa-mp does not sleep
		#endif

		sampgdk_SetTimerEx(1000, true, PrintFPS, nullptr, nullptr);
		
		SVR_AVGLSTFPSTD = TextDrawCreate(547.000000, 14.000000, "AFPS(0) LFPS(0)");
		TextDrawBackgroundColor(SVR_AVGLSTFPSTD, 255);
		TextDrawFont(SVR_AVGLSTFPSTD, 2);
		TextDrawLetterSize(SVR_AVGLSTFPSTD, 0.129999, 1.000000);
		TextDrawColor(SVR_AVGLSTFPSTD, -1);
		TextDrawSetOutline(SVR_AVGLSTFPSTD, 0);
		TextDrawSetProportional(SVR_AVGLSTFPSTD, 1);
		TextDrawSetShadow(SVR_AVGLSTFPSTD, 1);
		TextDrawSetSelectable(SVR_AVGLSTFPSTD, 0);

		return true;
	}
#ifdef _LOCALHOST_DEBUG
	DWORD lagger_emulator;
	CPerformanceCounter() : Base(-0x70000000) 
	{ 
		lagger_emulator = 0; 
	}
	bool OnRconCommand(std::string cmd)
	{
		switch (cmd[0])
		{
		case '1':
			lagger_emulator = 10;
			return true;
		case '2':
			lagger_emulator = 50;
			return true;
		case '3':
			lagger_emulator = 100;
			return true;
		case '4':
			lagger_emulator = 200;
			return true;
		case '5':
			lagger_emulator = 300;
			return true;
		case '6':
			lagger_emulator = 400;
			return true;
		case '7':
			lagger_emulator = 500;
			return true;
		case '8':
			lagger_emulator = 600;
			return true;
		case '9':
			lagger_emulator = 700;
			return true;
		case '0':
			lagger_emulator = 800;
			return true;
		case '-':
			lagger_emulator = 900;
			return true;
		case '=':
			lagger_emulator = 1000;
			return true;
		case '`':
			lagger_emulator = 0;
			return true;
		case '_':
			SendRconCommand("sleep 0");
			return true;
		case '+':
			SendRconCommand("sleep 1");
			return true;
		}
		return false;
	}
	void ProcessTick()
	{
		if (lagger_emulator)
		{
			Sleep(lagger_emulator);
		}
		PerformanceCounter::COUNTER.tick();
		#ifdef WIN32
		//if (PerformanceCounter::COUNTER.GetLastFPS() > 500)
		std::this_thread::sleep_for(std::chrono::microseconds(200));
		#endif
	}
#else
	CPerformanceCounter() : Base(-0x70000000) {}
	void ProcessTick()
	{
		PerformanceCounter::COUNTER.tick();
		#ifdef WIN32
		std::this_thread::sleep_for(std::chrono::microseconds(200));
		#endif
	}
#endif
}
_PerformanceCounter
;