/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the InputText parser class

	________________________________________________________________	
	Notes
		-	Isn't as advanced as SCANF but does it's job very good in a C++ way
			+ specialized for this GameMode 

	________________________________________________________________
	Dependencies
		WorldData

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

bool ParseInput::DoParse(char f, std::string& input, size_t& current_pos, unsigned char delim)
{
	static long _l;
	static unsigned long _ul;
	static long long _ll;
	static unsigned long long _ull;
	static float _f;
	static double _d;
	static long double _ld;
	static HexTo<long> _hl;
	static HexTo<unsigned long> _hul;
	static HexTo<long long> _hll;
	static HexTo<unsigned long long> _hull;

	if (input.size() && current_pos != std::string::npos)
	{
		switch (f)
		{
			case 'p':

			{
				std::string use;
				current_pos = input.find(delim);
				if (current_pos == std::string::npos)
				{
					use.assign(input);
					input.clear();
				}
				else
				{
					use.assign(input.substr(0, current_pos++));
					input.erase(input.begin(), input.begin() + current_pos);
				}
				if (use.size())
				{
					data.push_back(ParsePlayer({ ValidPlayerID(use), use }));
					return true;
				}
			}
			return false;

			case 's':

			data.push_back(input);
			input.clear();
			return true;

			case 'x':
			case 'X':
			case 'm':
			case 'M':
			case 'i':
			case 'l':
			case 'd':
			case 'I':
			case 'D':
			case 'L':
			case 'u':
			case 'U':
			case 'f':
			case 'F':
			case 'z':
			case 'Z':
			case 'q':
			case 'Q':
			{
				std::string use;
				current_pos = input.find(delim);
				if (current_pos == std::string::npos)
				{
					use.assign(input);
					input.clear();
				}
				else
				{
					use.assign(input.substr(0, current_pos++));
					input.erase(input.begin(), input.begin() + current_pos);
				}
				if (use.size())
				{
					switch (f)
					{
					case 'x':
						if (boost::conversion::try_lexical_convert<HexTo<long>>(use, _hl))
						{
							data.push_back(_hl.value);
							return true;
						}
						return false;
					case 'X':
						if (boost::conversion::try_lexical_convert<HexTo<unsigned long>>(use, _hul) && use[0] != '-')
						{
							data.push_back(_hul.value);
							return true;
						}
						return false;
					case 'm':
						if (boost::conversion::try_lexical_convert<HexTo<long long>>(use, _hll))
						{
							data.push_back(_hll.value);
							return true;
						}
						return false;
					case 'M':
						if (boost::conversion::try_lexical_convert<HexTo<unsigned long long>>(use, _hull) && use[0] != '-')
						{
							data.push_back(_hull.value);
							return true;
						}
						return false;
					case 'i':
					case 'l':
					case 'd':
						if (boost::conversion::try_lexical_convert<long>(use, _l))
						{
							data.push_back(_l);
							return true;
						}
						return false;
					case 'I':
					case 'D':
					case 'L':
						if (boost::conversion::try_lexical_convert<unsigned long>(use, _ul) && use[0] != '-')
						{
							data.push_back(_ul);
							return true;
						}
						return false;
					case 'u':
						if (boost::conversion::try_lexical_convert<long long>(use, _ll))
						{
							data.push_back(_ll);
							return true;
						}
						return false;
					case 'U':
						if (boost::conversion::try_lexical_convert<unsigned long long>(use, _ull) && use[0] != '-')
						{
							data.push_back(_ull);
							return true;
						}
						return false;
					case 'f':
						if (boost::conversion::try_lexical_convert<float>(use, _f))
						{
							data.push_back(_f);
							return true;
						}
						return false;
					case 'F':
						if (boost::conversion::try_lexical_convert<float>(use, _f) && use[0] != '-')
						{
							if (_f >= 0.0f)
							{
								data.push_back(_f);
								return true;
							}
						}
						return false;
					case 'z':
						if (boost::conversion::try_lexical_convert<double>(use, _d))
						{
							data.push_back(_d);
							return true;
						}
						return false;
					case 'Z':
						if (boost::conversion::try_lexical_convert<double>(use, _d) && use[0] != '-')
						{
							if (_d >= 0.0)
							{
								data.push_back(_d);
								return true;
							}
						}
						return false;
					case 'q':
						if (boost::conversion::try_lexical_convert<long double>(use, _ld))
						{
							data.push_back(_ld);
							return true;
						}
						return false;
					case 'Q':
						if (boost::conversion::try_lexical_convert<long double>(use, _ld) && use[0] != '-')
						{
							if (_ld >= 0.0)
							{
								data.push_back(_ld);
								return true;
							}
						}
						return false;
					}
				}
				return false;
			}

			case 'w':

			{
				std::string use;
				current_pos = input.find(delim);
				if (current_pos == std::string::npos)
				{
					use.assign(input);
					input.clear();
				}
				else
				{
					use.assign(input.substr(0, current_pos++));
					input.erase(input.begin(), input.begin() + current_pos);
				}
				data.push_back(use);
				return true;
			}
			case 'n':
			{
				std::string use;
				current_pos = input.find(delim);
				if (current_pos == std::string::npos)
				{
					use.assign(input);
					input.clear();
				}
				else
				{
					use.assign(input.substr(0, current_pos++));
					input.erase(input.begin(), input.begin() + current_pos);
				}
				boost::system::error_code ec;
				boost::asio::ip::address ip = boost::asio::ip::address::from_string(use, ec);
				if (!ec)
				{
					data.push_back(ip);
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

ParseInput::ParseInput(std::string _format, std::string input, unsigned char delim)
	: current_param(0), format(_format)
{
	delimeter = delim;
	ExecuteManualParse(input, delimeter);
}

ParseInput::ParseInput(unsigned char delim)
	: current_param(0), format("")
{
	delimeter = delim;
}

void ParseInput::ExecuteManualParse(std::string input, unsigned char delim)
{
	delimeter = delim;
	current_param = 0;
	if (input.size() && format.size())
	{
		data.clear();
		size_t current_pos = 0;
		for (auto f : format)
		{
			if (DoParse(f, input, current_pos, delimeter))
				++current_param;
			else
				break;
		}
	}
}

unsigned long ParseInput::Good()
{
	return current_param;
}

void ParseInput::SetFormat(std::string _format, unsigned char delim)
{
	delimeter = delim;
	format.assign(_format);
}