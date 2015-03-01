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

boost::posix_time::ptime ThreadedTimerLogger;
class Logger
{
public:
	std::ofstream file;
	std::string logname;
	std::string filename;
	boost::posix_time::ptime openfile;
	bool good;
	size_t buffer_size;
	char * Buffer;

	Logger(std::string _logname, size_t buffersize = 1024)
		: logname(_logname), buffer_size(buffersize)
	{
		boost::filesystem::create_directory(boost::filesystem::current_path() / "logs");
		if (!buffer_size)
		{
			Buffer = nullptr;
			file.rdbuf()->pubsetbuf(0, 0);
		}
		else
		{
			Buffer = new char[buffer_size];
			file.rdbuf()->pubsetbuf(Buffer, buffer_size);
		}
		openfile = boost::posix_time::second_clock::local_time();
		ThreadedTimerLogger = openfile;
		file.open(Functions::string_format("./logs/%04d%02d%02d_%s.log", static_cast<int>(openfile.date().year()), static_cast<int>(openfile.date().month()), static_cast<int>(openfile.date().day()), logname.c_str()), std::ios_base::binary | std::fstream::app | std::fstream::out);
		good = file.good();

	}
	~Logger()
	{
		delete []Buffer;
	}
	void UpdateFilename()
	{
		file.flush();
		file.close();
		openfile = ThreadedTimerLogger;
		file.open(Functions::string_format("./logs/%04d%02d%02d_%s.log", static_cast<int>(openfile.date().year()), static_cast<int>(openfile.date().month()), static_cast<int>(openfile.date().day()), logname.c_str()), std::ios_base::binary | std::fstream::app | std::fstream::out);
		good = file.good();
		file.rdbuf()->pubsetbuf(Buffer, buffer_size);
	}
	void ToLog(std::string towrite)
	{
		if (openfile.date().day_number() != ThreadedTimerLogger.date().day_number())
		{
			UpdateFilename();
		}
		if (good)
		{
			towrite.append("\r\n");
			file.write(towrite.c_str(), towrite.length());
		}
	}
};

struct LoggerHelper
{
	eLoggerType loggerid;
	std::string towrite;
};

concurrency::concurrent_queue<LoggerHelper> WriteToLogs;
std::unordered_map<eLoggerType, Logger*> ID_TO_LOGGER;

void gtLog(eLoggerType loggerid, std::string towrite)
{
	boost::posix_time::time_duration currenttime = boost::posix_time::second_clock::local_time().time_of_day();
	towrite.insert(0, Functions::string_format("[%02d:%02d:%02d]", currenttime.hours(), currenttime.minutes(), currenttime.seconds()));
	WriteToLogs.push({ loggerid, towrite });
}

void ThreadedLoggingWriterThread()
{
	LoggerHelper helper;
	while (true)
	{
		ThreadedTimerLogger = boost::posix_time::second_clock::local_time();
		while (WriteToLogs.try_pop(helper))
		{
			if (helper.loggerid > LOG_DUMMY && helper.loggerid < LOG_ALL)
			{
				ID_TO_LOGGER[helper.loggerid]->ToLog(helper.towrite);
				if (helper.loggerid != LOG_CALLBACKS)
				{
					helper.towrite.insert(10, "[" + ID_TO_LOGGER[helper.loggerid]->logname + "]");
					ID_TO_LOGGER[LOG_ALL]->ToLog(helper.towrite);
				}
			}
			else if (helper.loggerid == LOG_FLUSH_ALL)
			{
				for (auto i : ID_TO_LOGGER)
				{
					i.second->file.flush();
				}
			}
		}
		Sleep(5);
	}
}

#define ADD_LOGGER(type, buffersize) ID_TO_LOGGER[type] = new Logger(#type, buffersize); 
void InitializeGlobalThreadedLogger()
{
	ADD_LOGGER(LOG_CHAT,2048);
	ADD_LOGGER(LOG_COMMANDS, 2048);
	ADD_LOGGER(LOG_CONNECT, 2048);
	ADD_LOGGER(LOG_DISCONNECT, 2048);
	ADD_LOGGER(LOG_IP, 2048)
	ADD_LOGGER(LOG_KICK, 0);
	ADD_LOGGER(LOG_WARNING, 0);
	ADD_LOGGER(LOG_ERROR, 0);
	ADD_LOGGER(LOG_BAN, 0);
	ADD_LOGGER(LOG_DEATH, 2048);
	ADD_LOGGER(LOG_RCONCMD, 2048);
	ADD_LOGGER(LOG_RCONATTEMPT, 2048);
	ADD_LOGGER(LOG_AUDIT, 0);
	ADD_LOGGER(LOG_REGISTER, 2048);
	ADD_LOGGER(LOG_LOGIN, 2048);
	ADD_LOGGER(LOG_PRIVMSG, 2048);
	ADD_LOGGER(LOG_MAFIA, 2048);
	ADD_LOGGER(LOG_OTHER, 2048);
	ADD_LOGGER(LOG_MUTE, 2048);
	ADD_LOGGER(LOG_CMDMUTE, 2048);
	ADD_LOGGER(LOG_CALLBACKS, 0);
	ADD_LOGGER(LOG_ALL, 0);//8196
	new std::thread(ThreadedLoggingWriterThread);
}

static void SAMPGDK_CALL FlushAllData(int timerid, void *param)
{
	gtLog(LOG_FLUSH_ALL, "");
}

class ThreadedLoggingProcessor : public Extension::Base
{
public:
	ThreadedLoggingProcessor() : Base(ExecutionPriorities::ThreadedLogging) {}
	void Load()
	{
		InitializeGlobalThreadedLogger();
		sampgdk_SetTimerEx(30000, true, FlushAllData, nullptr, this);
	}
	bool OnPlayerConnect(int playerid) 
	{
		gtLog(LOG_CONNECT,Functions::string_format("[%d][%s][%s] Connected", playerid, _GetPlayerName(playerid).c_str(), _GetPlayerIP(playerid).c_str()));
		return true; 
	}
	bool OnPlayerDisconnect(int playerid, int reason) 
	{ 
		gtLog(LOG_DISCONNECT, Functions::string_format("[%d][%s][%s] Disconnected(%s)", playerid, Player[playerid].PlayerName.c_str(), Player[playerid].ipv4.to_string().c_str(), DisconnectReasons[reason].c_str()));
		return true; 
	}
	bool OnPlayerDeath(int playerid, int killerid, int reason) 
	{ 
		if (killerid != INVALID_PLAYER_ID)
			gtLog(LOG_DEATH, Functions::string_format("[%d][%s] killed by [%d][%s] with [%d]", playerid, Player[playerid].PlayerName.c_str(), killerid, Player[killerid].PlayerName.c_str(), reason));
		else
			gtLog(LOG_DEATH, Functions::string_format("[%d][%s] suicided", playerid, Player[playerid].PlayerName.c_str()));
		return true;
	}
	bool OnPlayerText(int playerid, std::string text)
	{
		gtLog(LOG_CHAT, Functions::string_format("[%d][%s]: %s", playerid, Player[playerid].PlayerName.c_str(), text.c_str()));
		return true;
	}
	bool OnPlayerCommandText(int playerid, std::string cmdtext) 
	{
		gtLog(LOG_COMMANDS, Functions::string_format("[%d][%s]: %s", playerid, Player[playerid].PlayerName.c_str(), cmdtext.c_str()));
		return true; 
	}
	bool OnRconCommand(std::string cmd) 
	{
		gtLog(LOG_RCONCMD, Functions::string_format("%s", cmd.c_str()));
		return false;
	}
	bool OnRconLoginAttempt(std::string ip, std::string password, bool success) 
	{ 
		gtLog(LOG_RCONATTEMPT, Functions::string_format("[%s][%s][%d]", ip.c_str(), password.c_str(),success));
		return true;
	}
	void OnPlayerGameBegin(int playerid)
	{
		gtLog(LOG_IP, Functions::string_format("[%d][%s][%s]", playerid, Player[playerid].PlayerName.c_str(), Player[playerid].ipv4.to_string().c_str()));
	}
	void Unload()
	{
		for (auto i : ID_TO_LOGGER)
		{
			i.second->file.flush();
		}
	}
} _ThreadedLoggingProcessor;