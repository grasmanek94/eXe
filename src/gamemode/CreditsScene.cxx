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

namespace CreditScene
{
	std::array<int, MAX_PLAYERS> TimerID;
	std::array<int, MAX_PLAYERS> ObjectID;

	static const size_t max_strings = 7;
	static const int CurrentVW = Zabawy::ZabawyBaseVirtualWorld - 1;
	static const std::array<language_string_ids, max_strings> info = 
	{ 
		L_credits_scene_1,
		L_credits_scene_2,
		L_credits_scene_3,
		L_credits_scene_4,
		L_credits_scene_5,
		L_credits_scene_6,
		L_credits_scene_7
	};
	std::array<int, max_strings> creditsobjects;

	Zabawy::SPos from = { 1595.5342f, -1329.4470f, 40.0f };
	Zabawy::SPos to = { 1588.5477f, -1339.6587f, 40.0f };

	void* timer_owner;

	void DoKillTimer(int playerid)
	{
		if (TimerID[playerid] != INVALID_TIMER_ID)
		{
			sampgdk_KillTimerEx(TimerID[playerid], timer_owner);
			TimerID[playerid] = INVALID_TIMER_ID;
			fixSetPlayerVirtualWorld(playerid, 0);
			StreamerLibrary::DestroyDynamicObject(ObjectID[playerid]);
			StopAudioStreamForPlayer(playerid);
		}
	}

	class CCreditScene : public Extension::Base
	{
	public:
		CCreditScene() {}
		bool OnGameModeInit() override
		{
			timer_owner = this;
			for (auto&i : TimerID)
			{
				i = INVALID_TIMER_ID;
			}
			//19353
			//2790
			//4238
			//4239
			//4729
			//4730
			//4731
			//7903
			//7909 perfect?
			//8323 perfecter?
			//18244 small

			static const int ObjectModelID = 7909;
			for (unsigned long l = 0; l < CURRENT_LANGUAGE_ID; ++l)
			{
				creditsobjects[0] = StreamerLibrary::CreateDynamicObject(ObjectModelID, to.x, to.y, to.z + (5.0f * (float)(max_strings - (1))), 0.0f, 0.0f, 135.0, CurrentVW - l);
				StreamerLibrary::SetDynamicObjectMaterialText(creditsobjects[0], 0, TranslateF(l, info[0], StaticVersionDeterminator.GetModeLastUpdateTime().c_str()), OBJECT_MATERIAL_SIZE_512x256, "Arial Black", 28, 0, 0xFFFF8200, 0x00000000, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
				for (int i = 1; i < max_strings; ++i)
				{
					creditsobjects[i] = StreamerLibrary::CreateDynamicObject(ObjectModelID, to.x, to.y, to.z + (5.0f * (float)(max_strings - (i + 1))), 0.0f, 0.0f, 135.0, CurrentVW - l);
					StreamerLibrary::SetDynamicObjectMaterialText(creditsobjects[i], 0, Translate(l, info[i]), OBJECT_MATERIAL_SIZE_512x256, "Arial Black", 28, 0, 0xFFFF8200, 0x00000000, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
				}
			}
			return true;
		}
		bool OnPlayerDisconnect(int playerid, int reason) override
		{
			DoKillTimer(playerid);
			return true;
		}
		bool OnPlayerDeath(int playerid, int killerid, int reason) override
		{
			DoKillTimer(playerid);
			return true;
		}
		bool OnPlayerSpawn(int playerid) override
		{
			DoKillTimer(playerid);
			return true;
		}
	} _CreditScene;

	void SetPlayerCameraBack(int timerid, void* param)
	{
		DoKillTimer((int)param);
		fixTogglePlayerSpectating((int)param, false);
		SetCameraBehindPlayer((int)param);
	}

	void StartSequence(int timerid, void* param)
	{
		int playerid = (int)param;
		fixTogglePlayerSpectating(playerid, true);	

		SetPlayerCameraPos(playerid, from.x, from.y, from.z + (5.0f*((float)(max_strings + 1))));
		SetPlayerCameraLookAt(playerid, to.x, to.y, to.z + (5.0f*((float)(max_strings + 1))), CAMERA_CUT);
		InterpolateCameraPos(playerid, from.x, from.y, from.z + (5.0f*((float)(max_strings + 1))), from.x, from.y, from.z, 60000, CAMERA_MOVE);
		TimerID[playerid] = sampgdk_SetTimerEx(60000, false, SetPlayerCameraBack, (void*)playerid, timer_owner);
	}

	std::string GetRandomMusic()
	{
		static const size_t elements = 3;
		static const std::string tracks[elements] =
		{ 
			"https://docs.google.com/uc?export=download&id=0BzUKBMbvP0EFOGZxREd4SDZJOUU&ext=.mp3", 
			"https://docs.google.com/uc?export=download&id=0BzUKBMbvP0EFOGF0bGtVMTI5alE&ext=.mp3", 
			"https://docs.google.com/uc?export=download&id=0BzUKBMbvP0EFZnpnLXJkMDlDTlE&ext=.mp3" 
		};

		return tracks[Functions::RandomGenerator->Random(0, elements)];
	}

	ZCMD(livecredits, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/napisy", "/lvcr" }))
	{
		if (TimerID[playerid] != INVALID_TIMER_ID)
		{
			return true;
		}
		ObjectID[playerid] = StreamerLibrary::CreateDynamicObject(1318, from.x, from.y, from.z + (5.0f*((float)(max_strings + 1))), 0.0, 0.0, 0.0, CurrentVW - Player[playerid].Language, -1, playerid);
		TeleportPlayer(playerid, from.x, from.y, from.z + (5.0f*((float)(max_strings - 1))), 0.0, false, 0, CurrentVW - Player[playerid].Language, "", 0, 0, 0.0, 0.0, false, true);
		PlayAudioStreamForPlayer(playerid, GetRandomMusic().c_str(), 0.0, 0.0, 0.0, 0.0, false);
		TimerID[playerid] = sampgdk_SetTimerEx(250, false, StartSequence, (void*)playerid, timer_owner);
		return true;
	}
};