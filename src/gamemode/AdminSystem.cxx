/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Implementation of the IP coloring system
		-	Implementation of administration functions which are needed to manage the server order
		-	Implementation of the permission management functions
	________________________________________________________________	
	Notes
		-	This source file makes use of the RakNet interface, beware of incorrect usage!
		-	This source file uses HOOKS! Make sure to use the correct addresses and application version

	________________________________________________________________
	Dependencies
		Extension
		ZeroCommand
		ZeroDialog
		Database
		Language

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"

namespace ColorIPFixer
{
	int HSVtoRGBcvScalar(int H, int S, int V)
	{
		float fR, fG, fB, t;
		const float FLOAT_TO_BYTE = 255.0f;
		const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
		float fH = (float)H * BYTE_TO_FLOAT;
		int iI;

		fH *= 6.0;
		iI = (int)fH;

		t = fH - floor(fH);

		switch (iI)
		{
		case 0:
			fR = 1.0;
			fG = t;
			fB = 0.0;
			break;
		case 1:
			fR = 1.0 - t;
			fG = 1.0;
			fB = 0.0;
			break;
		case 2:
			fR = 0.0;
			fG = 1.0;
			fB = 1.0 - t;
			break;
		case 3:
			fR = 0.0;
			fG = 1.0 - t;
			fB = 1.0;
			break;
		case 4:
			fR = t;
			fG = 0.0;
			fB = 1.0;
			break;
		default: // case 5 (or 6):
			fR = 1.0;
			fG = 0.0;
			fB = 1.0 - t;
			break;
		}

		return abs((int)(fR * FLOAT_TO_BYTE)) << 16 | abs((int)(fG * FLOAT_TO_BYTE)) << 8 | abs((int)(fB * FLOAT_TO_BYTE));
	}
}

class AdminSystemProcessor : public Extension::Base
{
public:
	AdminSystemProcessor()
	{

	}
	void DatabaseOperationReport(WorkerTransportData * data)
	{
		/*
			Nothing cryptic here, move on.
		*/
		switch (data->operation)
		{
			case DATABASE_REQUEST_OPERATION_CHANGE_PASSWORD_OFFLINE:
			{
				std::pair<std::string, language_string_ids> vec = boost::any_cast<std::pair<std::string, language_string_ids>> (data->ExtraData);
				int playerid = ValidPlayerID(vec.first);
				if (playerid != INVALID_PLAYER_ID)
				{
					ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_password_change), TranslatePF(playerid, vec.second, data->username.c_str()), "V", "X");
				}				
			}
			break;

			case DATABASE_REQUEST_OPERATION_UNBAN_USER:
			case DATABASE_REQUEST_OPERATION_UNBAN_IP:
			{
				std::pair<std::string, language_string_ids> vec = boost::any_cast<std::pair<std::string, language_string_ids>> (data->ExtraData);
				int playerid = ValidPlayerID(vec.first);
				if (playerid != INVALID_PLAYER_ID)
				{
					fixSendClientMessageF(playerid, -1, vec.second, true, true, data->username.c_str());
				}	
			}
			break;
		}
	}
} _AdminSystemProcessor;

//1133;1052;1053;1054;1055;1056;1057;1058;1083;1084;1085;1135;1137;1138;1139;1147;1149;1150;1190
size_t SendMessageToStaff(int color, std::string message, bool important, int sound)
{
	for (auto i : StaffOnline)
	{
		if (important)
			PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
		fixSendClientMessage(i, color, message, false, false);
	}
	return StaffOnline.size();
}

size_t SendMessageToVips(int color, std::string message, bool important, int sound)
{
	for (auto i : VipsOnline)
	{
		if (important)
			PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
		fixSendClientMessage(i, color, message, false, false);
	}
	return VipsOnline.size();
}

size_t SendMessageToSponsors(int color, std::string message, bool important, int sound)
{
	for (auto i : SponsorsOnline)
	{
		if (important)
			PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
		fixSendClientMessage(i, color, message, false, false);
	}
	return SponsorsOnline.size();
}

size_t SendMessageToHeadAdmins(int color, std::string message, bool important, int sound)
{
	for (auto i : HeadAdminsOnline)
	{
		if (important)
			PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
		fixSendClientMessage(i, color, message, false, false);
	}
	return HeadAdminsOnline.size();
}

size_t SendMessageToViceHeadAdmins(int color, std::string message, bool important, int sound)
{
	for (auto i : ViceHeadAdminsOnline)
	{
		if (important)
			PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
		fixSendClientMessage(i, color, message, false, false);
	}
	return ViceHeadAdminsOnline.size();
}

size_t SendMessageToAdmins(int color, std::string message, bool important, int sound)
{
	for (auto i : AdminsOnline)
	{
		if (important)
			PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
		fixSendClientMessage(i, color, message, false, false);
	}
	return AdminsOnline.size();
}

size_t SendMessageToModerators(int color, std::string message, bool important, int sound)
{
	for (auto i : ModeratorsOnline)
	{
		if (important)
			PlayerPlaySound(i, sound, 0.0, 0.0, 0.0);
		fixSendClientMessage(i, color, message, false, false);
	}
	return ModeratorsOnline.size();
}

ZCMDF(setpass, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({"/setpassword", "/resetpass", "/ressetpassword", "/ustawhaslo" }), "ps")
{
	if (parser.Good() == 2)
	{
		ParsePlayer temp(parser.Get<ParsePlayer>(0));
		if (temp.playerid == INVALID_PLAYER_ID)
		{
			std::string password(parser.Get<std::string>(1));
			if (password.size() > 4)
			{
				WorkerRequest.push(new WorkerTransportData({ INVALID_PLAYER_ID, temp.PlayerNameOrID, nullptr, DATABASE_REQUEST_OPERATION_CHANGE_PASSWORD_OFFLINE, DATABASE_POINTER_TYPE_USER, 0, std::vector<std::string>({ Player[playerid].PlayerName, password }) }));
				gtLog(LOG_AUDIT, Functions::string_format("[%d][%s][ADMIN_REQUEST_CHANGE_PASSWORD][%s]->[%s]", playerid, Player[playerid].PlayerName.c_str(), temp.PlayerNameOrID.c_str(), password.c_str()));
				return true;
			}
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_setpass_usage);
	return true;
}

ZCMDF(unbanip, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({}), "ns")
{
	if (parser.Good())
	{
		boost::asio::ip::address ip = parser.Get<boost::asio::ip::address>(0);
		if (ip.is_v4())
		{
			int type = 0;
			if (parser.Good() == 2)
			{
				if (boost::iequals(parser.Get<std::string>(1), "full"))
				{
					type = DATABASE_REQUEST_OPERATION_UNBAN_FULL;
				}
			}
			WorkerRequest.push(new WorkerTransportData({ type, "", nullptr, DATABASE_REQUEST_OPERATION_UNBAN_IP, DATABASE_POINTER_TYPE_USER, ip.to_v4().to_ulong(), Player[playerid].PlayerName }));
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_unbanip_usage);
	return true;
}

ZCMDF(unban, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({}), "ps")
{
	if (parser.Good())
	{
		ParsePlayer user = parser.Get<ParsePlayer>(0);
		if (user.playerid == INVALID_PLAYER_ID)
		{
			int type = 0;
			if (parser.Good() == 2)
			{
				if (boost::iequals(parser.Get<std::string>(1), "full"))
				{
					type = DATABASE_REQUEST_OPERATION_UNBAN_FULL;
				}
			}
			WorkerRequest.push(new WorkerTransportData({ type, user.PlayerNameOrID, nullptr, DATABASE_REQUEST_OPERATION_UNBAN_USER, DATABASE_POINTER_TYPE_USER, 0, Player[playerid].PlayerName }));
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_player_is_online);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_unban_usage);
	return true;
}

#define ADDPERMISSION(perm) {#perm , PERMISSION_##perm }

static const std::map<std::string, eCommandExecuteLevel, ci_less> CommandLevelStringToID =
{
	ADDPERMISSION(NONE),
	ADDPERMISSION(USER),
	ADDPERMISSION(VIP),
	ADDPERMISSION(SPONSOR),
	ADDPERMISSION(MODERATOR),
	ADDPERMISSION(ADMIN),
	ADDPERMISSION(VICEHEAD),
	ADDPERMISSION(HEADADMIN),
	ADDPERMISSION(WSPOLNIK),
	ADDPERMISSION(GAMER)
};

bool PrepareAdditionalPermissionLevels(int playerid, int changerid, unsigned long long& currentpermissionlevel, std::vector<std::string> userpermissions, std::vector<std::string>& failed_permissions, std::vector<std::string>& success_permissions, bool& requested_reset, bool give = true)
{
	requested_reset = false;
	unsigned long long giverMSB = GetMSB(Player[playerid].statistics.privilidges);
	for (auto i : userpermissions)
	{
		auto f = CommandLevelStringToID.find(i);
		if (f != CommandLevelStringToID.end())
		{
			//if permission_none requested all target players permissions are set to 0
			if (f->second == PERMISSION_NONE && !give)
			{
				requested_reset = true;
				break;
			}
			//user can't give permission higher or same than himself, only Gamer_Z can
			if ((unsigned long long)f->second < giverMSB || Player[playerid].statistics.privilidges & PERMISSION_GAMER)
			{
				if (give)
					bSetBitTrue(currentpermissionlevel, f->second);
				else
					bSetBitFalse(currentpermissionlevel, f->second);
				success_permissions.push_back(i);
				continue;
			}
			else
			{
				failed_permissions.push_back(i + TranslateP(changerid, L_PAP_LackOfPermissions));
			}
		}
		else
		{
			failed_permissions.push_back(i + TranslateP(changerid, L_PAP_DoesNotExist));
		}
	}
	if (requested_reset)
	{
		if (Player[changerid].statistics.privilidges > Player[playerid].statistics.privilidges || !give)
			return false;
		currentpermissionlevel = 0;
	}
	return true;
}

void DisplayStaff(int playerid)
{
	std::string lista(TranslateP(playerid, L_Staff_CurrentlyOnline));
	if (StaffOnline.size())
	{
		for (auto i : StaffOnline)
			lista += Functions::string_format("\t[%3d] " + Player[i].PlayerName + "\n", i);
	}
	else
	{
		lista += TranslateP(playerid, L_Staff_CurrentlyNone);
	}
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_Staff_DisplayTitle), lista, "V", "X");
}

ZCMD(admin, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({"/admins","/personel","/staff"}))
{
	DisplayStaff(playerid);
	return true;
}

ZCMD(report, PERMISSION_NONE, RESTRICTION_NONE, cmd_alias({ "/raport", "/r", "/cheat", "/cheater", "/zaraportuj", "/zaraportoj" }))//có¿, trzeba traktowaæ wszystkich równo, nawet upoœledzonych gramatycznie
{
	std::string::size_type pos = params.find(' ');
	std::string id;
	std::string reason;
	if (params.npos != pos)
	{
		reason.assign(params.substr(pos + 1));
		id.assign(params.substr(0, pos));
	}
	if (id.size())
	{
		if (!reason.size() || reason.size() > 64)
		{
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_report_reason_TooShortOrLong);
			return true;
		}
		int reportid = ValidPlayerID(id);
		if (reportid != INVALID_PLAYER_ID && reportid != playerid)
		{
			GivePlayerAchievement(playerid, EAM_Reporter, 1);
			SendMessageToStaff(0xFF0000FF, Functions::string_format("[%3d]" + Player[playerid].PlayerName + " REPORTED [%3d]" + Player[reportid].PlayerName + " REASON: " + reason, playerid, reportid), true);
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_report_send);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_report_usage);
	return true;
}

void RearrangePermissions(int playerid)
{
	if (Player[playerid].statistics.privilidges >= PERMISSION_VIP)
		VipsOnline.insert(playerid);
	else
		VipsOnline.erase(playerid);

	if (Player[playerid].statistics.privilidges >= PERMISSION_SPONSOR)
		SponsorsOnline.insert(playerid);
	else
		SponsorsOnline.erase(playerid);

	if (Player[playerid].statistics.privilidges >= PERMISSION_MODERATOR)
		ModeratorsOnline.insert(playerid);
	else
		ModeratorsOnline.erase(playerid);

	if (Player[playerid].statistics.privilidges >= PERMISSION_ADMIN)
		AdminsOnline.insert(playerid);
	else
		AdminsOnline.erase(playerid);

	if (Player[playerid].statistics.privilidges >= PERMISSION_VICEHEAD)
		ViceHeadAdminsOnline.insert(playerid);
	else
		ViceHeadAdminsOnline.erase(playerid);

	if (Player[playerid].statistics.privilidges >= PERMISSION_HEADADMIN)
		HeadAdminsOnline.insert(playerid);
	else
		HeadAdminsOnline.erase(playerid);

	if (Player[playerid].statistics.privilidges >= PERMISSION_MODERATOR)
		StaffOnline.insert(playerid);
	else
		StaffOnline.erase(playerid);
}

ZCMD(promote, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/givealevel", "/setpermission", "/givepermission" }))
{
	std::string::size_type pos = params.find(' ');
	if (params.npos != pos)
	{
		std::string permissions(params.substr(pos + 1));
		std::string id(params.substr(0, pos));
		if (id.size() && permissions.size())
		{
			int receiverid = ValidPlayerID(id);
			if (receiverid != INVALID_PLAYER_ID)
			{
				std::vector<std::string> permissions_split;
				std::vector<std::string> failures;
				std::vector<std::string> successions;
				bool request_reset;
				Functions::string_split(permissions, ' ', permissions_split);
				if (permissions_split.size())
				{
					bool ret = PrepareAdditionalPermissionLevels(playerid, receiverid, Player[receiverid].statistics.privilidges, permissions_split, failures, successions, request_reset, true);
					if (ret)
					{
						if (Player[playerid].Language == Player[receiverid].Language)
						{
							std::string message(Functions::string_format(TranslateP(playerid, L_promote_info_msg_1), Player[receiverid].PlayerName.c_str(), Player[playerid].PlayerName.c_str()));

							if (failures.size())
							{
								message.append(TranslateP(playerid, L_promote_add_error));
								for (auto i : failures)
								{
									message.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\n");
								}
							}

							if (successions.size())
							{
								message.append(TranslateP(playerid, L_promote_add_success));
								for (auto i : successions)
								{
									message.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\n");
								}
							}

							ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), message, "V", "X");
							ShowPlayerCustomDialog(receiverid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), message, "V", "X");
						}
						else
						{
							std::string message_this(Functions::string_format(TranslateP(playerid, L_promote_info_msg_1), Player[receiverid].PlayerName.c_str(), Player[playerid].PlayerName.c_str()));
							std::string message_receiver(Functions::string_format(TranslateP(receiverid, L_promote_info_msg_1), Player[receiverid].PlayerName.c_str(), Player[playerid].PlayerName.c_str()));

							if (failures.size())
							{
								message_this.append(TranslateP(playerid, L_promote_add_error));
								message_receiver.append(TranslateP(receiverid, L_promote_add_error));
								for (auto i : failures)
								{
									message_this.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\n");
									message_receiver.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\n");
								}
							}

							if (successions.size())
							{
								message_this.append(TranslateP(playerid, L_promote_add_success));
								message_receiver.append(TranslateP(receiverid, L_promote_add_success));
								for (auto i : successions)
								{
									message_this.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\n");
									message_receiver.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\n");
								}
							}

							ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), message_this, "V", "X");
							ShowPlayerCustomDialog(receiverid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(receiverid, L_word_ranks), message_receiver, "V", "X");
						}

						RearrangePermissions(receiverid);

						gtLog(LOG_AUDIT, Functions::string_format("[%d][%s][PERMISSION_GIVE_CHANGE_REQUEST]->[%s]->[%s][RET:%d]", playerid, Player[playerid].PlayerName.c_str(), Player[receiverid].PlayerName.c_str(), permissions.c_str(), successions.size()));
						return true;
					}
					fixSendClientMessage(playerid, Color::COLOR_ERROR, L_promote_noaccess);
					return true;
				}
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_promote_params_error);
				return true;
			}
			SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_promote_params_invalid);
		return true;
	}
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), TranslateP(playerid, L_promote_usage), "V", "X");
	return true;
}


ZCMD(depromote, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/takealevel", "/takepermission", "/unpromote" }))
{
	std::string::size_type pos = params.find(' ');
	if (params.npos != pos)
	{
		std::string permissions(params.substr(pos + 1));
		std::string id(params.substr(0, pos));
		if (id.size() && permissions.size())
		{
			int receiverid = ValidPlayerID(id);
			if (receiverid != INVALID_PLAYER_ID)
			{
				std::vector<std::string> permissions_split;
				std::vector<std::string> failures;
				std::vector<std::string> successions;
				bool request_reset;
				Functions::string_split(permissions, ' ', permissions_split);
				if (permissions_split.size())
				{
					bool ret = PrepareAdditionalPermissionLevels(playerid, receiverid, Player[receiverid].statistics.privilidges, permissions_split, failures, successions, request_reset, false);
					if (ret)
					{
						if (Player[playerid].Language == Player[receiverid].Language)
						{
							std::string message(Functions::string_format(TranslateP(playerid, L_promote_info_msg_1), Player[receiverid].PlayerName.c_str(), Player[playerid].PlayerName.c_str()));

							if (failures.size())
							{
								message.append(TranslateP(playerid, L_promote_take_error));
								for (auto i : failures)
								{
									message.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\r\n");
								}
							}

							if (successions.size())
							{
								message.append(TranslateP(playerid, L_promote_take_success));
								for (auto i : successions)
								{
									message.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\r\n");
								}
							}

							ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), message, "V", "X");
							ShowPlayerCustomDialog(receiverid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), message, "V", "X");
						}
						else
						{
							std::string message_this(Functions::string_format(TranslateP(playerid, L_promote_info_msg_1), Player[receiverid].PlayerName.c_str(), Player[playerid].PlayerName.c_str()));
							std::string message_receiver(Functions::string_format(TranslateP(receiverid, L_promote_info_msg_1), Player[receiverid].PlayerName.c_str(), Player[playerid].PlayerName.c_str()));

							if (failures.size())
							{
								message_this.append(TranslateP(playerid, L_promote_take_error));
								message_receiver.append(TranslateP(receiverid, L_promote_take_error));
								for (auto i : failures)
								{
									message_this.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\r\n");
									message_receiver.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\r\n");
								}
							}

							if (successions.size())
							{
								message_this.append(TranslateP(playerid, L_promote_take_success));
								message_receiver.append(TranslateP(receiverid, L_promote_take_success));
								for (auto i : successions)
								{
									message_this.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\r\n");
									message_receiver.append("\t\t• '{FF0000}" + i + "'{A9C4E4}\r\n");
								}
							}

							ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), message_this, "V", "X");
							ShowPlayerCustomDialog(receiverid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(receiverid, L_word_ranks), message_receiver, "V", "X");
						}

						RearrangePermissions(receiverid);

						gtLog(LOG_AUDIT, Functions::string_format("[%d][%s][PERMISSION_TAKE_CHANGE_REQUEST]->[%s]->[%s][RET:%d]", playerid, Player[playerid].PlayerName.c_str(), Player[receiverid].PlayerName.c_str(), permissions.c_str(), successions.size()));
						return true;
					}
					fixSendClientMessage(playerid, Color::COLOR_ERROR, L_promote_noaccess);
					return true;
				}
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_promote_params_error);
				return true;
			}
			SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_promote_params_invalid);
		return true;
	}
	ShowPlayerCustomDialog(playerid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(playerid, L_word_ranks), TranslateP(playerid, L_depromote_usage), "V", "X");
	return true;
}

static void SAMPGDK_CALL TDelayKick(int timerid, void *param)
{
	sampgdk_Kick((int)param);
}

void DelayKick(int playerid, int reason)
{
	if (reason < 4 && reason >= 0)
	{
		SendSpecialDeathMessage(playerid, 201, 0xDB465077, DisconnectReasonsSafe[reason]);
	}
	sampgdk_SetTimerEx(2, false, TDelayKick, (void*)playerid, nullptr);//mo¿e 2 ms coœ poprawi
}

void aKick(int kickid, int kreason, bool allip, int kickerid, std::string reason, std::string kickername, bool silent)
{
	if (kickerid != INVALID_PLAYER_ID)
		kickername = Player[kickerid].PlayerName;

	unsigned long long kick_identifier = Functions::GetTime();

	if (!reason.size())
		reason.assign("-");

	std::string forkickid(Functions::string_format(TranslateP(kickid, L_kicked_dialoginfo), kickername.c_str(), kick_identifier, Player[kickid].PlayerName.c_str(), reason.c_str()));
	std::string byebye(TranslateP(kickid, L_kicked_bye));

	if (!silent)
		fixSendClientMessageToAllF(Color::COLOR_ERROR, L_kicked_player, true, true, kickername.c_str(), Player[kickid].PlayerName.c_str(), reason.c_str());

	gtLog(LOG_KICK, Functions::string_format("[%d][%s]--KID:[%I64X]-->[%d][%s]:[%s]", kickerid, kickername.c_str(), kick_identifier, kickid, Player[kickid].PlayerName.c_str(), reason.c_str()));

	if (!allip)
	{
		ShowPlayerCustomDialog(kickid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, byebye, forkickid, "V", "X");
		DelayKick(kickid, kreason);
	}
	else
	{
		forkickid.append("\r\n\r\nIP: " + Player[kickid].ipv4.to_string());
		for (auto i : IPtoPlayers[Player[kickid].ipv4])
		{
			ShowPlayerCustomDialog(i, DLG_DUMMY, DIALOG_STYLE_MSGBOX, byebye, forkickid, "V", "X");
			DelayKick(i, kreason);
		}
	}
}

void aBan(int banid, int breason, int playerid, unsigned long long time, std::string reason, std::string banername, bool silent)
{
	if (playerid != INVALID_PLAYER_ID)
		banername = Player[playerid].PlayerName;

	unsigned long long ban_identifier = Functions::GetTime();

	if (!reason.size())
		reason.assign("-");
	
	if (!time)
		Player[banid].statistics.banned = 2114380800000;//ban expires 01-01-2037
	else
		Player[banid].statistics.banned = ban_identifier + (time * 60000);

	Player[banid].statistics.ban_reason = reason;
	Player[banid].statistics.ban_who = banername;
	Player[banid].statistics.bannedip = Player[banid].ipv4.to_ulong();
	Player[banid].statistics.bannedidentificator = ban_identifier;

	std::string forkickid(Functions::string_format(TranslateP(banid, L_banned_dialogdisplay), banername.c_str(), ban_identifier, Player[banid].PlayerName.c_str(), Player[banid].ipv4.to_string().c_str(), reason.c_str(), StaticVersionDeterminator.GetWebUrl().c_str(), Functions::GetTimeStrFromMs(Player[banid].statistics.banned).c_str(), Functions::GetTimeStrFromMs(ban_identifier).c_str()));

	fixSendClientMessageToAllF(Color::COLOR_ERROR, L_banned_player, true, true, banername.c_str(), Player[banid].PlayerName.c_str(), reason.c_str());
	for (auto i : IPtoPlayers[Player[banid].ipv4])
	{
		ShowPlayerCustomDialog(i, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(banid, L_banned_ipandaccount), forkickid, "V", "X");
		DelayKick(i, breason);
	}

	WorkerRequest.push(new WorkerTransportData({ 0, "", new ban(Player[banid].ipv4.to_ulong(), Player[banid].statistics.banned, ban_identifier, Player[banid].PlayerName, banername, reason), DATABASE_REQUEST_OPERATION_SAVE_BAN, DATABASE_POINTER_TYPE_BAN, Player[banid].ipv4.to_ulong(), 0 }));

	gtLog(LOG_BAN, Functions::string_format("[%d][%s]--BID:[%I64X]--TIME:[%I64u]-->[%d][%s][%s]:[%s]", playerid, banername.c_str(), ban_identifier, Player[banid].statistics.banned, banid, Player[banid].PlayerName.c_str(), Player[banid].ipv4.to_string().c_str(), reason.c_str()));
}

ZCMD(kick, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/k", "/wyrzuc", "/papa" }))
{
	std::string::size_type pos = params.find(' ');
	std::string id;
	std::string reason;
	if (params.npos != pos)
	{
		reason.assign(params.substr(pos + 1));
		id.assign(params.substr(0, pos));
	}
	else
	{
		id = params;
	}
	if (id.size())
	{
		int kickid = ValidPlayerID(id);
		if (kickid != INVALID_PLAYER_ID)
		{
			aKick(kickid, 2, false, playerid, reason);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_kick_usage);
	return true;
}

ZCMDF(banuj, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/ban", "/zbanuj", "/czasowyban" }), "ps")
{
	if (parser.Good() == 2)
	{
		std::string reason(parser.Get<std::string>(1));
		ParseInput dodatkowe("Us", reason);

		unsigned long long time = 0;
		
		if (dodatkowe.Good() == 2)
		{
			time = dodatkowe.Get<unsigned long long>(0);
			reason = dodatkowe.Get<std::string>(1);
		}

		int banid = parser.Get<ParsePlayer>(0).playerid;
		if (banid != INVALID_PLAYER_ID)
		{
			aBan(banid, 3, playerid, time, reason);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_ban_usage_1);
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_ban_usage_2);
	return true;
}

ZCMDF(mute, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/ucisz", "/mutuj", "/stojpysk" }), "pUs")
{
	if (parser.Good() >= 2)
	{
		int muteid = parser.Get<ParsePlayer>(0).playerid;
		unsigned long long time = parser.Get<unsigned long long>(1);
		std::string reason("-");
		if (parser.Good() == 3)
			reason = parser.Get<std::string>(2);

		if (muteid != INVALID_PLAYER_ID)
		{
			unsigned long long TimeNow = Functions::GetTime();
			Player[muteid].ChatMute = TimeNow + (time * 1000);
			
			if (TimeNow == 0)
			{
				//Message::SetServerMessage(L_mm_mute_player, 1150);
				SendClientMessage(muteid, Color::COLOR_INFO, L_mm_mute_player);
			}
			else
			{
				//Message::SetServerMessageF(L_mm_mute_player_announce, NO_SOUND, Player[playerid].PlayerName.c_str(), Player[muteid].PlayerName.c_str(), reason.c_str());
				SendClientMessageToAllF(Color::COLOR_ERROR, L_mm_mute_player_announce, false, false, Player[playerid].PlayerName.c_str(), Player[muteid].PlayerName.c_str(), reason.c_str());

				ShowPlayerCustomDialog(muteid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, TranslateP(muteid, L_mute_player_title), Functions::string_format(TranslateP(muteid, L_mute_player_info), Player[playerid].PlayerName.c_str(), TimeNow, Player[muteid].PlayerName.c_str(), reason.c_str(), StaticVersionDeterminator.GetWebUrl().c_str(), time), "V", "X");

				gtLog(LOG_MUTE, Functions::string_format("[%d][" + Player[playerid].PlayerName + "]--MID:[%I64X]--TIME:[%I64u]-->[%d][" + Player[muteid].PlayerName + "]:[" + reason + "]", playerid, TimeNow, time, muteid));
			}
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_mute_usage);
	return true;
}

ZCMD(aclearchat, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/aclear-chat", "/awyczysc-czat", "/aclch", "/cc" }))
{
	for (size_t i = 0; i < 50; ++i)
		fixSendClientMessageToAll(-1, " ");
	return true;
}

ZCMDF(aexplode, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({}), "p")
{
	float x, y, z;
	if (parser.Good())
	{
		int targetid = parser.Get<ParsePlayer>().playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			GetPlayerPos(targetid, &x, &y, &z);
			CreateExplosion(x,y,z, 2, 50.0);
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_aexplode_executed);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_aexplode_usage);
	return true;
}

ZCMDF(aslap, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({}), "p")
{
	float x, y, z, vx, vy, vz, hp;
	if (parser.Good())
	{
		int targetid = parser.Get<ParsePlayer>().playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			if (Player[targetid].CurrentVehicle)
			{
				GetVehiclePos(Player[targetid].CurrentVehicle, &x, &y, &z);
				GetVehicleVelocity(Player[targetid].CurrentVehicle, &vx, &vy, &vz);
				fixSetVehiclePos(Player[targetid].CurrentVehicle, x, y, z + 0.5);
				SetVehicleVelocity(Player[targetid].CurrentVehicle, vx, vy, vz);				
			}
			else
			{
				GetPlayerPos(targetid, &x, &y, &z);
				GetPlayerVelocity(targetid, &vx, &vy, &vz);
				fixSetPlayerPos(targetid, x, y, z + 4.0);
				SetPlayerVelocity(targetid, vx, vy, vz);
			}
			GetPlayerHealth(targetid, &hp);
			if (hp >= 11.0)
			{
				fixSetPlayerHealth(targetid, hp - 10.0);
				fixSendClientMessage(playerid, Color::COLOR_INFO2, L_aslap_executed);
			}
			else
			{
				fixSetPlayerHealth(targetid, hp + 10.0);
				fixSendClientMessage(playerid, Color::COLOR_INFO2, L_aslap_reversed);
			}
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_aslap_usage);
	return true;
}

ZCMDF(apaddhp, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({"/addhp"}), "pf")
{
	float hp;
	if (parser.Good() == 2)
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			GetPlayerHealth(targetid, &hp);
			fixSetPlayerHealth(targetid, hp + parser.Get<float>(1));
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_apaddhp_executed);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_apaddhp_usage);
	return true;
}

ZCMDF(apsethp, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({"/sethp"}), "pf")
{
	if (parser.Good() == 2)
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			fixSetPlayerHealth(targetid, parser.Get<float>(1));
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_apsethp_executed);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_apsethp_usage);
	return true;
}

ZCMDF(apaddarmor, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({ "/apaddarmour", "/addarmour", "/addarmor" }), "pf")
{
	float hp;
	if (parser.Good() == 2)
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			GetPlayerHealth(targetid, &hp);
			fixSetPlayerHealth(targetid, hp + parser.Get<float>(1));
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_apaddarmor_executed);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_apaddarmor_usage);
	return true;
}

ZCMDF(apsetarmor, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({"/apsetarmour", "/setarmour", "/setarmor"}), "pf")
{
	if (parser.Good() == 2)
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			fixSetPlayerHealth(targetid, parser.Get<float>(1));
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_apsetarmor_executed);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_apsetarmor_usage);
	return true;
}

ZCMDF(apvaddhp, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({"/addvehhp" }), "pf")
{
	float hp;
	if (parser.Good() == 2)
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			if (Player[targetid].CurrentVehicle)
			{
				GetVehicleHealth(Player[targetid].CurrentVehicle, &hp);
				safeSetVehicleHealth(Player[targetid].CurrentVehicle, hp + parser.Get<float>(1));
				fixSendClientMessage(playerid, Color::COLOR_INFO2, L_apvaddhp_executed);
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_targetplayer_not_in_vehicle);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_apvaddhp_usage);
	return true;
}

ZCMDF(apvsethp, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({"/setvehhp"}), "pf")
{
	if (parser.Good() == 2)
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		if (targetid != INVALID_PLAYER_ID)
		{
			if (Player[targetid].CurrentVehicle)
			{
				safeSetVehicleHealth(Player[targetid].CurrentVehicle, parser.Get<float>(1));
				fixSendClientMessage(playerid, Color::COLOR_INFO2, L_apvsethp_executed);
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_targetplayer_not_in_vehicle);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_apvsethp_usage);
	return true;
}

ZCMDF(atp, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({}), "ppd")
{
	float x, y, z, a;
	if (parser.Good() >= 2)
	{
		int from = parser.Get<ParsePlayer>(0).playerid;
		int to = parser.Get<ParsePlayer>(1).playerid;

		if (from != INVALID_PLAYER_ID && to != INVALID_PLAYER_ID && from != to)
		{
			if (CheckCommandAllowed(from, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_IN_VEHICLE_OR_ONFOOT, false) && CheckCommandAllowed(to, RESTRICTION_NOT_IN_A_GAME | RESTRICTION_IN_VEHICLE_OR_ONFOOT, false))
			{
				GetPlayerPos(to, &x, &y, &z);
				if (Player[to].CurrentVehicle)				
					GetVehicleZAngle(Player[to].CurrentVehicle, &a);
				else
					GetPlayerFacingAngle(to, &a);

				TeleportPlayer(from, x, y, z, a, parser.Good() == 3, GetPlayerInterior(to), Player[to].WorldID, "", 0, 0, 0.7, 0.7, false, true);
				
				//ugly but meh, how bad can it be? Tests don't seem to show any big performance loss
				if (playerid != to && playerid != from)
					fixSendClientMessageF(playerid, Color::COLOR_INFO, L_atp_move_info, false, false, Player[playerid].PlayerName.c_str(), Player[from].PlayerName.c_str(), Player[to].PlayerName.c_str());
				fixSendClientMessageF(to, Color::COLOR_INFO, L_atp_move_info, false, false, Player[playerid].PlayerName.c_str(), Player[from].PlayerName.c_str(), Player[to].PlayerName.c_str());
				fixSendClientMessageF(from, Color::COLOR_INFO, L_atp_move_info, false, false, Player[playerid].PlayerName.c_str(), Player[from].PlayerName.c_str(), Player[to].PlayerName.c_str());
				
				return true;
			}
			fixSendClientMessage(playerid, Color::COLOR_ERROR, L_atp_move_error);
			return true;
		}
		fixSendClientMessage(playerid, Color::COLOR_ERROR, L_atp_id_error);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_atp_usage);
	return true;
}

void aBanIp(int playerid, unsigned long long time, boost::asio::ip::address_v4 ipv4, std::string reason)
{
	unsigned long long ban_identifier = Functions::GetTime();

	if (!reason.size())
		reason.assign("-");

	unsigned long long banned_to = 0;
	if (!time)
		banned_to = 2114380800000;//ban do 01-01-2037
	else
		banned_to = ban_identifier + (time * 60000);

	WorkerRequest.push(new WorkerTransportData({ 0, "", new ban(ipv4.to_ulong(), banned_to, ban_identifier, "", Player[playerid].PlayerName, reason), DATABASE_REQUEST_OPERATION_SAVE_BAN, DATABASE_POINTER_TYPE_BAN, ipv4.to_ulong(), 0 }));

	gtLog(LOG_BAN, Functions::string_format("[%d][%s]--BID:[%I64X]--TIME:[%I64u]-->[IP][%s]:[%s]", playerid, Player[playerid].PlayerName.c_str(), ban_identifier, banned_to, ipv4.to_string().c_str(), reason.c_str()));
}

ZCMDF(banip, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({ "/ipban" }), "nUs")
{
	if (parser.Good() == 3)
	{
		boost::asio::ip::address ip = parser.Get<boost::asio::ip::address>(0);
		unsigned long long time = parser.Get<unsigned long long>(1);
		std::string reason(parser.Get<std::string>(2));

		if (ip.is_v4())
		{
			aBanIp(playerid, time, ip.to_v4(), reason);
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_banip_success);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_banip_usage);
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_ban_usage_2);
	return true;
}

ZCMDF(banhost, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({ "/hostban" }), "nUs")
{
	if (parser.Good() == 3)
	{
		boost::asio::ip::address ip = parser.Get<boost::asio::ip::address>(0);
		unsigned long long time = parser.Get<unsigned long long>(1);
		std::string reason(parser.Get<std::string>(2));

		if (ip.is_v4())
		{
			unsigned long long TimeNow = Functions::GetTime();
			unsigned long long banned_time = (time == 0) ? 2114380800000 : TimeNow + (time * 60000);

			HostBan::AddHostBan(ip.to_v4(), 0x0000, 0xFFFF, banned_time, TimeNow, "", Player[playerid].PlayerName, reason);
			fixSendClientMessage(playerid, Color::COLOR_INFO2, L_banhost_success);
			gtLog(LOG_BAN, Functions::string_format("[%d][%s]--BID:[%I64X]--TIME:[%I64u]-->[%d][%s][%s]:[%s]", playerid, Player[playerid].PlayerName.c_str(), TimeNow, banned_time, -1, "HOSTBAN", ip.to_string().c_str(), reason.c_str()));
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_banhost_usage_1);
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_banhost_usage_2);
	return true;
}

ZCMDF(unbanhost, PERMISSION_ADMIN, RESTRICTION_NONE, cmd_alias({ "/hostunban" }), "n")
{
	if (parser.Good())
	{
		boost::asio::ip::address ip = parser.Get<boost::asio::ip::address>(0);
		if (ip.is_v4())
		{
			if (HostBan::RemoveHostBan(ip.to_v4()))
				fixSendClientMessage(playerid, Color::COLOR_INFO2, L_unbanhost_success);
			else
				fixSendClientMessage(playerid, Color::COLOR_ERROR, L_unbanhost_failed);
			return true;
		}
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_unbanhost_usage);
	return true;
}

ZCMDF(emulate, PERMISSION_GAMER, RESTRICTION_NONE, cmd_alias({ "/hemulate" }), "ps")
{
	if (parser.Good())
	{
		int targetid = parser.Get<ParsePlayer>(0).playerid;
		std::string emulation(parser.Get<std::string>(1));
		if (targetid != INVALID_PLAYER_ID && targetid != playerid)
		{
			if (emulation[0] == '/')
				OnPlayerCommandText(targetid, emulation.c_str());
			else
				OnPlayerText(targetid, emulation.c_str());
			fixSendClientMessage(playerid, Color::COLOR_INFO3, L_emulate_executed);
			return true;
		}
		SendClientMessage(playerid, Color::COLOR_ERROR, L_invalid_playerid);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_emulate_usage);
	return true;
}

ZCMD(emulateall, PERMISSION_GAMER, RESTRICTION_NONE, cmd_alias({ "/hemulate-all", "/hemulateall", "/emulate-all" }))
{
	if (params.size())
	{
		AntiCmdSpam::LastCmd[playerid] = 0;
		if (params[0] == '/')
		{
			for (auto targetid : PlayersOnline)
				OnPlayerCommandText(targetid, params.c_str());
		}
		else
		{
			for (auto targetid : PlayersOnline)
				OnPlayerText(targetid, params.c_str());
		}
		fixSendClientMessage(playerid, Color::COLOR_INFO3, L_emulate_executed);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_emulateall_usage);
	return true;
}

bool invalid_td_chars(char c)
{
	return c < 0x20 || c > 0x7D || c == 0x25;
}

ZCMD(ann, PERMISSION_MODERATOR, RESTRICTION_NONE, cmd_alias({ "/announce" }))
{
	params.erase(std::remove_if(params.begin(), params.end(), invalid_td_chars), params.end());
	if (params.size() && params.size() < 57)
	{
		Message::AddAnnouncementMessage(Player[playerid].PlayerName + ": " + params);
		return true;
	}
	fixSendClientMessage(playerid, Color::COLOR_ERROR, L_announce_usage);
	return true;
}