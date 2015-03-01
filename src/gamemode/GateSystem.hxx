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

struct SGateObjectEntry
{
	glm::vec3 InitialPos;
	glm::vec3 InitialRot;
	glm::vec3 EndPos;
	glm::vec3 EndRot;
	SGateObjectEntry(
		float x, float y, float z,
		float rx, float ry, float rz,
		float ex, float ey, float ez,
		float erx, float ery, float erz
		);
	SGateObjectEntry();
	void Set(
		float x, float y, float z,
		float rx, float ry, float rz,
		float ex, float ey, float ez,
		float erx, float ery, float erz);
};

struct SInternalGateEntry
{
	int modelid;
	int objectid;
	SGateObjectEntry GateObjectEntry;
	SInternalGateEntry();
	SInternalGateEntry(int modelid, int objectid, const SGateObjectEntry& GateObjectEntry);
};

class CGate
{
	std::vector<SInternalGateEntry> GateEntries;
	bool closed;
	bool created;
	float movespeed;
public:
	CGate();
	CGate(float movespeed, const std::vector<SInternalGateEntry> & gates);
	bool Create(int worldid = -1, int interiorid = -1);
	bool Create(float _movespeed, const std::vector<SInternalGateEntry> & gates, int worldid = -1, int interiorid = -1);
	bool Open();
	bool Close();
	bool Switch();
	bool IsClosed();
};