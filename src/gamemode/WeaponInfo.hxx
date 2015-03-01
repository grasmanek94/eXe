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
#pragma once
#include "GameMode.hxx"

struct SWeaponInfo
{
	std::string weaponName;
	unsigned int weaponid;
	bool buyable;
	long long cost_money;
	long long cost_respect;
	double minlevel;
	unsigned int ammu_per_money;
	bool ismagazine;
	bool bulletweapon;
	bool onetimebuy;
	unsigned int category;
	bool CanKillWithIt;
	bool AC_CanBulletSync;
	unsigned long long min_time_between_serie;//measured over last four shots
	unsigned long long min_time_absolut_possible;
	float maxkilldistance;
	bool explosionkill;
	bool firekill;
	float mindamage;
	float maxdamage;
	float sentrygundamage;
};

extern const std::vector<SWeaponInfo> WeaponInfo;
void GivePlayerBuyWeapon(int playerid, int weaponid, int ammo = 0, bool free = false);