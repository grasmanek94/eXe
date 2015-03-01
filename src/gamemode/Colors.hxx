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

namespace Color
{
	const unsigned int COLOR_INFO = 0xE53E00FF; // slightly red, use with caution
	const unsigned int COLOR_INFO2 = 0xFFE5A1FF;
	const unsigned int COLOR_INFO3 = 0x7DBB00FF;
	const unsigned int COLOR_ERROR = 0xD04528FF; // old = B01010 | maybe = CA3800 | new = B03010
	const unsigned int COLOR_JOININFO = 0xA0A0A0FF;
	const unsigned int COLOR_LEAVEINFO = 0x808080FF;
	const unsigned int COLOR_ADMIN = 0xFF0000FF;
	const unsigned int COLOR_ADMIN3 = 0xFF3366FF;
	const unsigned int COLOR_PRIVATEMSG_IN = 0xFFFF00FF;
	const unsigned int COLOR_PRIVATEMSG_OUT = 0xFF9900AA;
	const unsigned int COLOR_PRIVATEMSG = 0xFFD0ACFF;
	const unsigned int COLOR_IC = 0xFFA115FF;
	const unsigned int COLOR_ME = 0x72C100FF;
	const unsigned int COLOR_GM = 0xFFFF00FF;
	const unsigned int COLOR_VIPSAY = 0x0097C1FF;
	const unsigned int COLOR_VIP = 0x008DE5FF;
	const unsigned int COLOR_NORMALUSER = 0x909090FF;
	const unsigned int COLOR_FSUSER = 0x00A1FFFF;
	const unsigned int COLOR_VIPUSER = 0xFACB00FF;
	const unsigned int COLOR_GMUSER = 0x58A100FF;
	const unsigned int COLOR_ADMINUSER = 0xA50000FF;
	const unsigned int COLOR_RCONUSER = 0xC00000FF;
	const int COLOR_ALPHA = 150;

	inline const unsigned int ToChat(unsigned int fullcolor)
	{
		return (fullcolor >> 8) & 0xFFFFFF;
	}
	inline const unsigned int ToFull(unsigned int color, unsigned char transparency)
	{
		return (color << 8) + transparency;
	}
};