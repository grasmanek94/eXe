/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementing the boilerplate code/definitions to create commands easily in source files

	________________________________________________________________	
	Notes
		-	If you are limited by the C++ allowed characters in a class name,
			use cmd_alias to create any command alias you want, input is a string

		-	There are three different main defines:
				• ZERO_COMMAND  - Can define restrictions, permissions and aliasses
				• ZERO_COMMAND3 - Can define restrictions and permissions
				• ZERO_COMMANDF - Can define restrictions, permissions, aliasses and input text parse format

	________________________________________________________________
	Dependencies
		CommandParamsParser
 */
#pragma once
#include "GameMode.hxx"
#include "CommandParamsParser.hxx"

namespace SAMPGDK
{
	namespace ZeroCMD
	{
		class Command
		{
		public:
			Command() {}
			int ExecuteLevel;
			int Restrictions;
			std::vector<std::string> alias;
			ParseInput parser;
			bool parserenabled;
			virtual bool do_command(int playerid, std::string params) { return true; };
		};
		namespace Internal
		{
			extern std::map<std::string, Command*> * command_map;
			void register_command(Command* cmd, std::string name);
			void register_command_fake(std::string name, std::string _format = "");
		};
	};
};

//VS and clang have a mangled name limit of 2048 chars, g++ "unlimited" (well, limited by memory).
#define ZERO_COMMAND(name, min_execute_level, this_restrictions, aliasses) \
class cmd ## name : public SAMPGDK::ZeroCMD::Command\
	{\
		public:\
		cmd ## name() { parserenabled = false; alias = std::vector<std::string>(aliasses); ExecuteLevel = (min_execute_level); Restrictions = (this_restrictions); SAMPGDK::ZeroCMD::Internal::register_command(this, "/"#name); for(auto i: (alias)) { SAMPGDK::ZeroCMD::Internal::register_command(this, i); } }\
		bool do_command(int playerid, std::string params);\
	};\
	cmd ## name cmd ## name ## __execute;\
	bool cmd ## name::do_command(int playerid, std::string params)

typedef std::initializer_list<std::string> cmd_alias;
#define ZERO_COMMAND3(name, min_execute_level, this_restrictions) ZERO_COMMAND(name, min_execute_level, this_restrictions, cmd_alias({}))

#define ZERO_COMMANDF(name, min_execute_level, this_restrictions, aliasses, format) \
class cmd ## name : public SAMPGDK::ZeroCMD::Command\
	{\
		public:\
		cmd ## name() { parserenabled = true; parser.SetFormat(format); alias = std::vector<std::string>(aliasses); ExecuteLevel = (min_execute_level); Restrictions = (this_restrictions); SAMPGDK::ZeroCMD::Internal::register_command(this, "/"#name); for(auto i: (alias)) { SAMPGDK::ZeroCMD::Internal::register_command(this, i); } }\
		bool do_command(int playerid, std::string params);\
	};\
	cmd ## name cmd ## name ## __execute;\
	bool cmd ## name::do_command(int playerid, std::string params)

bool OnPlayerCommandReceived(int playerid, std::string& command, std::string& params, int min_execute_level, int this_restrictions);
bool OnPlayerCommandExecuted(int playerid, std::string& command, std::string& params, bool success);

//note that the 'command' is always in lower-case because of the transformation done in the main processing step
void OnUnknownCommand(int playerid, std::string& command, std::string& params);
bool ZCMD_OnPlayerCommandText(int playerid, std::string& cmdtext);

#define ZCMD ZERO_COMMAND
#define ZCMD3 ZERO_COMMAND3
#define ZCMDF ZERO_COMMANDF
#define ZCMDP ZERO_COMMANDF
#define ZERO_COMMANDP ZERO_COMMANDF

bool ZCMD_CALL_COMMAND(int playerid, std::string command, std::string params);

inline bool ZCMD_COMMAND_EXISTS(std::string command)
{
	return SAMPGDK::ZeroCMD::Internal::command_map->find(command) != SAMPGDK::ZeroCMD::Internal::command_map->end();
}