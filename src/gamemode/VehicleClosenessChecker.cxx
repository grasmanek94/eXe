/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	This extension checks if there are any vehicles too 
			close to each other

	________________________________________________________________	
	Notes

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

#if defined _LOCALHOST_DEBUG
struct TempStruct
{
	int VehicleID;
	int PickupID;
	glm::vec3 Pos;
};
std::vector<TempStruct> TooClose;
#endif

void VehicleClosenessCheckerDelayed(int timerid, void* param)
{
	std::set<int> UncheckedVehicles;
	std::set<int> CheckedVehicles;

	std::array<glm::vec3, 2> pos;
	for (int i = 0; i < (MAX_VEHICLES + 1); ++i)
	{
		if (IsValidVehicle(i))
			UncheckedVehicles.insert(i);
	}
#if defined _LOCALHOST_DEBUG
	std::cout << "Going to check " << UncheckedVehicles.size() << " vehicles..." << std::endl;
	size_t count = 0;
	size_t TooCloseCount = 0;
#endif
	while (UncheckedVehicles.size())
	{
		int vehicleid = *UncheckedVehicles.begin();

		if (CheckedVehicles.find(vehicleid) == CheckedVehicles.end())
		{
			GetVehiclePos(vehicleid, &pos[0].x, &pos[0].y, &pos[0].z);
			for (auto vehicleid2 : UncheckedVehicles)
			{
#if defined _LOCALHOST_DEBUG
				++count;
#endif
				if (vehicleid != vehicleid2 && 
					!IsVehicleTrain(GetVehicleModel(vehicleid)) && 
					!IsVehicleTrain(GetVehicleModel(vehicleid2)) && 
					!IsVehicleTrainTrailer(GetVehicleModel(vehicleid)) &&
					!IsVehicleTrainTrailer(GetVehicleModel(vehicleid2)))
				{
					GetVehiclePos(vehicleid2, &pos[1].x, &pos[1].y, &pos[1].z);
					float distsquared = 
						pow(pos[1].x - pos[0].x, 2.0) + 
						pow(pos[1].y - pos[0].y, 2.0) + 
						pow(pos[1].z - pos[0].z, 2.0);
					if (distsquared < 1.0)
					{

#if defined _LOCALHOST_DEBUG
						TempStruct _TempStruct;
						_TempStruct.VehicleID = vehicleid;
						_TempStruct.PickupID = StreamerLibrary::CreateDynamicPickup(1318, 1, pos[0].x, pos[0].y, pos[0].z + 4.0);
						_TempStruct.Pos.x = pos[0].x;
						_TempStruct.Pos.y = pos[0].y;
						_TempStruct.Pos.z = pos[0].z;

						TooClose.insert(TooClose.begin(), _TempStruct);
						TooCloseCount++;

						std::cout 
							<< "\tWarning: Vehicles too close @ " 
							<< pos[0].x << ", " << pos[0].y << ", " << pos[0].z 
							<< "| Regex: '.*" 
							<< (int)pos[0].x << "\\..*" << (int)pos[0].y << "\\..*" << (int)(pos[0].z/10.0f) << ".*\\..*'"
							<< std::endl;
#endif
						DestroyVehicle(vehicleid2);
						CheckedVehicles.insert(vehicleid2);

					}
				}
			}
			CheckedVehicles.insert(vehicleid);
		}
		UncheckedVehicles.erase(UncheckedVehicles.begin());
	}

#if defined _LOCALHOST_DEBUG
	std::cout << "... done " << count << " iterations" << std::endl;
	std::cout << TooCloseCount << " Vehicles too close\n";
#endif
}

class VehicleClosenessChecker : public Extension::Base
{
public:
	bool OnGameModeInit()
	{
		sampgdk_SetTimerEx(2000, false, VehicleClosenessCheckerDelayed, nullptr, nullptr);
		return true;
	}
#if defined _LOCALHOST_DEBUG
	bool OnPlayerPickUpDynamicPickup(int playerid, int pickupid)
	{
		for (auto i : TooClose)
		{
			if (i.PickupID == pickupid)
			{
				std::string DebugString(Functions::string_format("VehID: %i, X: %f, Y: %f, Z: %f", i.VehicleID, i.Pos.x, i.Pos.y, i.Pos.z));
				SendClientMessage(playerid, -1, DebugString.c_str());
				std::cout << DebugString << std::endl; /* Copy and paste reasons */
			}
		}
		return true;
	}
#endif
} _VehicleClosenessChecker;