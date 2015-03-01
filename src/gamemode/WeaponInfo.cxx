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

const std::vector<SWeaponInfo> WeaponInfo = 
{
	{"Fist"					,  0,  false,	0,			0,		0.0,	0		, false, false, true , 0 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Brass knuckles"		,  1,  true,	500,		10,		1.0,	1		, false, false, true , 0 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Golf club"			,  2,  true,	500,		10,		1.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Nite stick"			,  3,  true,	500,		10,		1.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Knife"				,  4,  true,	0,			0,		0.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Baseball bat"			,  5,  true,	500,		10,		1.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Shovel"				,  6,  true,	500,		10,		1.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Pool cue"				,  7,  true,	500,		10,		1.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Kantana"				,  8,  true,	10000,		30,		1.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Chainsaw"				,  9,  true,	10000,		30,		2.0,	1		, false, false, true , 1 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Purple dildo"			,  10, true,	500,		10,		1.0,	1		, false, false, true , 10, true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Short vibrator"		,  11, true,	500,		10,		1.0,	1		, false, false, true , 10, true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Long vibrator"		,  12, true,	500,		10,		1.0,	1		, false, false, true , 10, true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"White dildo"			,  13, true,	500,		10,		1.0,	1		, false, false, true , 10, true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Flowers"				,  14, true,	500,		10,		1.0,	1		, false, false, true , 10, true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Cane"					,  15, true,	500,		10,		1.0,	1		, false, false, true , 10, true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Grenades"				,  16, true,	50000,		30,		3.0,	100		, false, false, true , 8 , false, false, 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 0.0f},
	{"Tear gas"				,  17, true,	10,			30,		3.0,	1		, true,  true, false , 8 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Molotov cocktail"		,  18, true,	50000,		30,		3.0,	100		, false, false, true , 8 , false, false, 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 0.0f},
	{"Vehicle missile"		,  19, false,	0,			0,		0.0,	0		, false, false, false, 0 , false, false, 0,		0,		0.0f,  false, false	 , 0.0f, 0.0f, 0.0f},
	{"Hydra flare"			,  20, false,	0,			0,		0.0,	0		, false, false, false, 0 , false, false, 0,		0,		0.0f,  false, false	 , 0.0f, 0.0f, 0.0f},
	{"Jetpack"				,  21, false,	0,			0,		0.0,	0		, false, false, false, 0 , false, false, 0,		0,		0.0f,  false, false	 , 0.0f, 0.0f, 0.0f},
	{"9mm Pistol"			,  22, true,	1500,		30,		2.0,	17		, true , true , false, 2 , true , true , 350,	90,		250.0f,  false, false, 0.0f, 0.0f, 1.0f},
	{"Silenced pistol"		,  23, true,	1500,		30,		2.0,	17		, true , true , false, 2 , true , true , 700,	170,	250.0f,  false, false, 0.0f, 0.0f, 1.0f},
	{"Desert eagle"			,  24, true,	0,			0,		0.0,	9999	, false,  true , false,2 , true , true , 2800,	700,	250.0f,  false, false, 0.0f, 0.0f, 7.0f},
	{"Shotgun"				,  25, true,	1500,		30,		1.0,	40		, true,  true , false, 3 , true , true , 2000,	500,	100.0f,  false, false, 0.0f, 0.0f, 5.0f},
	{"Sawn-off shotgun"		,  26, true,	1500,		70,		3.0,	30		, true,  true , false, 3 , true , true , 600,	50 ,	100.0f,  false, false, 0.0f, 0.0f, 6.0f},
	{"Combat shotgun"		,  27, true,	1500,		70,		3.0,	20		, true,  true , false, 3 , true , true , 600,	150,	100.0f,  false, false, 0.0f, 0.0f, 7.0f},
	{"Micro Uzi(Mac 10)"	,  28, true,	1500,		30,		1.0,	50		, true , true , false, 4 , true , true , 20,	5  ,	250.0f,  false, false, 0.0f, 0.0f, 1.0f},
	{"Mp5"					,  29, true,	1500,		70,		4.0,	30		, true , true , false, 4 , true , true , 20,	5  ,	250.0f,  false, false, 0.0f, 0.0f, 2.0f},
	{"Ak47"					,  30, true,	1000,		30,		1.0,	30		, true , true , false, 5 , true , true , 200,	50 ,	250.0f,  false, false, 0.0f, 0.0f, 3.0f},
	{"M4"					,  31, true,	0,			0,		0.0,	9999	, false, true , false, 5 , true , true , 200,	50 ,	250.0f,  false, false, 0.0f, 0.0f, 3.0f},
	{"Tec 9"				,  32, true,	0,			0,		0.0,	9999	, false, true , false, 4 , true , true , 20,	5  ,	250.0f,  false, false, 0.0f, 0.0f, 2.0f},
	{"Country rifle"		,  33, true,	1000,		10,		1.0,	25		, true , true , false, 6 , true , true , 2000,	500,	250.0f,  false, false, 0.0f, 0.0f, 5.0f},
	{"Sniper rifle"			,  34, true,	3500,		70,		4.0,	41		, true , true , false, 6 , true , true , 2000,	500,	250.0f,  false, false, 0.0f, 0.0f, 5.0f},
	{"Rpg"					,  35, true,	30000,		115,	10.0,	1		, true,  true , false, 7 , true , false, 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 0.0f},
	{"Heat seeking rocket"	,  36, true,	30000,		115,	10.0,	1		, true,  true , false, 7 , true , false, 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 0.0f},
	{"Flame-thrower"		,  37, true,	100,		115,	10.0,	10		, true, true , false,  7 , false, false, 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 0.0f},
	{"Mini gun"				,  38, true,	1000000,	3000,	23.0,	100		, true,  true , false, 7 , true , true , 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 2.0f},
	{"Satchel charges"		,  39, true,	1000,		90,		10.0,	1		, true,  true, false,  8 , true , false, 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 0.0f},
	{"Detonator"			,  40, true,	0,			0,		0.0,	1		, false, false, true , 12, false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{"Spray can"			,  41, true,	10,			10,		1.0,	1		, true,  true , false, 9 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Fire extinguisher"	,  42, true,	10,			10,		1.0,	1		, true,  true , false, 9 , true , false, 0,		0,		10.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Camera"				,  43, true,	0,			0,		0.0,	9999	, false, false, true , 9 , false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{"Night vision"			,  44, false,	0,			0,		0.0,	0		, false, false, false, 11, false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{"Thermal goggles"		,  45, false,	0,			0,		0.0,	0		, false, false, false, 11, false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{"Parachute"			,  46, true,	0,			0,		0.0,	9999	, false, false, true , 11, false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{"Fake Pistol"			,  47, false,	0,			0,		0.0,	0		, false, false, true , 0 , false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{""						,  48, false,	0,			0,		0.0,	0		, false, false, false, 0 , false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{"Vehicle"				,  49, false,	0,			0,		0.0,	0		, false, false, false, 0 , true, false, 0,		0,		30.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Helicopter Blades"	,  50, false,	0,			0,		0.0,	0		, false, false, false, 0 , true, false, 0,		0,		30.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{"Explosion"			,  51, false,	0,			0,		0.0,	0		, false, false, false, 0 , true, false, 0,		0,		30.0f,  false, false , 0.0f, 0.0f, 0.0f},
	{""						,  52, false,	0,			0,		0.0,	0		, false, false, false, 0 , false, false, 0,		0,		250.0f,  false, false, 0.0f, 0.0f, 0.0f},
	{"Drowned"				,  53, false,	0,			0,		0.0,	0		, false, false, false, 0 , false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
	{"Splat"				,  54, false,	0,			0,		0.0,	0		, false, false, false, 0 , false, false, 0,		0,		0.0f,  false, false  , 0.0f, 0.0f, 0.0f},
};

void GivePlayerEquippedWeapons(int playerid, bool IN_SELF = false, bool self_remove = false)
{
	int count = 0;
	for (auto& i : Player[playerid].weapondata.equipped)
	{
		if (i < 48)
		{
			if (i && Player[playerid].weapondata.weapon[i].Owned)
			{
				safeGivePlayerWeapon(playerid, i, Player[playerid].weapondata.weapon[i].Ammo);
				++count;
			}
		}
	}
	if (!count && !IN_SELF && !self_remove)
	{
		Player[playerid].weapondata.equipped[WeaponInfo[4].category] = 4;
		Player[playerid].weapondata.equipped[WeaponInfo[24].category] = 24;
		Player[playerid].weapondata.equipped[WeaponInfo[31].category] = 31;
		Player[playerid].weapondata.equipped[WeaponInfo[32].category] = 32;
		GivePlayerEquippedWeapons(playerid, true);
	}
}

class PersistentWeaponsHelper : public Extension::Base
{
public:
	bool OnPlayerSpawn(int playerid) override
	{
		if (!Player[playerid].KeepInGame && Player[playerid].CurrentGameID == nullptr)
		{
			Player[playerid].weapondata.weapon[4].Ammo = 1;
			Player[playerid].weapondata.weapon[24].Ammo = 9999;
			Player[playerid].weapondata.weapon[32].Ammo = 9999;
			Player[playerid].weapondata.weapon[31].Ammo = 9999;
			GivePlayerEquippedWeapons(playerid);
		}
		return true;
	}
	void OnPlayerGameBegin(int playerid) override
	{
		if (Player[playerid].weapondata.weapon[5].Owned == 0)
		{
			Player[playerid].weapondata.weapon[5].Owned = 1;
			Player[playerid].weapondata.weapon[24].Owned = 1;
			Player[playerid].weapondata.weapon[31].Owned = 1;
			Player[playerid].weapondata.weapon[32].Owned = 1;

			Player[playerid].OwnedWeapons.insert(5);
			Player[playerid].OwnedWeapons.insert(24);
			Player[playerid].OwnedWeapons.insert(31);
			Player[playerid].OwnedWeapons.insert(32);
		}
	}
	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ) override
	{
		if (Player[playerid].CurrentGameID == nullptr && weaponid > 0 && weaponid < 48)
		{
			if (WeaponInfo[weaponid].bulletweapon && weaponid != 24 && weaponid != 32)
			{
				int ammo = GetPlayerAmmo(playerid) - 1;
				if (ammo < 0)
				{
					ammo = 0;
				}
				if (ammo < Player[playerid].weapondata.weapon[weaponid].Ammo || ammo == 1 || ammo == 0)
				{
					if (ammo == 1)
					{
						SetPlayerAmmo(playerid, weaponid, 0);
						ammo = 0;
					}		
					Player[playerid].weapondata.weapon[weaponid].Ammo = ammo;
				}
				else//ammo hack?
				{
					SetPlayerAmmo(playerid, weaponid, Player[playerid].weapondata.weapon[weaponid].Ammo);
				}
			}
		}
		return true;
	}
} _PersistentWeaponsHelper;

std::string PreparePlayerWepaonsDialog(int playerid)
{
	std::string output(TranslateP(playerid, L_weapons_dialog_text_a));
	for (auto i : WeaponInfo)
	{
		if (i.buyable)
		{
			if (i.cost_respect == 0 && i.cost_money == 0 && i.minlevel <= Player[playerid].GetLevel())
			{
				Player[playerid].weapondata.weapon[i.weaponid].Owned = 1;
				Player[playerid].OwnedWeapons.insert(i.weaponid);
				Player[playerid].weapondata.weapon[i.weaponid].Ammo = 9999;
			}

			if (Player[playerid].weapondata.weapon[i.weaponid].Owned)
			{
				if (Player[playerid].weapondata.equipped[i.category] == i.weaponid)//equipped
				{
					output.append(TranslatePF(playerid, L_weapons_dialog_text_dequip, i.weaponid, i.weaponName.c_str()));
				}
				else//not equipped
				{
					output.append(TranslatePF(playerid, L_weapons_dialog_text_equip, i.weaponid, i.weaponName.c_str()));
				}

				if (i.ammu_per_money > 0 && i.cost_money > 0)
				{
					if (i.ismagazine)
					{
						output.append(TranslatePF(playerid, L_weapons_dialog_text_ammo, i.ammu_per_money, i.cost_money));
					}
					else if (!i.onetimebuy)
					{
						output.append(Functions::string_format(" | {00FF00}$%I64u{FFFFFF}", i.cost_money));
					}
				}

				if (i.ismagazine && !i.onetimebuy)
				{
					output.append(TranslatePF(playerid, L_weapons_dialog_text_magazinestatus, Player[playerid].weapondata.weapon[i.weaponid].Ammo));
				}
			}
			else//not owned
			{
				output.append(TranslatePF(playerid, L_weapons_dialog_text_buy, i.weaponid, i.weaponName.c_str(), i.cost_respect));
				if (i.onetimebuy)
				{
					output.append(Functions::string_format("& {00FF00}$%I64u{FFFFFF}", i.cost_money));
				}
				output.append(TranslatePF(playerid, L_weapons_dialog_text_level, i.minlevel));
			}
			output.append("\n");
		}
	}
	output.append(TranslateP(playerid, L_weapons_dialog_text_b));
	return output;
}

void ShowPersistentWeaponsMenu(int playerid)
{
	ShowPlayerCustomDialog(playerid, "kupbronie", DIALOG_STYLE_LIST, TranslateP(playerid, L_weapons_dialog_title), PreparePlayerWepaonsDialog(playerid), TranslateP(playerid, L_Confirm), TranslateP(playerid, L_Cancel));
}

ZCMD(bronie, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/broniemenu", "/weapon", "/giveweapon", "/bron", "/kupbron", "/weapons", "/wapens", "/wapen", "/gun", "/guns", "/buygun", "/buyguns", "/buyweapon", "/buyweapons" }))
{
	ShowPersistentWeaponsMenu(playerid);
	return true;
}

void GivePlayerBuyWeapon(int playerid, int weaponid, int ammo, bool free)
{
	if (WeaponInfo[weaponid].onetimebuy)
	{
		if (!free)
		{
			Player[playerid].GiveMoney(-WeaponInfo[weaponid].cost_money);
		}

		Player[playerid].weapondata.weapon[weaponid].Ammo = WeaponInfo[weaponid].ammu_per_money;
	}

	if (!free)
	{
		Player[playerid].GiveScore(-WeaponInfo[weaponid].cost_respect);
	}

	if (ammo)
	{
		Player[playerid].weapondata.weapon[weaponid].Ammo = ammo;
	}

	Player[playerid].weapondata.weapon[weaponid].Owned = 1;
	Player[playerid].OwnedWeapons.insert(weaponid);
}

ZERO_DIALOG(kupbronie)
{
	if (response && CheckCommandAllowed(playerid, RESTRICTION_NOT_IN_A_GAME, false))
	{
		char action = inputtext[0];
		int weaponid = 0;

		ParseInput parser("wws", inputtext);
		if (parser.Good() == 3)
		{
			if (Functions::is_number_unsigned(
				boost::replace_all_copy(
				boost::replace_all_copy(
				parser.Get<std::string>(1),
				"(", ""),
				")", ""),
				weaponid))
			{
				if (weaponid > 0 && weaponid < 48)
				{
					std::string message;
					switch (action)
					{
					case '$':
						if (Player[playerid].weapondata.weapon[weaponid].Owned)
						{
							message.assign(TranslateP(playerid, L_weapons_dialog_text_weaponowned));
						}
						else if (WeaponInfo[weaponid].minlevel > Player[playerid].GetLevel())
						{
							message.assign(TranslateP(playerid, L_weapons_dialog_text_lowlevel));
						}
						else if (WeaponInfo[weaponid].cost_respect > Player[playerid].statistics.respect)
						{
							message.assign(TranslateP(playerid, L_weapons_dialog_text_lowscore));
						}
						else if (!WeaponInfo[weaponid].buyable)
						{
							message.assign(TranslateP(playerid, L_weapons_dialog_text_invalidweaponid));
						}
						else if (WeaponInfo[weaponid].onetimebuy && WeaponInfo[weaponid].cost_money > Player[playerid].statistics.money)
						{
							message.assign(TranslateP(playerid, L_weapons_dialog_text_lowcash));
						}
						else
						{
							GivePlayerBuyWeapon(playerid, weaponid);

							ShowPersistentWeaponsMenu(playerid);
							return;
						}
						break;
					case '<':
						if (Player[playerid].weapondata.equipped[WeaponInfo[weaponid].category] == (unsigned int)weaponid)
						{
							Player[playerid].weapondata.equipped[WeaponInfo[weaponid].category] = 0;
							if (WeaponInfo[weaponid].ismagazine)
							{
								SetPlayerAmmo(playerid, weaponid, 0);
							}
							else
							{
								safeResetPlayerWeapons(playerid);
								GivePlayerEquippedWeapons(playerid, false, true);
							}
							ShowPersistentWeaponsMenu(playerid);
							return;
						}
						else
						{
							message.assign(TranslateP(playerid, L_weapons_dialog_text_notequiped));
						}
						break;
					case '>':
						if (Player[playerid].weapondata.weapon[weaponid].Owned)
						{
							if (Player[playerid].weapondata.weapon[weaponid].Ammo)
							{
								if (Player[playerid].weapondata.equipped[WeaponInfo[weaponid].category] != 0)
								{
									SetPlayerAmmo(playerid, Player[playerid].weapondata.equipped[WeaponInfo[weaponid].category], 0);
									Player[playerid].weapondata.equipped[WeaponInfo[weaponid].category] = 0;
								}
								Player[playerid].weapondata.equipped[WeaponInfo[weaponid].category] = weaponid;
								safeGivePlayerWeapon(playerid, weaponid, 1);
								SetPlayerAmmo(playerid, weaponid, Player[playerid].weapondata.weapon[weaponid].Ammo);
								ShowPersistentWeaponsMenu(playerid);
								return;
							}
							else
							{
								message.assign(TranslateP(playerid, L_weapons_dialog_text_noammo));
							}
						}
						else
						{
							message.assign(TranslateP(playerid, L_weapons_dialog_text_needbuy));
						}
						break;
					default:
						ShowPersistentWeaponsMenu(playerid);
						return;
						break;
					}

					ShowPlayerCustomDialog(playerid, "kupbroniemessage", DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_weapons_dialog_title), message, "V", "X");
					return;
				}
			}
		}
		ShowPersistentWeaponsMenu(playerid);
	}
}

ZCMDF(amunicja, PERMISSION_NONE, RESTRICTION_NOT_AFTER_FIGHT | RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/ammo", "/buyammo" }), "DD")
{
	if (parser.Good() == 2)
	{
		unsigned long weaponid = parser.Get<unsigned long>(0);
		unsigned long ammo = parser.Get<unsigned long>(1);
		if (weaponid > 0 && weaponid < 48)
		{
			if (WeaponInfo[weaponid].buyable && WeaponInfo[weaponid].bulletweapon)
			{
				if (Player[playerid].weapondata.weapon[weaponid].Owned)
				{
					if (ammo > 0)
					{
						if (!(Player[playerid].weapondata.weapon[weaponid].Ammo >= 25 && (weaponid == 35 || weaponid == 36)))
						{
							unsigned long topay = (unsigned long)ceil((double)WeaponInfo[weaponid].cost_money / (double)WeaponInfo[weaponid].ammu_per_money * (double)ammo);
							if (topay <= Player[playerid].statistics.money)
							{
								Player[playerid].weapondata.weapon[weaponid].Ammo += ammo;
								if (Player[playerid].weapondata.equipped[WeaponInfo[weaponid].category] == weaponid)
								{
									safeGivePlayerWeapon(playerid, weaponid, ammo);
								}
								Player[playerid].GiveMoney(-(long long)topay);
								fixSendClientMessage(playerid, -1, TranslatePF(playerid, L_buyammo_success, WeaponInfo[weaponid].weaponName.c_str(), Player[playerid].weapondata.weapon[weaponid].Ammo));
								return true;
							}
							fixSendClientMessage(playerid, -1, L_weapons_dialog_text_lowcash);
							return true;
						}
						fixSendClientMessage(playerid, -1, L_buyammo_error_maxammo);
						return true;
					}
					fixSendClientMessage(playerid, -1, L_buyammo_error_0ammo);
					return true;
				}
				fixSendClientMessage(playerid, -1, L_weapons_dialog_text_needbuy);
				return true;
			}
		}
		fixSendClientMessage(playerid, -1, L_buyammo_error_badbulletweapon);
		return true;
	}
	fixSendClientMessage(playerid, -1, L_buyammo_usage);
	return true;
}

ZERO_DIALOG(kupbroniemessage)
{
	if (response)
	{
		ShowPersistentWeaponsMenu(playerid);
	}
}