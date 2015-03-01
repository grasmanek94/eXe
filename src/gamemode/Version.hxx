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

class VersionDesignator
{
private:
	std::map<std::string,char> Months;
	std::string Version;
	std::string LastUpdateTime;
	std::string DeveloperStartDate;
	std::tm make_tm(int year, int month, int day, int hour, int minute, int second);
	std::string Weburl;
public:
	int
		Month,
		Day,
		Hour,
		Minute,
		Second,
		Year;
	VersionDesignator();
	std::string& GetModeVersion();
	std::string& GetModeLastUpdateTime();
	std::string& GetModeStartDate();
	std::string& GetWebUrl();
};

extern VersionDesignator StaticVersionDeterminator;