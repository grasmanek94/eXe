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
#pragma once

namespace HostBan
{
	inline const unsigned short* GetSplitIP(const unsigned long& ip)
	{
		return (const unsigned short*)&ip;
	}

	class cban_entry
	{
	public:
		unsigned long long bannedtime;
		unsigned long long bannedidentificator;
		std::string banneduser;
		std::string whobannedthisuser;
		std::string banreason;

		cban_entry()
			:
			bannedtime(0),
			banneduser(""),
			whobannedthisuser(""),
			banreason(""),
			bannedidentificator(0)
		{}
		cban_entry
			(
			unsigned long long bannedtime,
			unsigned long long bannedidentificator,
			std::string banneduser,
			std::string whobannedthisuser,
			std::string banreason
			) :
			bannedtime(bannedtime),
			banneduser(banneduser),
			whobannedthisuser(whobannedthisuser),
			banreason(banreason),
			bannedidentificator(bannedidentificator)
		{}
	};

	struct CHostBan
	{
		unsigned short range_begin;
		unsigned short range_end;

		cban_entry ban_entry;

		CHostBan() : range_begin(0), range_end(0xFFFF){}
		CHostBan(unsigned short range_begin, unsigned short range_end) : range_begin(range_begin), range_end(range_end){}

		bool IsBanned(unsigned short lower_part)
		{
			return lower_part <= range_end && lower_part >= range_begin;
		}
	};

	extern std::map<unsigned short, CHostBan> HostBans;

	struct CHostBanEntry
	{
		unsigned short host;
		CHostBan ranges;
		CHostBanEntry(unsigned short host, unsigned short range_begin, unsigned short range_end) :
			host(host), ranges(range_begin, range_end)
		{}
		CHostBanEntry(){}
		void AddToMap()
		{
			HostBans.insert(std::pair<unsigned short, CHostBan>(host, ranges));
		}
	};

	cban_entry* IsIpHostBanned(boost::asio::ip::address_v4 &ip);
	void AddHostBan(const boost::asio::ip::address_v4 ip, unsigned short range_begin, unsigned short range_end, unsigned long long bannedtime, unsigned long long bannedidentificator, std::string banneduser, std::string whobannedthisuser, std::string banreason);
	bool RemoveHostBan(const boost::asio::ip::address_v4 ip);
	void DumpHostBans();
	void ReloadHostBans();
};