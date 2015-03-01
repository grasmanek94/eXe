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

namespace Zabawy
{
	std::vector<Base*> * Games;
	std::map<std::string, Base*, ci_less> * CmdToGame;
	std::set<int>* GameVirtualWorlds;
	std::map<int/*timerid*/, ZabawyTimerInternalResolver> ZabawyTimers;
	std::map<int/*worldid*/, Base* /*gameid*/>* WorldToGameResolver;
	int GlobalGamesTextDraw;

	bool IsPlayerStaging(int playerid)
	{
		return Player[playerid].StagingForGameID != nullptr;
	}

	bool PlayerStageCheckWithMessage(int playerid, unsigned char gameid)
	{
		if (IsPlayerStaging(playerid) && Player[playerid].StagingForGameID->CurrentGameID != gameid)
		{
			SendClientMessage(playerid, Color::COLOR_INFO2, L_games_stagecheck_msg);
			return false;
		}
		return true;
	}

	unsigned char Base::GetDisplayColor()
	{
		if (!Joinable())
		{
			return 'r';
		}

		if (Staging())
		{
			return 'b';
		}

		if (TYPE == eST_Zabawa)
		{
			return 'y';
		}

		if (TYPE == eST_Race)
		{
			return 'p';
		}

		return 'g';
	}

	std::string GameTextDrawString;
	bool need_update_td = true;

	void NeedUpdate()
	{
		need_update_td = true;
	}

	void UpdateGamesListTextDraw(int timerid, void * param)
	{
		if (need_update_td)
		{
			GameTextDrawString.clear();
			for (auto &game : *Games)
			{
				GameTextDrawString.append(Functions::string_format("~%c~~h~" + game->CurrentGameCommandName + "~w~(%02d)", game->GetDisplayColor(), game->PlayersInGame.size()));
				if (game->IsExe24PlusGame())
				{
					GameTextDrawString.append("~r~]");
				}
				GameTextDrawString.append("~n~");
			}
			TextDrawSetString(GlobalGamesTextDraw, GameTextDrawString.c_str());
			need_update_td = false;
		}
	}

	std::string GamesList[LANGUAGES_SIZE];
	std::string GamesFullList[LANGUAGES_SIZE];

	bool GamePrioritizer(Base *&i, Base *&j)
	{
		if (i->TYPE == j->TYPE)
		{
			return _stricmp(i->CurrentGameCommandName.c_str(), j->CurrentGameCommandName.c_str()) < 0;
		}
		return i->TYPE < j->TYPE;
	}

	class ZabawyProcessor : public Extension::Base
	{
	public:
		void Load() override
		{
			std::sort(Games->begin(), Games->end(), GamePrioritizer);
			for (auto &game : *Games)
			{
				game->PostInitOnLoad();
			}
		}

		ZabawyProcessor() : Extension::Base(-1) {}

		void OnPlayerGameBegin(int playerid) override
		{
			TextDrawShowForPlayer(playerid, GlobalGamesTextDraw);
		}
		bool OnPlayerConnect(int playerid) override
		{
			for (auto&i : *Games)
			{
				i->OnPlayerConnect(playerid);
			}
			return true;
		}
		bool OnPlayerDisconnect(int playerid, int reason) override
		{
			if (Player[playerid].StagingForGameID != nullptr)
			{
				Player[playerid].StagingForGameID->PlayerRequestCencelStaging(playerid, Zabawy::PlayerRequestExitTypeDisconnect);
			}
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Player[playerid].CurrentGameID->PlayerRequestGameExit(playerid, Zabawy::PlayerRequestExitTypeDisconnect);
			}
			return true;
		}
		bool OnPlayerDeath(int playerid, int killerid, int reason) override
		{
			if (Player[playerid].StagingForGameID != nullptr)
			{
				Player[playerid].StagingForGameID->PlayerRequestCencelStaging(playerid, Zabawy::PlayerRequestExitTypeDeath);
			}
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Zabawy::Base* gameid = Player[playerid].CurrentGameID;
				gameid->OnPlayerDeath(playerid, killerid, reason);
				gameid->PlayerRequestGameExit(playerid, Zabawy::PlayerRequestExitTypeDeath);
			}
			return true;
		}
		bool OnPlayerCommandReceived(int playerid, std::string& command, std::string& params, int min_execute_level, int this_restrictions) override
		{
			auto pGame = CmdToGame->find(command);
			if (pGame != CmdToGame->end())
			{
				if (CheckCommandAllowed(playerid, pGame->second->GetCommandRestrictions()))
				{
					if (PlayerStageCheckWithMessage(playerid, pGame->second->CurrentGameID))
					{
						if (pGame->second->Joinable())
						{
							if (pGame->second->OnGameCommandExecute(playerid, params))
							{
								GivePlayerAchievement(playerid, EAM_ZABAWA, 1);
								extOnPlayerEnterMiniGame(playerid, pGame->second);
								SendClientMessage(playerid, Color::COLOR_INFO2, L_games_exit_info);
							}
						}
					}
				}
				return false;
			}
			return true;
		}
		void ProcessTick() override
		{
			for (auto&i : *Games)
			{
				i->ProcessTick();
			}
		}
		bool OnGameModeInit() override
		{
			GlobalGamesTextDraw = TextDrawCreate(600.000000, 125.000000, " ");
			TextDrawBackgroundColor(GlobalGamesTextDraw, 255);
			TextDrawFont(GlobalGamesTextDraw, 2);
			TextDrawLetterSize(GlobalGamesTextDraw, 0.180000, 1.000000);
			TextDrawColor(GlobalGamesTextDraw, -1);
			TextDrawSetOutline(GlobalGamesTextDraw, 0);
			TextDrawSetProportional(GlobalGamesTextDraw, 1);
			TextDrawSetShadow(GlobalGamesTextDraw, 1);
			TextDrawSetSelectable(GlobalGamesTextDraw, 0);

			sampgdk_SetTimerEx(1000, true, UpdateGamesListTextDraw, nullptr, this);

			for (auto&i : *Games)
			{
				i->OnGameModeInit();
			}

			//init game list once
			if (!GamesList[0].size())
			{
				for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
				{
					for (auto &game : *Games)
					{
						GamesList[i].append("\t• " + Translate(i, game->CurrentGameName) + " - " + game->CurrentGameCommandName + "\n");
					}

					GamesFullList[i].assign(Functions::string_format(Translate(i, L_games_all_list_stub), GamesList[i].c_str()));
				}
			}
			return true;
		}

		bool OnPlayerSpawn(int playerid) override
		{
			if (Player[playerid].CurrentGameID != nullptr)
			{
				if (Player[playerid].Mafia != nullptr)
				{
					Player[playerid].Mafia->RemoveLabel(playerid);
				}
				if (Player[playerid].KeepInGame)
				{
					Player[playerid].CurrentGameID->OnKeepInGameSpawn(playerid);
				}
			}
			return true;
		}
		bool OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float x, float y, float z) override
		{
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Player[playerid].CurrentGameID->OnPlayerShootDynamicObject(playerid, weaponid, objectid, x, y, z);
			}
			return true;
		}
		bool OnPlayerLeaveDynamicArea(int playerid, int areaid) override
		{
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Player[playerid].CurrentGameID->OnPlayerLeaveDynamicArea(playerid, areaid);
			}
			return true;
		}
		bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override
		{
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Player[playerid].CurrentGameID->OnPlayerStateChange(playerid, newstate, oldstate);
			}
			return true;
		}
		bool OnDynamicObjectMoved(int objectid)  override
		{ 
			int worldid = StreamerLibrary::GetDynamicObjectVirtualWorld(objectid);
			if (GameVirtualWorlds->find(worldid) != GameVirtualWorlds->end())
			{
				WorldToGameResolver->at(worldid)->OnDynamicObjectMoved(objectid);
			}
			return true; 
		}
		bool OnPlayerPickUpDynamicPickup(int playerid, int pickupid) override
		{
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Player[playerid].CurrentGameID->OnPlayerPickUpDynamicPickup(playerid, pickupid);
			}
			return true;
		}
		bool OnPlayerEnterRaceCheckpoint(int playerid) override
		{ 
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Player[playerid].CurrentGameID->OnPlayerEnterRaceCheckpoint(playerid);
			}
			return true; 
		}
		bool OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart) override
		{
			if (issuerid != INVALID_PLAYER_ID && Player[playerid].CurrentGameID != nullptr)
			{
				if (Player[playerid].CurrentGameID->HeadShotsEnabled && bodypart == 9)
				{
					if (Player[playerid].CurrentGameID->enableallweaponheadshot ||
						Player[playerid].CurrentGameID->HeadShotWeapons.find(weaponid) != Player[playerid].CurrentGameID->HeadShotWeapons.end())
					{
						fixSetPlayerHealth(playerid, 0.0f);
					}
				}
				if (Player[playerid].CurrentGameID->OneHitOneKill)
				{
					fixSetPlayerHealth(playerid, 0.0f);
				}
			}
			return true;
		}
		bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)  override
		{ 
			if (Player[playerid].CurrentGameID != nullptr)
			{
				Player[playerid].CurrentGameID->OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
			}
			return true; 
		}
	} _ZabawyProcessor;

	ZCMD(exit, 0, RESTRICTION_NONE, cmd_alias({ "/wyjdz", "/verlaat" }))
	{
		if (Player[playerid].StagingForGameID != nullptr)
		{
			Player[playerid].StagingForGameID->PlayerRequestCencelStaging(playerid, Zabawy::PlayerRequestExitTypeExit);
			SendClientMessage(playerid, Color::COLOR_INFO2, L_games_exited);
		}
		if (Player[playerid].CurrentGameID != nullptr)
		{
			Player[playerid].CurrentGameID->PlayerRequestGameExit(playerid, Zabawy::PlayerRequestExitTypeExit);
			SendClientMessage(playerid, Color::COLOR_INFO2, L_games_exited);
		}
		return true;
	}

	ZCMD(games, 0, 0, cmd_alias({ "/zabawy", "/zabawypomoc", "/zhelp", "/areny", "/arena" }))
	{
		ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_games_help), GamesFullList[Player[playerid].Language], "V", "X");
		return true;
	}

	Base::Base(language_string_ids CurrentGameName_, std::string CurrentGameCommandName_, eSystemTypes type, cmd_alias aliasses)
	{
		if (Games == nullptr)
		{
			Games = new std::vector < Base* > ;
		}

		if (CmdToGame == nullptr)
		{
			CmdToGame = new std::map < std::string, Base*, ci_less > ;
		}

		if (GameVirtualWorlds == nullptr)
		{
			GameVirtualWorlds = new std::set < int > ;
		}

		if (WorldToGameResolver == nullptr)
		{
			WorldToGameResolver = new std::map < int/*worldid*/, Base* /*gameid*/ > ;
		}

		CurrentGameName = CurrentGameName_;
		CurrentGameCommandName = CurrentGameCommandName_;
		TYPE = type;

		SAMPGDK::ZeroCMD::Internal::register_command_fake(CurrentGameCommandName_);
		CmdToGame->insert(std::pair<std::string, Base*>(CurrentGameCommandName, this));

		for (auto alias : aliasses)
		{
			SAMPGDK::ZeroCMD::Internal::register_command_fake(alias);
			CmdToGame->insert(std::pair<std::string, Base*>(alias, this));
		}	

		Games->push_back(this);
	}

	void Base::PostInitOnLoad()
	{
		for (size_t i = 0; i < Games->size(); ++i)
		{
			if (Games->at(i) == this)
			{
				CurrentGameID = i;
				CurrentGameVW = ZabawyBaseVirtualWorld + CurrentGameID;
				GameVirtualWorlds->insert(CurrentGameVW);
				WorldToGameResolver->insert(std::pair<int, Base* >(CurrentGameVW, this));
			}
		}
	}

	void Base::AddPlayer(int playerid, bool keep_in_game_after_death, int newcolor)
	{
		NeedUpdate();

		fixSetPlayerVirtualWorld(playerid, CurrentGameVW);
		Player[playerid].CurrentGameID = this;
		Player[playerid].KeepInGame = keep_in_game_after_death;

		if (newcolor)
		{
			SetPlayerColor(playerid, newcolor);
		}

		PlayersInGame.insert(playerid);
	}
	
	void Base::RemoveAllPlayers(bool remove_only_staging)
	{
		if (PlayersInGame.size())
		{
			NeedUpdate();
			while (PlayersInGame.size())
			{
				Player[*PlayersInGame.begin()].StagingForGameID = nullptr;
				if (!remove_only_staging)
				{
					fixSetPlayerVirtualWorld(*PlayersInGame.begin(), 0);
					Player[*PlayersInGame.begin()].CurrentGameID = nullptr;
					PlayersInGame.erase(PlayersInGame.begin());
				}
			}
		}
	}

	void Base::PlayerExitGame(int playerid)
	{	
		auto foundplayer = PlayersInGame.find(playerid);
		if (foundplayer != PlayersInGame.end())
		{
			PlayersInGame.erase(foundplayer);
			NeedUpdate();

			if (Player[playerid].StagingForGameID == this)
			{
				Player[playerid].StagingForGameID = nullptr;
			}

			if (Player[playerid].CurrentGameID == this)
			{
				Player[playerid].CurrentGameID = nullptr;
			}

			Player[playerid].KeepInGame = false;

			if (Player[playerid].WorldID == CurrentGameVW)
			{
				fixSetPlayerVirtualWorld(playerid, 0);

				if (TYPE == eSystemTypes::eST_Race)
				{
					DisablePlayerRaceCheckpoint(playerid);
				}

				Player[playerid].CleanupPlayerVehicleNicely();
				fixTogglePlayerControllable(playerid, true);
				SpawnPlayer(playerid);		
			}
		}
	}

	void Base::AddStagingPlayer(int playerid, int newcolor)
	{
		if (Player[playerid].StagingForGameID == this)
		{
			Player[playerid].StagingForGameID = nullptr;
			fixSetPlayerVirtualWorld(playerid, CurrentGameVW);
			Player[playerid].CurrentGameID = this;
			if (newcolor)
			{
				SetPlayerColor(playerid, newcolor);
			}
		}
	}

	void Base::SetPlayerStaging(int playerid)
	{
		NeedUpdate();
		PlayersInGame.insert(playerid);
		Player[playerid].StagingForGameID = this;
		Player[playerid].KeepInGame = false;
	}

	bool timerdestroyprotect = false;

	void ZabawyBaseTimerLauncher(int timerid, void * param)
	{
		auto element = ZabawyTimers.find(timerid);
		if (element != ZabawyTimers.end())//should always happen but just to be sure, we're dealing with pointerz here(!)
		{
			element->second.function(timerid, element->second.data_vector);
			if (!timerdestroyprotect && !element->second.repeat)
			{
				ZabawyTimers.erase(element);
			}
		}
		timerdestroyprotect = false;
	}

	bool Base::DestroyTimer(int timerid)
	{
		timerdestroyprotect = true;
		auto found = ZabawyTimers.find(timerid);
		if (found != ZabawyTimers.end())
		{
			ZabawyTimers.erase(found);
			sampgdk_KillTimerEx(timerid, this);
		}
		return false;
	}

	void Base::DestroyTimerValidate(int& timerid)
	{
		if (timerid != INVALID_TIMER_ID)
		{
			DestroyTimer(timerid);
			timerid = INVALID_TIMER_ID;
		}
	}

	void Base::SwitchGameTimer(int& timerid, int newtimerid)
	{
		int temp = timerid;
		timerid = newtimerid;
		DestroyTimerValidate(temp);	
	}

	const std::set<int> Base::GetPlayersCopy()
	{
		std::set<int> PlayersInGame_copy;
		std::copy(PlayersInGame.begin(), PlayersInGame.end(), std::insert_iterator< std::set<int> >(PlayersInGame_copy, PlayersInGame_copy.begin()));
		return PlayersInGame_copy;
	}

	void Base::EnableHeadShotsFromAllWeapons(bool enable)
	{
		enableallweaponheadshot = enable;
		HeadShotsEnabled = HeadShotWeapons.size() || enableallweaponheadshot;
	}

	void Base::EnableHeadShotsFromWeapon(int weaponid, bool enable)
	{
		if (enable)
		{
			HeadShotWeapons.insert(weaponid);
		}
		else
		{
			HeadShotWeapons.erase(weaponid);
		}
		HeadShotsEnabled = HeadShotWeapons.size() || enableallweaponheadshot;
	}

	/*
	'r' = 114 82 |1 0 1 0 0 1 0| 1 !Joinable 0 !Staging 1 !Staging
	'b' = 98 66  |1 0 0 0 0 1 0| Staging
	'g' = 103 71 |1 0 0 0 1 1 1| other

	1000010
	00-0-0-
	*/
	void MakeAllDynamicObjects(std::vector<SObject>& objects, int worldid, int interiorid, int playerid, float streamdistance, float drawdistance)
	{
		for (auto i : objects)
		{
			StreamerLibrary::CreateDynamicObject(i.ModelID, i.x, i.y, i.z, i.rx, i.ry, i.rz, worldid, interiorid, playerid, streamdistance, drawdistance);
		}
	}
};
