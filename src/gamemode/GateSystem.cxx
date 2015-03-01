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

SGateObjectEntry::SGateObjectEntry(
	float x, float y, float z,
	float rx, float ry, float rz,
	float ex, float ey, float ez,
	float erx, float ery, float erz
	)
	:
	InitialPos(x, y, z), InitialRot(rx, ry, rz),
	EndPos(ex, ey, ez), EndRot(erx, ery, erz)
{}

SGateObjectEntry::SGateObjectEntry(){}

void SGateObjectEntry::Set(
	float x, float y, float z,
	float rx, float ry, float rz,
	float ex, float ey, float ez,
	float erx, float ery, float erz)
{
	InitialPos = glm::vec3(x, y, z);
	InitialRot = glm::vec3(rx, ry, rz);
	EndPos = glm::vec3(ex, ey, ez); 
	EndRot = glm::vec3(erx, ery, erz);
}

SInternalGateEntry::SInternalGateEntry(){}

SInternalGateEntry::SInternalGateEntry(int modelid, int objectid, const SGateObjectEntry& GateObjectEntry)
	:modelid(modelid), objectid(objectid), GateObjectEntry(GateObjectEntry)
{}

CGate::CGate() : closed(false), created(false), movespeed(0.0f) {}

CGate::CGate(float movespeed, const std::vector<SInternalGateEntry> & gates) :
	closed(false), 
	created(false),
	movespeed(movespeed),
	GateEntries(gates)
{}

bool CGate::Create(int worldid, int interiorid)
{
	if (!created)
	{
		created = true;
		for (auto& gate : GateEntries)
		{
			gate.objectid = StreamerLibrary::CreateDynamicObject(gate.modelid, gate.GateObjectEntry.InitialPos.x, gate.GateObjectEntry.InitialPos.y, gate.GateObjectEntry.InitialPos.z, gate.GateObjectEntry.InitialRot.x, gate.GateObjectEntry.InitialRot.y, gate.GateObjectEntry.InitialRot.z, worldid, interiorid);
		}
		return true;
	}
	return false;
}

bool CGate::Create(float _movespeed, const std::vector<SInternalGateEntry> & gates, int worldid, int interiorid)
{
	if (!created)
	{
		movespeed = _movespeed;
		GateEntries = gates;
		created = true;
		for (auto& gate : GateEntries)
		{
			gate.objectid = StreamerLibrary::CreateDynamicObject(gate.modelid, gate.GateObjectEntry.InitialPos.x, gate.GateObjectEntry.InitialPos.y, gate.GateObjectEntry.InitialPos.z, gate.GateObjectEntry.InitialRot.x, gate.GateObjectEntry.InitialRot.y, gate.GateObjectEntry.InitialRot.z, worldid, interiorid);
		}
		return true;
	}
	return false;
}

bool CGate::Open()
{
	if (created && closed)
	{
		for (auto& gate : GateEntries)
		{
			StreamerLibrary::MoveDynamicObject
			(
				gate.objectid,
				gate.GateObjectEntry.InitialPos.x, gate.GateObjectEntry.InitialPos.y, gate.GateObjectEntry.InitialPos.z,
				movespeed,
				gate.GateObjectEntry.InitialRot.x, gate.GateObjectEntry.InitialRot.y, gate.GateObjectEntry.InitialRot.z
			);
		}
		closed = false;
		return true;
	}
	return false;
}

bool CGate::Close()
{
	if (created && !closed)
	{
		for (auto& gate : GateEntries)
		{
			StreamerLibrary::MoveDynamicObject
			(
				gate.objectid,
				gate.GateObjectEntry.EndPos.x, gate.GateObjectEntry.EndPos.y, gate.GateObjectEntry.EndPos.z,
				movespeed,
				gate.GateObjectEntry.EndRot.x, gate.GateObjectEntry.EndRot.y, gate.GateObjectEntry.EndRot.z
			);
		}
		closed = true;
		return true;
	}
	return false;
}

bool CGate::Switch()
{
	return closed ? Open() : Close();
}

bool CGate::IsClosed()
{
	return closed;
}