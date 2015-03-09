/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the core language system

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies
		Extension
		MessageManager

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
 */
#include "GameMode.hxx"

namespace Language
{
	unsigned long CURRENT_LANGUAGE_ID = 0;

	std::array<std::array<std::string, MAX_LANGUAGE_STRINGS>, LANGUAGES_SIZE> Translations;
	std::array<std::string, LANGUAGES_SIZE> Languages;
	std::array<std::set<int>, LANGUAGES_SIZE> PlayersUsingLanguage;
	std::map<unsigned long, unsigned long> LanguageIDToIndex;
	std::array<unsigned long, LANGUAGES_SIZE> IndexToLanguageID;
	std::string language_list;

	namespace InternalLanguage
	{
		void Add(const std::map<language_string_ids, std::string> &translation)
		{
			if (CURRENT_LANGUAGE_ID < LANGUAGES_SIZE)
			{
				for (auto t : translation)
				{
					Translations[CURRENT_LANGUAGE_ID][t.first].assign(t.second);
				}

				IndexToLanguageID[CURRENT_LANGUAGE_ID] = fnv_32a_str((char*)Translations[CURRENT_LANGUAGE_ID][L_language_name].c_str(), 0);
				LanguageIDToIndex[IndexToLanguageID[CURRENT_LANGUAGE_ID]] = CURRENT_LANGUAGE_ID;

				Languages[CURRENT_LANGUAGE_ID].assign(Translations[CURRENT_LANGUAGE_ID][L_language_name]);

				language_list.append(Languages[CURRENT_LANGUAGE_ID] + "\n");

				for (size_t i = 0; i < MAX_LANGUAGE_STRINGS; ++i)
				{
					if (translation.find((language_string_ids)i) == translation.end())
					{
						std::cout << "         WARNING: Missing string id [" << i << "] in language id [" << Functions::string_format("%02X", CURRENT_LANGUAGE_ID) << "; " << Functions::string_format(" % 08X", IndexToLanguageID[CURRENT_LANGUAGE_ID]) << "]" << std::endl;
					}
					if (
						std::count(Translations[0][i].begin(), Translations[0][i].end(), '%')
						!=
						std::count(Translations[CURRENT_LANGUAGE_ID][i].begin(), Translations[CURRENT_LANGUAGE_ID][i].end(), '%')
						)
					{
						std::cout << "         CRITICAL ERROR: Unmatched format specifier found in string id [" << i << "] in language id [" << Functions::string_format("%02X", CURRENT_LANGUAGE_ID) << ";" << Functions::string_format("%08X", IndexToLanguageID[CURRENT_LANGUAGE_ID]) << "]" << std::endl;
					}
				}
				std::cout << "     Added language: '" << Translations[CURRENT_LANGUAGE_ID][L_language_name] << "' with id [" << Functions::string_format("%02X", CURRENT_LANGUAGE_ID) << ";" << Functions::string_format("%08X", IndexToLanguageID[CURRENT_LANGUAGE_ID]) << "] and [" << translation.size() << "] entries." << std::endl;
				++CURRENT_LANGUAGE_ID;
			}
		}
	};

	void StaticLanguageTextDrawsRefresh(int playerid, bool alreadyset)
	{
		Message::RefreshTextDrawsOnLanguageChange(playerid, alreadyset);
	}

	void AddPlayerToLanguage(int playerid, unsigned long language)
	{
		if (language < CURRENT_LANGUAGE_ID)
		{
			if (PlayersUsingLanguage[language].find(playerid) == PlayersUsingLanguage[language].end())
			{
				for (auto& lang : PlayersUsingLanguage)
				{
					lang.erase(playerid);
				}
				PlayersUsingLanguage[language].insert(playerid);
				if (Player[playerid].Language != language)
				{
					StaticLanguageTextDrawsRefresh(playerid, false);
					Player[playerid].Language = language;
					StaticLanguageTextDrawsRefresh(playerid, true);
				}
				else
				{
					Player[playerid].Language = language;
				}
			}
		}
	}

	class CLanguageCleaner : public Extension::Base
	{
	public:
		CLanguageCleaner() : Base(-2000) {}
		bool OnPlayerDisconnect(int playerid, int reason) override
		{
			PlayersUsingLanguage[Player[playerid].Language].erase(playerid);
			return true;
		}
	} _CLanguageCleaner;

	const std::string& Translate(unsigned long language, language_string_ids string_id)
	{
		return Translations[language][string_id];
	}

	const std::string& TranslateP(int playerid, language_string_ids string_id)
	{
		return Translations[Player[playerid].Language][string_id];
	}

	const std::string TranslateF(unsigned long language, language_string_ids string_id, ...)
	{
		std::string fmt(Translations[language][string_id]);
		{
			int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
			std::string str;
			std::unique_ptr<char[]> formatted;
			va_list ap;
			while (1) 
			{
				formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
				strcpy_s(&formatted[0], n, fmt.c_str());
				va_start(ap, string_id);
				final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
				va_end(ap);
				if (final_n < 0 || final_n >= n)
				{
					n += abs(final_n - n + 1);
				}
				else
				{
					break;
				}
			}
			fmt.assign(formatted.get());
		}
		return fmt;
	}

	const std::string TranslatePF(int playerid, language_string_ids string_id, ...)
	{
		std::string fmt(Translations[Player[playerid].Language][string_id]);
		{
			int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
			std::string str;
			std::unique_ptr<char[]> formatted;
			va_list ap;
			while (1) 
			{
				formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
				strcpy_s(&formatted[0], n, fmt.c_str());
				va_start(ap, string_id);
				final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
				va_end(ap);
				if (final_n < 0 || final_n >= n)
				{
					n += abs(final_n - n + 1);
				}
				else
				{
					break;
				}
			}
			fmt.assign(formatted.get());
		}
		return fmt;
	}

	void SendClientMessageToAllF(int color, language_string_ids string_id, int playSound, bool displayPrefix, ...)
	{
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
		{
			if (PlayersUsingLanguage[i].size())
			{
				std::string fmt(Translate(i, string_id));
				{
					int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
					std::string str;
					std::unique_ptr<char[]> formatted;
					va_list ap;
					while (1) 
					{
						formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
						strcpy_s(&formatted[0], n, fmt.c_str());
						va_start(ap, displayPrefix);
						final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
						va_end(ap);
						if (final_n < 0 || final_n >= n)
						{
							n += abs(final_n - n + 1);
						}
						else
						{
							break;
						}
					}
					fmt.assign(formatted.get());
					FixMessageWithBTag(color, fmt, displayPrefix);
				}

				if (fmt.size() > 144)
				{
					fmt.resize(144);
				}

				for (auto& playerid : PlayersUsingLanguage[i])
				{
					if (playSound)
					{
						PlayerPlaySound(playerid, playSound, 0.0, 0.0, 0.0);
					}
					sampgdk_SendClientMessage(playerid, color, fmt.c_str());
				}
			}
		}
	}

	void SendClientMessageF(int playerid, int color, language_string_ids string_id, int playSound, bool displayPrefix, ...)
	{
		std::string fmt(Translate(Player[playerid].Language, string_id));
		{
			int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
			std::string str;
			std::unique_ptr<char[]> formatted;
			va_list ap;
			while (1) 
			{
				formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
				strcpy_s(&formatted[0], n, fmt.c_str());
				va_start(ap, displayPrefix);
				final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
				va_end(ap);
				if (final_n < 0 || final_n >= n)
				{
					n += abs(final_n - n + 1);
				}
				else
				{
					break;
				}
			}
			fmt.assign(formatted.get());
			FixMessageWithBTag(color, fmt, displayPrefix);
		}

		if (playSound)
		{
			PlayerPlaySound(playerid, playSound, 0.0, 0.0, 0.0);
		}

		if (fmt.size() > 144)
		{
			fmt.resize(144);
		}

		sampgdk_SendClientMessage(playerid, color, fmt.c_str());
	}

	void SendClientMessageToAll(int color, language_string_ids string_id, int playSound, bool displayPrefix)
	{
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
		{
			if (PlayersUsingLanguage[i].size())
			{
				std::string message(Translate(i, string_id));
				FixMessageWithBTag(color, message, displayPrefix);

				if (message.size() > 144)
				{
					message.resize(144);
				}

				for (auto& playerid : PlayersUsingLanguage[i])
				{
					if (playSound)
					{
						PlayerPlaySound(playerid, playSound, 0.0, 0.0, 0.0);
					}

					sampgdk_SendClientMessage(playerid, color, message.c_str());
				}
			}
		}
	}

	void SendClientMessage(int playerid, int color, language_string_ids string_id, int playSound, bool displayPrefix)
	{
		::fixSendClientMessage(playerid, color, Translate(Player[playerid].Language, string_id), playSound, displayPrefix);
	}

	//if color error then sound id if > 0 make it 1085 else if not color error and soundid not 0 then make soundid 1150
	void fixSendClientMessageToAllF(int color, language_string_ids string_id, int playSound, bool displayPrefix, ...)
	{
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
		{
			if (PlayersUsingLanguage[i].size())
			{
				std::string fmt(Translate(i, string_id));
				{
					int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
					std::string str;
					std::unique_ptr<char[]> formatted;
					va_list ap;
					while (1) 
					{
						formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
						strcpy_s(&formatted[0], n, fmt.c_str());
						va_start(ap, displayPrefix);
						final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
						va_end(ap);
						if (final_n < 0 || final_n >= n)
						{
							n += abs(final_n - n + 1);
						}
						else
						{
							break;
						}
					}
					fmt.assign(formatted.get());
					FixMessageWithBTag(color, fmt, displayPrefix);
				}

				if (fmt.size() > 144)
				{
					fmt.resize(144);
				}

				for (auto& playerid : PlayersUsingLanguage[i])
				{
					if (playSound)
					{
						PlayerPlaySound(playerid, playSound, 0.0, 0.0, 0.0);
					}

					sampgdk_SendClientMessage(playerid, color, fmt.c_str());
				}
			}
		}
	}

	void fixSendClientMessageF(int playerid, int color, language_string_ids string_id, int playSound, bool displayPrefix, ...)
	{
		std::string fmt(Translate(Player[playerid].Language, string_id));
		{
			int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
			std::string str;
			std::unique_ptr<char[]> formatted;
			va_list ap;
			while (1) 
			{
				formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
				strcpy_s(&formatted[0], n, fmt.c_str());
				va_start(ap, displayPrefix);
				final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
				va_end(ap);
				if (final_n < 0 || final_n >= n)
				{
					n += abs(final_n - n + 1);
				}
				else
				{
					break;
				}
			}
			fmt.assign(formatted.get());
			FixMessageWithBTag(color, fmt, displayPrefix);
		}

		if (playSound)
		{
			PlayerPlaySound(playerid, playSound, 0.0, 0.0, 0.0);
		}

		if (fmt.size() > 144)
		{
			fmt.resize(144);
		}

		sampgdk_SendClientMessage(playerid, color, fmt.c_str());
	}

	void fixSendClientMessageToAll(int color, language_string_ids string_id, int playSound, bool displayPrefix)
	{
		for (size_t i = 0; i < LANGUAGES_SIZE; ++i)
		{
			if (PlayersUsingLanguage[i].size())
			{
				std::string message(Translate(i, string_id));
				FixMessageWithBTag(color, message, displayPrefix);

				if (message.size() > 144)
				{
					message.resize(144);
				}

				for (auto& playerid : PlayersUsingLanguage[i])
				{
					if (playSound)
					{
						PlayerPlaySound(playerid, playSound, 0.0, 0.0, 0.0);
					}

					sampgdk_SendClientMessage(playerid, color, message.c_str());
				}
			}
		}
	}

	void fixSendClientMessage(int playerid, int color, language_string_ids string_id, int playSound, bool displayPrefix)
	{
		::fixSendClientMessage(playerid, color, Translate(Player[playerid].Language, string_id), playSound, displayPrefix);
	}
};

//languages
#include "LangPolish.hxx"
#include "LangEnglish.hxx"