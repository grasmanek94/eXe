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

namespace Message
{
	void RefreshTextDrawsOnLanguageChange(int playerid, bool alreadyset);
	void AddAnnouncementMessage(std::string message);
	void AddAnnouncementMessage(language_string_ids string_id);
	void AddAnnouncementMessage2(language_string_ids string_id, language_string_ids string_id_2);
	void AddAnnouncementMessageF(language_string_ids string_id, ...);
};