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

namespace Functions
{
	float gen_random_float(float min, float max);
	extern boost::posix_time::ptime * time_t_epoch;
	unsigned long long GetTime();
	unsigned long long  GetTimeSeconds();
	//only for positive numbers
	std::string string_format_big(const std::string fmt, ...);
	std::string string_format(const std::string fmt, ...);
	std::wstring string_format(const std::wstring fmt, ...);
	std::vector<std::string> &string_split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> string_split(const std::string &s, char delim);
	std::vector<std::wstring> &string_split(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems);
	std::vector<std::wstring> string_split(const std::wstring &s, wchar_t delim);
	std::wstring s2ws(const std::string& str);
	std::string ws2s(const std::wstring& wstr);
	std::string GetTimeStrFromMs(unsigned long long ms);
	std::string GetTimeStrFromMsm(unsigned long long ms);
	unsigned char GetColorBrightness(unsigned char R, unsigned char G, unsigned char B);
	unsigned char GetColorBrightness(int color);
	template<typename T> bool is_number_signed(std::string s, T& val)
	{
		T _Val;
		if (boost::conversion::try_lexical_convert<T>(s, _Val))
		{
			val = _Val;
			return true;
		}
		return false;
	}
	template<typename T> bool is_number_unsigned(const std::string& s, T& val)
	{
		T _Val;
		if (boost::conversion::try_lexical_convert<T>(s, _Val))
		{
			if (_Val >= 0)
			{
				val = _Val;
				return true;
			}
		}
		return false;
	}
	//only for positive numbers
	template<typename T> bool is_xnumber_m(std::string s, T& val)
	{
		HexTo<T> _Val;
		if (boost::conversion::try_lexical_convert<HexTo<T>>(s, _Val))
		{
			val = _Val.value;
			return true;
		}
		return false;
	}
	template<typename T> bool is_xnumber(const std::string& s, T& val)
	{
		HexTo<T> _Val;
		if (boost::conversion::try_lexical_convert<HexTo<T>>(s, _Val))
		{
			if (_Val.value >= 0)
			{
				val = _Val.value;
				return true;
			}
		}
		return false;
	}
	//only for positive numbers
	template<typename T> bool is_fnumber_m(std::string s, T& val)
	{
		T _Val;
		if (boost::conversion::try_lexical_convert<T>(s, _Val))
		{
			val = _Val;
			return true;
		}
		return false;
	}
	template<typename T> bool is_fnumber(const std::string& s, T& val)
	{
		T _Val;
		if (boost::conversion::try_lexical_convert<T>(s, _Val))
		{
			if (_Val >= 0)
			{
				val = _Val;
				return true;
			}
		}
		return false;
	}
	namespace ASCII
	{
		static char Letters[45][6][16] = 
		{
			{   {"   "},
				{"   "},
				{"   "},
				{"   "},
				{" _ "},
				{"(_)"}},
			{{""},{""},{""},{""},{""},{""}},
			{   {" _____ "},
				{"|  _  |"},
				{"| |/' |"},
				{"|  /| |"},
				{"\\ |_/ /"},
				{" \\___/ "}},
			{   {" __  "},
				{"/  | "},
				{"`| | "},
				{" | | "},
				{"_| |_"},
				{"\\___/"}},
			{   {" _____ "},
				{"/ __  \\"},
				{"`' / /'"},
				{"  / /  "},
				{"./ /___"},
				{"\\_____/"}},
			{   {" _____ "},
				{"|____ |"},
				{"    / /"},
				{"    \\ \\"},
				{".___/ /"},
				{"\\____/ "}},
			{   {"   ___ "},
				{"  /   |"},
				{" / /| |"},
				{"/ /_| |"},
				{"\\___  |"},
				{"    |_/"}},
			{   {" _____ "},
				{"|  ___|"},
				{"|___ \\ "},
				{"    \\ \\"},
				{"/\\__/ /"},
				{"\\____/ "}},
			{   {"  ____ "},
				{" / ___|"},
				{"/ /___ "},
				{"| ___ \\"},
				{"| \\_/ |"},
				{"\\_____/"}},
			{   {" ______"},
				{"|___  /"},
				{"   / / "},
				{"  / /  "},
				{"./ /   "},
				{"\\_/    "}},
			{   {" _____ "},
				{"|  _  |"},
				{" \\ V / "},
				{" / _ \\ "},
				{"| |_| |"},
				{"\\_____/"}},
			{   {" _____ "},
				{"|  _  |"},
				{"| |_| |"},
				{"\\____ |"},
				{".___/ /"},
				{"\\____/ "}},
			{{""},{""},{""},{""},{""},{""}},
			{{""},{""},{""},{""},{""},{""}},
			{{""},{""},{""},{""},{""},{""}},
			{{""},{""},{""},{""},{""},{""}},
			{{""},{""},{""},{""},{""},{""}},
			{{""},{""},{""},{""},{""},{""}},
			{{""},{""},{""},{""},{""},{""}},
			{   {"  ___  "},
				{" / _ \\ "},
				{"/ /_\\ \\"},
				{"|  _  |"},
				{"| | | |"},
				{"\\_| |_/"}},
			{   {"______ "},
				{"| ___ \\"},
				{"| |_/ /"},
				{"| ___ \\"},
				{"| |_/ /"},
				{"\\____/ "}},
			{   {" _____ "},
				{"/  __ \\"},
				{"| /  \\/"},
				{"| |    "},
				{"| \\__/\\"},
				{" \\____/"}},
			{   {"______ "},
				{"|  _  \\"},
				{"| | | |"},
				{"| | | |"},
				{"| |/ / "},
				{"|___/  "}},
			{   {" _____ "},
				{"|  ___|"},
				{"| |__  "},
				{"|  __| "},
				{"| |___ "},
				{"\\____/ "}},
			{   {"______ "},
				{"|  ___|"},
				{"| |_   "},
				{"|  _|  "},
				{"| |    "},
				{"\\_|    "}},
			{   {" _____ "},
				{"|  __ \\"},
				{"| |  \\/"},
				{"| | __ "},
				{"| |_\\ \\"},
				{" \\____/"}},
			{   {" _   _ "},
				{"| | | |"},
				{"| |_| |"},
				{"|  _  |"},
				{"| | | |"},
				{"\\_| |_/"}},
			{   {" _____ "},
				{"|_   _|"},
				{"  | |  "},
				{"  | |  "},
				{" _| |_ "},
				{" \\___/ "}},
			{   {"   ___ "},
				{"  |_  |"},
				{"    | |"},
				{"    | |"},
				{"/\\__/ /"},
				{"\\____/ "}},
			{   {" _   __"},
				{"| | / /"},
				{"| |/ / "},
				{"|    \\ "},
				{"| |\\  \\"},
				{"\\_| \\_/"}},
			{   {" _     "},
				{"| |    "},
				{"| |    "},
				{"| |    "},
				{"| |____"},
				{"\\_____/"}},
			{   {"___  ___"},
				{"|  \\/  |"},
				{"| .  . |"},
				{"| |\\/| |"},
				{"| |  | |"},
				{"\\_|  |_/"}},
			{   {" _   _ "},
				{"| \\ | |"},
				{"|  \\| |"},
				{"| . ` |"},
				{"| |\\  |"},
				{"\\_| \\_/"}},
			{   {" _____ "},
				{"|  _  |"},
				{"| | | |"},
				{"| | | |"},
				{"\\ \\_/ /"},
				{" \\___/ "}},
			{   {"______ "},
				{"| ___ \\"},
				{"| |_/ /"},
				{"|  __/ "},
				{"| |    "},
				{"\\_|    "}},
			{   {" _____ "},
				{"|  _  |"},
				{"| | | |"},
				{"| | | |"},
				{"\\ \\/' /"},
				{" \\_/\\_\\"}},
			{   {"______ "},
				{"| ___ \\"},
				{"| |_/ /"},
				{"|    / "},
				{"| |\\ \\ "},
				{"\\_| \\_|"}},
			{   {" _____ "},
				{"/  ___|"},
				{"\\ `--. "},
				{" `--. \\"},
				{"/\\__/ /"},
				{"\\____/ "}},
			{   {" _____ "},
				{"|_   _|"},
				{"  | |  "},
				{"  | |  "},
				{"  | |  "},
				{"  \\_/  "}},
			{   {" _   _ "},
				{"| | | |"},
				{"| | | |"},
				{"| | | |"},
				{"| |_| |"},
				{" \\___/ "}},
			{   {" _   _ "},
				{"| | | |"},
				{"| | | |"},
				{"| | | |"},
				{"\\ \\_/ /"},
				{" \\___/ "}},
			{   {" _    _ "},
				{"| |  | |"},
				{"| |  | |"},
				{"| |/\\| |"},
				{"\\  /\\  /"},
				{" \\/  \\/ "}},
			{   {"__   __"},
				{"\\ \\ / /"},
				{" \\ V / "},
				{" /   \\ "},
				{"/ /^\\ \\"},
				{"\\/   \\/"}},
			{   {"__   __"},
				{"\\ \\ / /"},
				{" \\ V / "},
				{"  \\ /  "},
				{"  | |  "},
				{"  \\_/  "}},
			{   {" ______"},
				{"|___  /"},
				{"   / / "},
				{"  / /  "},
				{"./ /___"},
				{"\\_____/"}}
		};
		std::string GenerateArt(std::string &str);
	};
	class RandomFromSeed
	{
	public:
		boost::mt11213b gener;
		boost::normal_distribution<> normal;
		boost::variate_generator<boost::mt11213b&, boost::normal_distribution<> > rng;
		RandomFromSeed()
			: gener(1), normal(0,1), rng(gener, normal)
		{
			SetSeed(0);
		}
		void SetSeed(uint32_t seed)
		{
			rng.engine().seed(seed);
			rng.distribution().reset();
		}
		long long Random(long long inclusive_min = 0, long long exclusive_max = (0x7FFFFFFFFFFFFFFF - 1))
		{
			long long diff = exclusive_max - inclusive_min;

			//fix division by 0
			if (diff == 0)
				return inclusive_min;

			return (((unsigned long long)(rng() * 100000000.0)) % diff) + inclusive_min;
		}
	};
	extern RandomFromSeed * RandomGenerator;
};
void Report_Error(std::string error, std::string file, std::string function, int line);

#define REPORT_ERROR_IF_ANY(e) Report_Error(e,__FILE__,__FUNCTION__,__LINE__)