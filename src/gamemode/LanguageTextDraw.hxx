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

class CLanguageTextDraw
{
	std::array<int, LANGUAGES_SIZE> TextDraws;
	language_string_ids id;
public:
	CLanguageTextDraw();
	void Create(
		float x, float y,
		language_string_ids str_id,
		int alignment,
		int backcolor,
		int font,
		float lettersizex, float lettersizey,
		int color,
		int outline,
		int proportional,
		int shadow,
		int selectable);
	void ShowForPlayer(int playerid);
	void Show();
	void HideForPlayer(int playerid);
	void Hide();
	void SetString(std::string text);
	void SetString(int languageid, std::string text);
	void SetStringF(language_string_ids string_id, ...);
	void SetString(language_string_ids string_id);
	void SetStringSF(language_string_ids string_id, std::string suffix, ...);
	void SetStringS(language_string_ids string_id, std::string suffix);
	void SetColor(int color);
};