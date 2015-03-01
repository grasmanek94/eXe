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

CLanguageTextDraw::CLanguageTextDraw()
{

}

void CLanguageTextDraw::Create(
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
	int selectable)
{
	id = str_id;
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		TextDraws[i] = TextDrawCreate(x, y, Translate(i, str_id).c_str());
		TextDrawAlignment(TextDraws[i], alignment);
		TextDrawBackgroundColor(TextDraws[i], backcolor);
		TextDrawFont(TextDraws[i], font);
		TextDrawLetterSize(TextDraws[i], lettersizex, lettersizey);
		TextDrawColor(TextDraws[i], color);
		TextDrawSetOutline(TextDraws[i], outline);
		TextDrawSetProportional(TextDraws[i], proportional);
		TextDrawSetShadow(TextDraws[i], shadow);
		TextDrawSetSelectable(TextDraws[i], selectable);
	}
}

void CLanguageTextDraw::ShowForPlayer(int playerid)
{
	TextDrawShowForPlayer(playerid, TextDraws[Player[playerid].Language]);
}

void CLanguageTextDraw::Show()
{
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		if (PlayersUsingLanguage[i].size())
		{
			for (auto playerid : PlayersUsingLanguage[i])
			{
				TextDrawShowForPlayer(playerid, TextDraws[i]);
			}
		}
	}
}

void CLanguageTextDraw::HideForPlayer(int playerid)
{
	TextDrawHideForPlayer(playerid, TextDraws[Player[playerid].Language]);
}

void CLanguageTextDraw::Hide()
{
	for (auto i : TextDraws)
		TextDrawHideForAll(i);
}

void CLanguageTextDraw::SetString(std::string text)
{
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		TextDrawSetString(TextDraws[i], text.c_str());
	}
}

void CLanguageTextDraw::SetString(int languageid, std::string text)
{
	TextDrawSetString(TextDraws[languageid], text.c_str());
}

void CLanguageTextDraw::SetStringF(language_string_ids string_id ...)
{
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		int final_n, n = 8 + ((int)Translations[i][string_id].size()) * 2; // reserve 2 times as much as the length of the fmt_str
		std::string str;
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) {
			formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
			strcpy_s(&formatted[0], n, Translations[i][string_id].c_str());
			va_start(ap, string_id);
			final_n = vsnprintf(&formatted[0], n, Translations[i][string_id].c_str(), ap);
			va_end(ap);
			if (final_n < 0 || final_n >= n)
				n += abs(final_n - n + 1);
			else
				break;
		}
		TextDrawSetString(TextDraws[i], formatted.get());
	}
}

void CLanguageTextDraw::SetString(language_string_ids string_id)
{
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		TextDrawSetString(TextDraws[i], Translations[i][string_id].c_str());
	}
}

void CLanguageTextDraw::SetStringSF(language_string_ids string_id, std::string suffix, ...)
{
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		int final_n, n = 8 + ((int)Translations[i][string_id].size()) * 2; // reserve 2 times as much as the length of the fmt_str
		std::string str;
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) {
			formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
			strcpy_s(&formatted[0], n, Translations[i][string_id].c_str());
			va_start(ap, suffix);
			final_n = vsnprintf(&formatted[0], n, Translations[i][string_id].c_str(), ap);
			va_end(ap);
			if (final_n < 0 || final_n >= n)
				n += abs(final_n - n + 1);
			else
				break;
		}
		TextDrawSetString(TextDraws[i], (std::string(formatted.get()) + suffix).c_str());
	}
}

void CLanguageTextDraw::SetStringS(language_string_ids string_id, std::string suffix)
{
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		TextDrawSetString(TextDraws[i], (Translations[i][string_id] + suffix).c_str());
	}
}

void CLanguageTextDraw::SetColor(int color)
{
	for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
	{
		TextDrawColor(TextDraws[i], color);
	}
}