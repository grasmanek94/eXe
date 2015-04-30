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

int VehicleModelInfo[612];

// templated version of my_equal so it could work with both char and wchar_t
template<typename charT>
struct my_equal {
	my_equal(const std::locale& loc) : loc_(loc) 
	{
	
	}
	bool operator()(charT ch1, charT ch2) 
	{
		return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
	}
private:
	const std::locale& loc_;
};

// find substring (case insensitive)
template<typename T>
int ci_find_substr(const T& str1, const T& str2, const std::locale& loc = std::locale())
{
	T::const_iterator it = std::search(str1.begin(), str1.end(),
		str2.begin(), str2.end(), my_equal<T::value_type>(loc));
	if (it != str1.end())
	{
		return it - str1.begin();
	}
	else return -1; // not found
}

ZERO_COMMAND(f, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_ONLY_DRIVER, cmd_alias({ "/flip" }))
{
	float a;
	GetVehicleZAngle(Player[playerid].CurrentVehicle, &a);
	SetVehicleZAngle(Player[playerid].CurrentVehicle, a);
	GivePlayerAchievement(playerid, EAM_LubieDachowac, 1);
	return true;
}

void CreatePlayerVehicleSafe(int playerid, int modelid, float ax, float ay, float az)
{
	float x, y, z, a;
	GetPlayerPos(playerid, &x, &y, &z);
	SetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, &a);
	Player[playerid].CleanupPlayerVehicleNicely();
	Player[playerid].PlayerVehicle = fixCreateVehicle(modelid, x + ax, y + ay, z + az, a, -1, -1, 84600);
	VehicleSpawnedBy[Player[playerid].PlayerVehicle] = playerid;
	SetVehicleNumberPlate(Player[playerid].PlayerVehicle, Player[playerid].PlayerName.c_str());
	LinkVehicleToInterior(Player[playerid].PlayerVehicle, GetPlayerInterior(playerid));
	SetVehicleVirtualWorld(Player[playerid].PlayerVehicle, Player[playerid].WorldID);
	fixPutPlayerInVehicle(playerid, Player[playerid].PlayerVehicle, 0);
}

bool IsDisallowedVehicle(int modelid)
{
	static const std::unordered_set<int> disallowedVehicles({ 
		VEH_TYPE_PREDATOR, 
		VEH_TYPE_HUNTER, 
		VEH_TYPE_RHINO, 
		VEH_TYPE_RCBARON, 
		VEH_TYPE_SEASPAR, 
		VEH_TYPE_HYDRA,
		VEH_TYPE_RUSTLER 
	});

	return (disallowedVehicles.find(modelid) != disallowedVehicles.end());
}

ZERO_COMMAND(v, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_ONLY_ON_FOOT, cmd_alias({ "/car", "/pojazd", "/p", "/vehicle", "/veh" }))
{
	if (params.empty())
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_v_usage);
		return true;
	}
	boost::replace_all(params, " ", "");
	int ModelID = 999;
	if (!Functions::is_number_unsigned(params, ModelID))
	{
		for (int i = 400; i < 612; i++)
		{
			if (ci_find_substr(NazwyPojazdowNS[i], params) != -1)
			{
				if (i != 447)
				{
					ModelID = i;
					break;
				}
				else//modelid == 447 (seasparrow)
				{
					if (ci_find_substr<std::string>(params, "se") == -1)//did user search for '*se*'?
					{
						ModelID = 469;//normal sparrow
					}
					else
					{
						ModelID = 447;//seasparrow		
					}
					break;
				}
			}
		}
	}

	if (ModelID >= 0 && ModelID < 10)
	{
		if (Player[playerid].PrywatnePojazdy[ModelID].ModelID)
		{
			GivePlayerAchievement(playerid, EAM_PrivCarSpawned, 1);
			GivePlayerAchievement(playerid, EAM_CarSpawner, 1);
			CreatePlayerVehicleSafe(playerid, Player[playerid].PrywatnePojazdy[ModelID].ModelID, 0.0, 0.0, 1.5);
			Player[playerid].PrywatnePojazdy[ModelID].Apply(Player[playerid].PlayerVehicle);
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_v_slot_error);
		return true;
	}
	if (ModelID >= 10 && ModelID < 12)
	{
		if (Player[playerid].Mafia != nullptr)
		{
			if (!Player[playerid].Mafia->CheckAllowedWithMessage(playerid, MAFIA_ACTION_SPAWNCAR) || !Player[playerid].Mafia->PrivateVehicle[ModelID - 10].ModelID)
			{
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_v_slot_error);
				return true;
			}
			GivePlayerAchievement(playerid, EAM_MafiaCarSpawned, 1);
			GivePlayerAchievement(playerid, EAM_CarSpawner, 1);
			CreatePlayerVehicleSafe(playerid, Player[playerid].Mafia->PrivateVehicle[ModelID - 10].ModelID, 0.0, 0.0, 1.5);
			Player[playerid].Mafia->PrivateVehicle[ModelID - 10].Apply(Player[playerid].PlayerVehicle);
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_v_slot_error_mafia);
		return true;
	}

	if (ModelID < 400 || ModelID > 611)
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_v_no_car_found_error);
		return true;
	}

	if (IsVehicleTrain(ModelID) || IsVehicleTrainTrailer(ModelID) || IsDisallowedVehicle(ModelID))
	{
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_v_cannot_spawn_error);
		return true;
	}

	GivePlayerAchievement(playerid, EAM_CarSpawner, 1);

	CreatePlayerVehicleSafe(playerid, ModelID, 0.0, 0.0, 1.5);
	return true;
}

#ifdef _LOCALHOST_DEBUG
ZCMD3(v2, 0, 0)
{
	ZCMD_CALL_COMMAND(playerid, "/v", params);
	if (Player[playerid].PlayerVehicle)
	{
		VehicleSpawnedBy[Player[playerid].PlayerVehicle] = INVALID_PLAYER_ID;
		Player[playerid].PlayerVehicle = 0;
	}
	return true;
}
#endif

ZERO_COMMAND(cars, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_ONLY_ON_FOOT, cmd_alias({ "/pojazdy", "/vehicles", "/vehs" }))
{
	ShowPlayerCustomDialog(playerid, "vehiclespawner_a", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_carlist), 
		 TranslateP(playerid, L_vehicles_carlist), "V", "X");
	return true;
}

ZERO_DIALOG(vehiclespawner_b)
{
	if (response)
	{
		if (inputtext.compare("<–") == 0)
		{
			ShowPlayerCustomDialog(playerid, "vehiclespawner_a", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_carlist),
				TranslateP(playerid, L_vehicles_carlist), "V", "X");
		}
		else
		{
			ZCMD_CALL_COMMAND(playerid, "/v", inputtext);
		}
	}
}

std::string	NazwyPojazdow[612];
std::string	NazwyPojazdowNS[612];

void ProcessVehicleNames(const std::vector<std::string> &names, const std::vector<int>& modelids)
{
	for (size_t i = 0; i < names.size(); ++i)
	{
		int modelid = modelids[i];
		std::string name = names[i];
		NazwyPojazdow[modelid] = name;
		NazwyPojazdow[modelid - 400] = name;
	}
}

void ProcessAddVehicles()
{
	static const std::vector<std::string> Airplanes			= { "Andromada", "AT-400", "Beagle", "Cropduster", "Dodo", "Hydra", "Nevada", "Rustler", "Shamal", "Skimmer", "Stuntplane" };
	static const std::vector<std::string> Helicopters		= { "Cargobob", "Hunter", "Leviathan", "Maverick", "News Maverick", "Police Maverick", "Raindance", "Seasparrow", "Sparrow" };
	static const std::vector<std::string> Bikes				= { "BF-400", "Bike", "BMX", "Faggio", "FCR-900", "Freeway", "Mountain Bike", "NRG-500", "PCJ-600", "Pizzaboy", "Quad", "Sanchez", "Wayfarer" };
	static const std::vector<std::string> Kabriolets		= { "Comet", "Feltzer", "Stallion", "Windsor" };
	static const std::vector<std::string> Industrial		= { "Benson", "Bobcat", "Burrito", "Boxville", "Boxburg", "Cement Truck", "DFT-30", "Flatbed", "Linerunner", "Mule", "Newsvan", "Packer", "Petrol Tanker", "Pony", "Roadtrain", "Rumpo", "Sadler", "Sadler Shit", "Topfun", "Tractor", "Trashmaster", "Utility Van", "Walton", "Yankee", "Yosemite" };
	static const std::vector<std::string> Lowriders			= { "Blade", "Broadway", "Remington", "Savanna", "Slamvan", "Tahoma", "Tornado", "Voodoo" };
	static const std::vector<std::string> OffRoad			= { "Bandito", "BF Injection", "Dune", "Huntley", "Landstalker", "Mesa", "Monster", "Monster A", "Monster B", "Patriot", "Rancher A", "Rancher B", "Sandking" };
	static const std::vector<std::string> PublicService		= { "Ambulance", "Barracks", "Bus", "Cabbie", "Coach", "Cop Bike (HPV-1000)", "Enforcer", "FBI Rancher", "FBI Truck", "Firetruck", "Firetruck LA", "Police Car (LSPD)", "Police Car (LVPD)", "Police Car (SFPD)", "Ranger", "Rhino", "S.W.A.T", "Taxi" };
	static const std::vector<std::string> Saloons			= { "Admiral", "Bloodring Banger", "Bravura", "Buccaneer", "Cadrona", "Clover", "Elegant", "Elegy", "Emperor", "Esperanto", "Fortune", "Glendale Shit", "Glendale", "Greenwood", "Hermes", "Intruder", "Majestic", "Manana", "Merit", "Nebula", "Oceanic", "Picador", "Premier", "Previon", "Primo", "Sentinel", "Stafford", "Sultan", "Sunrise", "Tampa", "Vincent", "Virgo", "Willard", "Washington" };
	static const std::vector<std::string> Sports			= { "Alpha", "Banshee", "Blista Compact", "Buffalo", "Bullet", "Cheetah", "Club", "Euros", "Flash", "Hotring Racer", "Hotring Racer A", "Hotring Racer B", "Infernus", "Jester", "Phoenix", "Sabre", "Super GT", "Turismo", "Uranus", "ZR-350" };
	static const std::vector<std::string> Combi				= { "Moonbeam", "Perenniel", "Regina", "Solair", "Stratum" };
	static const std::vector<std::string> Boats				= { "Coastguard", "Dinghy", "Jetmax", "Launch", "Marquis", "Predator", "Reefer", "Speeder", "Squallo", "Tropic" };
	static const std::vector<std::string> Trailers			= { "Article Trailer", "Article Trailer 2", "Article Trailer 3", "Baggage Trailer A", "Baggage Trailer B", "Farm Trailer", "Freight Flat Trailer (Train)", "Freight Box Trailer (Train)", "Petrol Trailer", "Streak Trailer (Train)", "Stairs Trailer", "Utility Trailer" };
	static const std::vector<std::string> Trains			= { "Brownstreak (Train)", "Freight (Train)" };
	static const std::vector<std::string> TrainTrailers		= { "Freight Flat Trailer (Train)", "Freight Box Trailer (Train)", "Streak Trailer (Train)" };
	static const std::vector<std::string> Unique			= { "Baggage", "Brownstreak (Train)", "Caddy", "Camper A", "Camper B", "Combine Harvester", "Dozer", "Dumper", "Forklift", "Freight (Train)", "Hotknife", "Hustler", "Hotdog", "Kart", "Mower", "Mr Whoopee", "Romero", "Securicar", "Stretch", "Sweeper", "Tram", "Towtruck", "Tug", "Vortex" };
	static const std::vector<std::string> RemoteControl		= { "RC Bandit", "RC Baron", "RC Raider", "RC Goblin", "RC Tiger", "RC Cam" };

	static const std::vector<int> modelid_Airplanes			= { 592, 577, 511, 512, 593, 520, 553, 476, 519, 460, 513 };
	static const std::vector<int> modelid_Helicopters		= { 548, 425, 417, 487, 488, 497, 563, 447, 469 };
	static const std::vector<int> modelid_Bikes				= { 581, 509, 481, 462, 521, 463, 510, 522, 461, 448, 471, 468, 586 };
	static const std::vector<int> modelid_Kabriolets		= { 480, 533, 439, 555 };
	static const std::vector<int> modelid_Industrial		= { 499, 422, 482, 498, 609, 524, 578, 455, 403, 414, 582, 443, 514, 413, 515, 440, 543, 605, 459, 531, 408, 552, 478, 456, 554 };
	static const std::vector<int> modelid_Lowriders			= { 536, 575, 534, 567, 535, 566, 576, 412 };
	static const std::vector<int> modelid_OffRoad			= { 568, 424, 573, 579, 400, 500, 444, 556, 557, 470, 489, 505, 495 };
	static const std::vector<int> modelid_PublicService		= { 416, 433, 431, 438, 437, 523, 427, 490, 528, 407, 544, 596, 598, 597, 599, 432, 601, 420 };
	static const std::vector<int> modelid_Saloons			= { 445, 504, 401, 518, 527, 542, 507, 562, 585, 419, 526, 604, 466, 492, 474, 546, 517, 410, 551, 516, 467, 600, 426, 436, 547, 405, 580, 560, 550, 549, 540, 491, 529, 421 };
	static const std::vector<int> modelid_Sports			= { 602, 429, 496, 402, 541, 415, 589, 587, 565, 494, 502, 503, 411, 559, 603, 475, 506, 451, 558, 477 };
	static const std::vector<int> modelid_Combi				= { 418, 404, 479, 458, 561 };
	static const std::vector<int> modelid_Boats				= { 472, 473, 493, 595, 484, 430, 453, 452, 446, 454 };
	static const std::vector<int> modelid_Trailers			= { 435, 450, 591, 606, 607, 610, 569, 590, 584, 570, 608, 611 };
	static const std::vector<int> modelid_Unique			= { 485, 537, 457, 483, 508, 532, 486, 406, 530, 538, 434, 545, 588, 571, 572, 423, 442, 428, 409, 574, 449, 525, 583, 539 };
	static const std::vector<int> modelid_Trains			= { 537, 538 };
	static const std::vector<int> modelid_TrainTrailers		= { 569, 590, 570 };
	static const std::vector<int> modelid_RemoteControl		= { 441, 464, 465, 501, 564, 594 };
	static const std::vector<int> modelid_Armed				= { 425, 430, 432, 447, 464, 476, 520 };

	for (auto i : modelid_Airplanes)	{ VehicleModelInfo[i] = VEHICLE_AIR   | VEHICLE_AIRPLANE; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Helicopters)	{ VehicleModelInfo[i] = VEHICLE_AIR   | VEHICLE_HELICOPTER; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Bikes)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_BIKE; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Kabriolets)	{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Industrial)	{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Lowriders)	{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_OffRoad)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_PublicService){ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Saloons)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Sports)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Combi)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Boats)		{ VehicleModelInfo[i] = VEHICLE_WATER | VEHICLE_BOAT; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Trailers)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_TRAILER; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_Unique)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_CAR; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_RemoteControl){ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_REMOTECONTROL; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }

	for (auto i : modelid_Trains)		{ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_TRAIN; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }
	for (auto i : modelid_TrainTrailers){ VehicleModelInfo[i] = VEHICLE_LAND  | VEHICLE_TRAINTRAILER; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }

	for (auto i : modelid_Armed)		{ VehicleModelInfo[i] = VehicleModelInfo[i] | VEHICLE_ARMED; VehicleModelInfo[i - 400] = VehicleModelInfo[i]; }

	VehicleModelInfo[501] |= VEHICLE_HELICOPTER;
	VehicleModelInfo[101] |= VEHICLE_HELICOPTER;
	VehicleModelInfo[465] |= VEHICLE_HELICOPTER;
	VehicleModelInfo[65] |= VEHICLE_HELICOPTER;

	ProcessVehicleNames(Airplanes, modelid_Airplanes);
	ProcessVehicleNames(Helicopters, modelid_Helicopters);
	ProcessVehicleNames(Bikes, modelid_Bikes);
	ProcessVehicleNames(Kabriolets, modelid_Kabriolets);
	ProcessVehicleNames(Industrial, modelid_Industrial);
	ProcessVehicleNames(Lowriders, modelid_Lowriders);
	ProcessVehicleNames(OffRoad, modelid_OffRoad);
	ProcessVehicleNames(PublicService, modelid_PublicService);
	ProcessVehicleNames(Saloons, modelid_Saloons);
	ProcessVehicleNames(Sports, modelid_Sports);
	ProcessVehicleNames(Combi, modelid_Combi);
	ProcessVehicleNames(Boats, modelid_Boats);
	ProcessVehicleNames(Trailers, modelid_Trailers);
	ProcessVehicleNames(Unique, modelid_Unique);
	ProcessVehicleNames(RemoteControl, modelid_RemoteControl);

	for (size_t i = 0; i < 612; ++i)
	{
		NazwyPojazdowNS[i] = boost::replace_all_copy(NazwyPojazdow[i], " ", "");
	}
}

class MakeVehicleFuckingWorkAfter8FuckingHoursAndFuckHardCodingArrays : public Extension::Base
{
public:
	bool OnGameModeInit() override
	{
		ProcessAddVehicles();
		return true;
	}
} _MakeVehicleFuckingWorkAfter8FuckingHoursAndFuckHardCodingArrays;

ZERO_DIALOG(vehiclespawner_a)
{
	if (response)
	{
		switch (listitem)
		{
			case 0: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_planes), "Andromada\nAT-400\nBeagle\nCropduster\nDodo\nHydra\nNevada\nRustler\nShamal\nSkimmer\nStuntplane\n<–", "V", "X"); break;
			case 1: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_helis), "Cargobob\nHunter\nLeviathan\nMaverick\nNews Maverick\nPolice Maverick\nRaindance\nSeasparrow\nSparrow\n<–", "V", "X"); break;
			case 2: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_twowheeled), "BF-400\nBike\nBMX\nFaggio\nFCR-900\nFreeway\nMountain Bike\nNRG-500\nPCJ-600\nPizzaboy\nQuad\nSanchez\nWayfarer\n<–", "V", "X"); break;
			case 3: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_cabrio), "Comet\nFeltzer\nStallion\nWindsor\n<–", "V", "X"); break;
			case 4: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_industrial), "Benson\nBobcat\nBurrito\nBoxville\nBoxburg\nCement Truck\nDFT-30\nFlatbed\nLinerunner\nMule\nNewsvan\nPacker\nPetrol Tanker\nPony\nRoadtrain\nRumpo\nSadler\nSadler Shit\nTopfun\nTractor\nTrashmaster\nUtility Van\nWalton\nYankee\nYosemite\n<–", "V", "X"); break;
			case 5: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_lowriders), "Blade\nBroadway\nRemington\nSavanna\nSlamvan\nTahoma\nTornado\nVoodoo\n<–", "V", "X"); break;
			case 6: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_offroad), "Bandito\nBF Injection\nDune\nHuntley\nLandstalker\nMesa\nMonster\nMonster A\nMonster B\nPatriot\nRancher A\nRancher B\nSandking\n<–", "V", "X"); break;
			case 7: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_public_service), "Ambulance\nBarracks\nBus\nCabbie\nCoach\nCop Bike (HPV-1000)\nEnforcer\nFBI Rancher\nFBI Truck\nFiretruck\nFiretruck LA\nPolice Car (LSPD)\nPolice Car (LVPD)\nPolice Car (SFPD)\nRanger\nRhino\nS.W.A.T\nTaxi\n<–", "V", "X"); break;
			case 8: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_saloons), "Admiral\nBloodring Banger\nBravura\nBuccaneer\nCadrona\nClover\nElegant\nElegy\nEmperor\nEsperanto\nFortune\nGlendale Shit\nGlendale\nGreenwood\nHermes\nIntruder\nMajestic\nManana\nMerit\nNebula\nOceanic\nPicador\nPremier\nPrevion\nPrimo\nSentinel\nStafford\nSultan\nSunrise\nTampa\nVincent\nVirgo\nWillard\nWashington\n<–", "V", "X"); break;
			case 9: ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_sport), "Alpha\nBanshee\nBlista Compact\nBuffalo\nBullet\nCheetah\nClub\nEuros\nFlash\nHotring Racer\nHotring Racer A\nHotring Racer B\nInfernus\nJester\nPhoenix\nSabre\nSuper GT\nTurismo\nUranus\nZR-350\n<–", "V", "X"); break;
			case 10:ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_wagons), "Moonbeam\nPerenniel\nRegina\nSolair\nStratum\n<–", "V", "X"); break;
			case 11:ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_boats), "Coastguard\nDinghy\nJetmax\nLaunch\nMarquis\nPredator\nReefer\nSpeeder\nSquallo\nTropic\n<–", "V", "X"); break;
			case 12:ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_trailers), "Article Trailer\nArticle Trailer 2\nArticle Trailer 3\nBaggage Trailer A\nBaggage Trailer B\nFarm Trailer\nFreight Flat Trailer (Train)\nFreight Box Trailer (Train)\nPetrol Trailer\nStreak Trailer (Train)\nStairs Trailer\nUtility Trailer\n<–", "V", "X"); break;
			case 13:ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_unique), "Baggage\nBrownstreak (Train)\nCaddy\nCamper A\nCamper B\nCombine Harvester\nDozer\nDumper\nForklift\nFreight (Train)\nHotknife\nHustler\nHotdog\nKart\nMower\nMr Whoopee\nRomero\nSecuricar\nStretch\nSweeper\nTram\nTowtruck\nTug\nVortex\n<–", "V", "X"); break;
			case 14:ShowPlayerCustomDialog(playerid, "vehiclespawner_b", DIALOG_STYLE_LIST, TranslateP(playerid, L_vehicles_remotecontrol), "RC Bandit\nRC Baron\nRC Raider\nRC Goblin\nRC Tiger\nRC Cam\n<–", "V", "X"); break;
		}
	}
}

ZCMDF(vcolor, PERMISSION_NONE, RESTRICTION_ONLY_DRIVER | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/vkolor" }), "DD")
{
	if (parser.Good())
	{
		unsigned long colors[2] = { parser.Get<unsigned long>(0), parser.Get<unsigned long>(1) };
		if (parser.Good() == 1)
		{
			colors[1] = colors[0];
		}
		if (colors[0] < 256 && colors[1] < 256)
		{
			fixChangeVehicleColor(Player[playerid].CurrentVehicle, colors[0], colors[1]);
			return true;
		}
	}
	std::array<int, 2> colors = GetVehicleColor(Player[playerid].CurrentVehicle);
	fixSendClientMessageF(playerid, Color::COLOR_ERROR, L_vcolor_usage, true, true, colors[0], colors[1]);
	return true;
}

ZCMDF(vpaint, PERMISSION_NONE, RESTRICTION_ONLY_DRIVER | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/vpaintjob", "/vpaintjobid", "/paintjob" }), "D")
{
	if (parser.Good())
	{
		unsigned long paintjob = parser.Get<unsigned long>();
		if (paintjob < 4)
		{
			fixChangeVehiclePaintjob(Player[playerid].CurrentVehicle, paintjob);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, Functions::string_format(TranslateP(playerid, L_vpaint_usage), GetVehiclePaintjob(Player[playerid].CurrentVehicle)));
	return true;
}

ZCMD(tune, PERMISSION_NONE, RESTRICTION_ONLY_DRIVER | RESTRICTION_ONLY_IN_CAR | RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT, cmd_alias({ "/tuning", "/tunemenu", "/tune.menu", "/tune-menu" }))
{
	static const std::set<int> SFvehicles = { 562, 565, 559, 561, 560, 558 };
	static const std::set<int> LSvehicles = { 536, 575, 534, 567, 535, 576 };
	static const std::set<int> NotTuningVehicles = { 406, 407, 416, 423, 428, 427, 431, 432, 433, 437, 486, 490, 528, 532, 544, 571, 572, 574, 596, 597, 598, 599, 601 };

	int model = GetVehicleModel(Player[playerid].CurrentVehicle);
	if (NotTuningVehicles.find(model) != NotTuningVehicles.end())
	{
		return fixSendClientMessage(playerid, Color::COLOR_ERROR, TranslateP(playerid, L_tune_cannot_tune_error));
	}
	if (SFvehicles.find(model) != SFvehicles.end())
	{
		return ZCMD_CALL_COMMAND(playerid, "/tunesf", "");
	}
	if (LSvehicles.find(model) != LSvehicles.end())
	{
		return ZCMD_CALL_COMMAND(playerid, "/tunels", "");
	}
	return ZCMD_CALL_COMMAND(playerid, "/tunelv", "");
}

ZCMD3(nrg, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_ONLY_ON_FOOT)
{
	return ZCMD_CALL_COMMAND(playerid, "/v", "nrg");
}

ZCMD3(infernus, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_ONLY_ON_FOOT)
{
	return ZCMD_CALL_COMMAND(playerid, "/v", "infernus");
}

ZCMD3(turismo, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_ONLY_ON_FOOT)
{
	return ZCMD_CALL_COMMAND(playerid, "/v", "turismo");
}