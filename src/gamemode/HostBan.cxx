/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the hostban (and unban) system

	________________________________________________________________	
	Notes
		-	The commands are implemented in AdminSystem.cxx
		-	Considering the server files are on a RAM-Disk
			we can just dump the whole banlist onto the disk
			when changes occur (also: 21 MB even for a HDD isn't much)

	________________________________________________________________
	Dependencies
		Extension

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

namespace HostBan
{
	std::map<unsigned short, CHostBan> HostBans;//max == 21 MB
	bool DataNeedsSaving = false;

	cban_entry* IsIpHostBanned(boost::asio::ip::address_v4 &ip)
	{
		const unsigned short * parts = GetSplitIP(ip.to_ulong());
		auto found = HostBans.find(parts[1]);
		if (found == HostBans.end())
			return nullptr;
		if (!found->second.IsBanned(parts[0]))
			return nullptr;
		if (found->second.ban_entry.bannedtime < Functions::GetTime())
			return nullptr;
		return &found->second.ban_entry;
	}

	void AddHostBan(const boost::asio::ip::address_v4 ip, unsigned short range_begin, unsigned short range_end, unsigned long long bannedtime, unsigned long long bannedidentificator, std::string banneduser, std::string whobannedthisuser, std::string banreason)
	{
		DataNeedsSaving = true;
		const unsigned short * parts = GetSplitIP(ip.to_ulong());
		auto found = HostBans.find(parts[1]);
		if (found != HostBans.end())
		{
			if (bannedtime > bannedidentificator)
			{
				if (range_begin < found->second.range_begin)
					found->second.range_begin = range_begin;
				if (range_end > found->second.range_end)
					found->second.range_end = range_end;
				found->second.ban_entry.bannedtime = bannedtime;
				found->second.ban_entry.bannedidentificator = bannedidentificator;
				found->second.ban_entry.banneduser = banneduser;
				found->second.ban_entry.banreason = banreason;
				found->second.ban_entry.whobannedthisuser = whobannedthisuser;
			}
			else
			{
				HostBans.erase(found);
			}
		}
		else
		{
			auto inserted = HostBans.insert(std::pair<unsigned short, CHostBan>(parts[1], CHostBan(range_begin, range_end)));
			inserted.first->second.ban_entry.bannedtime = bannedtime;
			inserted.first->second.ban_entry.bannedidentificator = bannedidentificator;
			inserted.first->second.ban_entry.banneduser = banneduser;
			inserted.first->second.ban_entry.banreason = banreason;
			inserted.first->second.ban_entry.whobannedthisuser = whobannedthisuser;
		}
	}

	bool RemoveHostBan(const boost::asio::ip::address_v4 ip)
	{
		DataNeedsSaving = true;
		const unsigned short * parts = GetSplitIP(ip.to_ulong());
		auto found = HostBans.find(parts[1]);
		if (found == HostBans.end())
			return false;
		HostBans.erase(found);
		return true;
	}

	void DumpHostBans()
	{
		std::ofstream HostBansFile("HostBans.bin", std::ios_base::binary);
		if (HostBansFile.good())
		{
			for (auto& ban_entry : HostBans)
			{
				HostBansFile.write((char*)&ban_entry.first, sizeof(unsigned short));
				HostBansFile.write((char*)&ban_entry.second.range_begin, sizeof(unsigned short));
				HostBansFile.write((char*)&ban_entry.second.range_end, sizeof(unsigned short));
				HostBansFile.write((char*)&ban_entry.second.ban_entry.bannedidentificator, sizeof(unsigned long long));
				HostBansFile.write((char*)&ban_entry.second.ban_entry.bannedtime, sizeof(unsigned long long));

				{
					size_t str_len = ban_entry.second.ban_entry.banneduser.size() + 1;
					HostBansFile.write((char*)&str_len, sizeof(size_t));
					HostBansFile.write((char*)ban_entry.second.ban_entry.banneduser.c_str(), str_len);
				}

				{
					size_t str_len = ban_entry.second.ban_entry.whobannedthisuser.size() + 1;
					HostBansFile.write((char*)&str_len, sizeof(size_t));
					HostBansFile.write((char*)ban_entry.second.ban_entry.whobannedthisuser.c_str(), str_len);
				}

				{
					size_t str_len = ban_entry.second.ban_entry.banreason.size() + 1;
					HostBansFile.write((char*)&str_len, sizeof(size_t));
					HostBansFile.write((char*)ban_entry.second.ban_entry.banreason.c_str(), str_len);
				}
			}
			HostBansFile.close();
		}
	}

	void ReloadHostBans()
	{
		std::ifstream HostBansFile("HostBans.bin", std::ios_base::binary);
		if (HostBansFile && HostBansFile.good())
		{
			while (HostBansFile.peek() != EOF)
			{
				CHostBanEntry ban_entry;
				HostBansFile.read((char*)&ban_entry.host, sizeof(unsigned short));
				HostBansFile.read((char*)&ban_entry.ranges.range_begin, sizeof(unsigned short));
				HostBansFile.read((char*)&ban_entry.ranges.range_end, sizeof(unsigned short));
				HostBansFile.read((char*)&ban_entry.ranges.ban_entry.bannedidentificator, sizeof(unsigned long long));
				HostBansFile.read((char*)&ban_entry.ranges.ban_entry.bannedtime, sizeof(unsigned long long));

				{
					size_t str_len;
					char * str_data;
					HostBansFile.read((char*)&str_len, sizeof(size_t));
					str_data = new char[str_len];
					HostBansFile.read((char*)str_data, str_len);
					ban_entry.ranges.ban_entry.banneduser.assign(str_data);
					delete[] str_data;
				}

				{
					size_t str_len;
					char * str_data;
					HostBansFile.read((char*)&str_len, sizeof(size_t));
					str_data = new char[str_len];
					HostBansFile.read((char*)str_data, str_len);
					ban_entry.ranges.ban_entry.whobannedthisuser.assign(str_data);
					delete[] str_data;
				}

				{
					size_t str_len;
					char * str_data;
					HostBansFile.read((char*)&str_len, sizeof(size_t));
					str_data = new char[str_len];
					HostBansFile.read((char*)str_data, str_len);
					ban_entry.ranges.ban_entry.banreason.assign(str_data);
					delete[] str_data;
				}

				ban_entry.AddToMap();
			}
			HostBansFile.close();
		}
	}

	void TimedHostBanSaveCheck(int timerid, void* param)
	{
		if (DataNeedsSaving)
		{
			DataNeedsSaving = false;
			DumpHostBans();
		}
	}

	class HostBansProcessor : public Extension::Base
	{
	public:
		bool OnGameModeInit()
		{
			ReloadHostBans();
			sampgdk_SetTimerEx(60 * 60 * 1000, true, TimedHostBanSaveCheck, nullptr, nullptr);
			return true;
		}
	} _HostBansProcessor;
};