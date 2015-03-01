/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Structure definitions for the quick menu manager

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
#pragma warning(disable: 4503)
// PRESSED(keys)
#define PRESSED(a) \
	(((newkeys & (a)) == (a)) && ((oldkeys & (a)) != (a)))

// RELEASED(keys)
#define RELEASED(a) \
	(((newkeys & (a)) != (a)) && ((oldkeys & (a)) == (a)))

// HOLDING(keys)
#define HOLDING(a) \
	((newkeys & (a)) == (a))

#define KEY_OPEN_ACTION_MENU	KEY_YES
#define KEY_ITEM_DOWN			KEY_NO
#define KEY_ITEM_UP				KEY_CTRL_BACK
#define KEY_ITEM_UP_VEHICLE		KEY_CROUCH 

class PlayerDataManager
{
public:
	bool MenuShown;
	int PlayerTextDraw;
	int SelectedItem;
} ;

void MM_FixRefresh(int playerid);
namespace PlayerMenu
{
	typedef std::pair<int,boost::variant<std::string,int>> ActionSubset;
	typedef std::vector<ActionSubset> ActionData;

	class Function
	{
	public:
		virtual void ItemSelected(int,std::string) = 0;
	};

	class Display
	{
	public:
		typedef Function ActionFunction;
		typedef std::pair<std::string, ActionFunction*> MainMenuItemData;
	private:
		int amount_of_items;
		std::map<std::string, ActionFunction*> MenuItems;
		int playerid;
	public:
		PlayerDataManager Menu_Display;
		Display(int playerid);
		void SelectNextItem();
		void SelectPreviousItem();
		void AddItem(std::string item, ActionData& data, ActionFunction* f);
		void RemoveItem(std::string item);
		void Clear();
		std::string GetString();
		void OnItemSelect(int itemid);
	};
};