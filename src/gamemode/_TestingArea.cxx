/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	In this file pre-production / testing code may be placed

	________________________________________________________________	
	Notes
		-	Try to not enable the code in release

	________________________________________________________________
	Dependencies
		Anything you require

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

#if defined _LOCALHOST_DEBUG
ZCMD(tpadd, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ }))
{
	std::string::size_type pos = params.find(' ');
	std::string name, vehicle;
	if (params.npos != pos)
	{
		vehicle.assign(params.substr(pos + 1));
		name.assign(params.substr(0, pos));
		vehicle = "false";
	}
	else
	{
		name = params;
		vehicle = "true";
	}

	if (name.size())
	{
		float x, y, z, a;
		GetPlayerPos(playerid, &x, &y, &z);
		GetPlayerFacingAngle(playerid, &a);

		std::fstream file;
		file.open("teleports.txt", std::fstream::out | std::fstream::app);

		std::string tpFormat(Functions::string_format("\n\
			ZCMD3(%s, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT)\n\
			{\n\t\
			TeleportPlayer(playerid, %f, %f, %f, %f, %s, %d, %d, \x22\x22, 0, 0, 2.0, 2.0);\n\t\
			return true;\n\
			}", name.c_str(), x, y, z, a, vehicle.c_str(), GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid)));

		file.write(tpFormat.c_str(), tpFormat.length());
		file.close();

		std::string msg(Functions::string_format("Teleport /" + name + " has been added"));
		fixSendClientMessage(playerid, -1, msg);
		return true;
	}
	fixSendClientMessage(playerid, -1, "Usage: /tpadd 'name' <1> (type this if not vehicle)");
	return true;
}
/*
class CMafiaTest : public Extension::Base
{
public:
	void SendMafiaWebStats(mafia* i)
	{
		for (auto &k : Mafias)
		{
			if (k.LoadData(i))
			{
				k.SendStatsToWeb();
				return;
			}
		}
	}
	bool OnGameModeInit()
	{
		typedef odb::query<mafia> query;
		typedef odb::result<mafia> result;

		transaction t(userdb->begin());

		result r(userdb->query<mafia>(query::kills >= 0));//true somehow not work.

		for (result::iterator i(r.begin()); i != r.end(); ++i)
		{
			SendMafiaWebStats(&*i);
			std::cout << ":: " << i->name() << " ::" << std::endl;

			MafiaMemberDataMap AllMembers;
			ParseMembersAndPopulate(i->members(), AllMembers);
			double members = (double)AllMembers.size();
			double divisor = members * 0.0001 / ((double)(i->playtime()) / (double)((i->last_load() + 86400000) - (i->creation_date())));

			//ratio: exp(4):respect(6):money(3000):kills(1):deaths(1)
			double exp_rank =	(double)i->experience()	/ (divisor * 4.0);
			double score_rank = (double)i->score()		/ (divisor * 6.0);
			double money_rank = (double)i->money()		/ (divisor * 3000.0);
			double kd_rank =	((double)i->kills()		/ (divisor * 1.0)) -
								((double)i->deaths()	/ (divisor * 1.0));

			double calc_rank_idx = ((exp_rank + score_rank + money_rank + kd_rank) / 4.0);//average of all

			std::cout << "DIV: " << divisor << std::endl;
			std::cout << "ER:  " << exp_rank << std::endl;
			std::cout << "SR:  " << score_rank << std::endl;
			std::cout << "MR:  " << money_rank << std::endl;
			std::cout << "KR:  " << kd_rank << std::endl;
			std::cout << "CI:  " << calc_rank_idx << std::endl;
		}

		t.commit();

		return true;
	}
} _CMafiaTest;*/
#endif
/*
ZCMD3(msgtest, 0, 0)
{
	std::string onehundredfourtyfourcharsstring("");
	for (int i = 0; i < 144; ++i)
	{
		onehundredfourtyfourcharsstring.append(std::string(1, 'A' + rand() % 26));
	}
	sampgdk_SendClientMessage(playerid, -1, "144 chars: ");
	sampgdk_SendClientMessage(playerid, -1, onehundredfourtyfourcharsstring.c_str());
	sampgdk_SendClientMessage(playerid, -1, "145 chars: ");
	onehundredfourtyfourcharsstring.append("X");
	sampgdk_SendClientMessage(playerid, -1, onehundredfourtyfourcharsstring.c_str());

	onehundredfourtyfourcharsstring.clear();
	for (int i = 0; i < 144; ++i)
	{
		onehundredfourtyfourcharsstring.append(std::string(1, 'A' + rand() % 26));
	}
	onehundredfourtyfourcharsstring.insert(32, "{FF0000}");
	onehundredfourtyfourcharsstring.insert(64, "{00FF00}");
	onehundredfourtyfourcharsstring.insert(96, "{0000FF}");
	sampgdk_SendClientMessage(playerid, -1, "144 chars with color coding: ");
	sampgdk_SendClientMessage(playerid, -1, onehundredfourtyfourcharsstring.c_str());
	sampgdk_SendClientMessage(playerid, -1, "145 chars with color coding: ");
	onehundredfourtyfourcharsstring.append("X");
	sampgdk_SendClientMessage(playerid, -1, onehundredfourtyfourcharsstring.c_str());
	return true;
}*/