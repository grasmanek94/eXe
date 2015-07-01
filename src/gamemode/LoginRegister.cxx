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

std::string PlayerPasswordHash(int playerid, std::string inputtext)
{
	std::string lowercasename = Player[playerid].PlayerName;
	boost::algorithm::to_lower(lowercasename);
	return Whirlpool_Hash(lowercasename + Functions::string_format("%I64u", Player[playerid].statistics.registertime) + inputtext + Functions::string_format("%I64u", Player[playerid].statistics.registertime % 1756392));
}

std::string PlayerPasswordHash(user * player, std::string inputtext)
{
	std::string lowercasename = player->nickname();
	boost::algorithm::to_lower(lowercasename);
	return Whirlpool_Hash(lowercasename + Functions::string_format("%I64u", player->registertime()) + inputtext + Functions::string_format("%I64u", player->registertime() % 1756392));
}

std::string CreateAuthenticationCodeForPlayer(int playerid)
{
	return Whirlpool_Hash(Player[playerid].ipv4.to_string() + Player[playerid].PlayerName + Functions::string_format("%I64u", Functions::GetTime()) + Functions::string_format("%I64u", Player[playerid].statistics.registertime % 15684156548)).substr(0, 32);
}

bool CheckCommandAllowed(int playerid, int this_restrictions, bool showmessage)
{
	int state = GetPlayerState(playerid);
	unsigned long long timenow = Functions::GetTime();

	if (Player[playerid].CommandMute > timenow && !bIsBitEnabled(this_restrictions, RESTRICTION_CAN_USE_WHILE_MUTED))
	{
		if (showmessage)
		{
			fixSendClientMessageF(playerid, -1, L_cmdchatmute, true, true, ((Player[playerid].CommandMute - timenow) / 1000) + 1);
		}
		return false;
	}

	//if [expression1] [expression2] execution order is important here
	if ((Player[playerid].CurrentGameID != nullptr && !Zabawy::IsPlayerStaging(playerid)) &&
		(bIsBitEnabled(this_restrictions, RESTRICTION_NOT_IN_A_GAME) || (GetCommandPermissionGameID(this_restrictions) != Player[playerid].CurrentGameID->CurrentGameID && GetCommandPermissionGameID(this_restrictions) != 0)))
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_game);
		}
		return false;
	}
	else if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_IN_A_GAME) && 
		Player[playerid].CurrentGameID == nullptr)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyingame);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_NOT_IN_STAGING) && 
		Zabawy::IsPlayerStaging(playerid))
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_game);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_ON_FOOT) &&
		state != PLAYER_STATE_ONFOOT)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyonfoot);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_IN_VEHICLE) &&
		state != PLAYER_STATE_DRIVER && 
		state != PLAYER_STATE_PASSENGER)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyinvehicle);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_DRIVER) &&
		state != PLAYER_STATE_DRIVER)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyasdriver);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_LOGGED_IN) &&
		!Player[playerid].IsLoggedIn)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyloggedin);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_IS_IN_MAFIA) &&
		(
			!Player[playerid].statistics.MafiaName.size() || 
			Player[playerid].Mafia == nullptr
		))
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyinmafia);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_NOT_IN_MAFIA) &&
		Player[playerid].Mafia != nullptr)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_notinmafia);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_IN_CAR) &&
		(
			!Player[playerid].CurrentVehicle || 
			!bIsBitEnabled(VehicleModelInfo[GetVehicleModel(Player[playerid].CurrentVehicle)], VEHICLE_LAND)
		))
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyincar);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_IN_AIRPLANE) &&
		(
			!Player[playerid].CurrentVehicle || 
			!bIsBitEnabled(VehicleModelInfo[GetVehicleModel(Player[playerid].CurrentVehicle)], VEHICLE_AIRPLANE)
		))
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyinairplane);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_ONLY_IN_HELI) &&
		(
			!Player[playerid].CurrentVehicle || 
			!bIsBitEnabled(VehicleModelInfo[GetVehicleModel(Player[playerid].CurrentVehicle)], VEHICLE_HELICOPTER)
		))
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyinhelicopter);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_REGISTERED_AND_LOADED) &&
		(!Player[playerid].IsRegistered || !Player[playerid].DataLoaded))
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_onlyregisteredanddataloaded);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_NOT_AFTER_FIGHT) &&
		Player[playerid].LastDamageTime > timenow)
	{
		if (showmessage)
		{
			fixSendClientMessageF(playerid, -1, L_commandrestriction_afterfight, true, true, ((Player[playerid].LastDamageTime - timenow) / 1000) + 1);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_IS_NOT_CHAT_MUTED) &&
		Player[playerid].ChatMute > timenow)
	{
		if (showmessage)
		{
			fixSendClientMessageF(playerid, -1, L_cmdchatmute, true, true, ((Player[playerid].CommandMute - Functions::GetTime()) / 1000) + 1);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_SPAWNED) &&
		!Player[playerid].Spawned)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_spawned);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_USING_EXE24PLUS) &&
		!Player[playerid].Exe24ModEnabled)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_exe24plus_1);
			fixSendClientMessage(playerid, -1, L_commandrestriction_exe24plus_2);
		}
		return false;
	}

	if (bIsBitEnabled(this_restrictions, RESTRICTION_IN_VEHICLE_OR_ONFOOT) &&
		state != PLAYER_STATE_DRIVER && 
		state != PLAYER_STATE_PASSENGER && 
		state != PLAYER_STATE_ONFOOT)
	{
		if (showmessage)
		{
			fixSendClientMessage(playerid, -1, L_commandrestriction_vehorfoot);
		}
		return false;
	}

	return true;
}

void ShowLoginDialog(int playerid)
{
	ShowPlayerCustomDialog(playerid, "login", DIALOG_STYLE_PASSWORD, TranslateP(playerid, L_login_dialog_title), TranslateP(playerid, L_login_dialog_text), TranslateP(playerid, L_Login), TranslateP(playerid, L_Help));
}

void ShowQuestionDialog(int playerid)
{
	ShowPlayerCustomDialog(playerid, "register", DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_registerq_title), TranslateP(playerid, L_registerq_text), TranslateP(playerid, L_Yes), TranslateP(playerid, L_No));
}

void ShowRegisterDialog(int playerid)
{
	ShowPlayerCustomDialog(playerid, "register2", DIALOG_STYLE_PASSWORD, TranslateP(playerid, L_updatepass_title), TranslateP(playerid, L_register_text), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
}

void ShowUpdatePasswordDialog(int playerid)
{
	ShowPlayerCustomDialog(playerid, "updatepasswd", DIALOG_STYLE_PASSWORD, TranslateP(playerid, L_updatepass_title), TranslateP(playerid, L_register_text), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
}

void PerformPlayerSkinIdCheck(int playerid)
{
	if (Player[playerid].statistics.SkinModelID >= 0 && Player[playerid].statistics.SkinModelID < 300)
	{
		safeSetSpawnInfo(playerid, NO_TEAM, Player[playerid].statistics.SkinModelID, 0.0, 0.0, 3.0, 0.0, 0, 0, 0, 0, 0, 0);
	}
	else
	{
		safeSetSpawnInfo(playerid, NO_TEAM, 0, 0.0, 0.0, 3.0, 0.0, 0, 0, 0, 0, 0, 0);
	}
}

class LoginRegisterProcessor : public Extension::Base
{
public:
	LoginRegisterProcessor() : Base(ExecutionPriorities::LoginRegister){}
	bool OnGameModeInit() override
	{
		PrepareZeroDialog("languagechooser", DIALOG_STYLE_LIST, " ", language_list, "V", "");
		PrepareZeroDialog("languagechooserb", DIALOG_STYLE_LIST, " ", language_list, "V", "X");
		return true;
	}
	bool OnPlayerUpdate(int playerid) override
	{
		if (Player[playerid].IsRegistered)
		{
			return Player[playerid].IsLoggedIn;
		}
		return true;
	}
	bool OnPlayerDataAcquired(int playerid, int result) override
	{
		if (Player[playerid].Connected)
		{
			Player[playerid].AquiredData = true;
			if (result == DATABASE_REQUEST_OPERATION_FIND_LOAD_FOUND_AND_LOADED)
			{
				Player[playerid].InSkinSelection = false;
				Player[playerid].RequestSkinSelection = false;
				Player[playerid].IsRegistered = true;
				Player[playerid].IsLoggedIn = false;	
				
				auto findLanguageIndex = LanguageIDToIndex.find(Player[playerid].Achievementdata.CurrentlySelectedLanguage);
				
				if (Player[playerid].Achievementdata.DidPlayerSelectLanguage && findLanguageIndex != LanguageIDToIndex.end() && findLanguageIndex->second < CURRENT_LANGUAGE_ID)
				{
					Player[playerid].LanguageChosen = true;
					AddPlayerToLanguage(playerid, findLanguageIndex->second);
				}

				if (Player[playerid].statistics.banned > Functions::GetTime() && !Player[playerid].BanImmunity)
				{
					if (!Player[playerid].AlreadyKicked)
					{
						Player[playerid].AlreadyKicked = true;
						ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_banned_account),
							TranslatePF(playerid, L_banned_dialogdisplay,
							Player[playerid].statistics.ban_who.c_str(),
							Player[playerid].statistics.bannedidentificator,
							Player[playerid].PlayerName.c_str(),
							boost::asio::ip::address_v4(Player[playerid].statistics.bannedip).to_string().c_str(),
							Player[playerid].statistics.ban_reason.c_str(),
							StaticVersionDeterminator.GetWebUrl().c_str(),
							Functions::GetTimeStrFromMs(Player[playerid].statistics.banned).c_str(),
							Functions::GetTimeStrFromMs(Player[playerid].statistics.bannedidentificator).c_str()
							), "V", "X");
						DelayKick(playerid, 0xDEAD);
					}
				}
			}
			else
			{
				Player[playerid].IsRegistered = false;
				Player[playerid].IsLoggedIn = false;
				ShowPlayerZeroDialog(playerid, "languagechooser");
			}
		}
		return true;
	}
	bool Test(int playerid)
	{
		if (!Player[playerid].AquiredData)//aby ju¿ siê 2 dialogi nie miesza³y
		{
			return false;
		}
		if (Player[playerid].IsRegistered && !Player[playerid].IsLoggedIn)
		{
			if (Player[playerid].LanguageChosen)
			{
				ShowLoginDialog(playerid);
			}
			else
			{
				ShowPlayerZeroDialog(playerid, "languagechooser");
			}
			return false;
		}
		return true;
	}
	bool OnPlayerRequestSpawn(int playerid) override
	{
		return Test(playerid);
	}
	bool OnPlayerRequestClass(int playerid, int classid) override
	{
		Test(playerid);
		return true;
	}
	bool OnPlayerText(int playerid, std::string text) override
	{
		return Test(playerid);
	}
	bool OnPlayerSpawn(int playerid) override
	{
		PerformPlayerSkinIdCheck(playerid);
		return true;
	}
	bool OnPlayerCommandReceived(int playerid, std::string& command, std::string& params, int min_execute_level, int this_restrictions) override
	{
		if (!Test(playerid))
		{
			return false;
		}
		//allow unless proven otherwise
#ifndef _LOCALHOST_DEBUG 
#ifndef _FRIENDS
		if (Player[playerid].statistics.privilidges >= min_execute_level)
#else
		if(true)
#endif
#else
		if (true)
#endif
		{
			return CheckCommandAllowed(playerid, this_restrictions);
		}
		else
		{
			fixSendClientMessage(playerid, -1, L_command_nopermission);
			return false;
		}
	}
} _LoginRegisterProcessor;

bool TestIfAllowed(int playerid)
{
	return _LoginRegisterProcessor.Test(playerid);
}

static void SAMPGDK_CALL SpawnPlayerOnClassRequest(int timerid, void *param)
{
	ClearAnimations((int)param, false);
	PlayerPlaySound((int)param, MUSIC_OFF, 0.0, 0.0, 0.0);
	TogglePlayerSpectating((int)param, false);
}

void SpawnPlayerAnyway(int playerid)
{
	TogglePlayerSpectating(playerid, true);
	PerformPlayerSkinIdCheck(playerid);
	sampgdk_SetTimerEx(1, false, SpawnPlayerOnClassRequest, (void*)playerid, nullptr);
}

ZERO_DIALOG(languagechooser)
{
	if (listitem >= 0 && listitem < CURRENT_LANGUAGE_ID)
	{
		Player[playerid].LanguageID = IndexToLanguageID[listitem];
		Player[playerid].Achievementdata.DidPlayerSelectLanguage = 1;
		Player[playerid].LanguageChosen = true;
		Player[playerid].Achievementdata.CurrentlySelectedLanguage = Player[playerid].LanguageID;

		AddPlayerToLanguage(playerid, listitem);

		if (!Player[playerid].IsRegistered)
		{
			ShowQuestionDialog(playerid);
		}
		else
		{
			ShowLoginDialog(playerid);
		}
	}
	else
	{
		ShowPlayerZeroDialog(playerid, "languagechooser");
	}
}

ZERO_DIALOG(languagechooserb)
{
	if (response && listitem >= 0 && listitem < CURRENT_LANGUAGE_ID)
	{
		Player[playerid].LanguageID = IndexToLanguageID[listitem];
		Player[playerid].Achievementdata.DidPlayerSelectLanguage = 1;
		Player[playerid].LanguageChosen = true;
		Player[playerid].Achievementdata.CurrentlySelectedLanguage = Player[playerid].LanguageID;

		AddPlayerToLanguage(playerid, listitem);
	}
}

ZCMD(language, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/jêzyk", "/jezyk", "/jenzyk", "/lang", "/taal", "/setlanguage", "/changelang", "/setlang", "/changelanguage" }))
{
	ShowPlayerZeroDialog(playerid, "languagechooserb");
	return true;
}

ZERO_DIALOG(login_pomoc)
{
	if (response)
	{
		if (inputtext.compare(Player[playerid].statistics.verifiedmail) != 0 || Player[playerid].statistics.verifiedmail.size() == 0)
		{
			ShowPlayerCustomDialog(playerid, "login_pomoc", DIALOG_STYLE_INPUT, TranslateP(playerid, L_loginhelp_askmail_title), TranslateP(playerid, L_loginhelp_askmail_text), TranslateP(playerid, L_loginhelp_askmail_button_a), TranslateP(playerid, L_loginhelp_askmail_button_b));
			return;
		}
		unsigned long long TimeNow = Functions::GetTime();
		if (TimeNow - Player[playerid].statistics.lastmailsent < 1 * 15 * 60 * 1000)
		{
			fixSendClientMessage(playerid, -1, L_loginhelp_antyspam);
		}
		else
		{
			Player[playerid].TemporaryRecoveryCode = CreateAuthenticationCodeForPlayer(playerid);
			SendMail(0x7733, Player[playerid].statistics.verifiedmail, "recovery", "&nickname=" + Player[playerid].PlayerName + "&authcode=" + Player[playerid].TemporaryRecoveryCode + "&setip=" + Player[playerid].ipv4.to_string(), Player[playerid].PlayerName, Languages[Player[playerid].Language]);
			Player[playerid].statistics.lastmailsent = TimeNow;
			Player[playerid].PlayerDisconnect(true, true);//only save data
			fixSendClientMessage(playerid, -1, L_loginhelp_checkmail);
		}
		TestIfAllowed(playerid);
	}
	else
	{
		sampgdk_Kick(playerid);
	}
}

ZERO_DIALOG(login)
{
	if (!response)
	{
		//Kick(playerid);
		if (Player[playerid].statistics.verifiedmail.size())
		{
			ShowPlayerCustomDialog(playerid, "login_pomoc", DIALOG_STYLE_INPUT, TranslateP(playerid, L_loginhelp_askmail_title), TranslateP(playerid, L_loginhelp_askmail_text), TranslateP(playerid, L_loginhelp_askmail_button_a), TranslateP(playerid, L_loginhelp_askmail_button_b));
		}
		else
		{
			fixSendClientMessage(playerid, -1, L_login_noverifiedmail);
			TestIfAllowed(playerid);
		}
	}
	else
	{
		
		bool passwordokay = boost::iequals(Player[playerid].statistics.password, PlayerPasswordHash(playerid, inputtext));
		bool recoveryokay = (Player[playerid].statistics.verifiedrecoverycode.size() != 0 && !Player[playerid].statistics.verifiedrecoverycode.compare(inputtext));
		bool temprecoveryokay = (Player[playerid].TemporaryRecoveryCode.size() != 0 && !Player[playerid].TemporaryRecoveryCode.compare(inputtext));
	
#ifdef _LOCALHOST_DEBUG
		passwordokay = true;
#endif

		if (passwordokay || recoveryokay || temprecoveryokay)
		{
			unsigned long long TimeNow = Functions::GetTime();
			if (Player[playerid].TemporaryRecoveryCode.size())
			{
				Player[playerid].TemporaryRecoveryCode = "";
			}

			Player[playerid].IsLoggedIn = true;
			if (Player[playerid].statistics.verifiedmail.size() == 0)
			{
				ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_login_dialog_title), TranslateP(playerid, L_login_welcomeback), "V", "X");
			}
			gtLog(LOG_LOGIN, Functions::string_format("[%d][%s][%s] logged in success, privilidges: %08X", playerid, Player[playerid].PlayerName.c_str(), Player[playerid].ipv4.to_string().c_str(), Player[playerid].statistics.privilidges));
			Player[playerid].SetMoney(Player[playerid].statistics.money);
			Player[playerid].SetScore(Player[playerid].statistics.respect);
			Player[playerid].statistics.lastip = Player[playerid].ipv4.to_string();
			if (!Player[playerid].ShownConnectMessage)
			{
				if (Player[playerid].statistics.staticcolor)
				{
					Player[playerid].Color = Player[playerid].statistics.staticcolor;
					Player[playerid].UpdateFullColor();
				}
				SetPlayerColor(playerid, Player[playerid].FullColor);
				Player[playerid].ShownConnectMessage = true;
				Player[playerid].statistics.lastconnected = TimeNow;
				RearrangePermissions(playerid);
				extOnPlayerGameBegin(playerid);
			}
			if (Player[playerid].statistics.SkinModelID != 10000)
			{
				ClearAnimations(playerid, false);
				PlayerPlaySound(playerid, MUSIC_OFF, 0.0, 0.0, 0.0);
				Player[playerid].DoFirstSpawn = true;
				SpawnPlayerAnyway(playerid);
			}
			if (recoveryokay)
			{
				Player[playerid].statistics.verifiedrecoverycode = CreateAuthenticationCodeForPlayer(playerid);
				SendMail(0x7733, Player[playerid].statistics.verifiedmail, "reactloggedin", "&nickname=" + Player[playerid].PlayerName + "&authcode=" + Player[playerid].statistics.verifiedrecoverycode + "&regip=" + Player[playerid].statistics.registerip + "&setip=" + Player[playerid].ipv4.to_string(), Player[playerid].PlayerName, Languages[Player[playerid].Language]);
				Player[playerid].PlayerDisconnect(true);
			}

			//gracz ma VIP-a?
			if (!bIsBitEnabled(Player[playerid].statistics.privilidges, PERMISSION_VIP))
			{//nie ma
				//sprawdzenie czy gracz spe³nia wymagania:
				//-czas grania: 30h lub wiecej
				//-Level: 10.0 lub wiekszy
				//-E-Mail: zweryfikowany
				//-Zarejestrowany: ponad 21 dni
				//-Znalezione minimalnie 30 z³otych monet
				if (
					(Player[playerid].statistics.playtime >= (30 * 60 * 60)) && 
					(Player[playerid].GetLevel() >= 10.0) && 
					(Player[playerid].statistics.verifiedmail.size()) && 
					((TimeNow-Player[playerid].statistics.registertime) > (1000*60*60*24*21)) &&
					Player[playerid].Achievementdata.GoldCoins.count() >= 30
					)
				{//tak, spe³nia
					//daj VIP-a na sta³e
					Player[playerid].statistics.privilidges |= PERMISSION_VIP;
					fixSendClientMessage(playerid, 0xFF0000FF, L_importantmessage);
					fixSendClientMessage(playerid, 0x00FF00FF, L_vip_upgrade);
					SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_vip_upgrade);
				}
			}
		}
		else
		{
			ShowLoginDialog(playerid);
		}
	}
}

ZERO_DIALOG(register)
{
	if (!Player[playerid].ShownConnectMessage)
	{
		SetPlayerColor(playerid, Player[playerid].FullColor);
		Player[playerid].ShownConnectMessage = true;
		extOnPlayerGameBegin(playerid);
	}
	if (!response)
	{
		ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_information), TranslateP(playerid, L_register_canceled), "V", "X");
	}
	else
	{
		ShowRegisterDialog(playerid);
	}
}

ZERO_DIALOG(register2)
{
	if (Player[playerid].IsRegistered)
	{
		fixSendClientMessage(playerid, -1, L_already_registered);
		return;
	}
	if (!response)
	{
		ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_information), TranslateP(playerid, L_register_canceled), "V", "X");
		return;
	}
	if (inputtext.size() < 5)
	{
		ShowRegisterDialog(playerid);
		return;
	}

	std::string lowercasename = Player[playerid].PlayerName;
	boost::algorithm::to_lower(lowercasename);

	Player[playerid].statistics.nickname = lowercasename;
	Player[playerid].IsLoggedIn = true;
	Player[playerid].statistics.registertime = Functions::GetTime();
	Player[playerid].statistics.lastconnected = Player[playerid].statistics.registertime;
	Player[playerid].statistics.password = PlayerPasswordHash(playerid, inputtext);
	Player[playerid].statistics.displayname = Player[playerid].PlayerName;
	Player[playerid].statistics.registerip = Player[playerid].ipv4.to_string();
	Player[playerid].statistics.lastip = Player[playerid].ipv4.to_string();
	Player[playerid].LoadStats(0, true, true);
	Player[playerid].PlayerDisconnect(true);//only save data

	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_information), TranslatePF(playerid, L_register_success, inputtext.c_str()), "V", "X");
	gtLog(LOG_REGISTER, Functions::string_format("[%d][%s] registered", playerid, Player[playerid].PlayerName.c_str()));
}

ZCMD(rejestracja, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({"/register", "/createaccount", "/maakaccount", "/accountmaken", "/accountcreate", "/registreren"}))
{
	if (Player[playerid].IsRegistered)
	{
		fixSendClientMessage(playerid, -1, L_already_registered);
		return true;
	}
	ShowRegisterDialog(playerid);
	return true;
}

ZCMD(newpass, PERMISSION_NONE, RESTRICTION_REGISTERED_AND_LOADED, cmd_alias({ "/nowehaslo", "/newpassword" }))
{
	ShowUpdatePasswordDialog(playerid);
	return true;
}

ZERO_DIALOG(updatepasswd)
{
	if (response)
	{
		if (inputtext.size() < 5)
		{
			ShowUpdatePasswordDialog(playerid);
			return;
		}
		Player[playerid].statistics.password.assign(PlayerPasswordHash(playerid, inputtext));
		Player[playerid].PlayerDisconnect(true);
		ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_newpass_success_title), TranslatePF(playerid, L_newpass_success_text, inputtext.c_str()), "V", "X");
		fixSendClientMessage(playerid, -1, info);
		gtLog(LOG_AUDIT, Functions::string_format("[%d][%s] changed password", playerid, Player[playerid].PlayerName.c_str()));
	}
}

////email stuff////
void ShowEmailManagementDialog(int playerid)
{
	unsigned long long TimeNow = Functions::GetTime();

	std::string toshow;

	int changemail_color = (TimeNow - Player[playerid].statistics.lastmailsent < 1 * 15 * 60 * 1000) ? 0xFF0000 : 0x00FF00;
	int verifymail_color = (Player[playerid].statistics.mailverifiedtime || Player[playerid].statistics.email.size() == 0) ? 0xFF0000 : 0x00FF00;
	int sendverify_color = (TimeNow - Player[playerid].statistics.verificationsent < 1 * 15 * 60 * 1000 || Player[playerid].statistics.lastmailsent == 0) ? 0xFF0000 : 0x00FF00;

	ShowPlayerCustomDialog(playerid, "email_action_choose", DIALOG_STYLE_LIST, TranslateP(playerid, L_manageemail_title), TranslatePF(playerid, L_manageemail_text, changemail_color, verifymail_color, sendverify_color), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
}

ZCMD(email, PERMISSION_NONE, RESTRICTION_REGISTERED_AND_LOADED, cmd_alias({ "/nowyemail", "/mail", "/newmail", "/newemail" }))
{
	ShowEmailManagementDialog(playerid);
	return true;
}

ZERO_DIALOG(email_action_change)
{
	if (!response)
	{
		return;
	}
	unsigned long long TimeNow = Functions::GetTime();
	size_t at_sign = inputtext.find("@");
	size_t dot = inputtext.find_last_of(".");
	if (inputtext.size() < 5 || at_sign == std::string::npos || dot == std::string::npos || dot < at_sign)
	{
		ShowPlayerCustomDialog(playerid, "email_action_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_action_change_title), TranslateP(playerid, L_email_action_change_text_a), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
		return;
	}
	if (inputtext.compare(Player[playerid].statistics.email) == 0 || inputtext.compare(Player[playerid].statistics.verifiedmail) == 0)
	{
		ShowPlayerCustomDialog(playerid, "email_action_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_action_change_title), TranslateP(playerid, L_email_action_change_text_b), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
		return;
	}
	Player[playerid].statistics.email = inputtext;
	Player[playerid].statistics.recoverycode = CreateAuthenticationCodeForPlayer(playerid);
	Player[playerid].statistics.mailverifiedtime = 0;
	Player[playerid].statistics.lastmailsent = TimeNow;
	Player[playerid].statistics.verificationsent = TimeNow;

	Player[playerid].PlayerDisconnect(true);//only save data

	SendMail(0x7733, inputtext, "emailchange", "&nickname=" + Player[playerid].PlayerName + "&authcode=" + Player[playerid].statistics.recoverycode + "&regip=" + Player[playerid].statistics.registerip + "&setip=" + Player[playerid].ipv4.to_string(), Player[playerid].PlayerName, Languages[Player[playerid].Language]);
	std::string str_info(TranslatePF(playerid, L_email_action_success_text, inputtext.c_str()));
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_email_action_success_title), str_info, "V", "X");
	fixSendClientMessage(playerid, -1, str_info);
	gtLog(LOG_AUDIT, Functions::string_format("[%d][%s] changed email", playerid, Player[playerid].PlayerName.c_str())); 
}

ZERO_DIALOG(email_action_verify_owner_before_change)
{
	if (response)
	{
		if (inputtext.compare(Player[playerid].statistics.verifiedrecoverycode) != 0)
		{
			ShowPlayerCustomDialog(playerid, "email_action_verify_owner_before_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_action_change_title), TranslateP(playerid, L_email_change_verify_fail), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
			return;
		}
		ShowPlayerCustomDialog(playerid, "email_action_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_action_change_title), TranslateP(playerid, L_email_change_newemail), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
	}
}

ZERO_DIALOG(email_action_verify_email_change)
{
	if (response)
	{
		if (inputtext.compare(Player[playerid].statistics.recoverycode) != 0)
		{
			ShowPlayerCustomDialog(playerid, "email_action_verify_email_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_verify_title), TranslateP(playerid, L_email_verify_text), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
			return;
		}
		Player[playerid].statistics.verifiedrecoverycode = Player[playerid].statistics.recoverycode;
		Player[playerid].statistics.recoverycode = "";
		Player[playerid].statistics.verifiedmail = Player[playerid].statistics.email;
		Player[playerid].statistics.email = "";
		Player[playerid].statistics.mailverifiedtime = Functions::GetTime();

		Player[playerid].PlayerDisconnect(true);

		fixSendClientMessage(playerid, -1, L_email_verify_success);
	}
}

ZERO_DIALOG(email_action_choose)
{
	unsigned long long TimeNow = Functions::GetTime();
	if (response)
	{
		switch (listitem)
		{
		case 0:
//#ifndef _LOCALHOST_DEBUG
			if (TimeNow - Player[playerid].statistics.lastmailsent < 1 * 15 * 60 * 1000)
			{
				fixSendClientMessage(playerid, -1, L_email_activation_limit);
				ShowEmailManagementDialog(playerid);
				return;
			}
//#endif		
			if (Player[playerid].statistics.verifiedmail.size())
			{
				if (TimeNow - Player[playerid].statistics.verificationsent >= 1 * 15 * 60 * 1000)
				{
					SendMail(0x7733, Player[playerid].statistics.verifiedmail, "emailchange", "&nickname=" + Player[playerid].PlayerName + "&authcode=" + Player[playerid].statistics.verifiedrecoverycode + "&regip=" + Player[playerid].statistics.registerip + "&setip=" + Player[playerid].ipv4.to_string(), Player[playerid].PlayerName, Languages[Player[playerid].Language]);
					Player[playerid].statistics.verificationsent = TimeNow;
				}
				ShowPlayerCustomDialog(playerid, "email_action_verify_owner_before_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_action_change_title), TranslateP(playerid, L_email_change_verify_text_a), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
			}
			else
			{
				ShowPlayerCustomDialog(playerid, "email_action_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_action_change_title), TranslateP(playerid, L_email_change_newemail), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
			}
			break;
		case 1:
			if (Player[playerid].statistics.mailverifiedtime || Player[playerid].statistics.email.size() == 0)
			{
				fixSendClientMessage(playerid, -1, L_email_verification_error);
				ShowEmailManagementDialog(playerid);
				return;
			}
			ShowPlayerCustomDialog(playerid, "email_action_verify_email_change", DIALOG_STYLE_INPUT, TranslateP(playerid, L_email_verify_title), TranslateP(playerid, L_email_change_verify_text_b), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
			break;
		case 2:
			if (Player[playerid].statistics.mailverifiedtime || Player[playerid].statistics.email.size() == 0)
			{
				fixSendClientMessage(playerid, -1, L_email_verification_error);
				ShowEmailManagementDialog(playerid);
				return;
			}
			//#ifndef _LOCALHOST_DEBUG
			if (TimeNow - Player[playerid].statistics.lastmailsent < 1 * 60 * 60 * 1000)
			{
				fixSendClientMessage(playerid, -1, L_email_activation_limit);
				ShowEmailManagementDialog(playerid);
				return;
			}
			//#endif
			SendMail(0x7733, Player[playerid].statistics.email, "emailchange", "&nickname=" + Player[playerid].PlayerName + "&authcode=" + Player[playerid].statistics.verifiedrecoverycode + "&regip=" + Player[playerid].statistics.registerip + "&setip=" + Player[playerid].ipv4.to_string(), Player[playerid].PlayerName, Languages[Player[playerid].Language]);
			Player[playerid].statistics.lastmailsent = TimeNow;
			break;
		default:
			ShowEmailManagementDialog(playerid);
			break;
		}
	}
}