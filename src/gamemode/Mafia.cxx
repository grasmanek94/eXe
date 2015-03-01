/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the Mafia System

	________________________________________________________________	
	Notes
		-	

	________________________________________________________________
	Dependencies
		Extension
		Database
		WorldData
		ZeroCommand
		ZeroDialog
		MessageManager
		Language

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

std::map < std::string, CMafia*, ci_less > PlayerNameToMafia;
std::map < std::string, CMafia*, ci_less > MafiaProxy;
std::set < std::string, ci_less  > LoadingMafias;
std::set < std::string, ci_less  > LoadedMafias;
std::array<CMafia, MAX_MAFIAS> Mafias;

namespace Mafia
{
	namespace CreationCost
	{
		static const int Money = 1000000;
		static const short int Score = 825;
		static const double MinLevel = 11.0;
	};
	static const unsigned long long TimeBetweenSaves = 30*60*1000;//ms
	std::array<int,MAX_PLAYERS> TagTextLabel;
};

bool LoadMafia(std::string mafianame, int issuerid, std::string issuername)
{
	if (LoadedMafias.find(mafianame) == LoadedMafias.end())
	{
		if (LoadingMafias.find(mafianame) == LoadingMafias.end())
		{
			LoadingMafias.insert(mafianame);
			CreateWorkerRequest(issuerid, mafianame, nullptr, DATABASE_REQUEST_OPERATION_LOAD_MAFIA, DATABASE_POINTER_TYPE_MAFIA, (issuerid != INVALID_PLAYER_ID) ? (unsigned long)111 : 0, issuername);
		}
		return true;
	}
	return false;
}

bool ParseMembersAndPopulate(std::string members, MafiaMemberDataMap& memberdata)
{
	memberdata.clear();
	std::vector<std::string> temp;
	Functions::string_split(members, '|', temp);
	for (auto i : temp)
	{
		if (i.size() > 3)
		{
			auto fnd = i.find(':');
			std::string nickname(i.begin(), i.begin() + fnd);
			++fnd;
			std::string permissions(i.begin() + fnd, i.end());
			memberdata[nickname] = (char)std::stoi(permissions.substr(0, 4));
		}
	}
	return true;
}

std::string GetMafiaMembersString(MafiaMemberDataMap& memberdata)
{
	std::string temp;
	for (auto i : memberdata)
	{
		temp.append(Functions::string_format(i.first + ":%d|", i.second));
	}
	return temp;
}

static void SAMPGDK_CALL MafiaSaveUnsavedChanges(int timerid, void *param)
{
	for (auto &i : Mafias)
	{
		if (i.Valid() && i.unsaved_data)
		{
			i.Save();
		}
	}
}

class CMafiaProcessor : public Extension::Base
{
public:
	CMafiaProcessor() : Base(-1)
	{
	}
	bool OnGameModeInit() override
	{
		for (auto&labelid : Mafia::TagTextLabel)
		{
			labelid = INVALID_3DTEXT_ID;
		}

		for (int i = 0; i < MAX_MAFIAS; ++i)
		{
			Mafias[i].MafiaID = i;
		}

		#ifndef FAILSAFE_MODE
		sampgdk_SetTimerEx(Mafia::TimeBetweenSaves, true, MafiaSaveUnsavedChanges, nullptr, nullptr);
		#endif
		return true;
	}
	void OnPlayerGameBegin(int playerid) override
	{
		if (Player[playerid].IsRegistered)
		{
			if (!LoadMafia(Player[playerid].statistics.MafiaName, playerid, Player[playerid].PlayerName))
			{
				auto pointer = PlayerNameToMafia.find(Player[playerid].PlayerName); 
				if (pointer != PlayerNameToMafia.end())
				{
					pointer->second->OnConnect(playerid);
				}
			}
		}
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		if (Player[playerid].Mafia != nullptr)
		{
			Player[playerid].Mafia->OnDisconnect(playerid);
		}
		return true;
	}
	void DatabaseOperationReport(WorkerTransportData * data) override
	{
		switch (data->operation)
		{
			case DATABASE_REQUEST_OPERATION_CREATE_MAFIA:
			{		
				LoadingMafias.erase(data->Get<mafia>()->name());
				int playerid = ValidPlayerName(data->username);

				switch (data->ip)
				{
				case 1120://mafia already exists
				case 1122://error
					if (playerid != INVALID_PLAYER_ID)
					{
						if (Player[playerid].Connected)
						{
							fixSendClientMessage(data->playerid, Color::COLOR_ERROR, L_dbworker_mafia_creationerror);
							Player[playerid].GiveMoney(Mafia::CreationCost::Money);
							Player[playerid].GiveScore(Mafia::CreationCost::Score);
							gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] /mafia.create Error 1120/1122, GiveBack(Money:%d;Score:%d)", playerid, Player[playerid].PlayerName.c_str(), Mafia::CreationCost::Money, Mafia::CreationCost::Score));
						}
					}
					else
					{
						gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] /mafia.create Error 1120/1122, GiveBack(Money:%d;Score:%d) DISCONNECT", playerid, Player[playerid].PlayerName.c_str(), Mafia::CreationCost::Money, Mafia::CreationCost::Score));
					}
					break;
				case 1121://OK to go
					for (auto &i : Mafias)
					{
						if (i.LoadData(data->Get<mafia>()))
						{
							if (playerid != INVALID_PLAYER_ID)
							{
								if (Player[playerid].Connected)
								{
									Player[playerid].PlayerDisconnect(true);//only save data
								}
							}
							gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] /mafia.create Success 1121, Taken(Money:%d;Score:%d)", playerid, Player[playerid].PlayerName.c_str(), Mafia::CreationCost::Money, Mafia::CreationCost::Score));
							goto out;
						}
					}

					gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] /mafia.create Success 1121 but no free slots, Taken(Money:%d;Score:%d)", playerid, Player[playerid].PlayerName.c_str(), Mafia::CreationCost::Money, Mafia::CreationCost::Score));
					out:
					break;
				}
				break;
			}

			case DATABASE_REQUEST_OPERATION_LOAD_MAFIA_FOUND_AND_LOADED:
			{
				LoadingMafias.erase(data->username);
				for (auto &i: Mafias)
				{
					if (i.LoadData(data->Get<mafia>()))
					{
						return;
					}
				}
				break;
			}

			case DATABASE_REQUEST_OPERATION_LOAD_MAFIA_NOTFOUND:
			{
				LoadingMafias.erase(data->username);
				if (data->ip == 111)
				{
					int playerid = ValidPlayerName(boost::any_cast<std::string>(data->ExtraData));
					if (playerid != INVALID_PLAYER_ID)
					{
						Player[playerid].statistics.MafiaName = "";
					}
				}
				break;
			}
		}
	}
	bool OnPlayerSpawn(int playerid) override
	{
		if (Player[playerid].CurrentGameID == nullptr && Player[playerid].Mafia != nullptr)
		{
			if (Player[playerid].statistics.UseMafiaSkin)
			{
				SetPlayerSkin(playerid, Player[playerid].Mafia->skin);
			}

			if (Player[playerid].statistics.UseMafiaSpawn)
			{
				if (Player[playerid].Mafia->Base == nullptr)
				{
					TeleportPlayer(playerid, Player[playerid].Mafia->SpawnX, Player[playerid].Mafia->SpawnY, Player[playerid].Mafia->SpawnZ, Player[playerid].Mafia->SpawnA, false, 0, 0, "", 0, 0, 2.5, 2.5, false, true);
				}
				else
				{
					const glm::vec4 basepos = Player[playerid].Mafia->Base->GetSpawnPos();
					const std::array<float, 2> spawn_spread = Player[playerid].Mafia->Base->GetSpawnSpread();
					TeleportPlayer(playerid, basepos.x, basepos.y, basepos.z, basepos.w, false, 0, 0, "", 0, 0, spawn_spread[0], spawn_spread[1], false, true);
				}
			}

			Player[playerid].Mafia->RefreshLabel(playerid);
		}
		return true;
	}
} _CMafiaProcessor;
///////////////////////////////////////////////////////////////////////////////////////////////

ZCMD(mcreate, PERMISSION_NONE, RESTRICTION_NOT_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_REGISTERED_AND_LOADED | RESTRICTION_ONLY_LOGGED_IN, cmd_alias({ "/mafia.create", "/mafia.stworz", "/mafia.stwórz", "/maffia.create", "/maffia.stworz", "/maffia.stwórz" }))
{
	if (params.size() && params.size() < 25 && Player[playerid].GetLevel() >= Mafia::CreationCost::MinLevel && Player[playerid].statistics.money >= Mafia::CreationCost::Money && Player[playerid].statistics.respect >= Mafia::CreationCost::Score)
	{
		int result = SetPlayerName(playerid, boost::replace_all_copy(params, "-", "_").c_str());
		SetPlayerName(playerid, Player[playerid].PlayerName.c_str());
		if (result == -1)
		{ 
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_create_invalidname);
			return true;
		}	
		std::string lowercasename = params;
		boost::algorithm::to_lower(lowercasename);
		if (LoadedMafias.find(lowercasename) == LoadedMafias.end())
		{
			mafia * pmafia = new mafia(lowercasename);
			pmafia->displayname(params);
			pmafia->members(Player[playerid].PlayerName + ":3|");
			CreateWorkerRequest(playerid, Player[playerid].PlayerName, pmafia, DATABASE_REQUEST_OPERATION_CREATE_MAFIA, DATABASE_POINTER_TYPE_MAFIA, 112, 0);
			Player[playerid].GiveMoney(-Mafia::CreationCost::Money);
			Player[playerid].GiveScore(-Mafia::CreationCost::Score);
			gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] /mafia.create, Reserved(Money:%d;Score:%d)", playerid, Player[playerid].PlayerName.c_str(), Mafia::CreationCost::Money, Mafia::CreationCost::Score));
		}
		else
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_create_alreadyexists);
		}
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_create_usage_1);
	fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_create_usage_2, true, true, Mafia::CreationCost::MinLevel, Mafia::CreationCost::Score, Mafia::CreationCost::Money);
	return true;
}

ZCMD(mquit, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/mafia.quit", "/mafia.wyjdz", "/mafia.wyjdŸ", "/mafia.q", "/maffia.quit", "/maffia.wyjdz", "/maffia.wyjdŸ", "/maffia.q" }))
{
	if (Player[playerid].Mafia->Wlasciciel.size() == 1 && Player[playerid].Mafia->Wlasciciel.find(Player[playerid].PlayerName) != Player[playerid].Mafia->Wlasciciel.end() && Player[playerid].Mafia->AllMembers.size() > 1)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_quit_ownererror);
		return true;
	}

	Player[playerid].Mafia->RemoveMember(Player[playerid].PlayerName);
	gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] quit", playerid, Player[playerid].PlayerName.c_str()));
	return true;
}

ZCMD(rmmafia, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/mafia.delete", "/mafia.usun", "/mafia.usuñ", "/maffia.delete", "/maffia.usun", "/maffia.usuñ" }))
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_DELETE))
	{
		return true;
	}

	Player[playerid].Mafia->RemoveMafia();
	gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] deleted mafia", playerid, Player[playerid].PlayerName.c_str()));
	return true;
}

ZCMDF(mvsave, PERMISSION_NONE, RESTRICTION_ONLY_IN_VEHICLE | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mvzapisz", "/mzapiszpojazd", "/mprivcar", "/mafia.vsave", "/mafiavsave", "/mafia.vzapisz", "/mafia.zapiszpojazd", "/mafia.privcar", "/mafia.v", "/maffia.vsave", "/maffiavsave", "/maffia.vzapisz", "/maffia.zapiszpojazd", "/maffia.privcar", "/maffia.v" }), "D")
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_SAVECAR))
	{
		return true;
	}

	if (parser.Good())
	{
		unsigned long slot = parser.Get<unsigned long>();
		if (slot < 2)
		{
			int ModelID = GetVehicleModel(Player[playerid].CurrentVehicle);
			if (IsVehicleTrain(ModelID) || IsVehicleTrainTrailer(ModelID))
			{
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_vsave_error);
				return true;
			}
			Player[playerid].Mafia->PrivateVehicle[slot].Save(Player[playerid].CurrentVehicle);
			Player[playerid].Mafia->PrepareForSave();
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mm_mafia_vsave_success);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_vsave_usage_1);
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_vsave_usage_2);
	return true;
}

ZCMDF(minvite, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.invite", "/mafia.zapros", "/mafia.zaproœ", "/mafia.i", "/maffia.invite", "/maffia.zapros", "/maffia.zaproœ", "/maffia.i" }), "p")
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_INVITE))
	{
		return true;
	}

	if (parser.Good())
	{
		int inviteid = parser.Get<ParsePlayer>().playerid;

		if (inviteid != INVALID_PLAYER_ID)
		{
			if (Player[inviteid].Mafia == nullptr)
			{
				if (Player[inviteid].InviteMafia.find(Player[playerid].Mafia->Name) == Player[inviteid].InviteMafia.end())
				{
					Player[inviteid].InviteMafia.insert(Player[playerid].Mafia->Name);
					fixSendClientMessageF(inviteid, Color::COLOR_INFO3, L_mafia_invite_sent, true, true, Player[playerid].Mafia->Name.c_str(), Player[playerid].Mafia->Name.c_str());
					fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_invite_success);
					gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] invited [%d][%s]", playerid, Player[playerid].PlayerName.c_str(), inviteid, Player[inviteid].PlayerName.c_str()));
					return true;
				}
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_invite_alreadyinvited);
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_invite_alreadyinmafia);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_infive_usage);
	return true;
}

ZCMD(mjoin, PERMISSION_NONE, RESTRICTION_NOT_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_REGISTERED_AND_LOADED | RESTRICTION_ONLY_LOGGED_IN, cmd_alias({ "/mafia.join", "/mafia.dolacz", "/mafia.do³¹cz", "/mafia.j", "/maffia.join", "/maffia.dolacz", "/maffia.do³¹cz", "/maffia.j" }))
{
	auto found = Player[playerid].InviteMafia.find(params);
	if (params.size() && found != Player[playerid].InviteMafia.end())
	{
		auto foundmafia = MafiaProxy.find(params);
		if (foundmafia != MafiaProxy.end() && foundmafia->second != nullptr)
		{
			foundmafia->second->AddMember(playerid, 0);
			for (auto&i : foundmafia->second->OnlineMembers)
			{
				SendClientMessageF(i, Color::COLOR_INFO3, L_mm_mafia_join_success, true, true, Player[playerid].PlayerName.c_str(), params.c_str());
			}
			Player[playerid].PlayerDisconnect(true);
			gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] accepted invite from %s", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_join_error);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_join_usage);
	return true;
}

ZCMDF(mgivecash, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.givecash", "/mafia.dajkase", "/mafia.dajkasê", "/mafia.gc", "/maffia.givecash", "/maffia.dajkase", "/maffia.dajkasê", "/maffia.gc" }), "pu")
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_DISTRIBUTE_MONEY))
	{
		return true;
	}

	if (parser.Good() == 2)
	{
		int giveid = parser.Get<ParsePlayer>(0).playerid;
		long long money = parser.Get<long long>(1);
		if (giveid != INVALID_PLAYER_ID)
		{
			if (money >= 25000)
			{			
				if (Player[giveid].Mafia == Player[playerid].Mafia)
				{
					if (Player[giveid].Mafia->money >= money)
					{
						Player[giveid].Mafia->money -= money;
						Player[giveid].GiveMoney(money);
						Player[giveid].Mafia->PrepareForSave();
						
						GivePlayerAchievement(giveid, EAM_MafiaMnieKochaDollar, money);

						if (playerid != giveid)
						{
							fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_givecash_success_1, true, true, money, Player[giveid].Mafia->money);
							fixSendClientMessageF(giveid, Color::COLOR_INFO2, L_mafia_givecash_success_2, true, true, money, Player[playerid].PlayerName.c_str(), Player[giveid].Mafia->money);
						}
						else
						{
							fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_givecash_success_3, true, true, money, Player[giveid].Mafia->money);
						}
						gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] givecash '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
						return true;
					}
					fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_givecash_nocash);
					return true;
				}
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_player_notinmafiaerror);
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_givecash_casherror);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_mafia_givecash_usage, true, true, Player[playerid].Mafia->money);
	return true;
}

ZCMDF(mgiverespect, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.giverespect", "/mafia.giverespekt", "/mafia.dajrespekt", "/mafia.gr", "/maffia.giverespect", "/maffia.dajrespekt", "/maffia.gr" }), "pu")
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_DISTRIBUTE_RESPECT))
	{
		return true;
	}

	if (parser.Good() == 2)
	{
		int giveid = parser.Get<ParsePlayer>(0).playerid;
		long long score = parser.Get<long long>(1);
		if (giveid != INVALID_PLAYER_ID)
		{
			if (score > 0)
			{
				if (Player[giveid].Mafia == Player[playerid].Mafia)
				{
					if (Player[playerid].Mafia->score >= score)
					{
						Player[giveid].Mafia->score -= score;
						Player[giveid].Mafia->PrepareForSave();
						Player[giveid].GiveScore(score);

						GivePlayerAchievement(giveid, EAM_MafiaMnieKochaResp, score);

						if (playerid != giveid)
						{
							fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_givescore_success_1, true, true, score, Player[playerid].Mafia->score);
							fixSendClientMessageF(giveid, Color::COLOR_INFO2, L_mafia_givescore_success_2, true, true, score, Player[playerid].PlayerName.c_str(), Player[playerid].Mafia->score);
						}
						else
						{
							fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_givescore_success_3, true, true, score, Player[playerid].Mafia->score);
						}
						gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] giverespect '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
						return true;
					}
					fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_givescore_noscore);
					return true;
				}
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_player_notinmafiaerror);
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_givescore_scoreerror);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_mafia_givescore_usage, true, true, Player[playerid].Mafia->score);
	return true;
}

ZCMD(mdonatecash, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.donatecash", "/mafia.darujkase", "/mafia.darujkasê", "/mafia.dc", "/maffia.donatecash", "/maffia.darujkase", "/maffia.darujkasê", "/maffia.dc", "/maffia.dm", "/mafia.dm" }))
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_GIVE_MONEY))
	{
		return true;
	}

	long long money;
	if (Functions::is_number_unsigned(params, money))
	{
		if (Player[playerid].statistics.money >= money)
		{
			Player[playerid].GiveMoney(-money);
			Player[playerid].Mafia->money += money;
			Player[playerid].Mafia->PrepareForSave();
			fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_donatecash_success, true, true, money, Player[playerid].Mafia->money);
			gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] donatecash '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_donatecash_error);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_donatecash_usage);
	return true;
}

ZCMD(mdonaterespect, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.donaterespect", "/mafia.darujrespekt", "/mafia.dr", "/maffia.donaterespect", "/maffia.darujrespekt", "/maffia.dr" }))
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_GIVE_RESPECT))
	{
		return true;
	}

	long long score;
	if (Functions::is_number_unsigned(params, score))
	{
		if (Player[playerid].statistics.respect >= score)
		{
			Player[playerid].GiveScore(-score);
			Player[playerid].Mafia->score += score;
			Player[playerid].Mafia->PrepareForSave();
			fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_donatescore_success, true, true, score, Player[playerid].Mafia->score);
			gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] donaterespect '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_donatescore_error);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_donatescore_usage);
	return true;
}

ZCMD(mcolor, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/mafia.color", "/mafia.kolor", "/mafia.c", "/maffia.color", "/maffia.kolor", "/maffia.c" }))
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_CHANGE_COLOR))
	{
		return true;
	}

	int color;
	if (Functions::is_xnumber(params, color))
	{
		clamp(color, 0, 0xFFFFFF);

		if (Functions::GetColorBrightness(color) >= 33)
		{
			Player[playerid].Mafia->color = color;
			Player[playerid].Mafia->UpdateColor();
			Player[playerid].Mafia->PrepareForSave();
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_color_setsuccess);
			gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] setcolor '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_color_too_dark_error);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_mafia_color_set_usage, false, false, Player[playerid].Mafia->color);
	return true;
}

ZCMDF(mkick, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/mafia.kick", "/mafia.wyrzuc", "/mafia.wyrzuæ", "/mafia.k", "/maffia.kick", "/maffia.wyrzuc", "/maffia.wyrzuæ", "/maffia.k" }), "p")
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_KICK))
		return true;
	if (parser.Good())
	{
		std::string NickName("");

		int removeid = parser.Get<ParsePlayer>().playerid;
		if (removeid == INVALID_PLAYER_ID)
		{
			NickName = params;
		}
		else
		{
			NickName = Player[removeid].PlayerName;
			if (playerid == removeid)
			{
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_kick_cannotquit);
				return true;
			}
		}
		auto found = Player[playerid].Mafia->AllMembers.find(NickName);
		if (found == Player[playerid].Mafia->AllMembers.end())
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_playernotinmafia);
			return true;
		}
		if (Player[playerid].MafiaPermissions < found->second)
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_kick_permissionerror);
			return true;
		}
		Player[playerid].Mafia->RemoveMember(NickName);
		fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_kick_success);
		gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] kicked '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
		return true;

	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_kick_usage);
	return true;
}

ZCMDF(mstatus, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.setstatus", "/mafia.ustawstatus", "/mafia.s", "/maffia.setstatus", "/maffia.ustawstatus", "/maffia.s" }), "pd")
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_CHANGE_MEMBER_LEVEL))
	{
		return true;
	}
	if (parser.Good() == 2)
	{
		std::string NickName("");
		ParsePlayer temp = parser.Get<ParsePlayer>(0);

		if (temp.playerid == INVALID_PLAYER_ID)
		{
			NickName = temp.PlayerNameOrID;
		}
		else
		{
			NickName = Player[temp.playerid].PlayerName;
		}

		long level = parser.Get<long>(1);
			
		clamp<long>(level, 0, 3);

		auto found = Player[playerid].Mafia->AllMembers.find(NickName);
		if (found == Player[playerid].Mafia->AllMembers.end())
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_playernotinmafia);
			return true;
		}
		if (found->second == 3 && Player[playerid].Mafia->Wlasciciel.size() == 1)
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_status_ownererror);
			return true;
		}
		if (found->second > Player[playerid].MafiaPermissions)
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_status_permissionerror_1);
			return true;
		}
		if (level > Player[playerid].MafiaPermissions)
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_status_permissionerror_2);
			return true;
		}
		found->second = level;
		if (temp.playerid != INVALID_PLAYER_ID && playerid != temp.playerid)
		{
			fixSendClientMessageF(temp.playerid, Color::COLOR_INFO2, L_mafia_status_success_1, true, true, TranslateP(playerid, PermissionNames[level]).c_str(), Player[playerid].PlayerName.c_str());
			Player[temp.playerid].MafiaPermissions = level;
		}
		fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_status_success_2, true, true, NickName.c_str(), TranslateP(playerid, PermissionNames[level]).c_str());
		Player[playerid].Mafia->PrepareForSave();
		gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] setstatus '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_status_usage);
	return true;
}

static const std::map<std::string, MAFIA_ACTIONS> MafiaActionPermissionProxy =
{
	{ "INVITE", MAFIA_ACTION_INVITE },
	{ "KICK", MAFIA_ACTION_KICK },
	{ "SAVECAR", MAFIA_ACTION_SAVECAR },
	{ "SPAWNCAR", MAFIA_ACTION_SPAWNCAR },
	{ "CMLVL", MAFIA_ACTION_CHANGE_MEMBER_LEVEL },
	{ "PERM", MAFIA_ACTION_CHANGE_ACTION_PERMISSIONS },
	{ "COLOR", MAFIA_ACTION_CHANGE_COLOR },
	{ "DMONEY", MAFIA_ACTION_GIVE_MONEY },
	{ "GMONEY", MAFIA_ACTION_DISTRIBUTE_MONEY },
	{ "DRESPECT", MAFIA_ACTION_GIVE_RESPECT },
	{ "GRESPECT", MAFIA_ACTION_DISTRIBUTE_RESPECT },
	{ "DELETE", MAFIA_ACTION_DELETE },
	{ "SKIN", MAFIA_ACTION_SKIN },
	{ "SPAWN", MAFIA_ACTION_SPAWN },
	{ "TAG", MAFIA_ACTION_TAG }
};

ZCMDF(mpermissions, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.permissions", "/mafia.pozwolenia", "/mafia.p", "/maffia.permissions", "/maffia.pozwolenia", "/maffia.p" }), "wd")
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_CHANGE_ACTION_PERMISSIONS))
		return true;
	if (parser.Good() == 2)
	{
		auto found = MafiaActionPermissionProxy.find(parser.Get<std::string>(0));
		if (found != MafiaActionPermissionProxy.end())
		{
			long level = parser.Get<long>(0);

			clamp<long>(level, 0, 3);

			Player[playerid].Mafia->SetActionPermission(found->second, level);
			Player[playerid].Mafia->PrepareForSave();
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_permissions_success);
			gtLog(LOG_MAFIA, Functions::string_format("[%d][%s] setpermission '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_permissions_error);
		return true;
	}
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_dialog_title_info), TranslateP(playerid, L_mafia_permissions_usage), "V", "X");
	return true;
}

ZCMD(mlista, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.members", "/mafia.lista", "/mafia.l", "/mafia.m", "/mafia.list", "/maffia.members", "/maffia.lista", "/maffia.l", "/maffia.m", "/maffia.list" }))
{
	CMafia* mafiaptr = Player[playerid].Mafia;

	if (params.size() && params.size() < 25)
	{
		auto found = MafiaProxy.find(params);
		if (found != MafiaProxy.end())
		{
			mafiaptr = found->second;
		}
		else
		{
			mafiaptr = nullptr;
		}
	}

	if (mafiaptr == nullptr)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_lista_name_error);
		return true;
	}

	int counter = 0;
	std::string memberlist(TranslatePF(playerid, L_mafia_lista_info, mafiaptr->score, mafiaptr->money, mafiaptr->Name.c_str()));
	for (auto i : mafiaptr->AllMembers)
	{
		if (counter++ % 4 == 0)
		{
			memberlist.append("\n\t");
		}
		memberlist.append(i.first + "(" + TranslateP(playerid, PermissionNames[i.second]) + ")\t");
	}

	memberlist.append(TranslateP(playerid, L_mafia_lista_info_2));

	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_mafia_lista_title), memberlist, "V", "X");
	return true;
}

ZCMD(mafie, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/mafias", "/maffias" }))
{
	int counter = 0;
	int color = 0xFFFFFF;
	std::string mafialist(TranslateP(playerid, L_mafia_online_text));
	for (auto i : LoadedMafias)
	{
		if (counter++ % 4 == 0)
		{
			mafialist.append("\n\t");
		}

		if (MafiaProxy[i] != nullptr)
		{
			color = MafiaProxy[i]->color;
		} 
		else 
		{
			color = 0xFFFFFF;
		}

		mafialist.append(Functions::string_format("{%06X}", color) + i + "\t");
	}
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_mafia_online_title), mafialist, "V", "X");
	return true;
}

ZCMD(mskin, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/mafia.skin", "/mafia.sk", "/maffia.skin", "/maffia.sk" }))
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_SKIN))
	{
		return true;
	}

	if (Player[playerid].statistics.SkinModelID >= 0 && Player[playerid].statistics.SkinModelID <= 300)
	{
		Player[playerid].Mafia->ApplySkin(Player[playerid].statistics.SkinModelID);
	}

	fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_setskin);
	return true;
}

ZCMD(mspawn, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/mafia.spawn", "/mafia.sp", "/maffia.spawn", "/maffia.sp" }))
{
	if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_SPAWN))
		return true;

	float x, y, z, a;
	GetPlayerPos(playerid, &x, &y, &z);
	GetPlayerFacingAngle(playerid, &a);
	Player[playerid].Mafia->SpawnA = a;
	Player[playerid].Mafia->SpawnX = x;
	Player[playerid].Mafia->SpawnY = y;
	Player[playerid].Mafia->SpawnZ = z;
	Player[playerid].Mafia->PrepareForSave();

	fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_setspawn);
	return true;
}

ZCMD(mafiaskinon, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_ON_FOOT, cmd_alias({ "/mafia.skin.on", "/maffia.skin.on" }))
{
	Player[playerid].statistics.UseMafiaSkin = true;
	SetPlayerSkin(playerid, Player[playerid].Mafia->skin);

	fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_skinon);
	return true;
}

ZCMD(mafiaskinoff, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_ON_FOOT, cmd_alias({ "/mafia.skin.off", "/maffia.skin.off" }))
{
	Player[playerid].statistics.UseMafiaSkin = false;

	if (Player[playerid].statistics.SkinModelID >= 0 && Player[playerid].statistics.SkinModelID < 300)
		SetPlayerSkin(playerid, Player[playerid].statistics.SkinModelID);

	fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_skinoff);
	return true;
}

ZCMD(mafiaspawnon, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.spawn.on", "/maffia.spawn.on" }))
{
	Player[playerid].statistics.UseMafiaSpawn = true;

	fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_spawnon);
	return true;
}

ZCMD(mafiaspawnoff, PERMISSION_NONE, RESTRICTION_IS_IN_MAFIA, cmd_alias({ "/mafia.spawn.off", "/maffia.spawn.off" }))
{
	Player[playerid].statistics.UseMafiaSpawn = false;
	fixSendClientMessage(playerid, Color::COLOR_INFO2, L_mafia_spawnoff);
	return true;
}

/////////////////////////CMAFIA

CMafia::CMafia() :
	deleting(false),
	taken(false),
	Name(""),
	unsaved_data(false),
	RawMembers(""),
	bActionPermissions(std::numeric_limits<unsigned long long>::max()),
	score(0),
	money(0),
	color(0xFF3BC1),
	skin(0),
	SpawnX(0.0f),
	SpawnY(0.0f),
	SpawnZ(5.0f),
	SpawnA(0.0f),
	kills(0),
	deaths(0),
	experience(0),
	creation_date(0),
	playtime(0),
	last_load(0),
	tag(""),
	Base(nullptr)
{}

bool CMafia::Valid()
{
	return taken;
}

bool CMafia::LoadData(mafia* pointer)
{
	if (!pointer || Valid())
	{
		return false;
	}

	Base = nullptr;

	if (!boost::algorithm::iequals(dbmafia.displayname(), Name))//mafia unload fix? //#2: yup, works indeed
	{
		MafiaProxy.erase(Name);
	}

	dbmafia = *pointer;
	dbmafia.name(pointer->name());

	Name = dbmafia.displayname();
	MafiaProxy[Name] = this;

	unsigned long long TimeNow = Functions::GetTime();

	memcpy((void*)&PrivateVehicle, (void*)dbmafia.PrivateCars(), 144);
	dbmafia.last_load(TimeNow);
	last_load = TimeNow;
	skin = dbmafia.skin();
	SpawnX = dbmafia.SpawnX();
	SpawnY = dbmafia.SpawnY();
	SpawnZ = dbmafia.SpawnZ();
	SpawnA = dbmafia.SpawnA();
	score = dbmafia.score();
	money = dbmafia.money();
	kills = dbmafia.kills();
	deaths = dbmafia.deaths();
	experience = dbmafia.experience();
	creation_date = dbmafia.creation_date();
	if (creation_date == 0)
	{
		creation_date = TimeNow;
		dbmafia.creation_date(creation_date);
	}
	playtime = dbmafia.playtime();
	tag = dbmafia.tag();

	PrepareForSave();
	taken = true;
	deleting = false;

	bActionPermissions = std::bitset<64>(dbmafia.action_permissions());

	color = dbmafia.color();
	clamp<unsigned int>(color, 0, 0xFFFFFF);

	ParseMembersAndPopulate(dbmafia.members(), AllMembers);

	if (LoadedMafias.find(Name) == LoadedMafias.end())
	{
		LoadedMafias.insert(Name);
	}

	for (auto i : AllMembers)
	{
		PutIntoMembers(i.first, i.second);
		PlayerNameToMafia[i.first] = this;
		auto player = PlayerNameToID.find(i.first);
		if (player != PlayerNameToID.end())
		{
			OnConnect(player->second);
		}
	}

	extOnMafiaLoad(MafiaID, this);
	return true;
}

void CMafia::PutIntoMembers(std::string nickname, unsigned char level)
{
	Members.insert(nickname);
	if (level == 1)
	{
		WicePrezes.insert(nickname);
	}
	if (level == 2)
	{
		Prezes.insert(nickname);
	}
	if (level >= 3)
	{
		Wlasciciel.insert(nickname);
	}
}

void CMafia::RemoveFromMembers(std::string nickname)
{
	Members.erase(nickname);
	WicePrezes.erase(nickname);
	Prezes.erase(nickname);
	Wlasciciel.erase(nickname);
}

void CMafia::SendMessageToMafia(int color, std::string text)
{
	for (auto &i : OnlineMembers)
	{
		fixSendClientMessage(i, color, text);
	}
}

void CMafia::PutIntoOnlineMembers(int playerid, unsigned char level)
{
	OnlineMembers.insert(playerid);
	if (level == 1)
	{
		OnlineWicePrezes.insert(playerid);
	}
	if (level == 2)
	{
		OnlinePrezes.insert(playerid);
	}
	if (level >= 3)
	{
		OnlineWlasciciel.insert(playerid);
	}
}

void CMafia::UpdateColor()
{
	for (auto i : OnlineMembers)
	{
		Player[i].Color = color;
		Player[i].UpdateFullColor();
		if (Player[i].CurrentGameID == nullptr)
		{
			SetPlayerColor(i, Player[i].FullColor);
			RefreshLabel(i);
		}
	}
}

void CMafia::UpdateColor(int playerid)
{
	Player[playerid].Color = color;
	Player[playerid].UpdateFullColor();
	if (Player[playerid].CurrentGameID == nullptr)
	{
		SetPlayerColor(playerid, Player[playerid].FullColor);
		RefreshLabel(playerid);
	}
}

void CMafia::RemoveFromOnlineMembers(int playerid, bool reloadonly)
{
	OnlineMembers.erase(playerid);
	OnlineWicePrezes.erase(playerid);
	OnlinePrezes.erase(playerid);
	OnlineWlasciciel.erase(playerid);

	if (!reloadonly)
	{
		RemoveLabel(playerid);

		if (Player[playerid].statistics.staticcolor)
		{
			Player[playerid].Color = Player[playerid].statistics.staticcolor;
		}
		else
		{
			Player[playerid].Color = Functions::RandomGenerator->Random(0x000000, 0xFFFFFF);
		}

		Player[playerid].UpdateFullColor();
		SetPlayerColor(playerid, Player[playerid].FullColor);

		if (!OnlineMembers.size())
		{
			Save();
		}
	}
}

void CMafia::RefreshLabel(int playerid)
{
	if (Player[playerid].CurrentGameID != nullptr)
	{
		return;
	}

	if (Mafia::TagTextLabel[playerid] != INVALID_3DTEXT_ID)
	{
		StreamerLibrary::DestroyDynamic3DTextLabel(Mafia::TagTextLabel[playerid]);
	}
	Mafia::TagTextLabel[playerid] = StreamerLibrary::CreateDynamic3DTextLabel(Name, Player[playerid].FullColor, 0.0, 0.0, 0.5, 70.0, playerid, INVALID_VEHICLE_ID, 1, -1, -1, -1, 100.0);
}

bool CMafia::OnConnect(int playerid)
{
	if (!Valid())
	{
		return false;
	}

	Player[playerid].Mafia = this;
	Player[playerid].MafiaPermissions = AllMembers[Player[playerid].PlayerName];
	Player[playerid].statistics.MafiaName = Name;

	PutIntoOnlineMembers(playerid, Player[playerid].MafiaPermissions);
	UpdateColor(playerid);
	RefreshLabel(playerid);
	UpdatePlayerStatsTextDraw(playerid);
	fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_mafia_loaded_success, true, true, Name.c_str(), MafiaID);
	
	return true;
}

void CMafia::RemoveLabel(int playerid)
{
	if (Mafia::TagTextLabel[playerid] != INVALID_3DTEXT_ID)
	{
		StreamerLibrary::DestroyDynamic3DTextLabel(Mafia::TagTextLabel[playerid]);
		Mafia::TagTextLabel[playerid] = INVALID_3DTEXT_ID;
	}
}

bool CMafia::OnDisconnect(int playerid)
{
	if (!Valid())
	{
		return false;
	}

	RemoveFromOnlineMembers(playerid);

	if (OnlineMembers.size() == 0)
	{
		Save();
	}

	return true;
}

void CMafia::SaveStats()
{
	RawMembers = GetMafiaMembersString(AllMembers);

	dbmafia.members(RawMembers);
	dbmafia.action_permissions(bActionPermissions.to_ullong());
	dbmafia.color(color);
	dbmafia.skin(skin);
	dbmafia.SpawnX(SpawnX);
	dbmafia.SpawnY(SpawnY);
	dbmafia.SpawnZ(SpawnZ);
	dbmafia.SpawnA(SpawnA);
	dbmafia.score(score);
	dbmafia.money(money);
	dbmafia.kills(kills);
	dbmafia.deaths(deaths);
	dbmafia.experience(experience);
	dbmafia.creation_date(creation_date);
	playtime += (Functions::GetTime() - last_load);
	dbmafia.playtime(playtime);
	dbmafia.tag(tag);
	memcpy((void*)dbmafia.PrivateCars(), (void*)&PrivateVehicle, 144);
}

//unload only when no more members online
void CMafia::Save()
{
	if (!Valid())
		return;

	if (!deleting)
	{
		SaveStats();

		SendStatsToWeb(true);

		mafia * tmpmafia = new mafia(Name);
		*tmpmafia = dbmafia;

		CreateWorkerRequest(0, "", tmpmafia, DATABASE_REQUEST_OPERATION_SAVE_MAFIA, DATABASE_POINTER_TYPE_MAFIA, 0, 0);
	}

	unsaved_data = false;
	if (OnlineMembers.size() == 0)
	{
		extOnMafiaUnload(MafiaID, this);

		taken = false;
		if (Name.size())
		{
			LoadedMafias.erase(Name);
			MafiaProxy.erase(Name);
		}

		for (auto i : AllMembers)
		{
			PlayerNameToMafia.erase(i.first);
		}

		Members.clear();
		WicePrezes.clear();
		Prezes.clear();
		Wlasciciel.clear();
		AllMembers.clear();
	}
}

bool CMafia::RemoveMember(std::string nickname, bool IN_SELF)
{
	auto member = AllMembers.find(nickname);
	if (member == AllMembers.end())
	{
		return false;
	}

	PlayerNameToMafia.erase(nickname);
	AllMembers.erase(member);
	RemoveFromMembers(nickname);

	unsaved_data = true;

	bool AllMafia = false;
	if (AllMembers.size() == 0)
	{
		//remove mafia
		CreateWorkerRequest(0, Name, nullptr, DATABASE_REQUEST_OPERATION_DELETE_MAFIA, DATABASE_POINTER_TYPE_MAFIA, 0, 0);
		AllMafia = true;
		deleting = true;
	}

	PrepareForSave();

	auto player = PlayerNameToID.find(nickname);

	if (player != PlayerNameToID.end())
	{
		//online
		Player[player->second].Mafia = nullptr;
		Player[player->second].MafiaPermissions = 0;

		if (!AllMafia)
		{
			SendClientMessage(player->second, Color::COLOR_INFO3, L_mafia_deleted);
		}
		else
		{
			if (!IN_SELF)
			{
				RemoveMafia();
			}
			SendClientMessage(player->second, Color::COLOR_INFO3, L_mafia_kicked);
		}
		UpdatePlayerStatsTextDraw(player->second);
		RemoveFromOnlineMembers(player->second);
	}
	else
	{
		//offline
		CreateWorkerRequest(0, nickname, nullptr, DATABASE_REQUEST_OPERATION_REMOVE_PLAYER_FROM_MAFIA, DATABASE_POINTER_TYPE_MAFIA, 0, Name);
	}

	if (!AllMafia && OnlineMembers.size() == 0)
	{
		Save();
	}

	return true;
}

void CMafia::RemoveMafia()
{
	while (AllMembers.size() > 0)
	{
		RemoveMember(AllMembers.begin()->first, true);
	}

	LoadedMafias.erase(Name);

	if (Name.size())
	{
		MafiaProxy.erase(Name);
	}

	for (auto i : AllMembers)
	{
		PlayerNameToMafia.erase(i.first);
	}
	Members.clear();
	WicePrezes.clear();
	Prezes.clear();
	Wlasciciel.clear();
	AllMembers.clear();
}

void CMafia::ApplySkin(int skinid)
{
	for (auto&playerid : OnlineMembers)
	{
		if (Player[playerid].statistics.UseMafiaSkin && Player[playerid].CurrentGameID == nullptr)
		{
			SetPlayerSkin(playerid, skinid);
		}
	}
	skin = skinid;
	PrepareForSave();
}

void CMafia::ApplyTag(std::string _tag)
{
	tag = _tag;
	PrepareForSave();
}

bool CMafia::AddMember(int playerid, unsigned char level)
{
	if (Player[playerid].Mafia != nullptr)
	{
		if (Player[playerid].Mafia == this)
		{
			return true;
		}
		return false;
	}
	auto member = AllMembers.find(Player[playerid].PlayerName);
	if (member != AllMembers.end())
	{
		return true;
	}
	auto other_check_member = PlayerNameToMafia.find(Player[playerid].PlayerName);
	if (other_check_member != PlayerNameToMafia.end())
	{
		return other_check_member->second == this;
	}
	AllMembers.insert(MafiaMemberData(Player[playerid].PlayerName, level));
	PutIntoMembers(Player[playerid].PlayerName, level);
	PlayerNameToMafia[Player[playerid].PlayerName] = this;

	PrepareForSave();

	OnConnect(playerid);
	return true;
}

bool CMafia::ChangeMemberLevel(std::string nickname, unsigned char level)
{
	auto member = AllMembers.find(nickname);
	if (member == AllMembers.end())
	{
		return false;
	}
	auto player = PlayerNameToID.find(nickname);
	if (player != PlayerNameToID.end())
	{
		Player[player->second].MafiaPermissions = level;
		RemoveFromOnlineMembers(player->second, true);
		PutIntoOnlineMembers(player->second, level);
	}
	member->second = level;
	RemoveFromMembers(nickname);
	PutIntoMembers(nickname, level);
	PrepareForSave();
	return true;
}

void CMafia::SetActionPermission(MAFIA_ACTIONS action, unsigned char level)
{
	if (level > 3)
	{
		level = 3;
	}

	bActionPermissions[action] = false;
	bActionPermissions[action+1] = false;

	switch (level)
	{
	case 1:
		bActionPermissions[action] = true;
		break;
	case 2:
		bActionPermissions[action + 1] = true;
		break;
	case 3:
		bActionPermissions[action] = true;
		bActionPermissions[action + 1] = true;
		break;
	}
}

unsigned char CMafia::GetActionPermission(MAFIA_ACTIONS action)
{	
	return ((unsigned char)bActionPermissions[action]) + (2 * ((unsigned char)bActionPermissions[action+1]));
}

bool CMafia::IsPlayerAllowedAction(int playerid, MAFIA_ACTIONS action)
{
	if (Player[playerid].Mafia == this && GetActionPermission(action) <= Player[playerid].MafiaPermissions && Player[playerid].Connected)
	{
		return true;
	}
	return false;
}

bool CMafia::CheckAllowedWithMessage(int playerid, MAFIA_ACTIONS action)
{
	if (!IsPlayerAllowedAction(playerid, action))
	{
		fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_mafia_permission_error, true, true, TranslateP(playerid, PermissionNames[GetActionPermission(action)]).c_str());
		return false;
	}
	return true;
}

void CMafia::PrepareForSave()
{
	unsaved_data = true;
	#ifdef FAILSAFE_MODE
	Save();
	#endif
}

void CMafia::SendStatsToWeb(bool IN_MAFIA)
{
	if (!IN_MAFIA)
	{
		SaveStats();
	}

	std::string mafia_post(Functions::string_format(
		"name=%s&members=%u&color=%08X&creation_date=%I64u&deaths=%u&exp=%I64u&kills=%u&lastload=%I64u&money=%I64d&playtime=%I64u&score=%I64d&skin=%d&memberlist=%s",
		Name.c_str(),
		(unsigned long)AllMembers.size(),
		color,
		creation_date,
		deaths,
		experience,
		kills,
		last_load,
		money,
		playtime,
		score,
		skin,
		RawMembers.c_str()));

	sampgdk_HTTP(0, HTTP_POST, "exe24.info/top/addstatsmafia.php", mafia_post.c_str());//fire&forget
}