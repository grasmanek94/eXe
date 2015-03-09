/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the background database worker
		-	IP / Host ban system messages
		-	PlayerAccount loading logic
		-	ODB operations logic

	________________________________________________________________	
	Notes
		-	The database part in this file responsible for retrieveing
			and storing data is threaded, approach with care

	________________________________________________________________
	Dependencies
		Extension
		ZeroDialog
		ODB
		AdminSystem
		Language
		WorldData
		HostBan

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

concurrency::concurrent_queue<WorkerTransportData*> WorkerRequest;
concurrency::concurrent_queue<WorkerTransportData*> WorkerResponse;

bool DatabaseWorkerTryPersist(WorkerTransportData * data)
{
	try
	{
		odb::transaction t(userdb->begin());
		switch (data->type)
		{
		case DATABASE_POINTER_TYPE_BAN:
			if (data->Get<ban>())
			{
				userdb->persist(data->Get<ban>());
			}
			break;
		case DATABASE_POINTER_TYPE_USER:
			if (data->Get<user>())
			{
				userdb->persist(data->Get<user>());
			}
			break;
		case DATABASE_POINTER_TYPE_MAFIA:
			if (data->Get<mafia>())
			{
				userdb->persist(data->Get<mafia>());
			}
			break;
		case DATABASE_POINTER_TYPE_HOUSE:
			if (data->Get<house>())
			{
				userdb->persist(data->Get<house>());
			}
			break;
		case DATABASE_POINTER_TYPE_SENTRYGUN:
			if (data->Get<sentrygun>())
			{
				userdb->persist(data->Get<sentrygun>());
			}
			break;
		}
		t.commit();
		return true;
	}
	catch (...)
	{
		return false;
	}
}
bool DatabaseWorkerTrySave(WorkerTransportData* data)
{
	try
	{
		odb::transaction t(userdb->begin());
		switch (data->type)
		{
		case DATABASE_POINTER_TYPE_BAN:
			if (data->Get<ban>())
			{
				userdb->update(data->Get<ban>());
			}
			break;
		case DATABASE_POINTER_TYPE_USER:
			if (data->Get<user>())
			{
				userdb->update(data->Get<user>());
			}
			break;
		case DATABASE_POINTER_TYPE_MAFIA:
			if (data->Get<mafia>())
			{
				userdb->update(data->Get<mafia>());
			}
			break;
		case DATABASE_POINTER_TYPE_HOUSE:
			if (data->Get<house>())
			{
				userdb->update(data->Get<house>());
			}
			break;
		case DATABASE_POINTER_TYPE_SENTRYGUN:
			if (data->Get<sentrygun>())
			{
				userdb->update(data->Get<sentrygun>());
			}
			break;
		}
		t.commit();
		return true;
	}
	catch (...)
	{
		return DatabaseWorkerTryPersist(data);
	}
}

extern std::atomic<bool> running;
void DatabaseWorkerThread()
{
	Sleep(1000);
	while (!userdb)
	{ 
		Sleep(10); 
	}
	WorkerTransportData * data;
	while (running)
	{
		try
		{
			if(WorkerRequest.try_pop(data))
			{
				switch (data->operation)
				{
					case DATABASE_REQUEST_OPERATION_FIND_LOAD:
					{
						odb::transaction t(userdb->begin());
						boost::algorithm::to_lower(data->username);
						data->Get<user>(userdb->find<user>(data->username));
						t.commit();
						if (data->Get<user>() == nullptr)
						{
							data->operation = DATABASE_REQUEST_OPERATION_FIND_LOAD_NOTFOUND;
						}
						else
						{
							data->operation = DATABASE_REQUEST_OPERATION_FIND_LOAD_FOUND_AND_LOADED;
						}
						WorkerResponse.push(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_CHECK_BAN:
					{
						odb::transaction t(userdb->begin());
						data->Get<ban>(userdb->find<ban>(data->ip));
						t.commit();
						if (data->Get<ban>())
						{
							if (data->Get<ban>()->bannedtime() > Functions::GetTime())
							{
								//banned
								data->operation = DATABASE_REQUEST_OPERATION_CHECK_BAN;
								WorkerResponse.push(data);
							}
							else
							{
								data->Cleanup<ban>();
								CleanupWorkerData(data);
							}
						}
						else
						{
							data->Cleanup<ban>();
							CleanupWorkerData(data);
						}
						break;
					}

					case DATABASE_REQUEST_OPERATION_LOAD_MAFIA:
					{
						odb::transaction t(userdb->begin());
						boost::algorithm::to_lower(data->username);
						data->Get<mafia>(userdb->find<mafia>(data->username));
						t.commit();
						if (data->Get<mafia>() != nullptr)
						{
							data->operation = DATABASE_REQUEST_OPERATION_LOAD_MAFIA_FOUND_AND_LOADED;
						}
						else
						{
							data->operation = DATABASE_REQUEST_OPERATION_LOAD_MAFIA_NOTFOUND;
						}
						WorkerResponse.push(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_SAVE:
					{		
						if (data->Get<user>() != nullptr)
						{
							if (DatabaseWorkerTrySave(data))
							{
								if (data->ip != DATABASE_REQUEST_OPERATION_CREATE_USER)//CREATE_USER should be SAVE_DATA_WITHOUT_DELETION
								{
									data->Cleanup<user>();
									CleanupWorkerData(data);
								}
							}
							else
							{//something is seriously fucking wrong if this happened, but meh, do our usual stuff..
								data->Cleanup<user>();
								CleanupWorkerData(data);
							}
						}
						else
							CleanupWorkerData(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_CHANGE_PASSWORD_OFFLINE:
					{
						boost::algorithm::to_lower(data->username);
						std::vector<std::string> vec = boost::any_cast<std::vector<std::string>> (data->ExtraData);
						odb::transaction t(userdb->begin());
						language_string_ids id;
						if (data->Get<user>(userdb->find<user>(data->username)) != nullptr)
						{				
							data->Get<user>()->password(PlayerPasswordHash(data->Get<user>(), vec[1]));
							userdb->update(data->Get<user>());
							id = L_dbworker_userpwchange_success;
						}
						else
						{
							id = L_dbworker_userpwchange_failure;
						}
						t.commit();
						data->ExtraData = std::pair<std::string,language_string_ids>(vec[0], id);
						data->Cleanup<user>();
						WorkerResponse.push(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_UNBAN_USER:
					{
						odb::transaction t(userdb->begin());
						boost::algorithm::to_lower(data->username);
						data->Get<user>(userdb->find<user>(data->username));

						std::string buf(boost::any_cast<std::string>(data->ExtraData));
						language_string_ids id;
						if (data->Get<user>() != nullptr)
						{

							data->Get<user>()->banned(Functions::GetTime());
							userdb->update(data->Get<user>());

							if (data->playerid == DATABASE_REQUEST_OPERATION_UNBAN_FULL)
							{
								WorkerRequest.push(new WorkerTransportData({ 0, "", 0, DATABASE_REQUEST_OPERATION_UNBAN_IP, DATABASE_POINTER_TYPE_BAN, boost::asio::ip::address_v4::from_string(data->Get<user>()->lastip()).to_ulong(), buf }));
								id = L_dbworker_unban_usr_success_p;
							}
							else
							{
								id = L_dbworker_unban_usr_success_k;
							}

							data->Cleanup<user>();						
						}
						else
						{
							id = L_dbworker_unban_usr_fail;
						}
						t.commit();

						data->ExtraData = std::pair<std::string, language_string_ids>(buf, id);
						WorkerResponse.push(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_UNBAN_IP:
					{
						odb::transaction t(userdb->begin());

						std::string buf(boost::any_cast<std::string>(data->ExtraData));
						language_string_ids id;

						if (data->Get<ban>(userdb->find<ban>(data->ip)) != nullptr)
						{
							data->Get<ban>()->bannedtime(Functions::GetTime());
							userdb->update(data->Get<ban>());

							data->username.assign(boost::asio::ip::address_v4(data->ip).to_string());

							if (data->playerid == DATABASE_REQUEST_OPERATION_UNBAN_FULL)
							{
								WorkerRequest.push(new WorkerTransportData({ 0, data->Get<ban>()->banneduser(), 0, DATABASE_REQUEST_OPERATION_UNBAN_USER, DATABASE_POINTER_TYPE_USER, 0, buf }));
								id = L_dbworker_unban_ip_success_p;
							}
							else
							{
								id = L_dbworker_unban_ip_success_k;
							}
							data->Cleanup<ban>();
						}
						else
						{
							data->username.assign(boost::asio::ip::address_v4(data->ip).to_string());
							id = L_dbworker_unban_ip_fail;
						}						
						t.commit();

						data->ExtraData = std::pair<std::string, language_string_ids>(buf, id);
						WorkerResponse.push(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_REMOVE_PLAYER_FROM_MAFIA://for offline players
					{
						odb::transaction t(userdb->begin());
						boost::algorithm::to_lower(data->username);
						if (data->Get<user>(userdb->find<user>(data->username)) != nullptr)
						{
							std::string Mname(boost::any_cast<std::string>(data->ExtraData));
							if (boost::iequals(data->Get<user>()->MafiaName(), Mname))
							{
								data->Get<user>()->MafiaName("");
							}
							userdb->update(data->Get<user>());									
						}
						t.commit();
						data->Cleanup<user>();
						CleanupWorkerData(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_CREATE_MAFIA:
					{
						odb::transaction t(userdb->begin());
						mafia* ptr = userdb->find<mafia>(data->Get<mafia>()->name());
						t.commit();

						if (ptr != nullptr)
						{
							data->ip = 1120;
						}
						else
						{
							if (DatabaseWorkerTrySave(data))
							{
								data->ip = 1121;
							}
							else
							{
								data->ip = 1122;
							}
						}

						delete ptr;
						ptr = nullptr;

						WorkerResponse.push(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_DELETE_MAFIA:
					{
						boost::algorithm::to_lower(data->username);

						odb::transaction t(userdb->begin());
						
						if (data->Get<mafia>(userdb->find<mafia>(data->username)) != nullptr)
						{
							userdb->erase<mafia>(data->username);
						}

						t.commit();

						data->Cleanup<mafia>();
						CleanupWorkerData(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_DELETE_SENTRYGUN:
					{
						odb::transaction t(userdb->begin());

						userdb->erase<sentrygun>(data->ip);

						t.commit();

						data->Cleanup<sentrygun>();
						CleanupWorkerData(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_SAVE_BAN:
					{
						DatabaseWorkerTrySave(data);
						data->Cleanup<ban>();
						CleanupWorkerData(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_SAVE_MAFIA:
					{
						DatabaseWorkerTrySave(data);
						data->Cleanup<mafia>();
						CleanupWorkerData(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_SAVE_HOUSE:
					{
						DatabaseWorkerTrySave(data);
						data->Cleanup<house>();
						CleanupWorkerData(data);
						break;
					}

					case DATABASE_REQUEST_OPERATION_SAVE_SENTRYGUN:
					{
						DatabaseWorkerTrySave(data);
						data->Cleanup<sentrygun>();
						CleanupWorkerData(data);
						break;
					}
				}
			}
		}
		catch (...)
		{}
		Sleep(1);
	}
}

std::set<std::string, ci_less> LoadingData;

void _DatabaseOperationReport(WorkerTransportData * data)
{
	for (auto &i : *Extension::Extensions)
	{
		i.second->DatabaseOperationReport(data);
	}
}

class DatabaseWorker: public Extension::Base
{
public:
	DatabaseWorker() : Base()
	{
		
	}
	WorkerTransportData * data;

	void ProcessTick()
	{
		if (WorkerResponse.try_pop(data))
		{
			switch (data->operation)
			{
				case DATABASE_REQUEST_OPERATION_FIND_LOAD_FOUND_AND_LOADED:
				{
					LoadingData.erase(data->username);
					int playerid = ValidPlayerName(data->username);
					if (playerid != INVALID_PLAYER_ID)
					{
						data->playerid = playerid;
						LoadingData.erase(data->username);
						Player[playerid].LoadStats(data->Get<user>(), true);

						HostBan::cban_entry * banentry = HostBan::IsIpHostBanned(Player[playerid].ipv4);
						if (banentry == nullptr || Player[playerid].BanImmunity)
						{
							Worker_OnPlayerDataAcquired(playerid, DATABASE_REQUEST_OPERATION_FIND_LOAD_FOUND_AND_LOADED);
						}
						else if (banentry != nullptr && banentry->bannedtime > Functions::GetTime())
						{
							if (!Player[playerid].AlreadyKicked)
							{
								Player[playerid].AlreadyKicked = true;
								ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_banned_iphost),
									TranslatePF(playerid, L_banned_dialogdisplay,
									banentry->whobannedthisuser.c_str(),
									banentry->bannedidentificator,
									banentry->banneduser.c_str(),
									Player[playerid].ipv4.to_string().c_str(),
									banentry->banreason.c_str(),
									StaticVersionDeterminator.GetWebUrl().c_str(),
									Functions::GetTimeStrFromMs(banentry->bannedtime).c_str(),
									Functions::GetTimeStrFromMs(banentry->bannedidentificator).c_str()
									),
									"V", "X");
								DelayKick(playerid, 0xDEAD);
							}
						}
					}
					else
					{
						//weird, player disconnected in meantime?
						data->Cleanup<user>();
						CleanupWorkerData(data);
					}
					break;
				}
				case DATABASE_REQUEST_OPERATION_FIND_LOAD_NOTFOUND:
				{
					LoadingData.erase(data->username);
					//suggest to create account
					int playerid = ValidPlayerName(data->username);
					if (playerid != INVALID_PLAYER_ID)
					{
						HostBan::cban_entry * banentry = HostBan::IsIpHostBanned(Player[playerid].ipv4);
						if (banentry == nullptr || Player[playerid].BanImmunity)
						{
							Worker_OnPlayerDataAcquired(playerid, DATABASE_REQUEST_OPERATION_FIND_LOAD_NOTFOUND);
						}
						else if (banentry != nullptr && banentry->bannedtime > Functions::GetTime())
						{
							if (!Player[playerid].AlreadyKicked)
							{
								Player[playerid].AlreadyKicked = true;
								ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_banned_iphost),
									TranslatePF(playerid, L_banned_dialogdisplay,
									banentry->whobannedthisuser.c_str(),
									banentry->bannedidentificator,
									banentry->banneduser.c_str(),
									Player[playerid].ipv4.to_string().c_str(),
									banentry->banreason.c_str(),
									StaticVersionDeterminator.GetWebUrl().c_str(),
									Functions::GetTimeStrFromMs(banentry->bannedtime).c_str(),
									Functions::GetTimeStrFromMs(banentry->bannedidentificator).c_str()
									), "V", "X");
								DelayKick(playerid, 0xDEAD);
							}
						}
					}
					else
					{
						//weird, player disconnected in meantime?
						data->Cleanup<user>();
						CleanupWorkerData(data);
					}
					break;
				}
				case DATABASE_REQUEST_OPERATION_CHECK_BAN:
				{
					int playerid = ValidPlayerName(data->username);
					if (playerid != INVALID_PLAYER_ID)
					{
						if (!Player[playerid].BanImmunity)
						{
							if (data->Get<ban>()->bannedtime() > Functions::GetTime() && Player[playerid].ipv4.to_ulong() == data->ip)
							{
								if (!Player[playerid].AlreadyKicked)
								{
									Player[playerid].AlreadyKicked = true;
									ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_banned_ip), TranslatePF(playerid, L_banned_dialogdisplay, data->Get<ban>()->whobannedthisuser().c_str(), data->Get<ban>()->bannedidentificator(), data->Get<ban>()->banneduser().c_str(), Player[playerid].ipv4.to_string().c_str(), data->Get<ban>()->banreason().c_str(), StaticVersionDeterminator.GetWebUrl().c_str(), Functions::GetTimeStrFromMs(data->Get<ban>()->bannedtime()).c_str(), Functions::GetTimeStrFromMs(data->Get<ban>()->bannedidentificator()).c_str()), "V", "X");
									DelayKick(playerid, 0xDEAD);
								}
							}
						}
					}
					data->Cleanup<ban>();
					CleanupWorkerData(data);
					break;
				}
				default:
				{
					_DatabaseOperationReport(data);

					if (data != nullptr)
					{
						switch (data->type)
						{
						case DATABASE_POINTER_TYPE_BAN:
							data->Cleanup<ban>();
							break;
						case DATABASE_POINTER_TYPE_USER:
							data->Cleanup<user>();
							break;
						case DATABASE_POINTER_TYPE_MAFIA:
							data->Cleanup<mafia>();
							break;
						}
						CleanupWorkerData(data);
					}
					break;
				}
			}
		}
	}
} _DatabaseWorker;

std::thread * dbworkerthreadid = nullptr;
void DatabaseWorker_Activate()
{
	dbworkerthreadid = new std::thread(DatabaseWorkerThread);
}