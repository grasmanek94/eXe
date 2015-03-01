/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Neccesary structures and function definitions to communicate with the eXe24+ module

	________________________________________________________________	
	Notes
		-	This source file makes use of the RakNet interface, beware of incorrect usage!

	________________________________________________________________
	Dependencies
		GetPacket
		BitStream
		Structs

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

using namespace RakNet;

namespace Exe24Tool
{
	#ifndef SAMPGDK_STATIC
	CRemoteOptions* RemoteOptions = new CRemoteOptions;
	#endif
};

#ifdef SAMPGDK_STATIC

bool TogglePlayerFly(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.Fly = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.Fly));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerFlyMode(int playerid, bool helicopter)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.FlyModeIsHeli = helicopter;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.FlyModeIsHeli));
	bs.Write(helicopter);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerAirBreak(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.AirBrake = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.AirBrake));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Antibikefalloff(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Antibikefalloff;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerAntiBikeFallOff(int playerid, bool toggle)
{
	TogglePlayerPatch_Antibikefalloff(playerid, toggle);
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.AntiBikeFallOff = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.AntiBikeFallOff));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerBreakDance(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.BreakDance = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.BreakDance));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerRejoin(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.EnableRejoin = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnableRejoin));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerFullWeaponAim(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.FullWeaponAim = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.FullWeaponAim));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerGodMode(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.GodMode = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.GodMode));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPerfectHanding(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.PerfectHanding = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.PerfectHanding));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerQuickTurn(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.QuickTurn = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.QuickTurn));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Removespeedheightlimitsofaircraft(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Removespeedheightlimitsofaircraft;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool RemPlayerAircraftSpeedHeightLimits(int playerid, bool toggle)
{
	TogglePlayerPatch_Removespeedheightlimitsofaircraft(playerid, toggle);
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.RemAircraftSpeedHeightLimits = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.RemAircraftSpeedHeightLimits));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerSuperSpeed(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.Speed = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.Speed));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerSpeedBlurPatch(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.EnablePatch[Exe24Tool::Enablespeedblur] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[Exe24Tool::Enablespeedblur]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerSpeedBlur(int playerid, bool toggle)
{
	TogglePlayerSpeedBlurPatch(playerid, toggle);
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.SpeedBlur = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.SpeedBlur));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerSpider(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.Spider = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.Spider));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerSuperBrake(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.SuperBrake = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.SuperBrake));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerUnflip(int playerid, bool toggle)
{
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.Unflip = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.Unflip));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Walkdriveunderwater(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Walkdriveunderwater;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerWalkDriveUnderWater(int playerid, bool toggle)
{
	TogglePlayerPatch_Walkdriveunderwater(playerid, toggle);
	BitStream bs(3);
	Player[playerid].Exe24ModOptions.WalkDriveUnderWater = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.WalkDriveUnderWater));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Allcarshavenitro(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Allcarshavenitro;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_AntiDerail(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::AntiDerail;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Carsfloatawaywhenhit(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Carsfloatawaywhenhit;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Carshoveroverwater(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Carshoveroverwater;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Enablevehiclenamerendering(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Enablevehiclenamerendering;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Freepaintandspray(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Freepaintandspray;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Infiniteoxygen(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Infiniteoxygen;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Infiniterun(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Infiniterun;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_MegaJumpOnBike(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::MegaJumpOnBike;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_MegaJumpOnFoot(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::MegaJumpOnFoot;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Perfecthandling(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Perfecthandling;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_ScaledowntheHUD(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::ScaledowntheHUD;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Sprintonanysurface(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Sprintonanysurface;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Tankmode(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Tankmode;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_ToggleGTASAsHUD(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::ToggleGTASAsHUD;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Toggleradar(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Toggleradar;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

bool TogglePlayerPatch_Wheelsonly(int playerid, bool toggle)
{
	BitStream bs(3);
	size_t currentpatch = Exe24Tool::Patches::Wheelsonly;
	Player[playerid].Exe24ModOptions.EnablePatch[currentpatch] = toggle;
	bs.Write((unsigned char)77);
	bs.Write((unsigned char)Player[playerid].Exe24ModOptions.GetIndex(&Player[playerid].Exe24ModOptions.EnablePatch[currentpatch]));
	bs.Write(toggle);
	return pRakServer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, Player[playerid].RakNetID, false);
}

#endif