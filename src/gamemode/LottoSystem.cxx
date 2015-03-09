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

unsigned long long JackPot;
std::array<std::bitset<64>, MAX_PLAYERS> PlayerChosenNumbers;
std::set<int> Participants;

void LottoTimer(int timerid, void* param)
{
	std::array<size_t, MAX_PLAYERS> howmanyhits;
	SecureZeroMemory(&howmanyhits, MAX_PLAYERS*sizeof(size_t));

	std::bitset<64> LottoResult;
	int numbers[4] = { 0, 0, 0, 0 };
	int currentset = 0;
	while (LottoResult.count() < 4)
	{
		size_t i = Functions::RandomGenerator->Random(0, 60);
		if (LottoResult[i])
		{
			if (LottoResult[i + 1])
			{
				if (LottoResult[i + 2])
				{
					LottoResult[i + 3] = true;
					numbers[currentset++] = i + 3;
				}
				else
				{
					LottoResult[i + 2] = true;
					numbers[currentset++] = i + 2;
				}
			}
			else
			{
				LottoResult[i + 1] = true;
				numbers[currentset++] = i + 1;
			}
		}
		else
		{
			LottoResult[i] = true;
			numbers[currentset++] = i;
		}
	}
	size_t totalhits = 0;
	for (auto i : Participants)
	{
		howmanyhits[i] = (LottoResult & PlayerChosenNumbers[i]).count();
		totalhits += howmanyhits[i];
		PlayerChosenNumbers[i] = 0;
	}
	unsigned long long cash_per_hit = JackPot / (totalhits == 0 ? 1 : totalhits);
	for (auto i : Participants)
	{
		if (howmanyhits[i])
		{
			fixSendClientMessageF(i, -1, L_lotto_win, true, true, howmanyhits[i], cash_per_hit*howmanyhits[i]);
			Player[i].GiveMoney(cash_per_hit*howmanyhits[i]);
			GivePlayerAchievement(i, EAM_WygraneLotto, 1);
			GivePlayerAchievement(i, EAM_SumaWygranaLotto, cash_per_hit*howmanyhits[i]);
		}
		else
		{
			fixSendClientMessage(i, -1, L_lotto_lose);
		}
	}
	Participants.clear();
	if (totalhits)
	{
		JackPot = 0;
		fixSendClientMessageToAllF(-1, L_lotto_winannounce, true, true, numbers[0], numbers[1], numbers[2], numbers[3]);
	}
	else
	{
		JackPot += 2500;
		fixSendClientMessageToAllF(-1, L_lotto_loseannounce, true, true, JackPot, numbers[0], numbers[1], numbers[2], numbers[3]);
	}
}

class LottoSystemProcessor : public Extension::Base
{
public:
	LottoSystemProcessor() : Base(ExecutionPriorities::LottoSystem){}//make sure we come before save system in onplayerdisconnect?
	bool OnGameModeInit() override
	{
		sampgdk_SetTimerEx(900000, true, LottoTimer, nullptr, nullptr);
		return true;
	}
	bool OnPlayerConnect(int playerid) override
	{
		PlayerChosenNumbers[playerid] = 0;
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		auto ptr = Participants.find(playerid);
		if (ptr != Participants.end())
		{
			Player[playerid].GiveMoney(PlayerChosenNumbers[playerid].count() * 750);//because this
			SafeSubULL(JackPot, (PlayerChosenNumbers[playerid].count() * 750));
			Participants.erase(ptr);
		}
		return true;
	}
} _LottoSystemProcessor;

ZCMDF(lotto, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({}), "iiiiiiii")
{
	if (parser.Good())
	{
		if (!PlayerChosenNumbers[playerid].count())
		{
			if ((parser.Good() * 750) <= Player[playerid].statistics.money)
			{
				for (size_t i = 0; i < parser.Good(); ++i)
				{
					signed long temp = parser.Get<signed long>(i);
					if (temp >= 0 && temp < 64)
					{
						if (!PlayerChosenNumbers[playerid][temp])
						{
							PlayerChosenNumbers[playerid][temp] = true;
						}
						else
						{
							PlayerChosenNumbers[playerid] = 0;
							fixSendClientMessage(playerid, -1, L_lotto_number_error);
							return true;
						}
					}
					else
					{
						PlayerChosenNumbers[playerid] = 0;
						fixSendClientMessage(playerid, -1, L_lotto_number_error);
						return true;
					}
				}
				long long temp = (signed long long)(parser.Good() * 750);
				Player[playerid].GiveMoney(-temp);
				JackPot += (unsigned long long)temp;
				Participants.insert(playerid);
				fixSendClientMessage(playerid, -1, L_lotto_send_success);
				return true;
			}
			fixSendClientMessage(playerid, -1, L_error_insufficient_funds);
			return true;
		}
		fixSendClientMessage(playerid, -1, L_lotto_already_submitted);
		return true;
	}
	fixSendClientMessage(playerid, -1, L_lotto_usage_1);
	fixSendClientMessage(playerid, -1, L_lotto_usage_2);
	return true;
}