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

#ifdef VEHICLEMANAGER_ENABLE
CVehicleManager::CVehicleManager()
{
	for (VEHICLEID i = 0; i < (MAX_VEHICLES + 1); ++i)
	{
		Vehicles[i].ApplyIterators(this, i);
	}
}

CVehicleManager* VehicleManager = new CVehicleManager();

void CVehicle::ApplyIterators(CVehicleManager* manager, VEHICLEID _vehicleid)
{
	local_VehicleManager = manager;
	vehicleid = _vehicleid;
	local_VehicleManager->VehPtrToId.insert(std::pair<CVehicle*, VEHICLEID>(this, vehicleid));
	local_VehicleManager->IdToVehPtr.insert(std::pair<VEHICLEID, CVehicle*>(vehicleid, this));
	VehPtrToIdItr = local_VehicleManager->VehPtrToId.find(this);
	IdToVehPtrItr = local_VehicleManager->IdToVehPtr.find(vehicleid);
}

CVehicle::CVehicle()
	: created(false),
	vehicleid(0),
	modelid(0),
	driver(INVALID_PLAYER_ID),
	Color({ { -1, -1 } }),
	Paintjob(3),
	_interiorid(0),
	_virtualworld(0),
	Components({ { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }),
	//_doorlocked(0),
	//VehicleParameters({ { false, false, false, false, false, false, false } }),
	creationpos(20000.0, 20000.0, 20000.0, 180.0),
	plate("eXe24 DM"),
	destroyonrespawn(false)
{
	passengers.clear();
}

VEHICLEID CVehicle::ID()
{
	return vehicleid;
}

bool CVehicle::GetPos(float& x, float& y, float& z)
{
	return GetVehiclePos(vehicleid, &x, &y, &z);
}

bool CVehicle::GetPos(float& x, float& y, float& z, float &a)
{
	GetVehicleZAngle(vehicleid, &a);
	return GetVehiclePos(vehicleid, &x, &y, &z);
}

glm::vec3 CVehicle::GetPos3()
{
	float x, y, z;
	GetVehiclePos(vehicleid, &x, &y, &z);
	return glm::vec3(x, y, z);
}

glm::vec4 CVehicle::GetPos4()
{
	float x, y, z, a;
	GetVehiclePos(vehicleid, &x, &y, &z);
	GetVehicleZAngle(vehicleid, &a);
	return glm::vec4(x, y, z, a);
}

bool CVehicle::GetPos3(glm::vec3& pos)
{
	return GetVehiclePos(vehicleid, &pos.x, &pos.y, &pos.z);
}

bool CVehicle::GetPos4(glm::vec4& pos)
{
	GetVehicleZAngle(vehicleid, &pos.a);
	return GetVehiclePos(vehicleid, &pos.x, &pos.y, &pos.z);
}

bool CVehicle::SetPos(glm::vec3 position)
{
	return SetVehiclePos(vehicleid, position.x, position.y, position.z);
}

bool CVehicle::SetPos(glm::vec4 position)
{
	SetVehicleZAngle(vehicleid, position.w);
	return SetVehiclePos(vehicleid, position.x, position.y, position.z);
}

bool CVehicle::SetPos(float x, float y, float z)
{	
	return SetVehiclePos(vehicleid, x, y, z);
}

bool CVehicle::SetPos(float x, float y, float z, float a)
{
	SetVehicleZAngle(vehicleid, a);
	return SetVehiclePos(vehicleid, x, y, z);
}

float CVehicle::GetAngle()
{
	float a;
	GetVehicleZAngle(vehicleid, &a);
	return a;
}

bool CVehicle::GetAngle(float& a)
{	
	return GetVehicleZAngle(vehicleid, &a);
}

bool CVehicle::SetAngle(float a)
{
	return SetVehicleZAngle(vehicleid, a);
}

bool CVehicle::IsValid()
{
	return IsValidVehicle(vehicleid) && created;
}

float CVehicle::DistanceFromPoint(float x, float y, float z)
{
	return GetVehicleDistanceFromPoint(vehicleid, x, y, z);
}

float CVehicle::DistanceFromPoint(glm::vec3 position)
{
	return GetVehicleDistanceFromPoint(vehicleid, position.x, position.y, position.z);
}

bool CVehicle::IsStreamedIn(PLAYERID forplayerid)
{
	return IsVehicleStreamedIn(vehicleid, forplayerid);
}

glm::vec4 CVehicle::GetQuat()
{
	float x, y, z, w;
	GetVehicleRotationQuat(vehicleid, &w, &x, &y, &z);
	return glm::vec4(x, y, z, w);
}

bool CVehicle::GetQuat(glm::vec4& quat)
{	
	return GetVehicleRotationQuat(vehicleid, &quat.w, &quat.x, &quat.y, &quat.z);
}

bool CVehicle::GetQuat(float& x, float& y, float& z, float& w)
{		
	return GetVehicleRotationQuat(vehicleid, &w, &x, &y, &z);
}

bool CVehicle::Respawn()
{
	return SetVehicleToRespawn(vehicleid);
}

bool CVehicle::SetInterior(int interiorid)
{
	_interiorid = interiorid;
	return LinkVehicleToInterior(vehicleid, _interiorid);
}

int CVehicle::GetInterior()
{
	return _interiorid;
}

bool CVehicle::SetVirtualWorld(int worldid)
{
	_virtualworld = worldid;
	return SetVehicleVirtualWorld(vehicleid, _virtualworld);
}

int CVehicle::GetVirtualWorld()
{
	return _virtualworld;
}

bool CVehicle::DetachTrailer()
{
	return DetachTrailerFromVehicle(vehicleid);
}

bool CVehicle::HasTrailer()
{
	return IsTrailerAttachedToVehicle(vehicleid);
}

CVehicle* CVehicle::GetTrailer()
{
	VEHICLEID trailerid = GetVehicleTrailer(vehicleid);
	if (trailerid > 0 && trailerid <= MAX_VEHICLES)
		return &VehicleManager->Vehicles[trailerid];
	return nullptr;
}

VEHICLEID CVehicle::GetTrailer(VEHICLEID)
{
	return GetVehicleTrailer(vehicleid);
}

bool CVehicle::AddComponent(int componentid)
{
	int componentslot = GetVehicleComponentType(componentid);
	if (componentslot < 0 || componentslot > 13 || componentid < 1000 || componentid > 1193)
		return false;
	Components[componentslot] = componentid;
	return AddVehicleComponent(vehicleid, componentid);
}

bool CVehicle::RemoveComponent(int componentid)
{
	int componentslot = GetVehicleComponentType(componentid);
	if (componentslot < 0 || componentslot > 13 || componentid < 1000 || componentid > 1193)
		return false;
	Components[componentslot] = 0;
	return RemoveVehicleComponent(vehicleid, componentid);
}

bool CVehicle::ChangeColor(int color1, int color2)
{
	Color[0] = color1;
	Color[1] = color2;
	return ChangeVehicleColor(vehicleid, color1, color2);
}

bool CVehicle::ChangeColor(int color)
{
	Color[0] = color;
	Color[1] = color;
	return ChangeVehicleColor(vehicleid, color, color);
}

std::array<int, 2> CVehicle::GetColor()
{
	return Color;
}

void CVehicle::GetColor(int& color1, int& color2)
{
	color1 = Color[0];
	color2 = Color[1];
}

void CVehicle::GetColor(int& color)
{
	color = Color[0];
}

bool CVehicle::Repair()
{
	return RepairVehicle(vehicleid);
}

bool CVehicle::ChangePaintjob(int paintjobid)
{
	if (paintjobid < 0 || paintjobid > 3)
		return false;
	Paintjob = paintjobid;
	return ChangeVehiclePaintjob(vehicleid, Paintjob);
}

bool CVehicle::SetHealth(float health)
{
	return SetVehicleHealth(vehicleid, health);
}

float CVehicle::GetHealth()
{
	float hp;
	GetVehicleHealth(vehicleid, &hp);
	return hp;
}

bool CVehicle::GetHealth(float &health)
{
	return GetVehicleHealth(vehicleid, &health);
}

bool CVehicle::GetModelInfo(int infotype, float& X, float& Y, float& Z)
{
	return GetVehicleModelInfo(modelid, infotype, &X, &Y, &Z);
}

bool CVehicle::AttachTrailer(VEHICLEID trailerid)
{
	return AttachTrailerToVehicle(trailerid, vehicleid);
}

bool CVehicle::AttachTrailer(CVehicle* trailerid)
{
	if (local_VehicleManager->VehPtrToId.find(trailerid) != local_VehicleManager->VehPtrToId.end())
		return AttachTrailerToVehicle(trailerid->ID(), vehicleid);
	return false;
}

const int CVehicle::GetModel()
{
	return modelid;
}

bool CVehicle::SetNumberPlate(std::string strplate)
{
	plate.assign(strplate);
	return SetVehicleNumberPlate(vehicleid, plate.c_str());
}

const std::string CVehicle::GetNumberPlate()
{
	return plate;
}

bool CVehicle::SetVelocity(float X, float Y, float Z)
{
	return SetVehicleVelocity(vehicleid, X, Y, Z);
}

bool CVehicle::SetVelocity(glm::vec3 velocity)
{
	return SetVehicleVelocity(vehicleid, velocity.x, velocity.y, velocity.z);
}

bool CVehicle::GetVelocity(float& X, float& Y, float& Z)
{
	return GetVehicleVelocity(vehicleid, &X, &Y, &Z);
}

bool CVehicle::GetVelocity(glm::vec3 &velocity)
{
	return GetVehicleVelocity(vehicleid, &velocity.x, &velocity.y, &velocity.z);
}

glm::vec3 CVehicle::GetVelocity()
{
	float x, y, z;
	GetVehicleVelocity(vehicleid, &x, &y, &z);
	return glm::vec3(x, y, z);
}

void CVehicle::GetMatrix(glm::mat4x3& Mat4x3)
{
	//initial processing step - gathering information
	float rx, ry, rz, rw, x, y, z;
	GetQuat(rw, rx, ry, rz);
	GetPos(x, y, z);

	//initialized math for quaternion to matrix conversion {for sake of simplicity and efficiency}

	float
		x2 = rx * rx,
		y2 = ry * ry,
		z2 = rz * rz,
		xy = rx * ry,
		xz = rx * rz,
		yz = ry * rz,
		wx = rw * rx,
		wy = rw * ry,
		wz = rw * rz;

	//the maths required to convert a quat to a matrix
	Mat4x3[mp_PITCH][mi_X] = 1.0 - 2.0 * (y2 + z2);
	Mat4x3[mp_PITCH][mi_Y] = 2.0 * (xy - wz);
	Mat4x3[mp_PITCH][mi_Z] = 2.0 * (xz + wy);
	Mat4x3[mp_ROLL][mi_X] = 2.0 * (xy + wz);
	Mat4x3[mp_ROLL][mi_Y] = 1.0 - 2.0 * (x2 + z2);
	Mat4x3[mp_ROLL][mi_Z] = 2.0 * (yz - wx);
	Mat4x3[mp_YAW][mi_X] = 2.0 * (xz - wy);
	Mat4x3[mp_YAW][mi_Y] = 2.0 * (yz + wx);
	Mat4x3[mp_YAW][mi_Z] = 1.0 - 2.0 * (x2 + y2);
	//the gta vehicle matrix has the position in it, I want it to keep just like GTA does so I put the position in
	Mat4x3[mp_POS][mi_X] = x;
	Mat4x3[mp_POS][mi_Y] = y;
	Mat4x3[mp_POS][mi_Z] = z;
}

glm::mat4x3 CVehicle::GetMatrix()
{
	glm::mat4x3 Mat4x3;
	//initial processing step - gathering information
	float rx, ry, rz, rw, x, y, z;
	GetQuat(rw, rx, ry, rz);
	GetPos(x, y, z);

	//initialized math for quaternion to matrix conversion {for sake of simplicity and efficiency}

	float
		x2 = rx * rx,
		y2 = ry * ry,
		z2 = rz * rz,
		xy = rx * ry,
		xz = rx * rz,
		yz = ry * rz,
		wx = rw * rx,
		wy = rw * ry,
		wz = rw * rz;

	//the maths required to convert a quat to a matrix
	Mat4x3[mp_PITCH][mi_X] = 1.0 - 2.0 * (y2 + z2);
	Mat4x3[mp_PITCH][mi_Y] = 2.0 * (xy - wz);
	Mat4x3[mp_PITCH][mi_Z] = 2.0 * (xz + wy);
	Mat4x3[mp_ROLL][mi_X] = 2.0 * (xy + wz);
	Mat4x3[mp_ROLL][mi_Y] = 1.0 - 2.0 * (x2 + z2);
	Mat4x3[mp_ROLL][mi_Z] = 2.0 * (yz - wx);
	Mat4x3[mp_YAW][mi_X] = 2.0 * (xz - wy);
	Mat4x3[mp_YAW][mi_Y] = 2.0 * (yz + wx);
	Mat4x3[mp_YAW][mi_Z] = 1.0 - 2.0 * (x2 + y2);
	//the gta vehicle matrix has the position in it, I want it to keep just like GTA does so I put the position in
	Mat4x3[mp_POS][mi_X] = x;
	Mat4x3[mp_POS][mi_Y] = y;
	Mat4x3[mp_POS][mi_Z] = z;
	return Mat4x3;
}

glm::mat4x3 CVehicle::PositionAtOffset(glm::vec3 offsets, glm::vec3& return_pos)
{
	//initial processing step - gather information
	glm::mat4x3 Mat4x3;
	GetMatrix(Mat4x3);

	//offset calculation math
	return_pos.x = offsets.x * Mat4x3[mp_PITCH][mi_X] + offsets.y * Mat4x3[mp_ROLL][mi_X] + offsets.z * Mat4x3[mp_YAW][mi_X] + Mat4x3[mp_POS][mi_X];
	return_pos.y = offsets.x * Mat4x3[mp_PITCH][mi_Y] + offsets.y * Mat4x3[mp_ROLL][mi_Y] + offsets.z * Mat4x3[mp_YAW][mi_Y] + Mat4x3[mp_POS][mi_Y];
	return_pos.z = offsets.x * Mat4x3[mp_PITCH][mi_Z] + offsets.y * Mat4x3[mp_ROLL][mi_Z] + offsets.z * Mat4x3[mp_YAW][mi_Z] + Mat4x3[mp_POS][mi_Z];
	return Mat4x3;
}

bool CVehicle::Refresh(bool visualonly)
{
	if (!created)
		return false;
	if (!visualonly)
	{
		for (int i = 0; i < 14; ++i)
			AddVehicleComponent(vehicleid, Components[i]);
		SetVehicleNumberPlate(vehicleid, plate.c_str());
		LinkVehicleToInterior(vehicleid, _interiorid);
		SetVehicleVirtualWorld(vehicleid, _virtualworld);
	}
	ChangeVehicleColor(vehicleid, Color[0], Color[1]);
	ChangeVehiclePaintjob(vehicleid, Paintjob);
	return true;
}

CVehicle* CVehicle::Create(int vehicletype, float x, float y, float z, float rotation, int color1, int color2, int interiorid, int virtualworld, int respawn_delay, bool dorefresh, bool destroyonrespawn)
{
	if (vehicletype < 400 || vehicletype > 612)
		return nullptr;

	int tmp_vehid = INVALID_VEHICLE_ID;

	switch (vehicletype)
	{
	case 537:
	case 538:
	case 569:
	case 570:
	case 590:
		tmp_vehid = AddStaticVehicle(vehicletype, x, y, z, rotation, color1, color2);
		break;
	default:
		tmp_vehid = CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay);
		break;
	}

	auto iter = VehicleManager->IdToVehPtr.find(tmp_vehid);
	if (iter == VehicleManager->IdToVehPtr.end())
	{
		if (tmp_vehid != INVALID_VEHICLE_ID)
			DestroyVehicle(tmp_vehid);
		return nullptr;
	}

	CVehicle* newvehicle = iter->second;
	newvehicle->modelid = vehicletype;

	if (color1 == -1) newvehicle->Color[0] = Functions::RandomGenerator->Random(0, 0xFF);
	else newvehicle->Color[0] = color1;

	if (color2 == -1) newvehicle->Color[1] = Functions::RandomGenerator->Random(0, 0xFF);
	else newvehicle->Color[1] = color2;

	newvehicle->created = true;
	newvehicle->creationpos = glm::vec4(x, y, z, rotation);
	newvehicle->driver = INVALID_PLAYER_ID;
	newvehicle->passengers.clear();
	newvehicle->Components.fill(0);
	newvehicle->Paintjob = 3;
	newvehicle->plate.assign("eXe DM");
	newvehicle->_interiorid = interiorid;
	newvehicle->_virtualworld = virtualworld;
	newvehicle->destroyonrespawn = destroyonrespawn;

	if (dorefresh)
		newvehicle->Refresh();

	return newvehicle;

}

CVehicle* CVehicle::Create(_GC_VehicleSpawnInfo& info, bool dorefresh, bool destroyonrespawn)
{
	return Create(info.modelid, info.x, info.y, info.z, info.a, info.color1, info.color2, info.interiorid, info.virtualworld, info.respawndelay, dorefresh, destroyonrespawn);
}	

bool CVehicle::Destroy()
{
	created = false;
	if (driver < MAX_PLAYERS)
	{
		local_VehicleManager->PlayerCurrentlyDriving[driver] = nullptr;
		driver = INVALID_PLAYER_ID;
	}
	while (passengers.size())
	{
		PLAYERID passenger = *passengers.begin();
		if (passenger < MAX_PLAYERS)
			local_VehicleManager->PlayerCurrentlyInVehicle[passenger] = nullptr;
		passengers.erase(passengers.begin());
	}
	return DestroyVehicle(vehicleid);
}

bool CVehicle::DestroyOnRespawn()
{
	return destroyonrespawn;
}

bool CVehicle::IsAirplane() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_AIRPLANE);
}

bool CVehicle::IsHelicopter() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_HELICOPTER);
}

bool CVehicle::IsBike() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_BIKE);
}

bool CVehicle::IsTrailer() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_TRAILER);
}

bool CVehicle::IsRC() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_REMOTECONTROL);
}

bool CVehicle::IsArmed() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_ARMED);
}

bool CVehicle::IsLand() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_LAND);
}

bool CVehicle::IsAir() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_AIR);
}

bool CVehicle::IsBoat() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_BOAT);
}

bool CVehicle::IsWater() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_WATER);
}

bool CVehicle::IsCar() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_CAR);
}

bool CVehicle::IsTrain() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_TRAIN);
}

bool CVehicle::IsTrainTrailer() {
	return
		(VehicleModelInfo[modelid] & VEHICLE_TRAINTRAILER);
}

class CVehicleManagerProcessor : public Extension::Base
{
public:
	bool OnVehicleRespray(int playerid, int vehicleid, int color1, int color2)
	{
		CVehicle* vehicle = &VehicleManager->Vehicles[vehicleid];
		if (vehicle->IsValid())
			vehicle->ChangeColor(color1, color2);
		return true;
	}
	bool OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid)
	{
		CVehicle* vehicle = &VehicleManager->Vehicles[vehicleid];
		if (vehicle->IsValid())
			vehicle->ChangePaintjob(paintjobid);
		return true;
	}
	bool OnVehicleStreamIn(int vehicleid, int forplayerid)
	{
		CVehicle* vehicle = &VehicleManager->Vehicles[vehicleid];
		if (vehicle->IsValid())
			vehicle->Refresh(true);
		return true;
	}
	bool OnVehicleSpawn(int vehicleid)
	{
		CVehicle* vehicle = &VehicleManager->Vehicles[vehicleid];
		if (vehicle->DestroyOnRespawn())
			vehicle->Destroy();
		return true;
	}
	bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)
	{
		if (newkeys & KEY_SECONDARY_ATTACK)
		{
			CVehicle* vehicle = VehicleManager->PlayerCurrentlyDriving[playerid];
			if (vehicle && vehicle->IsRC())
			{
				float x, y, z;
				vehicle->GetPos(x, y, z);
				SetPlayerPos(playerid, x, y, z + 1.5);
				RemovePlayerFromVehicle(playerid);
			}
		}
		return true;
	}
	bool OnPlayerConnect(int playerid)
	{
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		return true;
	}
	bool OnPlayerStateChange(int playerid, int newstate, int oldstate)
	{
		return true;
	}
} _CVehicleManagerProcessor;
#endif