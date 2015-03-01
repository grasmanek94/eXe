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

namespace MafiaBase
{
	class System : public Extension::Base
	{
	private:
		CMafia* owner;
		std::string owner_name;

		std::vector<Zabawy::SObject> Objects;
		std::array<float, 6> AreaData;
		glm::vec4 SpawnPos;
		std::array<float, 2> SpawnSpread;

		int AreaID;
		std::set<int> ObjectsIds;

		bool OnGameModeInit() override;
		bool OnPlayerEnterDynamicArea(int playerid, int areaid) override;
		bool OnPlayerLeaveDynamicArea(int playerid, int areaid) override;
		void OnMafiaLoad(int id, CMafia* mafia) override;
		void OnMafiaUnload(int id, CMafia* mafia) override;

	public:
		const glm::vec4& GetSpawnPos();
		const std::array<float, 2>& GetSpawnSpread();

		System(const std::vector<Zabawy::SObject>& Objects,
			glm::vec4 SpawnPos,
			const std::array<float, 2>& SpawnSpread = { { 0.0, 0.0 } },
			float area_x1 = 20000.0, float area_y1 = 20000.0, float area_z1 = 20000.0,
			float area_x2 = 20000.0, float area_y2 = 20000.0, float area_z2 = 20000.0
			);
	};
};