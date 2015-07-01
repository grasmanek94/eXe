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

namespace Functions
{
	RandomFromSeed * RandomGenerator = nullptr;
	float gen_random_float(float min, float max)
	{
		static boost::mt19937 rng;
		boost::uniform_real<float> u(min, max);
		boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen(rng, u);
		return gen();
	}
	struct init_hack
	{
		init_hack()
		{
			RandomGenerator = new RandomFromSeed;
			RandomGenerator->SetSeed(GetTime() * (RandomGenerator->SetSeed((uint32_t)&RandomGenerator), RandomGenerator->Random()));
		}
	} _init_hack;

	//returns unix/epoch time(stamp) in miliseconds
	unsigned long long  GetTime()//should be enough for 60,000,000 years... I certainly won't exist anymore by then so I don't have to worry ;)
	{
		static const boost::posix_time::ptime time_t_epoch_l = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration diff = now - time_t_epoch_l;
		return diff.total_milliseconds();
	}
	std::string GetTimeStrFromMs(unsigned long long ms)
	{
		boost::posix_time::ptime datex(boost::posix_time::from_time_t((time_t)(ms / 1000)));
		int years = datex.date().year();
		int months = datex.date().month().as_number();
		int days = datex.date().day().as_number();
		int hours = datex.time_of_day().hours();
		int minutes = datex.time_of_day().minutes();
		int seconds = datex.time_of_day().seconds();
		return string_format("%02d-%02d-%04d %02d:%02d:%02d", days, months, years, hours, minutes, seconds);
	}
	std::string GetTimeStrFromMsm(unsigned long long ms)
	{
		boost::posix_time::ptime datex(boost::posix_time::from_time_t((time_t)(ms / 1000)));
		int years = datex.date().year();
		int months = datex.date().month().as_number();
		int days = datex.date().day().as_number();
		int hours = datex.time_of_day().hours();
		int minutes = datex.time_of_day().minutes();
		int seconds = datex.time_of_day().seconds();
		return string_format("%-4d-%02d-%02d %02d:%02d:%02d", days, months, years, hours, minutes, seconds);
	}
	std::string GetTimeStrFromAddMs2(unsigned long long ms)
	{
		boost::posix_time::ptime datex = boost::posix_time::microsec_clock::local_time();
		datex += boost::posix_time::milliseconds(ms);
		int years = datex.date().year();
		int months = datex.date().month().as_number();
		int days = datex.date().day().as_number();
		int hours = datex.time_of_day().hours();
		int minutes = datex.time_of_day().minutes();
		int seconds = datex.time_of_day().seconds();
		return string_format("%4d-%02d-%02d %02d:%02d:%02d", days, months, years, hours, minutes, seconds);
	}
	unsigned long long  GetTimeSeconds()//should be enough for 60,000,000,000 years... I certainly won't exist anymore by then so I don't have to worry ;)
	{
		static const boost::posix_time::ptime time_t_epoch_l = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration diff = now - time_t_epoch_l;
		return diff.total_seconds();
	}
	std::string string_format_big(const std::string fmt, ...)
	{
		int final_n, n = 16 + ((int)fmt.size()) * 10; // reserve 10 times as much as the length of the fmt_str
		std::string str;
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) 
		{
			formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
			strcpy_s(&formatted[0], n, fmt.c_str());
			va_start(ap, fmt);
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
		return std::string(formatted.get());
	}
	unsigned char GetColorBrightness(int color)
	{
		return GetColorBrightness((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, (color & 0x000000FF) >> 0 );
	}
	unsigned char GetColorBrightness(unsigned char R, unsigned char G, unsigned char B)
	{
		return (unsigned char)(sqrt(0.241*(double)R*(double)R + 0.691*(double)G*(double)G + 0.068*(double)B*(double)B));
	}
	std::string string_format(const std::string fmt, ...)
	{
		int final_n, n = 8 + ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
		std::string str;
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) 
		{
			formatted.reset(new char[n]); // wrap the plain char array into the unique_ptr
			strcpy_s(&formatted[0], n, fmt.c_str());
			va_start(ap, fmt);
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
		return std::string(formatted.get());
	}
	std::wstring string_format(const std::wstring fmt, ...)
	{
		int final_n, n = ((int)fmt.size()) * 2; // reserve 2 times as much as the length of the fmt_str
		std::wstring str;
		std::unique_ptr<wchar_t[]> formatted;
		va_list ap;
		while (1) 
		{
			formatted.reset(new wchar_t[n]); // wrap the plain char array into the unique_ptr
			wcscpy(&formatted[0], fmt.c_str());
			va_start(ap, fmt);
			final_n = _vsnwprintf(&formatted[0], n, fmt.c_str(), ap);
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
		return std::wstring(formatted.get());
	}
	std::wstring s2ws(const std::string& str)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
	std::vector<std::string> &string_split(const std::string &s, char delim, std::vector<std::string> &elems) 
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) 
		{
			elems.push_back(item);
		}
		return elems;
	}
	std::vector<std::string> string_split(const std::string &s, char delim) 
	{
		std::vector<std::string> elems;
		string_split(s, delim, elems);
		return elems;
	}
	std::vector<std::wstring> &string_split(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems) 
	{
		std::wstringstream ss(s);
		std::wstring item;
		while (std::getline(ss, item, delim))
		{
			elems.push_back(item);
		}
		return elems;
	}
	std::vector<std::wstring> string_split(const std::wstring &s, wchar_t delim)
	{
		std::vector<std::wstring> elems;
		string_split(s, delim, elems);
		return elems;
	}
	namespace ASCII
	{
		std::string GenerateArt(std::string &str)
		{
			std::string lines[6] = {"	","	","	","	","	","	"};
			for(auto i: str)
			{
				char d = i-46;
				if(d >= 0 && d < sizeof(Letters))
				{
					for(char x = 0; x < 6; ++x)
					{
						lines[x].append(Letters[d][x]);
					}
				}
			}
			for(char x = 0; x < 5; ++x)
			{
				lines[x].append("\n");
			}
			return lines[0]+lines[1]+lines[2]+lines[3]+lines[4]+lines[5];
		}
	};
};

