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

namespace MafiaBase
{
	bool System::OnGameModeInit()
	{
		for (auto i : Objects)
		{
			ObjectsIds.insert(StreamerLibrary::CreateDynamicObject(i.ModelID, i.x, i.y, i.z, i.rx, i.ry, i.rz, 0, 0, -1, 250.0, 250.0));
		}

		if (AreaData[0] != 20000.0)
		{
			AreaID = StreamerLibrary::CreateDynamicCube(
				std::min(AreaData[0], AreaData[3]),
				std::min(AreaData[1], AreaData[4]),
				std::min(AreaData[2], AreaData[5]),
				std::max(AreaData[0], AreaData[3]),
				std::max(AreaData[1], AreaData[4]),
				std::max(AreaData[2], AreaData[5]),
				0, 0);
		}
		else
		{
			AreaID = 0xFFFF;
		}

		return true;
	}

	bool System::OnPlayerEnterDynamicArea(int playerid, int areaid)
	{
		return true;
	}

	bool System::OnPlayerLeaveDynamicArea(int playerid, int areaid)
	{
		return true;
	}

	void System::OnMafiaLoad(int id, CMafia* mafia)
	{
		if (owner == nullptr && boost::iequals(mafia->Name, owner_name))
		{
			owner = mafia;
			owner->Base = this;
		}
	}

	void System::OnMafiaUnload(int id, CMafia* mafia)
	{
		if (owner != nullptr && boost::iequals(mafia->Name, owner_name))
		{
			owner->Base = nullptr;
			owner = nullptr;
		}
	}

	System::System(const std::vector<Zabawy::SObject>& Objects,
		glm::vec4 SpawnPos,
		const std::array<float, 2>& SpawnSpread,
		float area_x1, float area_y1, float area_z1,
		float area_x2, float area_y2, float area_z2
		)
		: Objects(Objects),
		SpawnPos(SpawnPos),
		SpawnSpread(SpawnSpread),
		AreaData({ { area_x1, area_y1, area_z1, area_x2, area_y2, area_z2 } }),
		owner(nullptr)
	{ }

	const glm::vec4& System::GetSpawnPos()
	{
		return SpawnPos;
	}

	const std::array<float, 2>& System::GetSpawnSpread()
	{
		return SpawnSpread;
	}
};