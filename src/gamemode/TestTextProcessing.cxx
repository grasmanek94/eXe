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

#ifdef _LOCALHOST_DEBUG

size_t count_substrings(const std::string& haystack, const std::string& needle)
{
	size_t count = 0;
	size_t current_pos = (size_t)-1;
	while (true)
	{
		current_pos = haystack.find(needle, current_pos+1);
		if (current_pos == std::string::npos)
			break;
		else ++count;
	}
	return count;
}

class TEST_ProcessStrings : public Extension::Base
{
public:
	bool OnGameModeInit()
	{
		int color_1 = 0xAABBCCDD;
		int color_2 = 0xABCDEF12;

		std::string string_1("The big blue giant {b}fucked{/b} his {b}fucking mother{/b} VERY hard.");
		std::string string_2("The big blue giant {b}fucked {b}the shit{/b}{/b} out of his {b}fucking mother{/b} VERY hard.");

		ProcessStringWithBTag(color_1, string_1);
		ProcessStringWithBTag(color_2, string_2);

		std::string find_1a("{77828E}");
		std::string find_1b("{AABBCC}");
		std::string find_2a("{778FA7}");
		std::string find_2b("{ABCDEF}");

		size_t count_1a = count_substrings(string_1, find_1a);
		size_t count_1b = count_substrings(string_1, find_1b);
		size_t count_2a = count_substrings(string_2, find_2a);
		size_t count_2b = count_substrings(string_2, find_2b);

		assert(count_1a == 2);
		assert(count_1b == 2);
		assert(count_2a == 3);
		assert(count_2b == 3);

		return true;
	}
} _TEST_ProcessStrings;
#endif