/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implements the accuracy of shots calculation for a player

	________________________________________________________________	
	Notes
		-	Only counts bullets allowed through the AntiCheat
	________________________________________________________________	
	TODO

	________________________________________________________________
	Dependencies
		Extension

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

class AccuracyData
{
	static const unsigned short max_shots = 256;
	static const unsigned short max_shots_min_one = max_shots - 1;

	std::bitset<max_shots> shot_state;
	unsigned short shots_fired;
	unsigned short current_shot;
public:
	AccuracyData()
	{
		Reset();
	}
	void Reset()
	{
		shots_fired = 0;
		current_shot = 0;
		shot_state.reset();
	}
	void AddShot(bool hit)
	{
		shot_state >>= 1;
		shot_state[max_shots_min_one] = hit;
		if (shots_fired != max_shots_min_one)
		{
			++shots_fired;
		}
	}
	unsigned short GetHits()
	{
		return (unsigned short)shot_state.count();
	}
	unsigned short GetShots()
	{
		return shots_fired;
	}
	float GetAccuracy()
	{
		if (shots_fired)
		{
			return ((float)shot_state.count() / (float)shots_fired) * 100.0f;
		}
		return 0.0f;
	}
	std::string GetStats()
	{
		return Functions::string_format("%d/%d (%2.2f%%)", GetHits(), GetShots(), GetAccuracy());
	}
	std::string GetHitStreaks()
	{
		bool hit = true;
		std::vector<unsigned short> HitStreaks;
		HitStreaks.push_back(0);
		for (unsigned short i = max_shots - shots_fired; i < max_shots; ++i)
		{
			if (shot_state[i] != hit)
			{
				HitStreaks.push_back(1);
				hit ^= 1;
			}
			else
			{
				++HitStreaks.back();
			}
		}
		std::array<std::string, 2> colors = { { "{FF0000}", "{FFFFFF}" } };
		std::string ret(colors[0] + std::to_string(HitStreaks[0]));

		for (unsigned short i = 1; i < HitStreaks.size(); ++i)
		{
			if (i % 80 == 0)
			{
				ret.append("\n\t");
			}
			ret.append("-" + colors[i % 2] + std::to_string(HitStreaks[i]));
		}
		ret.append(colors[1]);
		return ret;
	}
	std::string GetAll()
	{
		return GetStats() + "\n\t" + GetHitStreaks();
	}
};

std::array<AccuracyData, MAX_PLAYERS> PlayerAccuracy;

std::string GetAllAccuracyForAllPlayers()
{
	std::string ret("{FFFFFF}");
	for (auto playerid : PlayersOnline)
	{
		ret.append(Functions::string_format("[%3d]", playerid) + Player[playerid].PlayerName + ":" + PlayerAccuracy[playerid].GetAll() + "\n");
	}
	return ret;
}

class AccuracyMeasurement : public Extension::Base
{
public:
	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ) override
	{
		PlayerAccuracy[playerid].AddShot(hitid != INVALID_PLAYER_ID && hittype == BULLET_HIT_TYPE_PLAYER);
		return true;
	}
	bool OnPlayerConnect(int playerid) override
	{
		PlayerAccuracy[playerid].Reset();
		return true;
	}
} _AccuracyMeasurement;

ZCMD3(accuracy, PERMISSION_ADMIN, RESTRICTION_NONE)
{
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, "Accuracy", GetAllAccuracyForAllPlayers(), "V", "");
	return true;
}

ZCMDF(accuracyp, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({}), "p")
{
	if (parser.Good())
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, "Accuracy: " + Player[targetid].PlayerName, "{FFFFFF}" + PlayerAccuracy[targetid].GetAll(), "V", "");
		}
	}
	return true;
}

ZCMDF(accuracyr, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({}), "p")
{
	if (parser.Good())
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			PlayerAccuracy[targetid].Reset();
		}
	}
	return true;
}