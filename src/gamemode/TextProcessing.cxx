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

void ProcessStringWithBTag(unsigned int color, std::string& message)
{
	unsigned char tag_color[4];
	color = (color >> 8);
	(*(unsigned int*)tag_color) = color;
	tag_color[0] = (unsigned char)((float)tag_color[0] * 0.7f);
	tag_color[1] = (unsigned char)((float)tag_color[1] * 0.7f);
	tag_color[2] = (unsigned char)((float)tag_color[2] * 0.7f);
	tag_color[3] = (unsigned char)((float)tag_color[3] * 0.7f);
	boost::replace_all(message, "{/b}", Functions::string_format("{%06X}", color));
	boost::replace_all(message, "{b}", Functions::string_format("{%06X}", (*(unsigned int*)tag_color)));
}

void FixMessageWithBTag(unsigned int color, std::string& message, bool displayPrefix)
{
	boost::algorithm::replace_all(message, "%", "%%");
	ProcessStringWithBTag(color, message);
	if (displayPrefix) message.insert(0, "››› ");
}