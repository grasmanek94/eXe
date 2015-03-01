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
#include <string>
#include "../sampgdk/sampgdk.h"

#define STREAMER_TYPE_OBJECT (0)
#define STREAMER_TYPE_PICKUP (1)
#define STREAMER_TYPE_CP (2)
#define STREAMER_TYPE_RACE_CP (3)
#define STREAMER_TYPE_MAP_ICON (4)
#define STREAMER_TYPE_3D_TEXT_LABEL (5)
#define STREAMER_TYPE_AREA (6)

#define STREAMER_AREA_TYPE_CIRCLE (0)
#define STREAMER_AREA_TYPE_CYLINDER (1)
#define STREAMER_AREA_TYPE_SPHERE (2)
#define STREAMER_AREA_TYPE_RECTANGLE (3)
#define STREAMER_AREA_TYPE_CUBOID (4)
#define STREAMER_AREA_TYPE_POLYGON (5)

#define STREAMER_OBJECT_TYPE_GLOBAL (0)
#define STREAMER_OBJECT_TYPE_PLAYER (1)
#define STREAMER_OBJECT_TYPE_DYNAMIC (2)

#ifndef FLOAT_INFINITY
#define FLOAT_INFINITY ((float)0x7F800000)
#endif

// Enumerator

enum
{
	E_STREAMER_ATTACHED_OBJECT,
	E_STREAMER_ATTACHED_PLAYER,
	E_STREAMER_ATTACHED_VEHICLE,
	E_STREAMER_ATTACH_OFFSET_X,
	E_STREAMER_ATTACH_OFFSET_Y,
	E_STREAMER_ATTACH_OFFSET_Z,
	E_STREAMER_ATTACH_R_X,
	E_STREAMER_ATTACH_R_Y,
	E_STREAMER_ATTACH_R_Z,
	E_STREAMER_ATTACH_X,
	E_STREAMER_ATTACH_Y,
	E_STREAMER_ATTACH_Z,
	E_STREAMER_COLOR,
	E_STREAMER_DRAW_DISTANCE,
	E_STREAMER_EXTRA_ID,
	E_STREAMER_INTERIOR_ID,
	E_STREAMER_MAX_X,
	E_STREAMER_MAX_Y,
	E_STREAMER_MAX_Z,
	E_STREAMER_MIN_X,
	E_STREAMER_MIN_Y,
	E_STREAMER_MIN_Z,
	E_STREAMER_MODEL_ID,
	E_STREAMER_MOVE_R_X,
	E_STREAMER_MOVE_R_Y,
	E_STREAMER_MOVE_R_Z,
	E_STREAMER_MOVE_SPEED,
	E_STREAMER_MOVE_X,
	E_STREAMER_MOVE_Y,
	E_STREAMER_MOVE_Z,
	E_STREAMER_NEXT_X,
	E_STREAMER_NEXT_Y,
	E_STREAMER_NEXT_Z,
	E_STREAMER_PLAYER_ID,
	E_STREAMER_R_X,
	E_STREAMER_R_Y,
	E_STREAMER_R_Z,
	E_STREAMER_SIZE,
	E_STREAMER_STREAM_DISTANCE,
	E_STREAMER_STYLE,
	E_STREAMER_TEST_LOS,
	E_STREAMER_TYPE,
	E_STREAMER_WORLD_ID,
	E_STREAMER_X,
	E_STREAMER_Y,
	E_STREAMER_Z
};

namespace StreamerLibrary
{
	//call
	void Load();
	void Unload();
	//int AmxLoad(AMX *amx);//nn
	//int AmxUnload(AMX *amx);//nn
	void ProcessTick();
	bool OnPlayerConnect(int playerid);
	bool OnPlayerDisconnect(int playerid, int reason);
	bool OnPlayerEnterCheckpoint(int playerid);
	bool OnPlayerLeaveCheckpoint(int playerid);
	bool OnPlayerEnterRaceCheckpoint(int playerid);
	bool OnPlayerLeaveRaceCheckpoint(int playerid);
	bool OnPlayerPickUpPickup(int playerid, int pickupid);
	bool OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ);
	bool OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float x, float y, float z);
	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float x, float y, float z);

	//implement
	bool OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float fX, float fY, float fZ);
	bool OnPlayerSelectDynamicObject(int playerid, int objectid, int modelid, float fX, float fY, float fZ);
	bool OnPlayerEditDynamicObject(int playerid, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ);
	bool OnPlayerPickUpDynamicPickup(int playerid, int pickupid);
	bool OnPlayerLeaveDynamicRaceCP(int playerid, int checkpointid);
	bool OnPlayerEnterDynamicRaceCP(int playerid, int checkpointid);
	bool OnPlayerLeaveDynamicCP(int playerid, int checkpointid);
	bool OnPlayerEnterDynamicCP(int playerid, int checkpointid);
	bool OnPlayerLeaveDynamicArea(int playerid, int areaid);
	bool OnPlayerEnterDynamicArea(int playerid, int areaid);
	bool OnDynamicObjectMoved(int objectid);

	//functions
	// Natives (Settings) not implemented

	int Streamer_GetTickRate();
	int Streamer_SetTickRate(int rate);
	int Streamer_GetMaxItems(int type);
	int Streamer_SetMaxItems(int type, int items);
	int Streamer_GetVisibleItems(int type);
	int Streamer_SetVisibleItems(int type, int items);
	int Streamer_GetCellDistance(float &distance);
	int Streamer_SetCellDistance(float distance);
	int Streamer_GetCellSize(float &size);
	int Streamer_SetCellSize(float size);

	// Natives (Updates)

	int Streamer_ProcessActiveItems();
	int Streamer_ToggleIdleUpdate(int playerid, bool toggle);
	int Streamer_IsToggleIdleUpdate(int playerid);
	int Streamer_ToggleItemUpdate(int playerid, int type, bool toggle);
	int Streamer_IsToggleItemUpdate(int playerid, int type);
	int Streamer_Update(int playerid);
	int Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid = -1, int interiorid = -1);

	// Natives (Data Manipulation) not implemented

	int Streamer_GetFloatData(int type, int id, int data, float &result);
	int Streamer_SetFloatData(int type, int id, int data, float value);
	int Streamer_GetIntData(int type, int id, int data);
	int Streamer_SetIntData(int type, int id, int data, int value);
	int Streamer_GetArrayData(int type, int id, int data, int * dest, unsigned int maxdest);
	int Streamer_SetArrayData(int type, int id, int data, int * src, unsigned int maxsrc);
	int Streamer_IsInArrayData(int type, int id, int data, int value);
	int Streamer_AppendArrayData(int type, int id, int data, int value);
	int Streamer_RemoveArrayData(int type, int id, int data, int value);
	int Streamer_GetUpperBound(int type);

	// Natives (Miscellaneous) not implemented

	int Streamer_GetDistanceToItem(float x, float y, float z, int type, int id, float &distance, int dimensions = 3);
	int Streamer_GetItemInternalID(int playerid, int type, int streamerid);
	int Streamer_GetItemStreamerID(int playerid, int type, int internalid);
	int Streamer_IsItemVisible(int playerid, int type, int id);
	int Streamer_DestroyAllVisibleItems(int playerid, int type, int serverwide = 1);
	int Streamer_CountVisibleItems(int playerid, int type, int serverwide = 1);
	int Streamer_DestroyAllItems(int type, int serverwide = 1);
	int Streamer_CountItems(int type, int serverwide = 1);

	// Natives (Objects)

	int CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 200.0, float drawdistance = 0.0);
	int DestroyDynamicObject(int objectid);
	bool IsValidDynamicObject(int objectid);
	int SetDynamicObjectPos(int objectid, float x, float y, float z);
	int GetDynamicObjectPos(int objectid, float &x, float &y, float &z);
	int SetDynamicObjectRot(int objectid, float rx, float ry, float rz);
	int GetDynamicObjectRot(int objectid, float &rx, float &ry, float &rz);
	int MoveDynamicObject(int objectid, float x, float y, float z, float speed, float rx = -1000.0, float ry = -1000.0, float rz = -1000.0);
	int StopDynamicObject(int objectid);
	bool IsDynamicObjectMoving(int objectid);
	int AttachCameraToDynamicObject(int playerid, int objectid);
	int AttachDynamicObjectToVehicle(int objectid, int vehicleid, float offsetx, float offsety, float offsetz, float rx, float ry, float rz);
	int EditDynamicObject(int playerid, int objectid);
	int GetDynamicObjectMaterial(int objectid, int materialindex, int &modelid, std::string &txdname, std::string &texturename, int &materialcolor);
	int SetDynamicObjectMaterial(int objectid, int materialindex, int modelid, std::string txdname, std::string texturename, int materialcolor = 0);
	int GetDynamicObjectMaterialText(int objectid, int materialindex, std::string text, int &materialsize, std::string fontface, int &fontsize, int &bold, int &fontcolor, int &backcolor, int &textalignment);
	int SetDynamicObjectMaterialText(int objectid, int materialindex, std::string text, int materialsize = OBJECT_MATERIAL_SIZE_256x128, std::string fontface = "Arial", int fontsize = 24, int bold = 1, int fontcolor = 0xFFFFFFFF, int backcolor = 0, int textalignment = 0);

	// Natives (Pickups)

	int CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	int DestroyDynamicPickup(int pickupid);
	bool IsValidDynamicPickup(int pickupid);

	// Natives (Checkpoints)

	int CreateDynamicCP(float x, float y, float z, float size, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	int DestroyDynamicCP(int checkpointid);
	bool IsValidDynamicCP(int checkpointid);
	int TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle);
	int TogglePlayerAllDynamicCPs(int playerid, bool toggle);
	bool IsPlayerInDynamicCP(int playerid, int checkpointid);
	int GetPlayerVisibleDynamicCP(int playerid);

	// Natives (Race Checkpoints)

	int CreateDynamicRaceCP(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	int DestroyDynamicRaceCP(int checkpointid);
	bool IsValidDynamicRaceCP(int checkpointid);
	int TogglePlayerDynamicRaceCP(int playerid, int checkpointid, bool toggle);
	int TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle);
	bool IsPlayerInDynamicRaceCP(int playerid, int checkpointid);
	int GetPlayerVisibleDynamicRaceCP(int playerid);

	// Natives (Map Icons)

	int CreateDynamicMapIcon(float x, float y, float z, int type, int color, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0, int style = MAPICON_LOCAL);
	int DestroyDynamicMapIcon(int iconid);
	int IsValidDynamicMapIcon(int iconid);

	// Natives (3D Text Labels)

	int CreateDynamic3DTextLabel(std::string text, int color, float x, float y, float z, float drawdistance, int attachedplayer = INVALID_PLAYER_ID, int attachedvehicle = INVALID_VEHICLE_ID, int testlos = 0, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	int DestroyDynamic3DTextLabel(int id);
	int IsValidDynamic3DTextLabel(int id);
	int GetDynamic3DTextLabelText(int id, std::string &text);
	int UpdateDynamic3DTextLabelText(int id, int color, std::string text);

	// Natives (Areas)

	int CreateDynamicCircle(float x, float y, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
	int CreateDynamicCylinder(float x, float y, float minz, float maxz, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
	int CreateDynamicSphere(float x, float y, float z, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
	int CreateDynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid = -1, int interiorid = -1, int playerid = -1);
	int CreateDynamicCuboid(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid = -1, int interiorid = -1, int playerid = -1);
	int CreateDynamicCube(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid = -1, int interiorid = -1, int playerid = -1);
	int CreateDynamicPolygon(float *points, float minz = -FLOAT_INFINITY, float maxz = FLOAT_INFINITY, unsigned int max_points = 0, int worldid = -1, int interiorid = -1, int playerid = -1);
	int DestroyDynamicArea(int areaid);
	bool IsValidDynamicArea(int areaid);
	int GetDynamicPolygonPoints(int areaid, float *points, unsigned int max_points);
	int GetDynamicPolygonNumberPoints(int areaid);
	int TogglePlayerDynamicArea(int playerid, int areaid, bool toggle);
	int TogglePlayerAllDynamicAreas(int playerid, bool toggle);
	bool IsPlayerInDynamicArea(int playerid, int areaid, int recheck = 0);
	bool IsPlayerInAnyDynamicArea(int playerid, int recheck = 0);
	bool IsAnyPlayerInDynamicArea(int areaid, int recheck = 0);
	bool IsAnyPlayerInAnyDynamicArea(int recheck = 0);
	int GetPlayerDynamicAreas(int playerid, int *areas, unsigned int max_areas);
	int GetPlayerNumberDynamicAreas(int playerid);
	bool IsPointInDynamicArea(int areaid, float x, float y, float z);
	bool IsPointInAnyDynamicArea(float x, float y, float z);
	int AttachDynamicAreaToObject(int areaid, int objectid, int type = STREAMER_OBJECT_TYPE_DYNAMIC, int playerid = INVALID_PLAYER_ID);
	int AttachDynamicAreaToPlayer(int areaid, int playerid);
	int AttachDynamicAreaToVehicle(int areaid, int vehicleid);

	// Natives (Extended) not implemented

	int CreateDynamicObjectEx(int modelid, float x, float y, float z, float rx, float ry, float rz, float drawdistance = 0.0, float streamdistance = 200.0, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicPickupEx(int modelid, int type, float x, float y, float z, float streamdistance = 100.0, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicCPEx(float x, float y, float z, float size, float streamdistance = 100.0, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicRaceCPEx(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, float streamdistance = 100.0, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicMapIconEx(float x, float y, float z, int type, int color, int style = MAPICON_LOCAL, float streamdistance = 100.0, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamic3DTextLabelEx(std::string text, int color, float x, float y, float z, float drawdistance, int attachedplayer = INVALID_PLAYER_ID, int attachedvehicle = INVALID_VEHICLE_ID, bool testlos = false, float streamdistance = 100.0, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicCircleEx(float x, float y, float size, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicCylinderEx(float x, float y, float minz, float maxz, float size, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicSphereEx(float x, float y, float z, float size, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicRectangleEx(float minx, float miny, float maxx, float maxy, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicCuboidEx(float minx, float miny, float minz, float maxx, float maxy, float maxz, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicCubeEx(float minx, float miny, float minz, float maxx, float maxy, float maxz, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);
	int CreateDynamicPolygonEx(float *point, float minz = -FLOAT_INFINITY, float maxz = FLOAT_INFINITY, unsigned int max_points = NULL, int *worlds = NULL, int *interiors = NULL, int *players = NULL, unsigned int max_worlds = NULL, unsigned int max_interiors = NULL, unsigned int max_players = NULL);

	// Natives (Deprecated) not implemented

	int Streamer_TickRate(int rate);
	int Streamer_MaxItems(int type, int items);
	int Streamer_VisibleItems(int type, int items);
	int Streamer_CellDistance(float distance);
	int Streamer_CellSize(float size);
	int Streamer_CallbackHook(int callback, ...);

	int DestroyAllDynamicObjects();
	int CountDynamicObjects();
	int DestroyAllDynamicPickups();
	int CountDynamicPickups();
	int DestroyAllDynamicCPs();
	int CountDynamicCPs();
	int DestroyAllDynamicRaceCPs();
	int CountDynamicRaceCPs();
	int DestroyAllDynamicMapIcons();
	int CountDynamicMapIcons();
	int DestroyAllDynamic3DTextLabels();
	int CountDynamic3DTextLabels();
	int DestroyAllDynamicAreas();
	int CountDynamicAreas();

	//custom
	int GetDynamicObjectVirtualWorld(int objectid, int index);
	int GetDynamicObjectVirtualWorld(int objectid);
};