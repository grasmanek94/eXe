/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementing the boilerplate code/definitions to create dialogs easily in source files

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

namespace ZeroDLG
{
	class Dialog;
	static bool init = false;
	struct ZeroDialogInternal
	{
		Dialog* CurrentDialog[MAX_PLAYERS];
		std::map<std::string, Dialog*> dialog_map;
		ZeroDialogInternal();
		void register_dialog(Dialog* dialog, std::string name);
	};
	extern ZeroDialogInternal * __internal_dialog_registrar;
	class Dialog
	{
	protected:
		int
			dialog_style;
		std::string
			caption,
			info,
			button1,
			button2;
	public:
		Dialog();
		~Dialog();

			
		virtual void process_dialog(int playerid, int response, int listitem, std::string &inputtext) = 0;
		void Show(int playerid);
		void Show(int playerid, std::string &customtext);
		void ShowCustom(int playerid, int cdialog_style, std::string &ccaption, std::string &cinfo, std::string &cbutton1, std::string &cbutton2);
		void PrepareDialog(int dialog_style_, std::string &caption_, std::string &info_, std::string &button1_, std::string &button2_);
	};
};

#define ZERO_DIALOG(name) \
class dialog ## name : public ZeroDLG::Dialog\
	{\
		public:\
		dialog ## name() { if (!ZeroDLG::__internal_dialog_registrar){ZeroDLG::__internal_dialog_registrar = new ZeroDLG::ZeroDialogInternal;} ZeroDLG::__internal_dialog_registrar->register_dialog(this, #name); }\
		void process_dialog(int playerid, int response, int listitem, std::string& inputtext);\
	};\
	dialog ## name dialog ## name ## __execute;\
	inline void dialog ## name::process_dialog(int playerid, int response, int listitem, std::string& inputtext)

bool PrepareZeroDialog(std::string name, int dialog_style, std::string caption, std::string info, std::string button1, std::string button2);
bool ShowPlayerZeroDialog(int playerid, std::string name);
bool ShowPlayerZeroDialog(int playerid, std::string name, std::string text);
bool ShowPlayerCustomDialog(int playerid, std::string name, int dialog_style,std::string caption, std::string info, std::string button1, std::string button2);
bool HidePlayerZeroDialog(int playerid);
const static std::string DLG_DUMMY = "________dummy";