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

unsigned long gHighestHouseID = 0;

struct SHouseData
{
	unsigned long id;

	float world_posX;
	float world_posY;
	float world_posZ;
	float world_angle;

	float interior_posX;
	float interior_posY;
	float interior_posZ;
	float interior_angle;

	unsigned long long rented_to_date;
	unsigned long long rent_cost_cash_per_day;
	unsigned long long rent_cost_respect_per_day;

	int interiorid;

	bool rented_to_mafia;

	std::string rented_to_nickname;

	bool locked;

	house houseptr;

	int CurrentPickupType;

	SHouseData();

	SHouseData(house* _houseptr);

	void apply(house* _houseptr);

	void save();
};

std::set<SHouseData*> ToSaveHouse;

SHouseData::SHouseData()
{
	CurrentPickupType = -1;
}

SHouseData::SHouseData(house* _houseptr)
{
	CurrentPickupType = -1;
	apply(_houseptr);
}

void SHouseData::apply(house* _houseptr)
{
	id = _houseptr->id();

	world_posX = _houseptr->world_posX();
	world_posY = _houseptr->world_posY();
	world_posZ = _houseptr->world_posZ();
	world_angle = _houseptr->world_angle();

	interior_posX = _houseptr->interior_posX();
	interior_posY = _houseptr->interior_posY();
	interior_posZ = _houseptr->interior_posZ();
	interior_angle = _houseptr->interior_angle();

	rented_to_date = _houseptr->rented_to_date();
	rent_cost_cash_per_day = _houseptr->rent_cost_cash_per_day();
	rent_cost_respect_per_day = _houseptr->rent_cost_respect_per_day();

	interiorid = _houseptr->interiorid();

	rented_to_mafia = _houseptr->rented_to_mafia();

	rented_to_nickname = _houseptr->rented_to_nickname();

	locked = _houseptr->locked();
}

void SHouseData::save()
{
	houseptr.id(id);

	houseptr.world_posX(world_posX);
	houseptr.world_posY(world_posY);
	houseptr.world_posZ(world_posZ);
	houseptr.world_angle(world_angle);

	houseptr.interior_posX(interior_posX);
	houseptr.interior_posY(interior_posY);
	houseptr.interior_posZ(interior_posZ);
	houseptr.interior_angle(interior_angle);

	houseptr.rented_to_date(rented_to_date);
	houseptr.rent_cost_cash_per_day(rent_cost_cash_per_day);
	houseptr.rent_cost_respect_per_day(rent_cost_respect_per_day);

	houseptr.interiorid(interiorid);

	houseptr.rented_to_mafia(rented_to_mafia);

	houseptr.rented_to_nickname(rented_to_nickname);

	houseptr.locked(locked);

	#ifdef FAILSAFE_MODE
	house* save_house = new house;
	*save_house = (*ToSaveHouse.begin())->houseptr;
	save_house->id((*ToSaveHouse.begin())->id);
	CreateWorkerRequest(0, "", save_house, DATABASE_REQUEST_OPERATION_SAVE_HOUSE, DATABASE_POINTER_TYPE_HOUSE, 0, 0);
	ToSaveHouse.erase(ToSaveHouse.begin());
	#else
	ToSaveHouse.insert(this);
	#endif
}

std::map<house*, SHouseData*> SHouseProxy;
std::map<SHouseData*, int> HouseToPickup;
std::map<int, SHouseData*> PickupToHouse;
std::array<std::set<SHouseData*>, MAX_PLAYERS> HouseInvites;

std::map<std::string, SHouseData*, ci_less> PlayerHouse;

static const unsigned long HousesBaseVirtualWorld = 0x6000;
const unsigned long GetHouseVirtualWorldId(SHouseData* phouse)
{
	return HousesBaseVirtualWorld + phouse->id;
}

std::array<SHouseData*, MAX_PLAYERS> CurrentVisitingHouse;

//marker blue_house
//not_owned  player 
const std::array<int, 2> PickupIDOnOwner = { { 1273, 1272 } };

enum HouseStates
{
	eHS_none = 1273,
	eHS_owned = 1272
};

int GetHousePickup(house* ptrhouse)
{
	if (ptrhouse->rented_to_nickname().size() && ptrhouse->rented_to_date() > LowPrecisionTimerValue)
	{
		return PickupIDOnOwner[1];
	}
	return PickupIDOnOwner[0];
}

int GetHousePickup(SHouseData* ptrhouse)
{
	if (ptrhouse->rented_to_nickname.size() && ptrhouse->rented_to_date > LowPrecisionTimerValue)
	{
		return PickupIDOnOwner[1];
	}
	return PickupIDOnOwner[0];
}

bool FixHousePickup(SHouseData* ptrhouse)
{
	auto htp = HouseToPickup.find(ptrhouse);
	if (htp != HouseToPickup.end())
	{
		StreamerLibrary::DestroyDynamicPickup(htp->second);

		PickupToHouse.erase(htp->second);
		HouseToPickup.erase(htp);

		int type = GetHousePickup(ptrhouse);
		int housepickup = StreamerLibrary::CreateDynamicPickup(type, 1, ptrhouse->world_posX, ptrhouse->world_posY, ptrhouse->world_posZ, 0, 0);

		ptrhouse->CurrentPickupType = type;

		PickupToHouse.insert(std::pair<int, SHouseData*>(housepickup, ptrhouse));
		HouseToPickup.insert(std::pair<SHouseData*, int>(ptrhouse, housepickup));

		return true;
	}
	return false;
}

void HouseSaveTimer(int timerid, void* param)
{
	#ifndef FAILSAFE_MODE
	while (ToSaveHouse.size())
	{
		house* save_house = new house;
		*save_house = (*ToSaveHouse.begin())->houseptr;
		save_house->id((*ToSaveHouse.begin())->id);
		CreateWorkerRequest(0, "", save_house, DATABASE_REQUEST_OPERATION_SAVE_HOUSE, DATABASE_POINTER_TYPE_HOUSE, 0, 0);
		ToSaveHouse.erase(ToSaveHouse.begin());
	}
	#endif

	unsigned long long TimeNow = Functions::GetTime();
	for (auto& house_itr : SHouseProxy)
	{
		bool already_rented = house_itr.second->rented_to_date > TimeNow && house_itr.second->rented_to_nickname.size();
		if (
			(already_rented && house_itr.second->CurrentPickupType == eHS_none) || 
			(!already_rented && house_itr.second->CurrentPickupType == eHS_owned))
		{
			FixHousePickup(house_itr.second);
		}
	}
}

struct StaticHouseCreatorData
{
	float interior_posX;
	float interior_posY;
	float interior_posZ;
	float interior_angle;

	int interiorid;
};

enum HouseTypes
{
	HOUSE_SMALL_1,
	HOUSE_SMALL_2,
	HOUSE_SMALL_3,
	HOUSE_MEDIUM_1,
	HOUSE_MEDIUM_2,
	HOUSE_MEDIUM_3,
	HOUSE_LARGE_1,
	HOUSE_LARGE_2,
	HOUSE_LARGE_3,
	HOUSE_VILLA_1,
	HOUSE_MAXTYPES
};

std::array<StaticHouseCreatorData, HOUSE_MAXTYPES> StaticHouseData =
{{
	{ 261.1204, 1285.4740, 1080.2578, 356.6235, 4 },// int(4)_ciasny
	{ 140.1285, 1366.9539, 1083.8594, 351.7841, 5 },// int(5)_s
	{ 266.9561, 304.5312, 999.1484, 283.1967, 2 },// int(2)_maly
	{ 2495.6519, -1692.4067, 1014.7422, 183.5559, 3 },// int(3)_CJ
	{ -69.2907, 1352.8640, 1080.2109, 263.7908, 6 },// int(6)_zwykly
	{ 24.0949, 1341.0516, 1084.3750, 357.8535, 10 },// int(10)_maly_pietrowy
	{ 140.4198, 1366.6466, 1083.8594, 2.2977, 5 },// int(5)_pietrowy_ale_troche_pusty
	{ 2232.8921, -1115.0969, 1050.8828, 354.8335, 5 },// int(5)_maly_hotel
	{ 2318.1084, -1026.1829, 1050.2109, 358.3271, 9 },// int(9)_nawet_duzy
	{ 1297.9258, -796.9424, 1084.0078, 3.9673, 5 }// int(5)_maddog
}};

namespace HouseSystem
{
	bool CreateHouse(float x, float y, float z, float a, int type_id, unsigned long long rent_cost_cash_per_day, unsigned long long rent_cost_respect_per_day)
	{
		if (type_id >= 0 && type_id < HOUSE_MAXTYPES)
		{
			house * hptr = new house;

			hptr->id(++gHighestHouseID);

			hptr->world_posX(x);
			hptr->world_posY(y);
			hptr->world_posZ(z);
			hptr->world_angle(a);
			hptr->interior_posX(StaticHouseData[type_id].interior_posX);
			hptr->interior_posY(StaticHouseData[type_id].interior_posY);
			hptr->interior_posZ(StaticHouseData[type_id].interior_posZ);
			hptr->interior_angle(StaticHouseData[type_id].interior_angle);
			hptr->interiorid(StaticHouseData[type_id].interiorid);
			hptr->rent_cost_cash_per_day(rent_cost_cash_per_day);
			hptr->rent_cost_respect_per_day(rent_cost_respect_per_day);

			auto temp = std::pair<SHouseData*, int>(new SHouseData(hptr), StreamerLibrary::CreateDynamicPickup(eHS_none, 1, x, y, z, 0, 0));
			temp.first->CurrentPickupType = eHS_none;

			HouseToPickup.insert(temp);
			PickupToHouse.insert(std::pair<int, SHouseData*>(temp.second, temp.first));
			temp.first->save();

			delete hptr;

			return true;
		}
		return false;
	}
};

class HouseSystemProcessor : public Extension::Base
{
public:
	bool OnGameModeInit()
	{
		typedef odb::query<house> query;
		typedef odb::result<house> result;

		transaction t(userdb->begin());

		result r(userdb->query<house>(query::id >= 0));//true somehow not work.

		for (result::iterator i(r.begin()); i != r.end(); ++i)
		{
			int type = GetHousePickup(&*i);
			auto temp = std::pair<SHouseData*, int>(new SHouseData(&(*i)), StreamerLibrary::CreateDynamicPickup(type, 1, i->world_posX(), i->world_posY(), i->world_posZ(), 0, 0));
			temp.first->CurrentPickupType = type;

			HouseToPickup.insert(temp);
			PickupToHouse.insert(std::pair<int, SHouseData*>(temp.second, temp.first));
		}

		t.commit();

		unsigned int loadcount = 0;

		for (auto &i : HouseToPickup)
		{
			++loadcount;
			if (i.first->rented_to_nickname.size())
			{
					PlayerHouse.insert(std::pair<std::string, SHouseData*>(i.first->rented_to_nickname, i.first));
			}
			if (i.first->houseptr.id() > gHighestHouseID)
				gHighestHouseID = i.first->houseptr.id();
		}

		sampgdk_SetTimerEx(30 * 60 * 1000, true, HouseSaveTimer, nullptr, nullptr);

		std::cout << "Loaded " << loadcount << " houses" << std::endl;
		return true;
	}
	bool OnPlayerConnect(int playerid)
	{
		CurrentVisitingHouse[playerid] = nullptr;
		HouseInvites[playerid].clear();
		return true;
	}
	bool OnPlayerPickUpDynamicPickup(int playerid, int pickupid)
	{
		auto found = PickupToHouse.find(pickupid);
		if (found != PickupToHouse.end() && found->second != CurrentVisitingHouse[playerid])
		{
			CurrentVisitingHouse[playerid] = found->second;
			if (CurrentVisitingHouse[playerid] != nullptr)
			{
				fixSendClientMessage(playerid, -1, L_pickupmessage_house);
			}
		}
		return true;
	}
} _HouseSystemProcessor;

ZCMDF(dom, PERMISSION_NONE, RESTRICTION_REGISTERED_AND_LOADED | RESTRICTION_ONLY_LOGGED_IN | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_ON_FOOT, cmd_alias({"/house", "/huis", "/prop", "/property"}), "ws")
{
	if (boost::iequals(params, "tp"))
	{
		auto iter = PlayerHouse.find(Player[playerid].PlayerName);
		if (iter != PlayerHouse.end())
		{
			if (CheckCommandAllowed(playerid, RESTRICTION_NOT_AFTER_FIGHT))
				TeleportPlayer(playerid, iter->second->world_posX, iter->second->world_posY, iter->second->world_posZ, iter->second->world_angle, false, 0, 0, "", 0, 0, 0.0f, 0.0f, false, false);
			return true;
		}
		fixSendClientMessage(playerid, -1, L_house_nohouse);
		return true;
	}
	SHouseData* phouse = CurrentVisitingHouse[playerid];
	if (phouse != nullptr)
	{
		bool outside = (GetPlayerDistanceFromPoint(playerid, phouse->world_posX, phouse->world_posY, phouse->world_posZ) < 5.0f && GetPlayerInterior(playerid) == 0);
		bool inside = (unsigned long)Player[playerid].WorldID == GetHouseVirtualWorldId(phouse) && GetPlayerInterior(playerid) == phouse->interiorid;
		if (outside || inside)
		{
			unsigned long long TimeNow = Functions::GetTime();
			bool already_rented = phouse->rented_to_date > TimeNow && phouse->rented_to_nickname.size();
			bool isowner = boost::iequals(phouse->rented_to_nickname, Player[playerid].PlayerName);
			if (!parser.Good())
			{
				std::string information("");	
				if (already_rented)
				{
					information.append(TranslatePF(playerid, L_house_infostringbuilder_a, phouse->rented_to_nickname.c_str(), Functions::GetTimeStrFromMs(phouse->rented_to_date).c_str()));

					if (phouse->locked)
						information.append(TranslateP(playerid, L_house_infostringbuilder_locked));

					if (phouse->CurrentPickupType == eHS_none)
						FixHousePickup(phouse);
				}
				else
				{
					information.append(TranslateP(playerid, L_house_infostringbuilder_b));

					if (phouse->CurrentPickupType == eHS_owned)
						FixHousePickup(phouse);
				}

				information.append(TranslateP(playerid, L_house_infostringbuilder_c));
				if (phouse->rent_cost_cash_per_day)
					information.append(TranslatePF(playerid, L_house_infostringbuilder_money, phouse->rent_cost_cash_per_day, phouse->rent_cost_cash_per_day * 7, phouse->rent_cost_cash_per_day * 14, phouse->rent_cost_cash_per_day * 21));
				if (phouse->rent_cost_respect_per_day)
					information.append(TranslatePF(playerid, L_house_infostringbuilder_score, phouse->rent_cost_respect_per_day, phouse->rent_cost_respect_per_day * 7, phouse->rent_cost_respect_per_day * 14, phouse->rent_cost_respect_per_day * 21));

				information.append(TranslateP(playerid, L_house_infostringbuilder_d));

#ifndef _LOCALHOST_DEBUG
				if (Player[playerid].statistics.privilidges >= PERMISSION_GAMER)
#endif
				{
					information.append(TranslatePF(playerid, L_house_infostringbuilder_e, phouse->id));
				}

				ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_house_info_title), information, "V", "X");
			}
			else
			{
				std::string w(parser.Get<std::string>(0));
				std::string r("");
				ParseInput parser2;
				if (parser.Good() == 2)
					r.assign(parser.Get<std::string>(1));
				switch (w[0])
				{
					case 'p':
					case 'P':
					{
						auto iter = HouseInvites[playerid].find(phouse);
						if (outside && (!phouse->locked || isowner || iter != HouseInvites[playerid].end() || !already_rented))
							TeleportPlayer(playerid, phouse->interior_posX, phouse->interior_posY, phouse->interior_posZ, phouse->interior_angle, false, phouse->interiorid, GetHouseVirtualWorldId(phouse), "", 0, 0, 0.0f, 0.0f, false, false);
						else if (inside)
							TeleportPlayer(playerid, phouse->world_posX, phouse->world_posY, phouse->world_posZ, phouse->world_angle, false, 0, 0, "", 0, 0, 0.0f, 0.0f, false, false);
						else
							fixSendClientMessage(playerid, -1, L_house_enter_permissionerror);
					}
					break;
					case 'k':
					case 'K':
					{
						bool ownshouse = PlayerHouse.find(Player[playerid].PlayerName) != PlayerHouse.end();
						if (ownshouse && !isowner)
						{
							fixSendClientMessage(playerid, -1, L_house_rentlimit_reached);
							return true;
						}
						parser2.SetFormat("U");
						parser2.ExecuteManualParse(r);
						if (parser2.Good())
						{
							unsigned long long max_rent_days = 5 * 24 * 60 * 60 * 1000;
							if (Player[playerid].statistics.privilidges >= PERMISSION_VIP)
								max_rent_days *= 3;

							unsigned long long days = (parser2.Get<unsigned long long>()*24*60*60*1000);

							if (already_rented)
							{
								if (!isowner)
								{
									fixSendClientMessage(playerid, -1, L_house_owneronly_action);
									return true;
								}
								else
								{
									unsigned long long still_rented = phouse->rented_to_date - TimeNow;
									if (days > max_rent_days)
										days = max_rent_days;
									days -= still_rented;
								}
							}
							else
							{
								if (days > max_rent_days)
									days = max_rent_days;
							}

							if (days < 1 * 24 * 60 * 60 * 1000)
							{
								fixSendClientMessage(playerid, -1, L_house_rent_mintime_error);
								return true;
							}

							double ddays = (((double)days) / (1.0 * 24.0 * 60.0 * 60.0 * 1000.0));
							long long cash_cost = (long long)((double)phouse->rent_cost_cash_per_day * ddays);
							long long respect_cost = (long long)((double)phouse->rent_cost_respect_per_day * ddays);

							//use days here
							if (Player[playerid].statistics.money < cash_cost || Player[playerid].statistics.respect < respect_cost)
							{
								fixSendClientMessage(playerid, -1, L_house_resources_error);
								return true;
							}

							Player[playerid].GiveMoney(-cash_cost);
							Player[playerid].GiveScore(-respect_cost);

							if (phouse->rented_to_nickname.size())
							{
								auto iter_n = PlayerHouse.find(phouse->rented_to_nickname);
								if (iter_n != PlayerHouse.end())
									PlayerHouse.erase(iter_n);
							}

							phouse->rented_to_nickname.assign(Player[playerid].PlayerName);
							phouse->rented_to_date = TimeNow + days;

							if (phouse->CurrentPickupType == eHS_none)
								FixHousePickup(phouse);

							phouse->save();

							PlayerHouse[Player[playerid].PlayerName] = phouse;

							fixSendClientMessage(playerid, -1, L_house_rent_success);
						}
						else
						{
							fixSendClientMessage(playerid, -1, L_house_rent_usage);
							return true;
						}
					}
					break;

					case 'l':
					case 'L':
					{
						if (!isowner)
						{
							fixSendClientMessage(playerid, -1, L_house_owneronly_action);
							return true;
						}

						if (phouse->locked)
							fixSendClientMessage(playerid, -1, L_house_door_open);
						else
							fixSendClientMessage(playerid, -1, L_house_door_close);

						phouse->locked = !phouse->locked;

						phouse->save();

						return true;

					}
					break;

					case 'w':
					case 'W':
					{
						if (!isowner)
						{
							fixSendClientMessage(playerid, -1, L_house_owneronly_action);
							return true;
						}

						parser2.SetFormat("p");
						parser2.ExecuteManualParse(r);
						if (!parser2.Good())
						{
							fixSendClientMessage(playerid, -1, L_house_kick_usage);
							return true;
						}
						int inviteid = parser2.Get<ParsePlayer>(0).playerid;
						if (inviteid == INVALID_PLAYER_ID)
						{
							SendClientMessage(playerid, -1, L_invalid_playerid);
							return true;
						}

						bool target_inside = (unsigned long)Player[inviteid].WorldID == GetHouseVirtualWorldId(phouse) && GetPlayerInterior(inviteid) == phouse->interiorid;
						if (!target_inside)
						{
							fixSendClientMessage(playerid, -1, L_house_kick_error);
							return true;
						}
						TeleportPlayer(inviteid, phouse->world_posX, phouse->world_posY, phouse->world_posZ, phouse->world_angle, false, 0, 0, "", 0, 0, 0.0f, 0.0f, false, false);
						fixSendClientMessage(playerid, -1, L_house_kick_success_1);
						fixSendClientMessage(inviteid, -1, L_house_kick_success_2);
					}
					break;

					case 'z':
					case 'Z':
					{
						if (!isowner)
						{
							fixSendClientMessage(playerid, -1, L_house_owneronly_action);
							return true;
						}

						parser2.SetFormat("p");
						parser2.ExecuteManualParse(r);
						if (!parser2.Good())
						{
							fixSendClientMessage(playerid, -1, L_house_uninvite_usage);
							return true;
						}
						int inviteid = parser2.Get<ParsePlayer>(0).playerid;
						if (inviteid == INVALID_PLAYER_ID)
						{
							SendClientMessage(playerid, -1, L_invalid_playerid);
							return true;
						}

						auto iter = HouseInvites[inviteid].find(phouse);
						if (iter != HouseInvites[inviteid].end())
						{
							HouseInvites[inviteid].erase(iter);
							fixSendClientMessage(playerid, -1, L_house_uninvite_success_1);
						}
						else
						{
							HouseInvites[inviteid].insert(phouse);
							fixSendClientMessage(playerid, -1, L_house_uninvite_success_2);
						}
					}
					break;

					case 'q':
					case 'Q':
						if (isowner)
						{
							if (phouse->rented_to_nickname.size())
							{
								auto iter_n = PlayerHouse.find(phouse->rented_to_nickname);
								if (iter_n != PlayerHouse.end())
									PlayerHouse.erase(iter_n);
							}

							phouse->rented_to_nickname.assign("");

							if (phouse->CurrentPickupType == eHS_owned)
								FixHousePickup(phouse);

							phouse->save();

							fixSendClientMessage(playerid, -1, L_house_action_executed);
							return true;
						}
						break;
					//set respekt / day
					case '#':
					if (Player[playerid].statistics.privilidges >= PERMISSION_GAMER)
					{
						parser2.SetFormat("U");
						parser2.ExecuteManualParse(r);
						if (!parser2.Good())
						{
							fixSendClientMessage(playerid, -1, L_house_score_error);
							return true;
						}
						phouse->rent_cost_respect_per_day = parser2.Get<unsigned long long>();
						phouse->save();
						fixSendClientMessage(playerid, -1, L_house_action_executed);
						return true;
					}
					break;
					//set cash / day
					case '$':
					if (Player[playerid].statistics.privilidges >= PERMISSION_GAMER)
					{
						parser2.SetFormat("U");
						parser2.ExecuteManualParse(r);
						if (!parser2.Good())
						{
							fixSendClientMessage(playerid, -1, L_house_money_error);
							return true;
						}
						phouse->rent_cost_cash_per_day = parser2.Get<unsigned long long>();
						phouse->save();
						fixSendClientMessage(playerid, -1, L_house_action_executed);
						return true;
					}
					break;
					//remove current owner
					case ':':
					if (Player[playerid].statistics.privilidges >= PERMISSION_GAMER)
					{
						if (phouse->rented_to_nickname.size())
						{
							auto iter_n = PlayerHouse.find(phouse->rented_to_nickname);
							if (iter_n != PlayerHouse.end())
								PlayerHouse.erase(iter_n);
						}

						phouse->rented_to_nickname.assign("");

						if (phouse->CurrentPickupType == eHS_owned)
							FixHousePickup(phouse);

						phouse->save();

						fixSendClientMessage(playerid, -1, L_house_action_executed);
						return true;
					}
					break;
					//make player owner for 5 days
					case '+':
					if (Player[playerid].statistics.privilidges >= PERMISSION_GAMER)
					{
						parser2.SetFormat("p");
						parser2.ExecuteManualParse(r);
						if (!parser2.Good())
						{
							fixSendClientMessage(playerid, -1, L_house_player_supplyerror);
							return true;
						}
						int targetid = parser2.Get<ParsePlayer>().playerid;
						if (targetid == INVALID_PLAYER_ID)
						{
							fixSendClientMessage(playerid, -1, L_house_player_notonline);
							return true;
						}

						if (phouse->rented_to_nickname.size())
						{
							auto iter_n = PlayerHouse.find(phouse->rented_to_nickname);
							if (iter_n != PlayerHouse.end())
								PlayerHouse.erase(iter_n);
						}

						phouse->rented_to_nickname.assign(Player[targetid].PlayerName);
						phouse->rented_to_date = TimeNow + 5 * 24 * 60 * 60 * 1000;

						PlayerHouse[Player[targetid].PlayerName] = phouse;

						if (phouse->CurrentPickupType == eHS_none)
							FixHousePickup(phouse);

						phouse->save();
						fixSendClientMessage(playerid, -1, L_house_action_executed);
					}
					break;
					//set expire to X hours in future
					case '>':
					if (Player[playerid].statistics.privilidges >= PERMISSION_GAMER)
					{
						parser2.SetFormat("U");
						parser2.ExecuteManualParse(r);
						if (!parser2.Good())
						{
							fixSendClientMessage(playerid, -1, L_house_invalid_hours);
							return true;
						}
						phouse->rented_to_date = TimeNow + (parser2.Get<unsigned long long>() * 60 * 60 * 1000);
						phouse->save();
						fixSendClientMessage(playerid, -1, L_house_action_executed);
					}
					break;
					//set new type
					case '*':
					if (Player[playerid].statistics.privilidges >= PERMISSION_GAMER)
					{
						parser2.SetFormat("D");
						parser2.ExecuteManualParse(r);
						if (!parser2.Good())
						{
							fixSendClientMessage(playerid, -1, L_house_invalid_interiorid);
							return true;
						}
						unsigned long type_id = parser2.Get<unsigned long>();
						if (type_id >= HOUSE_MAXTYPES)
						{
							fixSendClientMessage(playerid, -1, L_house_invalid_interiorid);
							return true;
						}

						phouse->interior_posX=StaticHouseData[type_id].interior_posX;
						phouse->interior_posY=StaticHouseData[type_id].interior_posY;
						phouse->interior_posZ=StaticHouseData[type_id].interior_posZ;
						phouse->interior_angle=StaticHouseData[type_id].interior_angle;
						phouse->interiorid=StaticHouseData[type_id].interiorid;
						phouse->save();

						fixSendClientMessage(playerid, -1, L_house_action_executed);
						return true;
					}
					break;
				}
			}
			return true;
		}
	}
	fixSendClientMessage(playerid, -1, L_house_range_error);
	return true;
}

ZCMDF(domadd, PERMISSION_GAMER, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_ON_FOOT, cmd_alias({"/addhouse", "/houseadd"}), "DUU")
{
	static float x, y, z, a;
	if (parser.Good() == 3)
	{
		GetPlayerPos(playerid, &x, &y, &z);
		GetPlayerFacingAngle(playerid, &a);
		HouseSystem::CreateHouse(x, y, z, a, parser.Get<unsigned long>(0), parser.Get<unsigned long long>(1), parser.Get<unsigned long long>(2));
		return true;
	}
	fixSendClientMessage(playerid, -1, L_houseadd_usage_1);
	fixSendClientMessage(playerid, -1, L_houseadd_usage_2);
	return true;
}