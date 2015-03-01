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

std::unordered_set<int> Bountiies;

class HitmanListProcessor : public Extension::Base
{
public:
	void OnPlayerGameBegin(int playerid)
	{
		if (Player[playerid].statistics.bounty)
			Bountiies.insert(playerid);
	}
	bool OnPlayerDeath(int playerid, int killerid, int reason)
	{
		if (killerid != INVALID_PLAYER_ID)
			Bountiies.erase(playerid);
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		Bountiies.erase(playerid);
		return true;
	}
} _HitmanListProcessor;


ZCMDF(givebounty, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/glowa", "/g³owa", "/hitman" }), "pu")
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
					Player[giveid].statistics.bounty += money;
					Bountiies.insert(giveid);
					fixSendClientMessageF(playerid, Color::COLOR_INFO2, L_givebounty_sendtag, true, true, money, Player[giveid].statistics.bounty);
					fixSendClientMessageF(giveid, Color::COLOR_INFO3, L_givebounty_headtagged, true, true, money, Player[playerid].PlayerName.c_str(), Player[giveid].statistics.bounty);

					SafeAddULL(Player[playerid].Achievementdata.AchievementsMain[EAM_GivenTotalBounty], money);
					SafeAddULL(Player[giveid].Achievementdata.AchievementsMain[EAM_HadTotalBounty], money);

					SendClientMessageToAllF(Color::COLOR_ERROR, L_givebounty_pricechange, false, false, Player[giveid].PlayerName.c_str(), Player[giveid].statistics.bounty);
					gtLog(LOG_OTHER, Functions::string_format("[%d][%s] givebounty '%s'", playerid, Player[playerid].PlayerName.c_str(), params.c_str()));
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
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_givebounty_usage);
	return true;
}

ZCMD(hitmanlista, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/hitman.lista", "/hitmanlist", "/hitman.list", "/bounty.list", "/bountylist", "/bounties" }))
{
	std::string toshow;
	for (auto&i : Bountiies)
		toshow.append(Functions::string_format("\t%-3d " + Player[i].PlayerName + " $%I64u\n", i, Player[i].statistics.bounty));
	if (!toshow.size())
		toshow.append(TranslateP(playerid, L_bounty_noheadsonline));
	toshow.insert(0, TranslateP(playerid, L_bounty_preinfo));
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_bounty_title), toshow, "V", "X");
	return true;
}