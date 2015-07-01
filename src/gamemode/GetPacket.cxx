/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the FNV hashing algorithm
		-	Creating a connection cookie system for the sa-mp server to prevent
			faked packets allocating resources on the server
		-	Hook the sa-mp/RakNet receive function to implement packet control and means to receive information from eXe24+ module
		-	Provide means of executing the sa-mp/RakNet Send function to provide means of communication to eXe24+ module

	________________________________________________________________	
	Notes
		-	This source file makes use of the RakNet interface, beware of incorrect usage!
		-	This source file uses HOOKS! Make sure to use the correct addresses and application version

	________________________________________________________________
	Dependencies
		Extension
		BitStream
		Structs

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

int SendTo(SOCKET s, const char *data, int length, char ip[16], unsigned short port);
typedef int(__thiscall *FPTR_SocketLayerSendTo)(void* pSocketLayerObj, SOCKET s, const char *data, int length, unsigned int binaryAddress, unsigned short port);
FPTR_SocketLayerSendTo RealSocketLayerSendTo;

typedef void(__stdcall *FPTR_ProcessNetworkPacket)(const unsigned int binaryAddress, const unsigned short port, const char *data, const int length, void *rakPeer);//0x00456EF0
FPTR_ProcessNetworkPacket RealProcessNetworkPacket;

static const size_t MAX_OFFLINE_DATA_LENGTH = 400;

RakServer *pRakServer = nullptr;

bool inline IsGoodPongLength(size_t length) 
{
	return 
		length >= sizeof(unsigned char) + sizeof(RakNetTime) && 
		length < sizeof(unsigned char) + sizeof(RakNetTime) + MAX_OFFLINE_DATA_LENGTH;
}

std::set<unsigned long long> ip_whitelist;
std::set<unsigned long long> ip_whitelist_online;
std::array<unsigned long long, MAX_PLAYERS> PlayerIPSET;

const unsigned int iObjectSocketLayer = 0x004EDA71;
const unsigned int iRealProcessNetworkPacket = 0x00456EF0;
const unsigned int iSocketLayerSendTo = 0x004633A0;
const unsigned int iProcessQueryPacket = 0x00492660;
const unsigned int iCheckQueryFlood = 0x00492510;
/*
* 32 bit magic FNV-1a prime
*/
#define FNV_32_PRIME ((unsigned long)0x01000193)

/*
* fnv_32a_buf - perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a buffer
*
* input:
*	buf	- start of buffer to hash
*	len	- length of buffer in octets
*	hval	- previous hash value or 0 if first call
*
* returns:
*	32 bit hash as a static hash type
*
* NOTE: To use the recommended 32 bit FNV-1a hash, use FNV1_32A_INIT as the
* 	 hval arg on the first call to either fnv_32a_buf() or fnv_32a_str().
*/
unsigned long
fnv_32a_buf(void *buf, size_t len, unsigned long hval)
{
	unsigned char *bp = (unsigned char *)buf;	/* start of buffer */
	unsigned char *be = bp + len;		/* beyond end of buffer */

	/*
	* FNV-1a hash each octet in the buffer
	*/
	while (bp < be) {

		/* xor the bottom with the current octet */
		hval ^= (unsigned long)*bp++;

		/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
		hval *= FNV_32_PRIME;
#else
		hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
#endif
	}

	/* return our new hash value */
	return hval;
}


/*
* fnv_32a_str - perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a string
*
* input:
*	str	- string to hash
*	hval	- previous hash value or 0 if first call
*
* returns:
*	32 bit hash as a static hash type
*
* NOTE: To use the recommended 32 bit FNV-1a hash, use FNV1_32A_INIT as the
*  	 hval arg on the first call to either fnv_32a_buf() or fnv_32a_str().
*/
unsigned long fnv_32a_str(char *str, unsigned long hval)
{
	unsigned char *s = (unsigned char *)str;	/* unsigned string */

	/*
	* FNV-1a hash each octet in the buffer
	*/
	while (*s) {

		/* xor the bottom with the current octet */
		hval ^= (unsigned long)*s++;

		/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
		hval *= FNV_32_PRIME;
#else
		hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
#endif
	}

	/* return our new hash value */
	return hval;
}
///////////////////////////////////
size_t MyMagicNumber;

size_t shuffle[9];

/*
	Get a 'random' number from a pre-defined list (vector in this case)
*/
size_t get_vec_remove_num(std::vector<size_t>& vec)
{
	size_t num = rand() % vec.size();
	size_t retval = vec[num];
	vec.erase(vec.begin() + num);
	return retval;
}

/*
	Generate the pseudo-random numbers which will be used for creating the secret key for each ip:port
	to verify in a pong packet
*/
void generate_shuffles(int timerid, void * param)
{
	MyMagicNumber = (rand() % 0xFFFF) << 16 | (rand() % 0xFFFF);

	char * magic_sz = (char*)&MyMagicNumber;
	if (!magic_sz[0]) magic_sz[0] = 1;
	if (!magic_sz[1]) magic_sz[1] = 1;
	if (!magic_sz[2]) magic_sz[2] = 1;
	if (!magic_sz[3]) magic_sz[3] = 1;

	std::vector<size_t> vec;

	for (size_t i = 0; i < 16; ++i)
	{
		vec.push_back((size_t)(rand() % 6));
	}

	for (size_t i = 0; i < 8; ++i)
	{
		shuffle[i] = (get_vec_remove_num(vec)) + (get_vec_remove_num(vec));
	}

	shuffle[7] = rand() % 3;
}

/*
	Do some operations to generate a number specific to a ip:port, this has to be done so the attacker cannot spoof pong requests that easily...
*/
unsigned long MySecretReturnCode(const unsigned int binaryAddress, const unsigned short port)
{
	unsigned long long _a = MyMagicNumber << shuffle[0];
	unsigned long long _b = binaryAddress << shuffle[1];
	unsigned long long _c = port << shuffle[2];

	unsigned long long _d = (_a + _b) >> shuffle[3];
	unsigned long long _e = (_c + _d) >> shuffle[4];

	unsigned long long _f = (_a - _d) << shuffle[5];
	unsigned long long _g = (_c - _b) << shuffle[6];

	return (unsigned long)((_a*_b | _c * _d | _f * _g * _e) ^ shuffle[7]);
}

/*
	... and to ensure the attacker will have a though time breaking the algorithm we will throw a FNV hash algorithm over everything we generate
*/
unsigned long _final_security_code(const unsigned int ulong_ip, const unsigned short port)
{
	char ip_sz[5];
	char magic_sz[5];
	char port_sz[3];

	*(unsigned long*)ip_sz = ulong_ip;
	*(unsigned long*)magic_sz = MyMagicNumber;
	*(unsigned short*)port_sz = port;

	if (!ip_sz[0]) ip_sz[0] = 1;
	if (!ip_sz[1]) ip_sz[1] = 1;
	if (!ip_sz[2]) ip_sz[2] = 1;
	if (!ip_sz[3]) ip_sz[3] = 1;
	if (!port_sz[0]) port_sz[0] = 1;
	if (!port_sz[1]) port_sz[1] = 1;

	ip_sz[4] = 0;
	magic_sz[4] = 0;
	port_sz[2] = 0;

	return fnv_32a_str(magic_sz, (fnv_32a_str(port_sz, fnv_32a_str(ip_sz, MySecretReturnCode(ulong_ip, port)))));
}

#if _WIN32
#define REQUIRE_BYTESWAP//define or undefine if players timout after installing this plugin
#endif

#if __linux
//#define REQUIRE_BYTESWAP//define or undefine if players timout after installing this plugin
#endif

/*
	Soms systems have different endianness when getting the binaryAddress, it is needed for boost::asio::ip to be able to compare a string ip with a binary ip
	in OnIncommingConnection
*/
unsigned long inline asfa_swapbytes(unsigned long bytes)
{
#ifdef REQUIRE_BYTESWAP
#ifdef _WIN32
	return _byteswap_ulong(bytes);
#else
	return __builtin_bswap32(bytes);
#endif
#else
	return bytes;
#endif
}

//|Step 1. Hook |Step 2. Challenge |Step 3. PassThrough.
void __stdcall DetouredProcessNetworkPacket(const unsigned int binaryAddress, const unsigned short port, const char *data, const int length, void *rakPeer)
{
	static char ping[5] = { 8/*ID_PING*/, 0, 0, 0, 0 };
	//std::cout << "Packet from: " << std::hex << binaryAddress << std::endl;
	if (binaryAddress != 0x100007F)
	{
		unsigned int ip_data[2] = 
		{ 
			asfa_swapbytes(binaryAddress),
			port
		};
		//check if the ip is already "authenticated"
		if (ip_whitelist.find(*(unsigned long long*)ip_data) == ip_whitelist.end())
		{
			//if not, check if this is an authentication request, if yes and the correct code is bounced add the ip to the whitelist
			if (data[0] == 40/*ID_PONG*/ && IsGoodPongLength(length) && (*(unsigned long*)(data + 1)) == _final_security_code(binaryAddress, port))
			{
				ip_whitelist.insert(*(unsigned long long*)ip_data);
			}
			else//send a ping with the authentication code
			{		
				(*(int*)(ping + 1)) = _final_security_code(binaryAddress, port);
				RealSocketLayerSendTo((void*)iObjectSocketLayer, *(SOCKET*)((char*)pRakServer + 0xC20), (const char*)ping, 5, binaryAddress, port);
			}
			return;
		}
	}
	//if everything went allright, only machines that are alive can get to this point
	RealProcessNetworkPacket(binaryAddress, port, data, length, rakPeer);
}

static const size_t PLUGIN_DATA_RAKSERVER = 0xE2; // RakServerInterface* PluginGetRakServer()

/*
	Detouring is needed for the receive hooks to work
	So this code will do that
	I will not go into the specific workings because this is third party code
*/
void* Detour(unsigned char* src, unsigned char* dst, int num)
{
	if (src == (unsigned char*)0)
	{
		return (void*)0;
	}
	if (dst == (unsigned char*)0)
	{
		return (void*)0;
	}
	if (num < 5)
	{
		return (void*)0;
	}

	unsigned char *all = new unsigned char[5 + num];

#if defined __LINUX__
	size_t pagesize = sysconf(_SC_PAGESIZE);
	size_t addr1 = ((size_t)all / pagesize)*pagesize;
	size_t addr2 = ((size_t)src / pagesize)*pagesize;
	mprotect((void*)addr1, pagesize + num + 5, PROT_READ | PROT_WRITE | PROT_EXEC);
	mprotect((void*)addr2, pagesize + num, PROT_READ | PROT_WRITE | PROT_EXEC);
#else
	unsigned long dwProtect;
	VirtualProtect(all, 5 + num, PAGE_EXECUTE_READWRITE, &dwProtect);
	VirtualProtect(src, num, PAGE_EXECUTE_READWRITE, &dwProtect);
#endif

	memcpy(all, src, num);

	if (all[0] == 0xE9 || all[0] == 0xE8)
	{
		unsigned long val = *(unsigned long*)(&all[1]);
		*(unsigned long*)(&all[1]) = val + src - all;
	}

	all += num;
	unsigned long jmp1 = (unsigned long)((src + num) - (all + 5));

	all[0] = 0xE9;
	*(unsigned long*)(all + 1) = jmp1;

	unsigned long jmp2 = (unsigned long)(dst - src) - 5;

	all -= num;

	src[0] = 0xE9;
	*(unsigned long*)(src + 1) = jmp2;

	return (void*)all;
}

/*
	Not used but in case needed..
	[Dead code]
*/
void Retour(unsigned char* src, unsigned char** all, int num)
{
	if (all == (unsigned char**)0)
	{
		return;
	}
	if (*all == (unsigned char*)0)
	{
		return;
	}
	if (src == (unsigned char*)0)
	{
		return;
	}
	if (num < 5)
	{
		return;
	}

#if defined __LINUX__
	size_t pagesize = sysconf(_SC_PAGESIZE);
	size_t addr = ((size_t)src / pagesize)*pagesize;
	mprotect((void*)addr, pagesize + num, PROT_READ | PROT_WRITE | PROT_EXEC);
#else
	unsigned long dwProtect;
	VirtualProtect(src, num, PAGE_EXECUTE_READWRITE, &dwProtect);
#endif

	memcpy(src, *all, num);

	if (src[0] == 0xE9 || src[0] == 0xE8)
	{
		unsigned long val = *(unsigned long*)(&src[1]);
		*(unsigned long*)(&src[1]) = val + (*all) - src;
	}

	delete[](*all);
	*all = (unsigned char*)0;
}

/*
	We need to ensure we will not saturate our ip whitelist with dead (disconnected) entries
	We basically just make sure we have no memory leaks

	We do this by comparing the ip_whitelist to the connected ip:port list,
	if the entry from the whitelist is not online we remove it, easy.. right?
	Ofcourse Lol.
*/
void CleanupUnusedWhitelistSlots(int timerid, void * param)
{
	for (auto i = ip_whitelist.begin(); i != ip_whitelist.end();)
	{
		if (ip_whitelist_online.find(*i) == ip_whitelist_online.end())
		{
			i = ip_whitelist.erase(i);
		}
		else
		{
			++i;
		}
	}
}

/*
	This is for hooking the receive function, only this seems to work for now..
	don't ask why, I don't know
	Let me do black magic too!
*/
class CHookRakServer
{
public:
	static Packet* __thiscall Receive(void* ppRakServer);
};

using namespace RakNet;
typedef Packet*(__thiscall *FPTR)(void* pRakServer);

FPTR RaknetOriginalReceive;

//////////////ping flood protect//////////////////////

typedef int(__cdecl *FPTR_ProcessQueryPacket)(struct in_addr binaryAddress, u_short port, char *data, int length, SOCKET s);
FPTR_ProcessQueryPacket RealProcessQueryPacket;

typedef bool(__cdecl *FPTR_CheckQueryFlood)(struct in_addr binaryAddress);
FPTR_CheckQueryFlood FuncCheckQueryFlood;

int __cdecl DetouredProcessQueryPacket(struct in_addr binaryAddress, u_short port, char *data, int length, SOCKET s)
{
	if (data[10] == 'p' && FuncCheckQueryFlood(binaryAddress))
	{
		return 0;
	}
	return RealProcessQueryPacket(binaryAddress, port, data, length, s);
}

//////////////////////////////////////////////////////

class GetPacket_Processor : public Extension::Base
{
public:
	/*
		SA-MP allowed the IP:port to connect, let's make a note of it so we can keep track of which IP's have connected succesfully and which did not
	
		In this class we make sure to keep an eye on connected ip's, note which ip's have disconnected and make sure that all our players will not timeout
		because of the protections we put in place
	
	*/
	bool OnIncomingConnection(int playerid, std::string ip_address, int port) override
	{
		unsigned int ip_data[2] = { boost::asio::ip::address_v4::from_string(ip_address).to_ulong(), (unsigned int)port };
		if (PlayerIPSET[playerid] != 0)
		{
			ip_whitelist.erase(PlayerIPSET[playerid]);
		}
		PlayerIPSET[playerid] = *(unsigned long long*)ip_data;
		return true;
	}
	bool OnPlayerConnect(int playerid) override
	{
		ip_whitelist_online.insert(PlayerIPSET[playerid]);
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		ip_whitelist_online.erase(PlayerIPSET[playerid]);
		ip_whitelist.erase(PlayerIPSET[playerid]);
		PlayerIPSET[playerid] = 0;
		return true;
	}
	bool OnGameModeInit() override
	{
		Functions::RandomGenerator->SetSeed(Functions::GetTimeSeconds());
		generate_shuffles(0, 0);
		sampgdk_SetTimerEx(15000, true, generate_shuffles, nullptr, nullptr);
		sampgdk_SetTimerEx(60000, true, CleanupUnusedWhitelistSlots, nullptr, nullptr);
/////////////start antifull		
		int(*pfn_GetRakServer)(void) = (int(*)(void))global_ppData[PLUGIN_DATA_RAKSERVER];
		pRakServer = (RakServer*)pfn_GetRakServer();
		RealProcessNetworkPacket = reinterpret_cast<FPTR_ProcessNetworkPacket>(Detour((unsigned char*)iRealProcessNetworkPacket, (unsigned char*)DetouredProcessNetworkPacket, 7));
		RealSocketLayerSendTo = reinterpret_cast<FPTR_SocketLayerSendTo>(iSocketLayerSendTo);
/////////////end antifull

//////////////ping flood protect//////////////////////
		RealProcessQueryPacket = reinterpret_cast<FPTR_ProcessQueryPacket>(Detour((unsigned char*)iProcessQueryPacket, (unsigned char*)DetouredProcessQueryPacket, 7));
		FuncCheckQueryFlood = reinterpret_cast<FPTR_CheckQueryFlood>(iCheckQueryFlood);
//////////////////////////////////////////////////////

		int RecvFunc = ((int*)(*(void**)pRakServer))[10];
		RaknetOriginalReceive = reinterpret_cast<FPTR>(RecvFunc);

		DWORD temp;
		VirtualProtect((LPVOID)&((int*)(*(void**)pRakServer))[10], 4, PAGE_EXECUTE_READWRITE, &temp);
		((int*)(*(void**)pRakServer))[10] = (int)CHookRakServer::Receive;

		static_assert (sizeof(Packet) == 21, "SIZE IS WRONG");
		static_assert (sizeof(BYTE) == 1, "SIZE IS WRONG");
		static_assert (sizeof(unsigned char) == 1, "SIZE IS WRONG");
		static_assert (sizeof(char) == 1, "SIZE IS WRONG");
		static_assert (sizeof(unsigned short) == 2, "SIZE IS WRONG");
		static_assert (sizeof(short) == 2, "SIZE IS WRONG");
		static_assert (sizeof(unsigned int) == 4, "SIZE IS WRONG");
		static_assert (sizeof(int) == 4, "SIZE IS WRONG");
		static_assert (sizeof(unsigned long) == 4, "SIZE IS WRONG");
		static_assert (sizeof(long) == 4, "SIZE IS WRONG");
		static_assert (sizeof(unsigned long long) == 8, "SIZE IS WRONG");
		static_assert (sizeof(long long) == 8, "SIZE IS WRONG");

		return true;
	}
	/*
		The code below is just a packet send to the player to check if he/she has eXe24+ module installed
		If yes the module will reply with the same packet and module version
		If not the packet will be discarted

		The check is being done in the Receive function, and the logic is easy:
		Player connects: Module not installed
		Send the packet below
		If the receive functions gets a correct packet mark the player as having the module installed
	*/
	void OnPlayerGameBegin(int playerid) override
	{
		BitStream bs;
		bs.Write((unsigned char)77);
		bs.Write((unsigned char)66);
		bs.Write((bool)true);
		pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
	}
} 
_GetPacket_Processor
;

/*
	From SA-MP.

	Sometimes the first bytes of the packet are timestamps, it's what raknet does
	So this function essentially gets rid of it and returns a correct packet identifier.
	It also moves the data in the packet to make sure that you have the correct data when accessing the data member
	I don't know how raknet handles deletion of data but I hope as fucking hell this does not cause a memory leak each time we receive
	a packet with a timestamp
*/
BYTE GetPacketID(Packet *p)
{
	if (p == 0) 
	{ 
		return 255; 
	}

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		p->length -= (sizeof(unsigned char) + sizeof(unsigned long));
		p->bitSize -= (sizeof(unsigned char) + sizeof(unsigned long)) << 3;
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
	{
		return (unsigned char)p->data[0];
	}
}

#if defined ZABEZPIECZENIE_REMOTE_CONTROL
std::array<int, MAX_PLAYERS> ARC_PlayerVehicle;
std::array<int, MAX_VEHICLES + 1> ARC_VehicleDriver;

class RemoteControlDetector : public Extension::Base
{
public:
	bool OnGameModeInit() override
	{
		for (auto &i : ARC_VehicleDriver)
		{
			i = INVALID_PLAYER_ID;
		}
		return true;
	}
	bool OnPlayerConnect(int playerid) override
	{
		ARC_PlayerVehicle[playerid] = 0;
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason) override
	{
		if (ARC_PlayerVehicle[playerid])
		{
			VehicleDriver[ARC_PlayerVehicle[playerid]] = INVALID_PLAYER_ID;
			ARC_PlayerVehicle[playerid] = 0;
		}
		return true;
	}
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate) override
	{
		if (newstate == PLAYER_STATE_DRIVER)
		{		
			int vehicleid = GetPlayerVehicleID(playerid);
			if (vehicleid && ARC_VehicleDriver[vehicleid] == INVALID_PLAYER_ID)
			{
				ARC_PlayerVehicle[playerid] = vehicleid;
				ARC_VehicleDriver[vehicleid] = playerid;
				fixSendClientMessageToAll(-1, Functions::string_format("NEW DRIVER OF VEHICLE %d IS PLAYER %d", ARC_PlayerVehicle[playerid], playerid));
			}
		}
		if (oldstate == PLAYER_STATE_DRIVER)
		{
			fixSendClientMessageToAll(-1, Functions::string_format("OLD DRIVER OF VEHICLE %d IS PLAYER %d", ARC_PlayerVehicle[playerid], playerid));
			ARC_VehicleDriver[ARC_PlayerVehicle[playerid]] = INVALID_PLAYER_ID;
			ARC_PlayerVehicle[playerid] = 0;
		}
		return true;
	}
} _RemoteControlDetector;
#endif
/*

	This is our hooked receive function,
	this is already AFTER RPC's have been processed so don't get any hopes up for RPC packets here
	It is usefull for filtering packets and eXe24+ mod uses this for checking any responses from the client
	(as of now only to check if the mod is active)
*/
Packet* __thiscall CHookRakServer::Receive(void* ppRakServer)
{
	Packet* p = RaknetOriginalReceive(pRakServer);
	/*BYTE packetId = GetPacketID(p);
	if (p)
	{
		bool deallocate = false;
		switch (packetId)
		{
			case 77:
			{
				BitStream bsr(p->data, (p->bitSize / 8) + 1, false);

				unsigned char option;
				bool value;
				unsigned long long version;

				bsr.IgnoreBits(8);
				bsr.Read(option);
				bsr.Read(value);

				if (option == 66 && value == true)//Player has mod
				{
					bsr.Read(version);

					Player[p->playerIndex].Exe24ModVersion = version;

					if (Player[p->playerIndex].Exe24ModVersion < Exe24ModCurrentVersion)
					{
						Player[p->playerIndex].Exe24ModEnabled = false;
						fixSendClientMessage(p->playerIndex, 0xFF0000FF, "Server: 'eXe24+' [NEW VERSION AVAILABLE / NOWA WERSJA DOSTÊPNA / NIEUWE VERSIE IS BESCHIKBAAR] http://plus.exe24.info");
					}
					else
					{
						Player[p->playerIndex].Exe24ModEnabled = true;
						fixSendClientMessage(p->playerIndex, 0x00FF00FF, "Server: 'eXe24+' O.K. :)");
					}
				}
				break;
			}
#if defined ZABEZPIECZENIE_REMOTE_CONTROL
			case ID_VEHICLE_SYNC:
			{
				RakNet::BitStream bsVehicleSync((unsigned char *)p->data, p->length, false);
				VEHICLEID vehicleid;
				bsVehicleSync.IgnoreBits(8);
				bsVehicleSync.Read(vehicleid);
				if (vehicleid <= MAX_VEHICLES)
				{
					if (ARC_PlayerVehicle[p->playerIndex] != vehicleid && ARC_VehicleDriver[vehicleid] != 0xFFFF && ARC_VehicleDriver[vehicleid] != p->playerIndex)
					{
						debug_fixSendClientMessageToAll(std::string(__FUNCSIG__) +"->OnPlayerRemoteControl(int)");
					}
				}
			}
#endif
		}
		if (deallocate)
		{
			pRakServer->DeallocatePacket(p);
			p = 0;
		}
	}*/
	return p;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////