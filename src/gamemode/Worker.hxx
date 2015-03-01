/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Declaration of database worker prototypes and
			definition of database operation types and structures

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

void DatabaseWorker_Activate();
void SetupDatabase();

enum DATABASE_REQUEST_OPERATION
{
	DATABASE_REQUEST_OPERATION_FIND_LOAD,
	DATABASE_REQUEST_OPERATION_FIND_LOAD_FAIL,
	DATABASE_REQUEST_OPERATION_FIND_LOAD_NOTFOUND,
	DATABASE_REQUEST_OPERATION_FIND_LOAD_FOUND_AND_LOADED,

	DATABASE_REQUEST_OPERATION_SAVE,
	DATABASE_REQUEST_OPERATION_SAVE_OK,
	DATABASE_REQUEST_OPERATION_SAVE_FAIL,

	DATABASE_REQUEST_OPERATION_CHANGE_PASSWORD_OFFLINE,

	DATABASE_REQUEST_OPERATION_SAVE_BAN,
	DATABASE_REQUEST_OPERATION_CHECK_BAN,

	DATABASE_REQUEST_OPERATION_SAVE_MAFIA,
	DATABASE_REQUEST_OPERATION_LOAD_MAFIA,
	DATABASE_REQUEST_OPERATION_LOAD_MAFIA_FOUND_AND_LOADED,
	DATABASE_REQUEST_OPERATION_LOAD_MAFIA_NOTFOUND,
	DATABASE_REQUEST_OPERATION_REMOVE_PLAYER_FROM_MAFIA,
	DATABASE_REQUEST_OPERATION_CREATE_MAFIA,
	DATABASE_REQUEST_OPERATION_DELETE_MAFIA,

	DATABASE_REQUEST_OPERATION_CREATE_USER,

	DATABASE_REQUEST_OPERATION_UNBAN_USER,
	DATABASE_REQUEST_OPERATION_UNBAN_IP,
	DATABASE_REQUEST_OPERATION_UNBAN_FULL,

	DATABASE_REQUEST_OPERATION_SAVE_HOUSE,
	DATABASE_REQUEST_OPERATION_SAVE_SENTRYGUN,
	DATABASE_REQUEST_OPERATION_DELETE_SENTRYGUN
};

enum DATABASE_POINTER_TYPE
{
	DATABASE_POINTER_TYPE_UNSPECIFIED,
	DATABASE_POINTER_TYPE_USER,
	DATABASE_POINTER_TYPE_BAN,
	DATABASE_POINTER_TYPE_MAFIA,
	DATABASE_POINTER_TYPE_HOUSE,
	DATABASE_POINTER_TYPE_SENTRYGUN
};

struct WorkerTransportData
{
	int playerid;
	std::string username;
	void* pointer;
	DATABASE_REQUEST_OPERATION operation;
	DATABASE_POINTER_TYPE type;
	unsigned long ip;
	boost::any ExtraData;
	template<class C> C* Get()
	{
		return (C*)pointer;
	}
	template<class C> C* Get(C * toset)
	{
		pointer = (void*)toset;
		return (C*)pointer;
	}
	template<class C> void Cleanup()
	{
		delete Get<C>();
		Get<C>(nullptr);
	}
};

inline void CleanupWorkerData(WorkerTransportData*& data)
{
	delete data;
	data = nullptr;
}

extern database * userdb;
extern std::set<std::string, ci_less> LoadingData;
extern concurrency::concurrent_queue<WorkerTransportData*> WorkerRequest;
extern concurrency::concurrent_queue<WorkerTransportData*> WorkerResponse;
bool Worker_OnPlayerDataAcquired(int playerid, int result);

inline void CreateWorkerRequest(int playerid, std::string username, void* odb_data_pointer, DATABASE_REQUEST_OPERATION operation, DATABASE_POINTER_TYPE type, unsigned long ip, boost::any ExtraData)
{
	WorkerRequest.push(new WorkerTransportData({ playerid, username, odb_data_pointer, operation, type, ip, ExtraData}));
}

inline void CreateWorkerResponse(int playerid, std::string username, void* odb_data_pointer, DATABASE_REQUEST_OPERATION operation, DATABASE_POINTER_TYPE type, unsigned long ip, boost::any ExtraData)
{
	WorkerResponse.push(new WorkerTransportData({ playerid, username, odb_data_pointer, operation, type, ip, ExtraData }));
}

bool DatabaseWorkerTryPersist(WorkerTransportData * data);
bool DatabaseWorkerTrySave(WorkerTransportData* data);