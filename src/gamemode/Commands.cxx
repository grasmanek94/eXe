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

unsigned char GetCommandPermissionGameID(int restrictions)
{
	return (restrictions & 0x3FC00) >> 10;
}

int SetCommandPermissionGameID(int restriction, unsigned char GameID)
{
	bSetBitFalse(restriction,
		RESERVED_BIT1_FOR_GAME_ID |
		RESERVED_BIT2_FOR_GAME_ID |
		RESERVED_BIT3_FOR_GAME_ID |
		RESERVED_BIT4_FOR_GAME_ID |
		RESERVED_BIT5_FOR_GAME_ID |
		RESERVED_BIT6_FOR_GAME_ID |
		RESERVED_BIT7_FOR_GAME_ID |
		RESERVED_BIT8_FOR_GAME_ID
		);
	return restriction | (GameID << 10);
}

ZCMDF(skin, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_ON_FOOT, cmd_alias({"/skinid", "/newskin", "/setskin" }), "D")
{
	if (parser.Good())
	{
		unsigned long skinid = parser.Get<unsigned long>();
		if (skinid < 300 && skinid != 74)
		{
			SetPlayerSkin(playerid, skinid);
			Player[playerid].statistics.SkinModelID = skinid;
			return true;
		}
	}
	fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_setskin_usage, true, true, GetPlayerSkin(playerid));
	return true;
}

ZCMDF(givecash, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/dajkase", "/geefgeld", "/givemoney" }), "pu")
{
	if (parser.Good() == 2)
	{
		int giveid = parser.Get<ParsePlayer>(0).playerid;
		long long money = parser.Get<long long>(1);

		if (giveid != INVALID_PLAYER_ID && playerid != giveid)
		{
			if (money >= 25000)
			{
				if (Player[playerid].statistics.money >= money)
				{
					Player[playerid].GiveMoney(-money);
					Player[giveid].GiveMoney(money);

					fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_givecash_success_1, true, true, money, Player[playerid].statistics.money);
					fixSendClientMessageF(giveid, Color::COLOR_INFO2, L_givecash_success_2, true, true, money, Player[playerid].PlayerName.c_str(), Player[giveid].statistics.money);

					gtLog(LOG_OTHER, Functions::string_format("[%d][%s] givecash '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
					return true;
				}
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_error_insufficient_funds);
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_funds_amount);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_givecash_usage, true, true, Player[playerid].statistics.money);
	return true;
}

ZCMDF(giverespect, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/dajrespekt", "/givescore", "/giverespekt" }), "pu")
{
	if (parser.Good() == 2)
	{
		int giveid = parser.Get<ParsePlayer>(0).playerid;
		long long respect = parser.Get<long long>(1);

		if (giveid != INVALID_PLAYER_ID && playerid != giveid)
		{
			if (respect > 0)
			{
				if (Player[playerid].statistics.respect >= respect)
				{
					Player[playerid].GiveScore(-respect);
					Player[giveid].GiveScore(respect);

					fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_givescore_success_1, true, true, respect, Player[playerid].statistics.respect);
					fixSendClientMessageF(giveid, Color::COLOR_INFO2, L_givescore_success_2, true, true, respect, Player[playerid].PlayerName.c_str(), Player[giveid].statistics.respect);

					gtLog(LOG_OTHER, Functions::string_format("[%d][%s] giverespect '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
					return true;
				}
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_givescore_error);
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mafia_givescore_scoreerror);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_givescore_usage, true, true, Player[playerid].statistics.respect);
	return true;
}

ZCMDF(agivecash, PERMISSION_GAMER, RESTRICTION_NONE, cmd_alias({ "/adajkase" }), "pu")
{
	if (parser.Good() == 2)
	{
		int giveid = parser.Get<ParsePlayer>(0).playerid;
		long long money = parser.Get<long long>(1);

		if (giveid != INVALID_PLAYER_ID)
		{
			Player[giveid].GiveMoney(money);

			fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_agivecash_success_1, true, true, money, giveid, Player[giveid].statistics.money);
			fixSendClientMessageF(giveid, Color::COLOR_INFO2, L_agivecash_success_2, true, true, money, Player[playerid].PlayerName.c_str(), Player[giveid].statistics.money);

			gtLog(LOG_OTHER, Functions::string_format("[%d][%s] agivecash '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_agivecash_usage, true, true, Player[playerid].statistics.money);
	return true;
}

ZCMDF(agiverespect, PERMISSION_GAMER, RESTRICTION_NONE, cmd_alias({ "/adajrespekt", "/agivescore" }), "pu")
{
	if (parser.Good() == 2)
	{
		int giveid = parser.Get<ParsePlayer>(0).playerid;
		long long respect = parser.Get<long long>(1);

		if (giveid != INVALID_PLAYER_ID)
		{
			Player[giveid].GiveScore(respect);

			fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_agivescore_success_1, true, true, respect, giveid, Player[giveid].statistics.respect);
			fixSendClientMessageF(giveid, Color::COLOR_INFO2, L_agivescore_success_2, true, true, respect, Player[playerid].PlayerName.c_str(), Player[giveid].statistics.respect);

			gtLog(LOG_OTHER, Functions::string_format("[%d][%s] agiverespect '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_agivescore_usage, true, true, Player[playerid].statistics.respect);
	return true;
}

ZCMDF(agiveexp, PERMISSION_GAMER, RESTRICTION_NONE, cmd_alias({ "/adajexp" }), "pu")
{
	if (parser.Good() == 2)
	{
		int giveid = parser.Get<ParsePlayer>(0).playerid;
		long long exp = parser.Get<long long>(1);

		if (giveid != INVALID_PLAYER_ID)
		{
			Player[giveid].GiveExperience(exp);

			fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_agiveexp_success_1, true, true, exp, giveid, Player[giveid].statistics.experience);
			fixSendClientMessageF(giveid, Color::COLOR_INFO2, L_agiveexp_success_2, true, true, exp, Player[playerid].PlayerName.c_str(), Player[giveid].statistics.experience);

			gtLog(LOG_OTHER, Functions::string_format("[%d][%s] agiveexp '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_agiveexp_usage, true, true, Player[playerid].statistics.experience);
	return true;
}


ZCMD(100hp, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/heal", "/hp", "/ulecz", "/zycie" }))
{
	GivePlayerAchievement(playerid, EAM_NononononoDead, 1);
	fixSetPlayerHealth(playerid, 100.0);
	return true;
}

ZCMD(armor, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/armour", "/kamizelka", "/pancerz", "/pancez" }))
{
	GivePlayerAchievement(playerid, EAM_NononononoArmour, 1);
	fixSetPlayerArmour(playerid, 100.0);
	return true;
}

ZCMD(napraw, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_DRIVER, cmd_alias({ "/fix", "/repair" }))
{
	if (Player[playerid].CurrentVehicleModel == 425)
	{
		float hp;
		GetVehicleHealth(Player[playerid].CurrentVehicle, &hp);
		if (hp < 250.0)
		{
			Player[playerid].LastDamageTime = Functions::GetTime() + 7000;
			CheckCommandAllowed(playerid, RESTRICTION_NOT_AFTER_FIGHT);
			return false;
		}
	}
	safeRepairVehicle(Player[playerid].CurrentVehicle);
	return true;
}

ZCMD(all, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/wszystko" }))
{
	GivePlayerAchievement(playerid, EAM_NononononoDead, 1);
	if (Player[playerid].IsDriver)
	{
		safeRepairVehicle(Player[playerid].CurrentVehicle);
	}
	fixSetPlayerArmour(playerid, 100.0);
	fixSetPlayerHealth(playerid, 100.0);
	return true;
}

ZCMD(clearchat, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/clear-chat", "/wyczysc-czat", "/clean-chat", "/cleanchat" }))
{
	for (size_t i = 0; i < 20; ++i)
	{
		fixSendClientMessage(playerid, -1, " ");
	}
	return true;
}

ZCMD(savepos, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/zpos", "/zapiszpozycje", "/saveloc" }))
{
	if (GetPlayerInterior(playerid) == 0)
	{
		if (!Player[playerid].CurrentVehicle)
		{
			GetPlayerPos(playerid, &Player[playerid].statistics.SaveX, &Player[playerid].statistics.SaveY, &Player[playerid].statistics.SaveZ);
			GetPlayerFacingAngle(playerid, &Player[playerid].statistics.SaveA);
		}
		else
		{
			GetVehiclePos(Player[playerid].CurrentVehicle, &Player[playerid].statistics.SaveX, &Player[playerid].statistics.SaveY, &Player[playerid].statistics.SaveZ);
			GetVehicleZAngle(Player[playerid].CurrentVehicle, &Player[playerid].statistics.SaveA);
		}
		fixSendClientMessage(playerid, Color::COLOR_INFO3, L_savepos_success);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_error_cant_usecmd_atm);
	return true;
}

ZCMD(loadpos, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT, cmd_alias({ "/lpos", "/ladujpozycje", "/loadloc" }))
{
	if (GetPlayerInterior(playerid) == 0)
	{
		TeleportPlayer(playerid, Player[playerid].statistics.SaveX, Player[playerid].statistics.SaveY, Player[playerid].statistics.SaveZ, Player[playerid].statistics.SaveA, params.size() > 0, 0, 0, "", 0, 0, 0.0, 0.0, false, true);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_error_cant_usecmd_atm);
	return true;
}

ZCMDF(color, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/kolor", "/stalykolor", "/setcolor", "/ustawkolor" }), "X")
{
	if (parser.Good())
	{
		unsigned long color = parser.Get<unsigned long>();
		if (color > 0xFFFFFF)
		{
			color = 0xFFFFFF;
		}
		if (color == 0)
		{
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_setcolor_removed);
			Player[playerid].statistics.staticcolor = color;
			if (Player[playerid].Mafia == nullptr)
			{
				Player[playerid].Color = Functions::RandomGenerator->Random(0x000000, 0xFFFFFF);
				Player[playerid].UpdateFullColor();
				SetPlayerColor(playerid, Player[playerid].FullColor);
			}
			return true;
		}
		else if (Functions::GetColorBrightness(color) >= 33)
		{
			Player[playerid].statistics.staticcolor = color;
			if (Player[playerid].Mafia == nullptr)
			{
				Player[playerid].Color = Player[playerid].statistics.staticcolor;
				Player[playerid].UpdateFullColor();
				SetPlayerColor(playerid, Player[playerid].FullColor);
			}
			fixSendClientMessage(playerid, Color::COLOR_INFO3, L_mafia_color_setsuccess);
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_color_too_dark_error);
		return true;
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_setcolor_usage, true, true, Player[playerid].Color);
	return true;
}

//TODO fix possible ip ad bug (though a VIP is very unlikely to spam IP's on the chat)
ZCMD(say, PERMISSION_NONE, RESTRICTION_IS_NOT_CHAT_MUTED, cmd_alias({ "/asay", "/vsay", "/ssay", "/msay", "/me" }))
{
	if (params.size() && params.size() < 111)
	{
		unsigned int MSB = GetMSB(Player[playerid].statistics.privilidges);
		switch (MSB)
		{
		case PERMISSION_NONE:
			OnPlayerText(playerid, params.c_str());
			return true;
		case PERMISSION_VIP:
			fixSendClientMessageToAll(-1, Functions::string_format("{EAF20A}%-3d {%06X}" + Player[playerid].PlayerName + " {EAF20A}(VIP){FFFFFF}: " + params, playerid, Player[playerid].Color), false, false);
			return true;
		case PERMISSION_SPONSOR:
			fixSendClientMessageToAll(-1, Functions::string_format("{E03DBA}%-3d {%06X}" + Player[playerid].PlayerName + " {E03DBA}(Sponsor){FFFFFF}: " + params, playerid, Player[playerid].Color), false, false);
			return true;
		case PERMISSION_MODERATOR:
			fixSendClientMessageToAll(-1, Functions::string_format("{B1E334}%-3d {%06X}" + Player[playerid].PlayerName + " {B1E334}(Mod){FFFFFF}: " + params, playerid, Player[playerid].Color), false, false);
			return true;
		default://admins
			fixSendClientMessageToAll(-1, Functions::string_format("{E8A854}%-3d {%06X}" + Player[playerid].PlayerName + " {E8A854}(Admin){FFFFFF}: " + params, playerid, Player[playerid].Color), false, false);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_say_usage);
	return true;
}

ZCMD3(a, PERMISSION_ADMIN, RESTRICTION_NONE)
{
	if (params.size() && params.size() < 136)
	{
		std::string sendmsg("@" + std::string(params.c_str()));
		OnPlayerText(playerid, sendmsg.c_str());
	}
	else
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_asay_usage);
	}
	return true;
}

ZCMD3(m, PERMISSION_MODERATOR, RESTRICTION_NONE)
{
	if (params.size() && params.size() < 136)
	{
		std::string sendmsg("&" + std::string(params.c_str()));
		OnPlayerText(playerid, sendmsg.c_str());
	}
	else
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_msay_usage);
	}
	return true;
}

ZCMD3(l, PERMISSION_NONE, RESTRICTION_NONE)
{
	if (params.size() && params.size() < 136)
	{
		std::string sendmsg("|" + std::string(params.c_str()));
		OnPlayerText(playerid, sendmsg.c_str());
	}
	else
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_lsay_usage);
	}
	return true;
}

ZCMDF(przelew, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/wplac", "/wyplac", "/bank", "/transfer" }), "u")
{
	if (parser.Good())
	{
		long long money = parser.Get<long long>(0);
		if (money < 0)
		{
			money = money*-1;
			if (money <= Player[playerid].statistics.bank)
			{
				Player[playerid].GiveMoney(money);
				Player[playerid].statistics.bank -= money;
				fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_bank_withdraw_success, true, true, money, Player[playerid].statistics.bank);

				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_bank_nofunds_bank);
			return true;
		}
		else
		{
			if (money <= Player[playerid].statistics.money)
			{
				Player[playerid].GiveMoney(-money);
				Player[playerid].statistics.bank += money;
				fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_bank_transfer_success, true, true, money, Player[playerid].statistics.bank);

				return true;

			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_bank_nofunds_here);
			return true;
		}
	}
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_bank_usage_1, true, true, Player[playerid].statistics.bank);
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_bank_usage_2);
	return true;
}
