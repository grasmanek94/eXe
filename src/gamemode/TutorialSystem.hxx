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

#if defined USE_TUTORIAL
enum TutorialSystemStates
{
	TSS_None = 0,
	TSS_Started = 1,
	//anything in between == in progress
	TSS_Finished = 100
};

enum TutorialSystem
{
	TS_BuyWeapon,
	TS_BuyAmmo,
	TS_EquipWeapon,
	TS_UnequipWeapon,
	TS_SpawnCarV,
	TS_VSaveCar,
	TS_SpawnVSaveCar,
	TS_Animation,
	TS_GoldCoins,
	TS_UseMenuHeal,
	TS_TelesView,
	TS_Odliczanie,
	TS_Ramps,
	TS_ZabawyJoin
};

namespace Tutorial
{
	static const inline std::string PressKey(std::string num, std::string key)
	{
		return "~w~" + num + ". Wcisnij przycisk '~g~" + key + "~w~'~n~";
	}

	static const inline std::string TutrName(std::string name)
	{
		return "~r~Poradnik " + name + "~n~";
	}

	static const inline std::string MakeDesc(std::string desc)
	{
		return "~w~Opis: " + desc + "~n~";
	}

	static const inline std::string AskInput(std::string num, std::string what)
	{
		return "~w~" + num + ". Wpisz: ~b~~h~" + what + "~n~";
	}

	static const inline std::string DoAction(std::string num, std::string what)
	{
		return "~w~" + num + ". Wykonaj czynnosc: ~y~~h~" + what + "~n~";
	}

	extern const std::vector<std::string> Text;
};
#endif