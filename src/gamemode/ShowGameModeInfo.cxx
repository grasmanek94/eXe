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

extern char GLOBVERSION[128];
std::string VERSION("YYMMDD.HHMM");
std::string LASTUPDATETIME("DD MMM YYYY HH:MM:SS");

std::tm VersionDesignator::make_tm(int year, int month, int day, int hour, int minute, int second)
{
	std::tm tm = { 0 };
	tm.tm_year = year - 1900; // years count from 1900
	tm.tm_mon = month - 1;    // months count from January=0
	tm.tm_mday = day;         // days count from 1
	tm.tm_hour = hour;
	tm.tm_min = minute;
	tm.tm_sec = second;

	return tm;
}

extern std::string VERSION;
extern std::string LASTUPDATETIME;

VersionDesignator::VersionDesignator()
{
	Months["Jan"] = 1; Months["Feb"] = 2; Months["Mar"] = 3;
	Months["Apr"] = 4; Months["May"] = 5; Months["Jun"] = 6;
	Months["Jul"] = 7; Months["Aug"] = 8; Months["Sep"] = 9;
	Months["Oct"] = 10; Months["Nov"] = 11; Months["Dec"] = 12;

	std::string version(GLOBVERSION);
	std::vector<std::string> tokens;
	//boost token compress on fixes issue with double isanyof
	boost::split(tokens, version, boost::is_any_of(": "), boost::token_compress_on);

	Month = Months[tokens[1]];
	Day = boost::lexical_cast<int>(tokens[2]);
	Hour = boost::lexical_cast<int>(tokens[3]);
	Minute = boost::lexical_cast<int>(tokens[4]);
	Second = boost::lexical_cast<int>(tokens[5]);
	Year = boost::lexical_cast<int>(tokens[6]);

	VERSION.assign(Functions::string_format("%02d%02d%02d.%02d%02d", Year-2000, Month, Day, Hour, Minute));
	LASTUPDATETIME.assign(Functions::string_format("%02d %s %04d %02d:%02d:%02d", Day, tokens[1].c_str(), Year, Hour, Minute, Second));

	Version.assign(VERSION);
	LastUpdateTime.assign(LASTUPDATETIME);

	Weburl.assign("exe24.info");
}

std::string& VersionDesignator::GetModeVersion()
{
	return Version;
}

std::string& VersionDesignator::GetModeLastUpdateTime()
{
	return LastUpdateTime;
}

std::string& VersionDesignator::GetModeStartDate()
{
	return DeveloperStartDate;
}

std::string& VersionDesignator::GetWebUrl()
{
	return Weburl;
}

VersionDesignator StaticVersionDeterminator;

void ShowGameModeInfo(int playerid)
{
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_SGMI_title), TranslatePF(playerid, L_SGMI_info, LASTUPDATETIME.c_str()), "V", "X");
}