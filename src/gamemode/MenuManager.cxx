/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the quick menu manager
		-	This source file manages the menu and the textdraws for it

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies
		Extension
		WorldData

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

class _HealVehicle : public PlayerMenu::Function
{
public:
	void ItemSelected(int playerid, std::string item)
	{
		ZCMD_CALL_COMMAND(playerid, "/fix", "");
	}
} HealVehicle;

class MenuManager: public Extension::Base
{
public:
	PlayerMenu::ActionData Data;
	bool OnPlayerKeyStateChange(int playerid,int newkeys, int oldkeys);
	bool OnPlayerConnect(int playerid);
	bool OnPlayerDisconnect(int playerid, int reason);
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate)
	{
		if (newstate == PLAYER_STATE_DRIVER)
		{
			Player[playerid].QuickMenu->AddItem("0. FIX", Data, &HealVehicle);
		}
		else
		{
			Player[playerid].QuickMenu->RemoveItem("0. FIX");
		}
		return true;
	}
} _MenuManager;

class _ExitMenu: public PlayerMenu::Function
{
public:
	void ItemSelected(int playerid,std::string item)
	{
	
	}
} ExitMenu;

class _HealMe : public PlayerMenu::Function
{
public:
	void ItemSelected(int playerid, std::string item)
	{
		ZCMD_CALL_COMMAND(playerid, "/100hp", "");
	}
} HealMe;

class _ArmorMe : public PlayerMenu::Function
{
public:
	void ItemSelected(int playerid, std::string item)
	{
		ZCMD_CALL_COMMAND(playerid, "/pancerz", "");
	}
} ArmorMe;

bool MenuManager::OnPlayerConnect(int playerid)
{
	Player[playerid].QuickMenu->Menu_Display.MenuShown = false;
	Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw = CreatePlayerTextDraw(playerid, 398.0f,216.0f,"_");
	
	PlayerTextDrawColor(playerid,Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw,0x115551EE);
	PlayerTextDrawFont(playerid,Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, 1);
	PlayerTextDrawLetterSize(playerid,Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, 0.3f, 1.1f);
	PlayerTextDrawAlignment(playerid,Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, 2);
	PlayerTextDrawUseBox(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, true);
	PlayerTextDrawTextSize(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, 120.0, 90.0);
	Player[playerid].QuickMenu->Menu_Display.SelectedItem = 0;
	//GameTextForPlayer(playerid,"~n~~n~~n~Use the ~k~~CONVERSATION_YES~ key to open the action menu and select an item~n~Use ~k~~CONVERSATION_NO~/~k~~GROUP_CONTROL_BWD~ keys to choose between items",7500,4);
	PlayerMenu::ActionData l_Data;
	Player[playerid].QuickMenu->AddItem("1. HEAL", l_Data, &HealMe);
	Player[playerid].QuickMenu->AddItem("2. ARMOUR", l_Data, &ArmorMe);
	Player[playerid].QuickMenu->AddItem("3. EXIT", l_Data, &ExitMenu);
	return true;
}

bool MenuManager::OnPlayerDisconnect(int playerid, int reason)
{
	Player[playerid].QuickMenu->Clear();
	return true;
}

void MM_FixRefresh(int playerid)
{
	if (Player[playerid].QuickMenu->Menu_Display.MenuShown)
	{
		//hide menu
		PlayerTextDrawHide(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw);
		Player[playerid].QuickMenu->Menu_Display.SelectedItem = 0;
		//show menu
		PlayerTextDrawShow(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw);
		PlayerTextDrawSetString(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, Player[playerid].QuickMenu->GetString().c_str());
	}
}

bool MenuManager::OnPlayerKeyStateChange(int playerid,int newkeys, int oldkeys)
{
	if (Player[playerid].CurrentGameID == nullptr)
	{
		if (PRESSED(KEY_OPEN_ACTION_MENU))
		{
			if (Player[playerid].QuickMenu->Menu_Display.MenuShown)
			{
				//hide menu
				PlayerTextDrawHide(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw);
				Player[playerid].QuickMenu->OnItemSelect(Player[playerid].QuickMenu->Menu_Display.SelectedItem);
				Player[playerid].QuickMenu->Menu_Display.MenuShown = false;
				Player[playerid].QuickMenu->Menu_Display.SelectedItem = 0;
			}
			else
			{
				//show menu
				PlayerTextDrawShow(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw);
				PlayerTextDrawSetString(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, Player[playerid].QuickMenu->GetString().c_str());
				Player[playerid].QuickMenu->Menu_Display.MenuShown = true;
			}
		}
	}
	if (Player[playerid].QuickMenu->Menu_Display.MenuShown)
	{
		if (Player[playerid].CurrentGameID != nullptr)
		{
			//hide menu
			PlayerTextDrawHide(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw);
			Player[playerid].QuickMenu->OnItemSelect(Player[playerid].QuickMenu->Menu_Display.SelectedItem);
			Player[playerid].QuickMenu->Menu_Display.MenuShown = false;
			Player[playerid].QuickMenu->Menu_Display.SelectedItem = 0;
		}
		else
		{
			if (PRESSED(KEY_ITEM_DOWN))
			{
				Player[playerid].QuickMenu->SelectNextItem();
				PlayerTextDrawSetString(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, Player[playerid].QuickMenu->GetString().c_str());
			}
			if ((!Player[playerid].CurrentVehicle && PRESSED(KEY_ITEM_UP)) || (Player[playerid].CurrentVehicle && PRESSED(KEY_ITEM_UP_VEHICLE)))
			{
				Player[playerid].QuickMenu->SelectPreviousItem();
				PlayerTextDrawSetString(playerid, Player[playerid].QuickMenu->Menu_Display.PlayerTextDraw, Player[playerid].QuickMenu->GetString().c_str());
			}
		}
	}
	return true;
}

////////////MENUMANAGER
namespace PlayerMenu
{
	Display::Display(int playerid)
		: playerid(playerid)
	{
		amount_of_items = 0;
	}
	void Display::SelectNextItem()
	{
		if (++Menu_Display.SelectedItem >= amount_of_items)
		{
			Menu_Display.SelectedItem = 0;
		}
	}
	void Display::SelectPreviousItem()
	{
		if (--Menu_Display.SelectedItem < 0)
		{
			Menu_Display.SelectedItem = 
				(amount_of_items > 0) ?
				amount_of_items - 1 : 0;
		}
	}
	void Display::AddItem(std::string item, ActionData& data, ActionFunction* f)
	{
		auto it = MenuItems.find(item);
		if (it == MenuItems.end())
		{
			MenuItems.emplace(MainMenuItemData(item, f));
		}
		amount_of_items = MenuItems.size();
		MM_FixRefresh(playerid);
	}
	void Display::RemoveItem(std::string item)
	{
		MenuItems.erase(item);
		amount_of_items = MenuItems.size();
		MM_FixRefresh(playerid);
	}
	void Display::Clear()
	{
		MenuItems.clear();
	}
	std::string Display::GetString()
	{
		std::string builder("");
		int CurrentIndex = 0;
		for (auto &i : MenuItems)
		{
			if (Menu_Display.SelectedItem == CurrentIndex++)
			{
				builder.append("~y~" + i.first + "~n~");
			}
			else
			{
				builder.append("~w~" + i.first + "~n~");
			}
		}
		return builder;
	}
	void Display::OnItemSelect(int itemid)
	{
		int CurrentIndex = 0;
		for (auto &i : MenuItems)
		{
			if (Menu_Display.SelectedItem == CurrentIndex++)
			{
				i.second->ItemSelected(playerid, i.first);
				return;
			}
		}
	}
};