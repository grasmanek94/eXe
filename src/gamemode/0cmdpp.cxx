/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the main command processor & system
		-	Implementing a dummy command

	________________________________________________________________	
	Notes
		-	the command '/' does nothing, no 'command doesn't exist' and no other messages..
			Probably a sa-mp thing.
			
	________________________________________________________________
	Dependencies
		CommandParamsParser

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

ZERO_COMMAND3(d________DUMMY, PERMISSION_NONE, RESTRICTION_NONE)
{
	return true;
}

namespace SAMPGDK
{
	namespace ZeroCMD
	{
		namespace Internal
		{
			std::map<std::string, Command*> * command_map;
			void register_command_fake(std::string name, std::string _format)
			{
				if (command_map == NULL)
				{
					command_map = new std::map < std::string, Command* > ;
				}

				std::string data(name);
				//transform command to lower case so even command names are case insensetive
				std::transform(data.begin(), data.end(), data.begin(), ::tolower);

				Command * cmd = new Command;
				if (_format.size())
				{
					cmd->parserenabled = true;
					cmd->parser.SetFormat(_format);
				}
				cmd->ExecuteLevel = 0;
				cmd->Restrictions = 0;
				command_map->insert(std::pair<std::string, Command*>(data, cmd));
			}
			void register_command(Command* cmd, std::string name)
			{ 
				if (command_map == NULL)
				{
					command_map = new std::map < std::string, Command* > ;
				}
				std::string data(name);
				//transform command to lower case so even command names are case insensetive
				std::transform(data.begin(), data.end(), data.begin(), ::tolower);

				if (command_map->find(name) != command_map->end())
				{
					std::cout << "ZEROCMD WARNING: Command already exists: " << data << std::endl;
				}
				command_map->insert(std::pair<std::string, Command*>(data, cmd));
			}
		};
	};
};

bool ZCMD_OnPlayerCommandText(int playerid, std::string& cmdtext)
{
	//declare strings we need
	std::string command;
	std::string parameters;

	//find the space in the command
	size_t space = cmdtext.find(' ');

	if(space == std::string::npos)
	{
		//if there is no space treat the whole string as the command
		command.assign(cmdtext);
	}
	else
	{
		//else split the input into a command and parameters, with the first space being the place to split, don't include first space into the parameters string
		command.assign(cmdtext.begin(), cmdtext.begin() + space);
		parameters.assign(cmdtext.begin() + (space + 1), cmdtext.end());
	}

	//make command call lowercase characters so the command call is case-insensetive
	std::transform(command.begin(), command.end(), command.begin(), ::tolower);

	//try to find the command / check if the command exists
	auto cmd = SAMPGDK::ZeroCMD::Internal::command_map->find(command);
	if (cmd != SAMPGDK::ZeroCMD::Internal::command_map->end())
	{
		//check if the user wants the issued command to proceed for the player with the parameters
		if (OnPlayerCommandReceived(playerid, command, parameters, cmd->second->ExecuteLevel, cmd->second->Restrictions))
		{
			//execute the command and check if it succeeded or not by return value
			//execute parser if applicable
			if (cmd->second->parserenabled)
			{
				cmd->second->parser.ExecuteManualParse(parameters);
			}
			//notify callback
			OnPlayerCommandExecuted(playerid, command, parameters, cmd->second->do_command(playerid, parameters));
			//return to this callback
			return true;
		}
		else
		{
			//programmer doesn't want command to proceed
			return true;
		}
	}
	//unknown command
	OnUnknownCommand(playerid,command,parameters);
	return true;
}

bool ZCMD_CALL_COMMAND(int playerid, std::string command, std::string params)
{
	auto executor = SAMPGDK::ZeroCMD::Internal::command_map->find(command);
	if (OnPlayerCommandReceived(playerid, command, params, executor->second->ExecuteLevel, executor->second->Restrictions))
	{
		if (executor->second->parserenabled)
		{
			executor->second->parser.ExecuteManualParse(params);
		}
		return OnPlayerCommandExecuted(playerid, command, params, executor->second->do_command(playerid, params));
	}
	return false;
}