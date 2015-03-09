/*
	Copyright (c) 2014 - 2015 Rafa≥ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa≥ Grasman <grasmanek94@gmail.com>
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
		Mateusz 'eider' CichoÒ
*/
#include "GameMode.hxx"

bool has_string_ip_ad(const std::string& s)
{
	static const std::string anything(".*");//anything
	static const std::string separator("[^0-9]+");//at least one character not a number
	static const std::string first_ip_part("([1-9][0-9]?|1[0-9]{2}|2[0-4][0-9]|25[0-5])");//1 .. 001 to 255
	static const std::string other_ip_part("([0-9]{1,2}|1[0-9]{2}|2[0-4][0-9]|25[0-5])");//0 .. 000 to 255
	static const std::string port_part("(102[4-9]|10[3-9][0-9]|1[1-9][0-9]{2}|[2-9][0-9]{3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-6])");//1024 .. 01024 to 65536
	
	static const boost::regex e(
		anything + 

		first_ip_part + separator + 
		other_ip_part + separator + 
		other_ip_part + separator + 
		other_ip_part + separator + 
		port_part + 

		anything, 
		boost::regex::optimize);

	return boost::regex_match(s, e);
}

const std::vector<std::string> badwords =
{
	"jebac",
	"jeban",
	"kurw",
	"korwa",
	"pierdol",
	"pierdal",
	"huj",
	"choj",
	"pizd",
	"kutas",
	"fuck",
	"cipa",
	"pedale",
	"szmat",
	"zajeb",
	"zjeb",
	"suka",
	"suko",
	"suki",
	"ciota",
	"cioto",
	"cioty"
};

char PolishReplacement[0xFF];

const std::map<std::string, std::string> PolishReplacementMap =
{
	{ "≥", "l" },
	{ "π", "a" },
	{ "Í", "e" },
	{ "Ê", "c" },
	{ "ø", "z" },
	{ "ü", "z" },
	{ "Û", "o" },
	{ "ú", "s" },
	{ "Ò", "n" },
	{ "£", "L" },
	{ "•", "A" },
	{ " ", "E" },
	{ "∆", "C" },
	{ "Ø", "Z" },
	{ "è", "Z" },
	{ "”", "O" },
	{ "å", "S" },
	{ "—", "N" }
};

struct CPolishReplacementInitHack
{
	CPolishReplacementInitHack()
	{
		for (unsigned char c = 0; c < 0xFF; ++c)
		{
			char tmpstr[2] = { c, 0 };
			std::string tmpstdstr(tmpstr);
			auto replacement = PolishReplacementMap.find(tmpstdstr);
			if (replacement == PolishReplacementMap.end())
			{
				PolishReplacement[c] = boost::to_lower_copy(tmpstdstr)[0];
			}
			else
			{
				PolishReplacement[c] = boost::to_lower_copy(replacement->second)[0];
			}
		}
	}
} _CPolishReplacementInitHack;


void FilterBadWords(std::string& s)
{
	std::string sc(s);
	for (auto& character : sc)
	{
		character = PolishReplacement[(unsigned char)character];
	}

	for (auto &badword : badwords)
	{
		size_t pos = sc.find(badword);
		size_t size = badword.size();
		size_t possize;

		while (pos != std::string::npos)
		{
			possize = pos + size;

			s.replace(s.begin() + pos, s.begin() + possize, "*");
			sc.replace(sc.begin() + pos, sc.begin() + possize, "*");

			pos = sc.find(badword);
		}
	}
}

bool is_message_allowed(std::string& s, bool allowmodify, bool modify_only_if_allowed)
{
	bool notallowed = has_string_ip_ad(s);
	if (allowmodify && (modify_only_if_allowed ? notallowed : true))
	{
		FilterBadWords(s);
	}
	return !notallowed;
}