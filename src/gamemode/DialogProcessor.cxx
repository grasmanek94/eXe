/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the ZeroDialog processor & system & functions 

	________________________________________________________________	
	Notes
		-	Dialog spoofing is impossible with this

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

namespace ZeroDLG
{
	ZeroDialogInternal * __internal_dialog_registrar;
}

class DialogProcessor : public Extension::Base
{
public:
	bool OnDialogResponse(int playerid, int dialogid, int response, int listitem, std::string inputtext)
	{
		if (ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] != nullptr)
		{
			ZeroDLG::Dialog* copy = ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid];
			ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] = nullptr;
			copy->process_dialog(playerid, response, listitem, inputtext);
		}
		return false;
	}
	bool OnPlayerConnect(int playerid)
	{
		ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] = nullptr;
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] = nullptr;
		return true;
	}
} _DialogProcessor;

bool PrepareZeroDialog(std::string name, int dialog_style, std::string caption, std::string info, std::string button1, std::string button2)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
	{
		return false;
	}
	dialog->second->PrepareDialog(dialog_style,caption,info,button1,button2);
	return true;
}

bool ShowPlayerZeroDialog(int playerid, std::string name)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
	{
		return false;
	}
	dialog->second->Show(playerid);
	return true;
}

bool ShowPlayerZeroDialog(int playerid, std::string name, std::string text)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
	{
		return false;
	}
	dialog->second->Show(playerid,text);
	return true;
}

bool ShowPlayerCustomDialog(int playerid, std::string name, int dialog_style,std::string caption, std::string info, std::string button1, std::string button2)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
	{
		return false;
	}
	dialog->second->ShowCustom(playerid,dialog_style,caption,info,button1,button2);
	return true;
}

bool HidePlayerZeroDialog(int playerid)
{
	ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] = nullptr;
	ShowPlayerDialog(playerid,-1,-1,"","","","");
	return true;
}

ZERO_DIALOG(________dummy){}

///////////////DIALOGPROCESSOR

namespace ZeroDLG
{
	ZeroDialogInternal::ZeroDialogInternal() {}
	void  ZeroDialogInternal::register_dialog(Dialog* dialog, std::string name)
	{
		if (!init)
		{
			init = true;
			for (size_t i = 0; i < MAX_PLAYERS; ++i)
			{
				CurrentDialog[i] = nullptr;
			}
		}
		dialog_map[name] = dialog;
	}

	Dialog::Dialog()
	{
		if (!__internal_dialog_registrar)
		{
			__internal_dialog_registrar = new ZeroDialogInternal;
		}
	}
	Dialog::~Dialog(){}
	void Dialog::Show(int playerid)
	{
		__internal_dialog_registrar->CurrentDialog[playerid] = this;
		ShowPlayerDialog(playerid, 1, dialog_style, caption.c_str(), info.c_str(), button1.c_str(), button2.c_str());
	}
	void Dialog::Show(int playerid, std::string &customtext)
	{
		__internal_dialog_registrar->CurrentDialog[playerid] = this;
		ShowPlayerDialog(playerid, 1, dialog_style, caption.c_str(), (info + customtext).c_str(), button1.c_str(), button2.c_str());
	}
	void Dialog::ShowCustom(int playerid, int cdialog_style, std::string &ccaption, std::string &cinfo, std::string &cbutton1, std::string &cbutton2)
	{
		__internal_dialog_registrar->CurrentDialog[playerid] = this;
		ShowPlayerDialog(playerid, 1, cdialog_style, ccaption.c_str(), cinfo.c_str(), cbutton1.c_str(), cbutton2.c_str());
	}
	void Dialog::PrepareDialog(int dialog_style, std::string &caption, std::string &info, std::string &button1, std::string &button2)
	{
		this->dialog_style = dialog_style;
		this->caption.assign(caption);
		this->info.assign(info);
		this->button1.assign(button1);
		this->button2.assign(button2);
	}
};