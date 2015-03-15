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
#include "LangMain.hxx"

namespace Language
{
	namespace English
	{
		const std::map<language_string_ids, std::string> translation =
		{
			{ L_language_name, "English [English-en_US]" },
			{ L_empty_underscore,"_"},
			{ L_empty,			 " "},
			{ L_invalid_playerid, "Invalid ID" },
			{ L_eyecmd_player_enabled, "EyeCmd on player has been turned on for you" },
			{ L_eyecmd_player_disabled, "EyeCmd on player has been turned off for you" },
			{ L_eyecmd_player_watching_everyone, "You are watching everywhere. Would you like more?" },
			{ L_eyecmd_player_usage, "Usage: {41BECC}/eye <id/nick> {FFFFFF} (use again to turn it off)" },
			{ L_eyecmdall_enabled, "Monitoring of all commands has been turned on" },
			{ L_eyecmdall_disabled, "Monitoring of all commands has been turned off" },
			{ L_global_chat_history, "Global chat history" },
			{ L_no_history, "History is empty" },
			{ L_eyeip_enabled, "EyeIP for all new connections has been turned on" },
			{ L_eyeip_disabled, "EyeIP for all new connections has been turned off" },
			{ L_mm_achievement_newlevel, "You have reached a new level" },
			{ L_local_message_too_long, "Message is too long, It hasn't been send" },
			{ L_cheats_detected, "DISALLOWED MODIFICATIONS HAVE BEEN DETECTED (s0beit, CLEO or others)" },
			{ L_chat_history, "Chat history" },
			{ L_lastchat_usage, "Usage: {41BECC}/apmh <id/nick>" },
			{ L_PM_history, "PM history" },
			{ L_lastpm_usage, "Usage: {41BECC}/apph <id/nick>" },
			{ L_CMD_history, "Command history" },
			{ L_lastcmd_usage, "Usage: {41BECC}/apch <id/nick>" },
			{ L_spectate_usage, "/spec <id/nick>" },
			{ L_player_not_connected, "Player is not connected" },
			{ L_player_not_spawned, "Player is not spawned" },
			{ L_spectate_generic_error, "This player cannot be spectated." },
			{ L_specoff_need_spectate_first, "You aren't spectating anyone" },
			{ L_games_stagecheck_msg, "You've already joined the countdown for another game" },
			{ L_games_exit_info, "To exit or cancel, use {41BECC}/exit" },
			{ L_games_exited, "You've left the mini-game" },
			{ L_games_all_list_stub, "{FFFFFF}Available mini-games:\n\n{FFFFFF}%s{FFFFFF}\n\nRemember: to exit any mini-game use {41BECC}/exit{FFFFFF}\nMini-games which have a red star next to them are only for eXe24+ users ({41BECC}/plus{FFFFFF})" },
			{ L_games_help, "Help - games" },
			{ L_games_ArenaGranaty_name, "Grenade Arena" },
			{ L_games_ArenaMinigun_name, "Minigun Arena" },
			{ L_games_ArenaOneDe_name, "OneDe Arena" },
			{ L_games_ArenaOneShot_name, "OneShot Arena" },
			{ L_games_ArenaPaintBall_name, "PaintBall Arena" },
			{ L_games_ArenaRPG_name, "RPG Arena" },
			{ L_games_ArenaSawnOff_name, "SawnOff Arena" },
			{ L_games_ArenaSniper_name, "Sniper Arena" },
			{ L_games_ArenaTankBattle_name, "TankWarks Arena" },
			{ L_games_ZabawaBagno_name, "Bagno Game" },
			{ L_games_ZabawaDerby_name, "Derby Game" },
			{ L_games_ArenaFlyingTank_name, "Flying Tanks Arena" },
			{ L_games_ZabawaHay_name, "Hay Game" },
			{ L_games_ZabawaTDM_name, "Team DeathMatch Game" },
			{ L_games_ArenaWigilia_name, "Eve" },
			{ L_password_change, "Password change" },
			{ L_setpass_usage, "Usage: {41BECC}/setpass <nick> <password>{FFFFFF} to change player password, user may not be connected to server" },
			{ L_player_is_online, "Player is online!" },
			{ L_unbanip_usage, "Usage: {41BECC}/unbanip <ip> (full){FFFFFF} enter 'full' for end if you want to unban account, which was banned with this IP" },
			{ L_unban_usage, "Usage: {41BECC}/unban <nick> (full){FFFFFF}  enter 'full' for end if you want to unban IP, which was banned with this account" },
			{ L_PAP_LackOfPermissions, "(No permission)" },
			{ L_PAP_DoesNotExist, "(Not exist)" },
			{ L_Staff_CurrentlyOnline, "Server staff online: \n\n" },
			{ L_Staff_CurrentlyNone, "\tNone" },
			{ L_Staff_DisplayTitle, "Staff Online" },
			{ L_report_reason_TooShortOrLong, "Reason have to be filled but shorter than 64 chars" },
			{ L_report_send, "A report has been sent, pleas wait until the server staff contacts you." },
			{ L_report_usage, "Usage: {41BECC}/r <id/nick> <reason>{FFFFFF} to report a player" },
			{ L_promote_info_msg_1, "{A9C4E4}Information for setting permissions for member %s by %s:\n" },
			{ L_promote_add_error, "\n\tThe following permissions failed to be added:\n" },
			{ L_promote_add_success, "\n\tThe following permissions have been added succesfully:\n" },
			{ L_word_ranks, "Ranks" },
			{ L_promote_noaccess, "You don't have permission. This ID probably has more permissions." },
			{ L_promote_params_error, "Params cannot be separated" },
			{ L_promote_params_invalid, "You haven't served good params" },
			{ L_promote_usage, "\
{A9C4E4}Available ranks:\n\
\t• NONE - Take away all ranks\n\
\t• USER - Give player rights of normal player\n\
\t• VIP - Give player rights of VIP - a\n\
\t• SPONSOR - Give player rights of Sponsor\n\
\t• MODERATOR - Give player rights of Moderator\n\
\t• ADMIN - Give player rights of Admin\n\
\t• VICEHEAD - Give player rights of Vice Head Admin\n\
\t• HEADADMIN - Give player rights of Head Admin\n\
\t• WSPOLNIK - Give player rights of Partner\n\
\t• GAMER - Give player rights of GAMER\n\
\n\
Usage: {41BECC}/promote <id/nick> <RANK1 RANK2 RANK3>{A9C4E4} to give rank for user.\n\
Example: '{41BECC}/promote Janek SPONSOR VIP{41BECC}'\n\
"
			},
			{ L_promote_take_error, "\r\n\tThe following ranks could not be taken away:\r\n" },
			{ L_promote_take_success, "\r\n\tThe following ranks have been taken away:\r\n" },
			{ L_depromote_usage, "\
{A9C4E4}Available ranksi:\
\t• USER		- Takes away normal player rights\r\n	\
\t• VIP			- Takes away VIP rights\r\n	\
\t• SPONSOR		- Takes away Sponsor rights\r\n	\
\t• MODERATOR	- Takes away Moderator rights\r\n	\
\t• ADMIN		- Takes away Administrator rights\r\n	\
\t• VICEHEAD	- Takes away Vice-Admin rights\r\n	\
\t• HEADADMIN	- Takes away Head-Admin rights\r\n	\
\t• WSPOLNIK	- Takes away Co-Owner rights\r\n	\
\t• GAMER		- Takes away GAMER rights\r\n	\
\r\n\
Usage: {41BECC}/depromote <id/nick> <RANGA1 RANGA2 RANGA3>{A9C4E4} to take away the selected ranks.\r\n\
Example: '{FF0000}/depromote Janek SPONSOR VIP{41BECC}'\r\n\
"
			},
			{ L_kicked_player, "%s has kicked player %s, reason: %s" },
			{ L_kicked_word, "none" },
			{ L_kicked_dialoginfo, "Kicker: %s\nKick ID: [%I64X] (save it if you want to report incident on forum!)\nKicked person: %s\nReason: %s" },
			{ L_kicked_bye, "Kicked" },
			{ L_banned_player, "%s has banned player %s, reason: %s" },
			{ L_banned_dialogdisplay, "{FFFFFF}Banning person: %s\nBan ID: {FF0000}[%I64X]{FFFFFF} (save it if you want to report incident on forum!)\nBanned person: %s\nBanned IP: %s\nReason: %s\r\n\r\nForum: %s\r\n\r\nBan expires: %s\r\n\r\nBan released: %s" },
			{ L_banned_ipandaccount, "Banned IP address and account" },
			{ L_banned_ip, "Banned IP address" },
			{ L_banned_iphost, "Banned HOST IP" },
			{ L_banned_account, "Banned Account" },
			{ L_kick_usage, "Usage: {41BECC}/kick <id/nick> (Optional:<reason>){FFFFFF} to kick the selected player" },
			{ L_ban_usage_1, "Usage: {41BECC}/ban <id/nick> (<amount of minutes>{FFFFFF}(0 or empty bans permanently){41BECC} <reason>)" },
			{ L_ban_usage_2, "Tip| Hours: Minutes | 1: 60 | 3: 180 | ~6: 360 | ~12: 700 | ~24: 1400 | ~7 Dni: 10,000 | ~31 Dni: 45,000" },
			{ L_mm_join_game, "%s has entered the game" },
			{ L_mm_bag_startcd, "Bagno Game will have begin in a moment, use /bag to join" },
			{ L_mm_bag_staging, "You have subscribed to mini-game Bagno" },
			{ L_mm_bag_not_enough_players, "Not enough players to start Bagno" },
			{ L_mm_bag_announce_winner, "Bagno has been won by player %s" },
			{ L_mm_drb_announce_winner, "Derby has been won by player %s" },
			{ L_mm_drb_startcd, "Derby Game will begin in a moment, use /drb to join" },
			{ L_mm_drb_staging, "You have subscribed to mini-game Derby" },
			{ L_mm_drb_not_enough_players, "Not enough players to start derby" },
			{ L_mm_mute_player, "You have been unmuted" },
			{ L_mm_mute_player_announce, "%s has muted player %s, reason: %s" },
			{ L_mute_player_title, "Mute for chat" },
			{ L_mute_player_info, "{FFFFFF}You have been muted on chat\n\nAdmin muting you: %s\nMute ID: {FF0000}[%I64X]{FFFFFF} (save it if you want to report incident on forum!)\nMuted person: %s\nReason: %s\r\n\r\nForum: %s\r\n\r\nMute will expire in %I64u seconds" },
			{ L_mute_usage, "Usage: {41BECC}/mute <id/nick> <amount seconds> (<reason>){FFFFFF}, 0 seconds to unmute" },
			{ L_mm_tdm_team_red, "Red" },
			{ L_mm_tdm_team_blue, "Blue" },
			{ L_mm_tdm_announce_winner, "TDM was won by Team " },
			{ L_mm_tdm_startcd, "TDM Game will begin in a moment, use /TDM to join" },
			{ L_mm_tdm_staging, "You have subscribed to mini-game TDM" },
			{ L_mm_tdm_not_enough_players, "Not enough players to start TDM" },
			{ L_mm_hay_announce_winner, "Hay has been won by player %s" },
			{ L_mm_hay_startcd, "Hay Game will have begin in a moment, use /hay to join" },
			{ L_mm_hay_staging, "You have subscribed to mini-game Hay" },
			{ L_mm_hay_not_enough_players, "Not enough players to start Hay" },
			{ L_aexplode_executed, "Explode executed" },
			{ L_aexplode_usage, "Usage: {41BECC}/aexplode <id/nick>" },
			{ L_aslap_executed, "Slap executed" },
			{ L_aslap_reversed, "Instead removing 10 HP, it has been added because the player has not enough health to take away" },
			{ L_aslap_usage, "Usage: {41BECC}/aslap <id/nick>" },
			{ L_apaddhp_executed, "Given HP Added" },
			{ L_apaddhp_usage, "Usage: {41BECC}/apaddhp <id/nick> <amount>" },
			{ L_apsethp_executed, "Given HP Set" },
			{ L_apsethp_usage, "Usage: {41BECC}/apsethp <id/nick> <amount>" },
			{ L_apaddarmor_executed, "Given Armor Added" },
			{ L_apaddarmor_usage, "Usage: {41BECC}/apaddarmor <id/nick> <amount>" },
			{ L_apsetarmor_executed, "Given Armor Set" },
			{ L_apsetarmor_usage, "Usage: {41BECC}/apsetarmor <id/nick> <amount>" },
			{ L_targetplayer_not_in_vehicle, "Player is not in a vehicle" },
			{ L_apvaddhp_executed, "HP Has been added to the vehicle the player is in" },
			{ L_apvaddhp_usage, "Usage: {41BECC}/apvaddhp <id/nick> <amount>" },
			{ L_apvsethp_executed, "HP Has been set to the vehicle the player is in" },
			{ L_apvsethp_usage, "Usage: {41BECC}/apvsethp <id/nick> <amount>" },
			{ L_atp_move_info, "%s has teleported %s to %s" },
			{ L_atp_move_error, "One of players cannot be teleported" },
			{ L_atp_id_error, "Either id is wrong or both are the same" },
			{ L_atp_usage, "Usage: {41BECC}/atp <id/nick> <id/nick to player>" },
			{ L_banip_success, "IP has been banned!" },
			{ L_banip_usage, "Usage: {41BECC}/banip <ip> <time in minutes> <reason>" },
			{ L_banhost_success, "HOST has been banned!" },
			{ L_banhost_usage_1, "Usage: {41BECC}/banhost <ip> <time in minutes> <reason>" },
			{ L_banhost_usage_2, "Example: {41BECC}/banhost 2.3.4.5 0 cheat{FFFFFF} bans host 2.3.*.* permanently with reason \"cheat\"" },
			{ L_unbanhost_success, "HOST has been unbanned!" },
			{ L_unbanhost_failed, "Such host is not banned" },
			{ L_unbanhost_usage, "Usage: {41BECC}/unbanhost <ip>{FFFFFF} np. {41BECC}/unbanhost 2.3.4.5 {FFFFFF}unbans host 2.3.*.*" },
			{ L_emulate_executed, "Command Executed" },
			{ L_emulate_usage, "Usage: {41BECC}/emulate <id/nick> <text or /command>{FFFFFF}" },
			{ L_emulateall_usage, "Usage: {41BECC}/emulateall <id/nick> <text or /command>{FFFFFF}" },
			{ L_dbworker_userpwchange_success, "Password has been changed for user %s" },
			{ L_dbworker_userpwchange_failure, "[SETPASS] Can't find user %s" },
			{ L_dbworker_unban_usr_success_p, "[P] User %s has been unbanned" },
			{ L_dbworker_unban_usr_success_k, "[K] User %s has been unbanned" },
			{ L_dbworker_unban_usr_fail, "[UNBAN] Can't find user %s" },
			{ L_dbworker_unban_ip_success_p, "[P] IP %s has been unbanned" },
			{ L_dbworker_unban_ip_success_k, "[K] IP %s has been unbanned" },
			{ L_dbworker_unban_ip_fail, "[UNBAN] Can't find IP '%s' in the banlist" },
			{ L_v_usage, "Usage: {41BECC} /v [carName / ModelID]" },
			{ L_v_slot_error, "No vehicle saved in given slot" },
			{ L_v_slot_error_mafia, "This slot is only for Mafia's" },
			{ L_v_no_car_found_error, "Vehicle not found!" },
			{ L_v_cannot_spawn_error, "Sorry, you aren't allowed to spawn this vehicle" },
			{ L_vehicles_carlist_head, "Vehicles" },
			{ L_vehicles_carlist, "Planes\nHelicopters\nTwoWheeled\nCabriolets\nIndustrial\nLowriders\nOff Road\nPublic Service\nSaloons\nSport\nWagons\nBoats\nTrailers\nUnique\nRC" },
			{ L_vehicles_planes, "Planes" },
			{ L_vehicles_helis, "Helicopters" },
			{ L_vehicles_twowheeled, "Two wheeled" },
			{ L_vehicles_cabrio, "Cabriolets" },
			{ L_vehicles_industrial, "Industrial" },
			{ L_vehicles_lowriders, "Lowriders" },
			{ L_vehicles_public_service, "Public service" },
			{ L_vehicles_sport, "Sport" },
			{ L_vehicles_wagons, "Wagons" },
			{ L_vehicles_boats, "Boats" },
			{ L_vehicles_trailers, "Trailers" },
			{ L_vehicles_unique, "Unique" },
			{ L_vehicles_offroad, "Off Road" },
			{ L_vehicles_saloons, "Saloons" },
			{ L_vehicles_remotecontrol, "Remote Control" },
			{ L_vcolor_usage, "Usage: {41BECC}/vcolor <color1> (<color2>){FFFFFF} - Colors are from 0 to 256 | Current colors: [%d, %d]" },
			{ L_vpaint_usage, "Usage: {41BECC} /vpaint <id>{FFFFFF} - Possible paintjobs: 0, 1 or 2, for removal use id 3 | Current PaintJobID : %d" },
			{ L_tune_cannot_tune_error, "This car cannot be tuned" },
			{ L_countdown_counter, "Countdown has started! Enter /cd if you want to see it too! %d sec remaining" },
			{ L_countdown_removed, "You have removed yourself from the countdown" },
			{ L_countdown_joined, "Countdown joined, use /cd stop to leave the countdown" },
			{ L_email_send_error, "An unknown critical error has occurred while dispatching an e-mail to you, please try again later" },
			{ L_email_send_success, "E-Mail sent successfully" },
			{ L_timed_score_add, "You have been given additional experience, score and money for your playtime!" },
			{ L_unknown_command, "Unknown command: {FF0000}" },
			{ L_lotto_win, "You have guessed %d lotto numbers and received $%I64u" },
			{ L_lotto_lose, "You have guessed 0 numbers in lotto and received nothing" },
			{ L_lotto_winannounce, "[LOTTO] JackPot has been divided to all winning players, lucky numbers: %d, %d, %d, %d" },
			{ L_lotto_loseannounce, "[LOTTO] No one hit the jackpot, the new jackpot is $%I64u! The lucky numbers were: %d, %d, %d, %d" },
			{ L_lotto_number_error, "Some of your numbers are repeating or outside the 0 .. 63 range" },
			{ L_lotto_send_success, "Your numbers have been submitted, please wait a maiximum of 15 minutes for the results. If you leave participation will be canceled" },
			{ L_lotto_already_submitted, "Please wait for the results, you are already participating" },
			{ L_lotto_usage_1, "Usage: {41BECC}/lotto <number 1> (optional:  <number 2> ... <number max 8>){FFFFFF} to take part with the selected numbers in lotto!" },
			{ L_lotto_usage_2, "Each number you take part with costs $750, all numbest have to be between 0 and 63" },
			{ L_error_insufficient_funds, "You do not have enough cash" },
			{ L_invalid_funds_amount, "Invalid money amount (minimum $25,000)" },
			{ L_stats_totalgivenbounty, "All your given bounties: $%I64u" },
			{ L_stats_mytotalbounty, "Your total bounty: $%I64u" },
			{ L_stats_collectedbounty, "Your total collected bounties: $%I64u" },
			{ L_stats_bountykillers, "Amount of players killed which has a bounty on their head: %I64u" },
			{ L_stats_vehicleflip, "I like flippin'! %I64u times!" },
			{ L_stats_pmspammer, "PM Spammer! %I64u messages!" },
			{ L_stats_chatspammer, "Chat Spammer! %I64u messages!" },
			{ L_stats_lottowins, "Lotto has been won %I64u times" },
			{ L_stats_totallottowon, "Total lotto won: $%I64u" },
			{ L_stats_quizgood, "Right answers in /quiz: %I64u" },
			{ L_stats_quizbad, "Wrong answers in /quiz: %I64u" },
			{ L_stats_amountteleported, "Traveler! Amount of teleports: %I64u" },
			{ L_stats_rampscount, "Ramp-Man! Placed %I64u ramps!" },
			{ L_stats_vehiclespawncount, "Automatic Vehicle Spawner! Spawned %I64u vehicles!" },
			{ L_stats_gamejoiner, "Mingler! Joined %I64u mini-games!" },
			{ L_stats_mng_kills, "Amount of kills on mini-game /MNG: %I64u" },
			{ L_stats_mng_deaths, "Amount of deaths on mini-game /MNG: %I64u" },
			{ L_stats_hay_wins, "Amount of wins on mini-game /HAY: %I64u" },
			{ L_stats_hay_loses, "Amount of losses on mini-game /HAY: %I64u" },
			{ L_stats_ode_kills, "Amount of kills on mini-game /ODE: %I64u" },
			{ L_stats_ode_deaths, "Amount of deaths on mini-game /ODE: %I64u" },
			{ L_stats_snp_kills, "Amount of kills on mini-game /SNP: %I64u" },
			{ L_stats_snp_deaths, "Amount of deaths on mini-game /SNP: %I64u" },
			{ L_stats_sof_kills, "Amount of kills on mini-game /SOF: %I64u" },
			{ L_stats_sof_deaths, "Amount of deaths on mini-game /SOF: %I64u" },
			{ L_stats_mafispammer, "Mafia Chat Spammer! %I64u messages!" },
			{ L_stats_vipspammer, "VIP Chat Spammer! %I64u messages!" },
			{ L_stats_sponsorspammer, "Sponsor Chat Spammer! %I64u messages!" },
			{ L_stats_modspammer, "Moderator Chat Spammer! %I64u messages!" },
			{ L_stats_adminspam, "Admin Chat Spammer! %I64u messages!" },
			{ L_stats_cheater, "CHEATER!!@@@$@^!! %I64u reports!" },
			{ L_stats_helpme, "HALPPPP! %I64u times viewed help information!" },
			{ L_stats_privatevehicles, "Only Mine!\\o/! %I64u spawned private vehicles!" },
			{ L_stats_mafiavehicles, "MAFIA \3 4 Life \3 ! %I64u spawned Mafia vehicles!" },
			{ L_stats_amounthealed, "Don't WANT2DIEEE!! %I64u times healed!" },
			{ L_stats_mammothtank, "TANK!! %I64u times armored!" },
			{ L_stats_acsuspect, "SUSPECT!! :P AntiCheat V2.6 suspected you %I64u times!" },
			{ L_stats_roleplayer, "RolePlay bitchez! Used %I64u animations!" },
			{ L_stats_gotmafiascore, "Mafia \3 my \3 momma! $%I64u received from Mafia!" },
			{ L_stats_givenmafiascore, "Mafia \3 my \3 momma! %I64u respect received from Mafia!" },
			{ L_stats_biggestkillstreak, "MEGGAAAA KILLAAA! %I64u killstreak!" },
			{ L_stats_currentkillstreak, "Current killstreak: %I64u!" },
			{ L_stats_headshots, "HEADSHOT! %I64u headshots!" },
			{ L_stats_waitforxp, "GIMME EXPPPPP! %I64u experience from timed auto-experience!" },
			{ L_stats_agr_kills, "Amount of kills on mini-game /AGR: %I64u" },
			{ L_stats_agr_deaths, "Amount of deaths on mini-game /AGR: %I64u" },
			{ L_stats_arpg_kills, "Amount of kills on mini-game /RPG: %I64u" },
			{ L_stats_arpg_deaths, "Amount of deaths on mini-game /RPG: %I64u" },
			{ L_stats_osh_kills, "Amount of kills on mini-game /OSH: %I64u" },
			{ L_stats_osh_deaths, "Amount of deaths on mini-game /OSH: %I64u" },
			{ L_stats_apb_kills, "Amount of kills on mini-game /APB: %I64u" },
			{ L_stats_apb_deaths, "Amount of deaths on mini-game /APG: %I64u" },
			{ L_stats_ctf_kills, "Amount of kills on mini-game /CTF: %I64u" },
			{ L_stats_ctf_deaths, "Amount of deaths on mini-game /CTF: %I64u" },
			{ L_stats_ctf_pickedupflags, "Amount of picked up flags on /CTF: %I64u" },
			{ L_stats_ctf_capturedflags, "Amount of delivered flags on /CTF: %I64u" },
			{ L_stats_wns_kills, "Amount of kills on mini-game /STK: %I64u" },
			{ L_stats_wns_deaths, "Amount of deaths on mini-game /STK: %I64u" },
			{ L_stats_ww3_kills, "Amount of kills on mini-game /WW3: %I64u" },
			{ L_stats_ww3_deaths, "Amount of deaths on mini-game /WW3: %I64u" },
			{ L_stats_seeknhide_finds, "Amount of people found on /HNS: %I64u" },
			{ L_stats_seeknhide_besttime, "Longest hide time on /HNS: %I64u milliseconds" },
			{ L_stats_drb_won, "Amount of wins on mini-game /DRB: %I64u" },
			{ L_stats_drb_lost, "Amount of losses on mini-game /DRB: %I64u" },
			{ L_stats_maze_fastesttime, "Fastest maze time: %I64u milliseconds" },
			{ L_stats_tdm_kills, "Amount of kills on mini-game /TDM: %I64u" },
			{ L_stats_tdm_deaths, "Amount of deaths on mini-game /TDM: %I64u" },
			{ L_stats_tdm_wins, "Amount of wins on mini-game /TDM: %I64u" },
			{ L_stats_tdm_loses, "Amount of losses on mini-game /TDM: %I64u" },
			{ L_stats_bmxparkourft, "Fastest time on BMX parkour: %I64u milliseconds" },
			{ L_stats_nrgparkourft, "Fastest time on NRG parkour: %I64u milliseconds" },
			{ L_stats_parkourft, "Fastest parkour time: %I64u milliseconds" },
			{ L_stats_spm_kills, "Amount of kills on mini-game /MSP: %I64u" },
			{ L_stats_spm_deaths, "Amount of deaths on mini-game /MSP: %I64u" },
			{ L_stats_spm_wins, "Amount of wins on mini-game /MSP: %I64u" },
			{ L_stats_spm_loses, "Amount of losses on mini-game /MSP: %I64u" },
			{ L_stats_mma_wins, "Amount of wins on mini-game /MMA: %I64u" },
			{ L_stats_mma_loses, "Amount of losses on mini-game /MMA: %I64u" },
			{ L_stats_wjg_kills, "Amount of kills on mini-game /WJG: %I64u" },
			{ L_stats_wjg_deaths, "Amount of deaths on mini-game /WJG: %I64u" },
			{ L_stats_wjg_wins, "Amount of wins on mini-game /WJG: %I64u" },
			{ L_stats_wjg_loses, "Amount of losses on mini-game /WJG: %I64u" },
			{ L_stats_racekartft, "Fastest Kart track time: %I64u milliseconds" },
			{ L_stats_bag_kills, "Amount of kills on mini-game /BAG: %I64u" },
			{ L_stats_bag_deaths, "Amount of deaths on mini-game /BAG: %I64u" },
			{ L_stats_bag_wins, "Amount of wins on mini-game /BAG: %I64u" },
			{ L_stats_bag_loses, "Amount of losses on mini-game /BAG: %I64u" },
			{ L_stats_new_achievement, "Congratz! You have earned a new achievement, use /osg to view your achievements" },
			{ L_stats_achievement_info, "Achievements\nStatistics\nAdditional Statistics (1)\nAdditional Statistics (2)\nKills with weapons\nDeaths by weapons\nKillstreaks with weapons\nSame-Weapon kills\nSame-Weapon deaths\nWeapon Levels\nUse /osg <id/nick> to view information about other players" },
			{ L_stats_achievement_title, "Statistics & Achievements of player %s" },
			{ L_stats_txt_none, "No Achievements" },
			{ L_stats_amount_not_achieved, "\n\nAchievements to earn: %d" },
			{ L_stats_additional_none, "No additional statistics" },
			{ L_stats_additional_info_1, "Additional Statistics (1) for %s" },
			{ L_stats_additional_info_2, "Additional Statistics (2) for %s" },
			{ L_stats_killsfromweapons, "Kills with weapons for %s" },
			{ L_stats_killedbyweapons, "Deaths by weapons for %s" },
			{ L_stats_biggestkillstreaks, "Biggest killstreak from weapon for %s" },
			{ L_stats_killswithsameweapons, "Same-Weapon killd for %s" },
			{ L_stats_deathswithsameweapons, "Same-Weapon deaths for %s" },
			{ L_stats_weaponlevels, "Weapon Levels for %s" },
			{ L_anim_invalid_category, "Invalid Category" },
			{ L_anim_title, "Animations" },
			{ L_anim_display_info, "\
{FFFFFF}Info: The list is in the format {41BECC}[animation category name - amount of animations in category]{FFFFFF}\n\
To apply an animation use {41BECC}/anim <category name> <id>{FFFFFF},\n\
If the amount of animations in a category equals 1 the id parameter is optional. Ids' range from 0 to <amount of animations in category>\n\
To stop any animation use {41BECC}/animstop{FFFFFF}\n\
Available animations:\n"
			},
			{ L_setskin_usage, "Usage: {41BECC}/skin <ModelID>{FFFFFF}, Current ModelID: %d" },
			{ L_savecar_cannot_save, "Cannot save this vehicle" },
			{ L_mm_savecar_success, "Vehicle saved" },
			{ L_savecar_usage_1, "Usage: {41BECC}/vsave <slot>{FFFFFF} (slot from 0 to 9)" },
			{ L_savecar_usage_2, "You can respawn a saved vehicle with: {41BECC}/v <slot>" },
			{ L_ramp_succes, "Ramp type set!" },
			{ L_ramp_usage, "Usage: {41BECC}/ramp <id>{FFFFFF} - id from 1 to 6, 0 to turn off ramps" },
			{ L_tip_01, "~b~~h~~h~TIP: You can use /help to open the Help & Information Center" },
			{ L_tip_02, "~b~~h~~h~TIP: If you want to save your cool tuned vehicle use /vsave (slot)~n~~b~~h~~h~You can spawn it at a later time with /v (slot)" },
			{ L_tip_03, "~b~~h~~h~TIP: If you want to be VIP, check out /vip" },
			{ L_tip_04, "~b~~h~~h~TIP: Remember that you can check your achievements and stats any time~n~~b~~h~~h~Use /osg to do it" },
			{ L_tip_05, "~b~~h~~h~TIP: Are you planning on staying here? Set your /email to protect your account" },
			{ L_tip_06, "~b~~h~~h~TIP: Remember that you have an additional menu available at the key ~k~~CONVERSATION_YES~" },
			{ L_tip_07, "~b~~h~~h~TIP: Available teleports: /teles" },
			{ L_tip_08, "~b~~h~~h~TIP: You can create any vehicle you like! Use /v or /cars" },
			{ L_tip_09, "~b~~h~~h~TIP: You can spawn your Mafia vehicle with /v 10 and /v 11!" },
			{ L_tip_10, "~b~~h~~h~TIP: Want your own Mafia? Use /mafia.create" },
			{ L_tip_11, "~b~~h~~h~TIP: Remember to look at the details because~n~~b~~h~~h~Golden Coins are hidden pretty good!" },
			{ L_tip_12, "~b~~h~~h~TIP: Do you want someone to die?~n~~b~~h~~h~Use /hitman" },
			{ L_tip_13, "~b~~h~~h~TIP: Participation in mini-games ensures much fun and many experience points!" },
			{ L_tip_14, "~b~~h~~h~TIP: Need a countdown? Use /cd~n~~b~~h~~h~Others can join your /cd too!" },
			{ L_tip_15, "~b~~h~~h~TIP: Using /guns you will be able to upgrade your weapon kit~n~~b~~h~~h~Use /ammo to buy ammunition for your weapons" },
			{ L_tip_16, "~b~~h~~h~TIP: With /pmblock and /pmunblock you can decide~n~~b~~h~~h~who may contact you using /PM!" },
			{ L_tip_17, "~b~~h~~h~TIP: With /ramp (id) you can set yourself a nice ramp~n~~b~~h~~h~which will spawn any time you hit the fire key" },
			{ L_tip_18, "~b~~h~~h~TIP: Available mini games can be viewed using /games" },
			{ L_tip_19, "~b~~h~~h~TIP: Some race events allow fix/respawn/unflip with the keys:~n~~k~~CONVERSATION_YES~/~k~~VEHICLE_HORN~/~k~~TOGGLE_SUBMISSIONS~" },
			{ L_tip_20, "~b~~h~~h~TIP: Use ~k~~CONVERSATION_YES~ to bring quick access menu~n~~b~~h~~h~Use ~k~~GROUP_CONTROL_BWD~ and ~k~~CONVERSATION_NO~ to control it" },
			{ L_bounty_noheadsonline, "\tNone" },
			{ L_bounty_preinfo, "List of players with bounties on their heads:\n\n" },
			{ L_bounty_title, "The Living Dead" },
			{ L_givebounty_usage, "Usage: {41BECC}/hitman <id/nick> <amount of cash>" },
			{ L_givebounty_pricechange, "The bounty on player %s has risen to $%I64u!" },
			{ L_givebounty_headtagged, "$%I64u has been added to your bounty by %s from his own account, total bounty: $%I64u" },
			{ L_givebounty_sendtag, "Transfer in the height of $%I64u for the bounty of the selected player is succesful, total bounty: $%I64u" },
			{ L_SGMI_title, "Information about the Authors / GameMode and Contact" },
			{ L_SGMI_info, "\
{FF0000}Founder of server & programmer		:		{FFFFFF}Rafa³ 'Gamer_Z' Grasman\n\
{FF0000}Last Gamemode update		:		{FFFFFF}%s\n\
\n\
{FF0000}List of libraries used:		{FFFFFF}\n\
\n\
\t\t{65DB88}Library Name 				{D1BB2C}Author\n\
\n\
\t•	{65DB88}AntiCheat V2.6				{D1BB2C}Gamer_Z\n\
\t•	{65DB88}Boost						{D1BB2C}boost.org\n\
\t•	{65DB88}ODB						{D1BB2C}Code Synthesis\n\
\t•	{65DB88}MapAndreas Plugin				{D1BB2C}Kalcor\n\
\t•	{65DB88}MySQL						{D1BB2C}Oracle\n\
\t•	{65DB88}SampGDK					{D1BB2C}Zeex\n\
\t•	{65DB88}SampGDK Ext Mgr				{D1BB2C}Gamer_Z\n\
\t•	{65DB88}sqlite3						{D1BB2C}sqlite.org\n\
\t•	{65DB88}Streamer Plugin				{D1BB2C}Incognito\n\
\t•	{65DB88}Whirlpool Plugin				{D1BB2C}Y_Less\n\
\t•	{65DB88}ZeroCMD++					{D1BB2C}Gamer_Z\n\
\t•	{65DB88}ZeroDLG					{D1BB2C}Gamer_Z\n\
\n\
\t•	{65DB88}SA-MP						{D1BB2C}sa-mp.com\n\
\n\
{FF0000}Contact			:		{FFFFFF}exe24.info / gz0.nl\n\
{FF0000}Provider of Dedicated Server		:		{FFFFFF}OVH.NL\n\
{FF0000}Provider of forum & domain		:		{FFFFFF}STRATO.NL\n\
\n\
\n\
\n\
Special thanks to:\n\
IceCube, Oski20, C2A1,\n\
West, BlackPow3R, Arcade\n"
			},
			{ L_sentrygun_created, "SentryGun has been created" },
			{ L_sentrygun_usage, "Usage {41BECC}/sentryadd <range> <id/nick (owner, 0 for noone)>" },
			{ L_sentrygun_invalid_action, "Invalid action" },
			{ L_sentrygun_notfound, "ID sentry not found" },
			{ L_sentrygun_manage_usage, "Usage {41BECC}/sgm <turret ID> <action> <params>" },
			{ L_sentrygun_manage_nexuse, "Usage {41BECC}/sgm " },
			{ L_SendPM_ownID, "It is your ID!" },
			{ L_SendPM_pmAreOff, "You have turned off PMs, turn your PMs on to be able to send messages" },
			{ L_SendPM_receiverIsBlockedInfo, "The selected player currently cannot be contacted or either of you is blocking his/her PMs" },
			{ L_SendPM_tooLong, "Message too long" },
			{ L_pm_wasMutedByAdmin, "Your PM was muted by the administrator." },
			{ L_pm_turnOn, "You've turned PM on." },
			{ L_pm_turnOff, "You've turned PM off." },
			{ L_pm_usage, "Usage: {41BECC}/pm <id/nick> <text>{FFFFFF} to send a PM, {41BECC}/pm W{FFFFFF} to turn off PM, {41BECC}/pm Z{FFFFFF} to turn on PM" },
			{ L_pm_usage2, "           {41BECC}/reply <text>{FFFFFF} (or {41BECC}`<text>{FFFFFF}) to reply to last message" },
			{ L_reply_cannotFoundLastID, "Last conversationalist has not found." },
			{ L_reply_cannotBeEmpty, "Message can't be empty" },
			{ L_pmblock_playerBlockedInfo, "Player blocked!" },
			{ L_pmblock_usage, "Usage: {41BECC}/pmblock <id/nick>" },
			{ L_pmunblock_playerBlocked, "Player unblocked!" },
			{ L_pmunblock_usage, "Usage: {41BECC}/pmunblock <id/nick>" },
			{ L_mm_tdm_teamselector_red, "You are in the ~r~Red~w~ team"},
			{ L_mm_tdm_teamselector_blue, "You are in the ~b~Blue~w~ team" },

			{ L_help_timespan, "%02d days %02d h. %02d min. %02d sec." },
			{ L_help_stats_exe24mod_on, "{00FF00}Enabled" },
			{ L_help_stats_exe24mod_off, "{FF0000}Disabled" },
			{ L_help_stats_infostringadmin,
"{FFFFFF}Nick						{FFA321}%s\n\
\n\
{FFFFFF}Money						{FFA321}$%I64d\n\
{FFFFFF}Respect					{FFA321}%I64d\n\
{FFFFFF}Level					{FFA321}%d\n\
{FFFFFF}Progress to level %4d			{FFA321}%d%%\n\
{FFFFFF}Deaths					{FFA321}%I64u\n\
{FFFFFF}Suicides					{FFA321}%I64u\n\
{FFFFFF}Kills					{FFA321}%I64u\n\
{FFFFFF}Bounty				{FFA321}$%I64u\n\
{FFFFFF}Found Golden Coins			{FFA321}%3d/%3d\n\
{FFFFFF}Kicks by staff			{FFA321}%I64u\n\
{FFFFFF}Warns					{FFA321}%I64u\n\
{FFFFFF}Current session time				{FFA321}%s\n\
{FFFFFF}Total playtime			{FFA321}%s\n\
{FFFFFF}Skin model ID					{FFA321}%d\n\
{FFFFFF}Mafia						{FFA321}%s\n\
\n\
{FFFFFF}Premium Account Expires				{FFA321}%s\n\
{FFFFFF}Jail Expires					{FFA321}%s\n\
{FFFFFF}Registration Date				{FFA321}%s\n\
{FFFFFF}Last Ban expired				{FFA321}%s\n\
{FFFFFF}Last Ban reason				{FFA321}%s\n\
{FFFFFF}Last ban issuer				{FFA321}%s\n\
{FFFFFF}eXe24+ status				%s\n\
\n\
\n\
{FFFFFF}Ranks ({00FF00}Yes{FFFFFF}/{FF0000}No{FFFFFF}):\n\n\
\t\t\t\t\t\t{00FF00}Player\n\
\t\t\t\t\t\t{%06X}VIP\n\
\t\t\t\t\t\t{%06X}Sponsor\n\
\t\t\t\t\t\t{%06X}Moderator\n\
\t\t\t\t\t\t{%06X}Admin\n\
\t\t\t\t\t\t{%06X}Vice President\n\
\t\t\t\t\t\t{%06X}President\n\
\t\t\t\t\t\t{%06X}Server Co-Owner\n\
\t\t\t\t\t\t{%06X}Server Owner\n\
\n\
\n\
{FFFFFF}Check out the stats of other players who are online using {41BECC}/stats <id/nick>{FFFFFF}! :)"
			},
			{ L_help_stats_infostringuser,
"{FFFFFF}Nick						{FFA321}%s\n\
\n\
{FFFFFF}Money						{FFA321}$%I64d\n\
{FFFFFF}Respect					{FFA321}%I64d\n\
{FFFFFF}Level					{FFA321}%d\n\
{FFFFFF}Progress to level %4d			{FFA321}%d%%\n\
{FFFFFF}Deaths					{FFA321}%I64u\n\
{FFFFFF}Suicides					{FFA321}%I64u\n\
{FFFFFF}Kills					{FFA321}%I64u\n\
{FFFFFF}Bounty				{FFA321}$%I64u\n\
{FFFFFF}Found Golden Coins			{FFA321}%3d/%3d\n\
{FFFFFF}Kicks by staff			{FFA321}%I64u\n\
{FFFFFF}Current session time				{FFA321}%s\n\
{FFFFFF}Total playtime			{FFA321}%s\n\
{FFFFFF}Skin model ID					{FFA321}%d\n\
{FFFFFF}Mafia						{FFA321}%s\n\
\n\
{FFFFFF}Premium Account Expires				{FFA321}%s\n\
{FFFFFF}Registration Date				{FFA321}%s\n\
{FFFFFF}eXe24+ status				%s\n\
\n\
\n\
{FFFFFF}Ranks ({00FF00}Yes{FFFFFF}/{FF0000}No{FFFFFF}):\n\n\
\t\t\t\t\t\t{00FF00}Player\n\
\t\t\t\t\t\t{%06X}VIP\n\
\t\t\t\t\t\t{%06X}Sponsor\n\
\t\t\t\t\t\t{%06X}Staff\n\
\n\
\n\
{FFFFFF}Check out the stats of other players who are online using {41BECC}/stats <id/nick>{FFFFFF}! :)"
			},
			{ L_help_stats_title, "Statistics of player %s" },
			{ L_help_account_title, "Help - Account" },
			{ L_help_account_text,
"{41BECC}/register{FFFFFF} - create account\n\
{41BECC}/newpass{FFFFFF} - change password\n\
{41BECC}/email{FFFFFF} - set e-mail (only used for regaining access to lost accounts)\n"
			},
			{ L_help_monety_title, "Help - Golden Coins" },
			{ L_help_monety_text,
"{FFFFFF}Golden Coins are hidden artifacts at the least expected places on the server (only in the main world)\n\n\
When you find one, your task is to hit it with a bullet weapon (Sniper, Desert Eagle, etc.).\n\
If you succesfully hit - that coin is 'found', and you gain experience, cash and respect.\n\
finding Golden Coins is one of the requirements for becoming a VIP. More information is to be found using the command {41BECC}/vip{FFFFFF}\n\n\n\n\
P.S. Most Golden Coins are hidden really good! Keep looking for the details :)\n\n\
ProTip: Want ONE Golden Coin RIGHT NOW? Use {41BECC}/zmtp{FFFFFF} (if you haven't already found that one)\n\n\n\n\
Additional information: When all Golden Coins are found by a player, the total sum of earned cash equals ~$20,500,00 and total respect sum equals ~2,500" },
			{ L_help_plus_title, "Help - eXe24+" },
			{ L_help_plus_text,
"eXe24+ is a completely free addition to GTA:San Andreas Multiplayer made by the team at exe24.info.\n\
This module adds more functionality to the server and player (ONLY at mini-games with a red star) to achieve the next level of fun!\n\
\n\
The module adds the following functionality to control from the server:\n\
\n\
\t• Flying with any vehicle (both heli and plane mode)\n\
\t• Anti bike fall off\n\
\t• Break Dance\n\
\t• Real godmode\n\
\t• Prefect Handling\n\
\t• Removal of Aircraft speed and height limits\n\
\t• Speed Boost!\n\
\t• Super Brake\n\
\t• Driving Vehicles on Water\n\
\t• Mega Jump!\n\
\t• SpiderWheels!\n\
\t• And many more...\n\
\n\
\n\
\n\
eXe24+ required the SA-MP 0.3z-R1 client to be used(!)\n\
This client can be downloaded from the official SA:MP website: http://files.sa-mp.com/sa-mp-0.3z-R1-install.exe\n\
\n\
\n\
If all requirements are met, you can download the module and put it into the same folder as gta_sa.exe is.\n\
\n\
\n\
Remember to make backup copies of files that are being overwritten!\n\
\n\
You can download the module from http://plus.exe24.info/\n\
\n\
We hope you enjoy your stay at our server!"
			},
			{ L_help_vip_title, "Help - VIP" },
			{ L_help_vip_text,
"{EAF20A}VIP {FFFFFF}- this is a special rank that is {00FF00}automatically{FFFFFF} awarded by the server.\n\
Every player can earn this rank, no matte who or what they are, but the following requirements have to be met:\n\n\
\t30 or more hours playtime on the server (Current playtime: %I64u hours)\n\
\tLevel is equal or greater than 10 (Your level: %.0f)\n\
\tYou have set and verified a valid {41BECC}/email{FFFFFF} address (Status: %s)\n\
\tYour account is registered for more than 21 days (Your account is currently registered for %I64u days)\n\
\tYou have found at least 30 Golden Coins (You have currently found %d Golden Coins)\n\
\n\
\n\
When you meet all the requirements you need to reconnect to the server.\n\
The VIP rank will be automatically added when you log in the next time you meet all the requirements.\n\
\n\
\n\
VIP extras:\n\
\t• Lose only 10%% of your money (normal player: 33%%)\n\
\t• Always get a minimum of $10,000 & 3 respect for each kill (normal player: $7,500 & 2 respect)\n\
\t• Can rent houses for more days than a normal player\n\
\t• Your very own private VIP chat with other VIPs"
			},
			{ L_help_vip_mailverified, "Valid & Verified" },
			{ L_help_vip_mailnotverified, "Invalid & NOT Verified" },
			{ L_help_vehicle_title, "Help - Vehicles" },
			{ L_help_vehicle_text,
"{41BECC}/v{FFFFFF} - Create a vehicle of your choice\n\
{41BECC}/cars{FFFFFF} - List of vehicles\n\
{41BECC}/vcolor{FFFFFF} - Change the vehicle color\n\
{41BECC}/vpaint{FFFFFF} - Change the vehicle paintjob\n\
{41BECC}/vsave{FFFFFF} - Save your current vehicle\n\
{41BECC}/fix{FFFFFF} - Repair your vhicle\n\
{41BECC}/f{FFFFFF} - Flip your vehicle\n"
			},
			{ L_help_chat_title, "Help - Chat" },
			{ L_help_chat_text,
"{41BECC}|<text>{FFFFFF} - Proximity Chat\n\
{41BECC}\\<text>{FFFFFF} - Team Chat\n\
{41BECC}!<text>{FFFFFF} - Mafia Chat\n\
{41BECC}@<text>{FFFFFF} - Admin Chat\n\
{41BECC}#<text>{FFFFFF} - VIP Chat\n\
{41BECC}$<text>{FFFFFF} - Sponsor Chat\n\
{41BECC}&<text>{FFFFFF} - Moderator Chat\n\
{41BECC}`<text>{FFFFFF} - Reply to last PM\n"
			},
			{ L_help_staff_title, "Help - Staff" },
			{ L_help_staff_text,
"{41BECC}/report{FFFFFF} - Report a player violation to currently online staff\n\
{41BECC}/staff{FFFFFF} - Staff currently online\n"
			},
			{ L_help_global_title, "Help - Global" },
			{ L_help_global_text,
"{41BECC}/skin{FFFFFF} - change your skin\n\
{41BECC}/givecash{FFFFFF} - give your money to another player\n\
{41BECC}/giverespect{FFFFFF} - give your respect to another player\n\
{41BECC}/hitman{FFFFFF} - set a bounty for someone's head\n\
{41BECC}/hitman.list{FFFFFF} - lists all online players with a bounty on their head\n\
{41BECC}/anim{FFFFFF} - list of all available animations\n\
{41BECC}/clearchat{FFFFFF} - clear your chat\n\
{41BECC}/savepos{FFFFFF} - save your position\n\
{41BECC}/loadpos{FFFFFF} - load your saved position\n\
{41BECC}/color{FFFFFF} - set a permanent color\n\
{41BECC}/ramp{FFFFFF} - enable/set vehicle ramps\n\
{41BECC}/cd{FFFFFF} - start a countdown\n\
{41BECC}/tpto{FFFFFF} - teleport to another player\n\
{41BECC}/tphere{FFFFFF} - teleport another player to you\n\
{41BECC}/kill{FFFFFF} - suicide\n\
{41BECC}/respawn{FFFFFF} - respawn\n\
{41BECC}/bank{FFFFFF} - transfer money to/from your bank account\n\
{41BECC}/lotto{FFFFFF} - participate in lottery\n\
{41BECC}/osg{FFFFFF} -  View the achievements of yourself or any other player\n\
{41BECC}/stats{FFFFFF} - View the statistics of yourself or any other player\n\
{41BECC}/ghelp2{FFFFFF} -  Next help page\n"
			},
			{ L_help_global2_title, "Help - Global" },
			{ L_help_global2_text,
"{41BECC}/all{FFFFFF} - /100hp + /armor + /fix\n\
{41BECC}/100hp{FFFFFF} - heal yourself\n\
{41BECC}/armor{FFFFFF} - give yourself armor\n\
{41BECC}/ck{FFFFFF} - avoid carkill when below a car\n\
{41BECC}/interior{FFFFFF} - view your current interior\n\
{41BECC}/virtualworld{FFFFFF} - view your current virtual world\n\
{41BECC}/animid{FFFFFF} - view your current animation id\n\
{41BECC}/language{FFFFFF} - change your display language\n\
"
			},
			{ L_help_virtualworld, "Current Virtual World: %d" },
			{ L_help_animationid, "Current AnimID: %d" },
			{ L_help_acmd_title, "Help - Staff Commands" },
			{ L_help_acmd_text,
"{41BECC}/spec{FFFFFF} - spectate a player\n\
{41BECC}/specoff{FFFFFF} - turn spectate off\n\
{41BECC}/agiverespect{FFFFFF} - give respect to a player\n\
{41BECC}/agivecash{FFFFFF} - give cash to a player\n\
{41BECC}/agiveexp{FFFFFF} - give experience to a player\n\
{41BECC}/setpass{FFFFFF} -  set a new password for a account\n\
{41BECC}/promote{FFFFFF} - give player ranks\n\
{41BECC}/depromote{FFFFFF} -  take away player ranks\n\
{41BECC}/eye{FFFFFF} -  enable command spy on player\n\
{41BECC}/eyecmdall{FFFFFF} -  enable command spy on everyone\n\
{41BECC}/agmh{FFFFFF} -  view last 32 global chat messages\n\
{41BECC}/apmh{FFFFFF} -  view last 24 chat messages submitted by a player\n\
{41BECC}/apph{FFFFFF} -  view last 24 private messages of a player\n\
{41BECC}/apch{FFFFFF} -  view last 24 entered commands of a player\n\
{41BECC}/players{FFFFFF} -  list of players with ips'\n\
{41BECC}/kick{FFFFFF} -  kick a player\n\
{41BECC}/ban{FFFFFF} -  ban a player\n\
{41BECC}/unban{FFFFFF} -  unban a player\n\
{41BECC}/unbanip{FFFFFF} -  unban a ip\n\
{41BECC}/a{FFFFFF} -  write on the admin chat\n\
{41BECC}/m{FFFFFF} -  write on the moderator chat\n\
{41BECC}/aclch{FFFFFF} -  clear the global chat\n\
{41BECC}/eyeip{FFFFFF} -  view ip's of player in OnConnect/OnDisconnect\n\
{41BECC}/acmd2{FFFFFF} -  next page with Staff commands\n\
"
			},
			{ L_help_acmd2_title, "Help - Staff Commands 2" },
			{ L_help_acmd2_text,
"{41BECC}/aexplode{FFFFFF} - explode a player\n\
{41BECC}/aslap{FFFFFF} - remove 10 HP from a player and set the players Z velocity to jump\n\
{41BECC}/apaddhp{FFFFFF} - add/remove HP for a player\n\
{41BECC}/apsethp{FFFFFF} - set the HP of a player\n\
{41BECC}/apaddarmor{FFFFFF} - add/remove armor for a player\n\
{41BECC}/apsetarmor{FFFFFF} - set the armor of a player\n\
{41BECC}/apvaddhp{FFFFFF} - add/remove HP for the vehicle the player is currently in\n\
{41BECC}/apvsethp{FFFFFF} - set the HP for the vehicle the player is currently in\n\
{41BECC}/atp{FFFFFF} - teleport one player to another\n\
{41BECC}/dl{FFFFFF} - view additional vehicle information\n\
{41BECC}/banip{FFFFFF} - ban a ip\n\
{41BECC}/banhost{FFFFFF} - ban a host based on ip\n\
{41BECC}/unbanhost{FFFFFF} - unban a host based on ip\n\
{41BECC}/sentryadd{FFFFFF} - Create a sentry gun\n\
{41BECC}/sgm{FFFFFF} - Sentry Gun Management (range,exp,delete,setpos,owner,setarea,setareaex,ownermafia)\n\
{41BECC}/emulate{FFFFFF} - emulate chat text or a command for a player\n\
{41BECC}/emulate-all{FFFFFF} - emulate chat text or a command for all players\n\
{41BECC}/ips{FFFFFF} - view list of IPs that have more than one players\n\
{41BECC}/ann{FFFFFF} - announce a global message\n\
"
			},
			{ L_help_mafia_title, "Help - Mafia" },
			{ L_help_mafia_text,
"{41BECC}/mafia.create{FFFFFF} - Create your own mafia\n\
{41BECC}/mafia.q{FFFFFF} - Leave your current mafia\n\
{41BECC}/mafia.delete{FFFFFF} - Delete your current mafia\n\
{41BECC}/mafia.v{FFFFFF} - Save your current vehicle as the mafia vehicle\n\
{41BECC}/mafia.i{FFFFFF} - Invite a player to your mafia\n\
{41BECC}/mafia.j{FFFFFF} - Accept a mafia invite\n\
{41BECC}/mafia.gc{FFFFFF} - Give a member cash from the mafia account\n\
{41BECC}/mafia.gr{FFFFFF} - Give a member respect from the mafia account\n\
{41BECC}/mafia.dc{FFFFFF} - Donate your money to your mafia\n\
{41BECC}/mafia.dr{FFFFFF} - Donate your score to your mafia\n\
{41BECC}/mafia.c{FFFFFF} - Change your mafia color\n\
{41BECC}/mafia.k{FFFFFF} - Kick a player from your mafia\n\
{41BECC}/mafia.s{FFFFFF} - Set the rank of a member in your mafia\n\
{41BECC}/mafia.l{FFFFFF} - List all members of a given mafia\n\
{41BECC}/mafia.p{FFFFFF} - Set mafia command permissions\n\
{41BECC}/mafia.skin{FFFFFF} - Set the mafia skin\n\
{41BECC}/mafia.spawn{FFFFFF} - Set the mafia spawn\n\
{41BECC}/mafia.spawn.on{FFFFFF} - Enable spawning at the mafia spawn for yourself\n\
{41BECC}/mafia.spawn.off{FFFFFF} - Disable spawning at the mafia spawn for yourself\n\
{41BECC}/mafia.skin.on{FFFFFF} - Use the mafia skin\n\
{41BECC}/mafia.skin.off{FFFFFF} - Use your own skin\n\
{41BECC}/mafias{FFFFFF} - List of all mafias currently online\n"
			},
			{ L_help_pm_title, "Help - Private Messages" },
			{ L_help_pm_text,
"{41BECC}/pm{FFFFFF} - Send a private message to another player and/or disable/enable your PMs\n\
{41BECC}/pmblock{FFFFFF} - Block a player so he/she can't PM you\n\
{41BECC}/pmunblock{FFFFFF} - Unblock a player so he/she can PM you\n\
{41BECC}/reply{FFFFFF} - Reply to last PM\n"
			},
			{ L_help_teleport_title, "Teleport List" },
			{ L_help_serverinfo_title, "Server Description" },
			{ L_help_serverinfo_text,
"\
{FFFFFF}Gamer_Z eXtreme Party 24/7 - in short {41BECC}eXe24{FFFFFF}\n\
\n\
The server has been launched in October 2014, and is a continuation of the older brother - Polish eXtreme Party from 2009 which died in 2011\n\
\n\
The type gameplay on this server is {E88215}Modern DeathMatch{FFFFFF}. \n\
This means that you may kill anyone and everyone in any way you like (without voilating the /rules).\n\
\n\
Key points of our server:\n\
\n\
\t* We allow spawning vehicles like Rhino(Tank), Hunter(Apache) or Hydra(F16) (no shit, no hidden stuff, just {41BECC}/v rhino{FFFFFF} and kill everyone!)\n\
\t* DB, CK, HK, etc is {00FF00}allowed{FFFFFF}\n\
\t* Gameplay the way YOU want it!\n\
\n\
Our priority is a fun and friendly atmosphere during the game.\n\
That's why we chose not to offer any paid services,\n\
everything on the server is free and always will be (We promise)! We are AGAINST pay2play or pay2win!\n\
We go for quality!\n\
\n\
Not only does our server have DM-stuff, we also have a few extras like:\n\
\n\
\t* Race Events\n\
\t* Mini-games\n\
\t* Achievements\n\
\t* Challenges\n\
\t* And many more.. ( {41BECC}/help {00FF00}:){FFFFFF} )\n\
\n\
Not only do we offer our fun to individuals;\n\
We offer team based gameplay at various stages, our common team based gameplay relies on permanent Mafias.\n\
Mafias are comparable to gangs - but stronger and more eXtreme!\n\
Mafias are allowed to have up to 128 members.\n\
\n\
Our gamemode is created from the ground up using only what's best.\n\
Created with a optimal DeathMatch gameplay in mind - for both the server and players,\n\
only we can guarantee absolutely zero lags for the best killing and fun experience ever! \n\
The GameMode runs on a separate dedicated machine which is on a secure OVH network, this will guarantee stability and availability;\n\
With the code base 100 percent in C++ the greatest performance will be guaranteed;\n\
Our server runs with a stable one thousand seven hundred ticks per second [1700!] (Note that the average sa-mp server runs at 200 ticks per second)\n\
\n\
Everything on this server is taken care of - even the smallest details.\n\
\n\
Welcome to the eXtreme champions of DeathMatch - because we know how to create real fun!\n\
\n\
The whole eXe24 team wishes all players much fun!\n\
See you at our server :)\n\
"
			},
			{ L_help_rules_title, "Server Rules" },
			{ L_help_rules_text,
"\
By playing on the server you agree to comply to the server rules.\n\
\n\
The following outlined points are strictly prohibited:\n\
\n\
§1. Using any programs/addons/modules/scripts/modifications that give you an advantage over any other player (including CLEO!); (ban)\n\
§2. Using bugs related to GTA San Andreas; (kick or ban)\n\
§3. Insulting other members; (mute, kick, ban)\n\
§4. Escaping a fight with ALT+TAB, ESC or other means that pause the game; (kick or ban)\n\
§5. Impersonating other members; (ban)\n\
§6. Using abusive/vulgar/advertising nicknames/tags/mafia names/descriptions/etc. ; (kick, ban / permanent mafia removal)\n\
§7. Tryig to hack/guess/bruteforce the passwords of other members/RCON; (firewall ban, ban appeal is >IMPOSSIBLE<)\n\
§8. Crashing the server; (firewall ban, ban appeal is >IMPOSSIBLE<)\n\
§9. Advertising any SA-MP related services/servers by any means; (ban)\n\
§10. Distribution of information that would allow a individual or group break point §1; (ban)\n\
§11. Cooperation with a cheater by any means; (ban)\n\
§12. Obstruction of the game for other players by any means (including but not limited to encouraging writing /q /kill /exit etc.); (kick or ban)\n\
§13. Spamming / Spam; (mute, kick or ban)\n\
§14. Letting yourself get purposely killed so the other member can gain experience/score and/or cash; (ban for both members)\n\
§15. Writing false reports; (command mute, kick or ban)\n\
§16. Preventing/obstructing carrying out of events which staff members have announced in advance; (kick)\n\
$17. Going AFK on mini-games (kick);\n\
§18. Obstructing ongoing live-tests by staff done on the server; (kick)\n\
§19. Extracting account details of other members; (ban) \n\
"
			},
			{ L_help_house_title, "Help - Rentable houses" },
			{ L_help_house_text,
"{FFFFFF}The server has many available houses to all players.\n\
Each player is allowed to rent one house and manage it as he/she wishes.\n\
\n\
To rent a house, just get close to a door of any possession and use {41BECC}/house{FFFFFF}\n\
To teleport to your own rented house use {41BECC}/house tp"
			},
			{ L_help_center_title, "Help & Information Center (HIC)" },
			{ L_help_center_text,
"{41BECC}/vhelp{FFFFFF} - Vehicle help\n\
{41BECC}/pmhelp{FFFFFF} - PM help\n\
{41BECC}/khelp{FFFFFF} - Account command (register, change password, etc.)\n\
{41BECC}/tphelp{FFFFFF} - Teleport list\n\
{41BECC}/chhelp{FFFFFF} - Commands to interact with the server staff\n\
{41BECC}/ghelp{FFFFFF} - Global commands\n\
{41BECC}/mhelp{FFFFFF} - Mafia system help\n\
{41BECC}/zhelp{FFFFFF} - Mini-Games help\n\
{41BECC}/vip{FFFFFF} - Help about being a VIP (and how to become one)\n\
{41BECC}/zmhelp{FFFFFF} - Help about Golden Coins\n\
{41BECC}/chathelp{FFFFFF} - Help with the chat\n\
{41BECC}/domhelp{FFFFFF} - Help with rentable houses\n\
{41BECC}/serverinfo{FFFFFF} - Information about the server\n\
{41BECC}/lvcr{FFFFFF} - 'ending' credits\n\
{41BECC}/plus{FFFFFF} - information about eXe24+\n\
{41BECC}/rules{FFFFFF} - Server Rules\n\
Information about the Authors / GameMode, Contact\n"
			},
			{ L_help_center_admincmd, "{41BECC}/acmd{FFFFFF} - Staff Commands" },

			{ L_mafia_rank_worker, "Worker" },
			{ L_mafia_rank_vicepresident, "Vice President" },
			{ L_mafia_rank_president, "President" },
			{ L_mafia_rank_owner, "Owner" },
			{ L_dbworker_mafia_creationerror, "The chosen name for the mafia is invalid or taken, please choose another name" },
			{ L_mafia_create_invalidname, "The chosen name contains invalid characters" },
			{ L_mafia_create_alreadyexists, "A mafia with the chosen name already exists" },
			{ L_mafia_create_usage_1, "Usage: {41BECC}/mafia.create <name>" },
			{ L_mafia_create_usage_2, "To be able to create your own Mafia your leven needs to be at least %.0f and costs %d respect and $%d" },
			{ L_mafia_quit_ownererror, "You need to give any member the owner status of your mafia before leaving it." },
			{ L_mafia_vsave_error, "Cannot save this vehicle" },
			{ L_mm_mafia_vsave_success, "Vehicle saved" },
			{ L_mafia_vsave_usage_1, "Usage: {41BECC}/mvsave <slot>{FFFFFF} (slot 0 or 1)" },
			{ L_mafia_vsave_usage_2, "You can spawn the mafia vehicle with: {41BECC}/v 10 or /v 11" },
			{ L_mafia_invite_sent, "You have received an invite from Mafia '%s', use {41BECC}/mafia.join %s{FFFFFF} to join" },
			{ L_mafia_invite_success, "Invitation sent!" },
			{ L_mafia_invite_alreadyinvited, "The player has already an invitation" },
			{ L_mafia_invite_alreadyinmafia, "The player is already in a different Mafia" },
			{ L_mafia_infive_usage, "Usage: {41BECC}/mafia.i <id/nick>, player needs to be online" },
			{ L_mm_mafia_join_success, "%s joined the Mafia %s" },
			{ L_mafia_join_error, "You cannot join this Mafia because it's offline" },
			{ L_mafia_join_usage, "Usage: {41BECC}/mafia.j <mafia name>, you can only join Mafias which invited you" },
			{ L_mafia_givecash_success_1, "Transfer of $%I64u is succesfull, balance: $%I64d" },
			{ L_mafia_givecash_success_2, "$%I64u has been given to you by %s from the Mafia account, balance: $%I64d" },
			{ L_mafia_givecash_success_3, "$%I64u has been given to you from the Mafia account, balance: $%I64u" },
			{ L_mafia_givecash_nocash, "The Mafia account has insufficient funds" },
			{ L_mafia_player_notinmafiaerror, "Player is not in your Mafia" },
			{ L_mafia_givecash_casherror, "Invalid money amount (minimum $25,000)" },
			{ L_mafia_givecash_usage, "Usage: {41BECC}/mafia.gc <id/nick> <amount of money>, balance: $%I64u" },
			{ L_mafia_givescore_success_1, "Transfer of %I64u respect is succesfull, respect balance: %I64d" },
			{ L_mafia_givescore_success_2, "%I64u respect has been given to you by %s from the Mafia account, respect balance: %I64d" },
			{ L_mafia_givescore_success_3, "%I64u respect has been given to you from the Mafia account, respect balance: %I64d" },
			{ L_mafia_givescore_noscore, "The Mafia account does not have enough respect" },
			{ L_mafia_givescore_scoreerror, "Invalid respect amount" },
			{ L_mafia_givescore_usage, "Usage: {41BECC}/mafia.gr <id/nick> <amount of respect>, respect balance: %I64d" },
			{ L_mafia_donatecash_success, "$%I64u has been added to the Mafia account, balance: $%I64u" },
			{ L_mafia_donatecash_error, "You do not have that much money" },
			{ L_mafia_donatecash_usage, "Usage: {41BECC}/mafia.dc <amount of money>" },
			{ L_mafia_loaded_success, "Information: Your Mafia '%s' has been loaded succesfully with ID: %d" },
			{ L_mafia_deleted, "You have been removed from the mafia" },
			{ L_mafia_kicked, "Mafia has been deleted" },
			{ L_mafia_permission_error, "Only %s or higher can use this command" },
			{ L_mafia_spawnoff, "Mafia spawn disabled" },
			{ L_mafia_spawnon, "Mafia spawn enabled" },
			{ L_mafia_skinoff, "Mafia skin disabled" },
			{ L_mafia_skinon, "Mafia skin enabled" },
			{ L_mafia_setspawn, "Mafia spawn has been set" },
			{ L_mafia_setskin, "Mafia skin has been set" },
			{ L_mafia_online_title, "Mafias which are online" },
			{ L_mafia_online_text, "Online Mafias:\n" },
			{ L_mafia_donatescore_success, "%I64u respect has been added to the Mafia account, respect balance: %I64d" },
			{ L_mafia_donatescore_error, "You do not have that much respect" },
			{ L_mafia_donatescore_usage, "Usage: {41BECC}/mafia.dr <amount of respect>" },
			{ L_mafia_color_setsuccess, "Color set!" },
			{ L_color_too_dark_error, "Color is too dark" },
			{ L_mafia_color_set_usage, "Usage: {b}/mafia.c <color>{/b}, Example: {b}/mafia.c 77FFAA{/b}, pick a color at http://colorpicker.com | Current: %06X" },
			{ L_mafia_kick_cannotquit, "If you want to leave the Mafia use {41BECC}/mafia.q" },
			{ L_mafia_playernotinmafia, "Such player does not exist in your Mafia" },
			{ L_mafia_kick_permissionerror, "You cannot kick a player with a higher rank" },
			{ L_mafia_kick_success, "Player has been kicked from the Mafia" },
			{ L_mafia_kick_usage, "Usage: {41BECC}/mafia.k <id/nick>" },
			{ L_mafia_status_ownererror, "You cannot change the last owner of the Mafia" },
			{ L_mafia_status_permissionerror_1, "You cannot give yourself a higher rank than you already have" },
			{ L_mafia_status_permissionerror_2, "You cannot give a higher rank than you have" },
			{ L_mafia_status_success_1, "Your rank has been changed to '%s' by '%s'" },
			{ L_mafia_status_success_2, "You have set the rank of player '%s' to '%s'" },
			{ L_mafia_status_usage, "Usage: {41BECC}/mafia.s <id/nick> <rank(0,1,2,3)>{FFFFFF}| 0 - Worker, 1 - Vice President, 2 - President, 3 - Owner" },
			{ L_mafia_permissions_success, "Permission set" },
			{ L_mafia_permissions_error, "Unknown permission" },
			{ L_mafia_permissions_usage,"\
{FFFFFF}Usage: {41BECC}/mafia.p <permission> <rank(0,1,2,3)>{FFFFFF}\n\
\n\
Available Permissions:\n\
\tINVITE (Inviting players)\n\
\tKICK (Kicking players)\n\
\tSAVECAR (Saving the mafia vehicle)\n\
\tSPAWNCAR (Spawning the mafia vehicle with /v)\n\
\tCMLVL (who can set ranks for other players)\n\
\tPERM (using this command)\n\
\tCOLOR (who can change the mafia color)\n\
\tDMONEY (who can donate cash)\n\
\tGMONEY (who can give cash)\n\
\tDRESPECT (who can donate respect)\n\
\tGRESPECT (who can give respect)\n\
\tDELETE (who can delete the mafia)\n\
\tSKIN (who can change the mafia skin)\n\
\tSPAWN (who can change the mafia spawn)\n\
\n\
Example: {41BECC}/mafia.p KICK 0{FFFFFF} - allows all members to kick any member they like, default rank for all permissions is '3'" 
			},
			{ L_dialog_title_info, "Info" },
			{ L_mafia_lista_name_error, "Such Mafia does not exist or is offline" },
			{ L_mafia_lista_info, "Respect Balance: %I64u\nMoney Balance: $%I64u\nMembers of Mafia '%s':\n" },
			{ L_mafia_lista_info_2, "\n\nYou can check out another Mafia using {41BECC}/mafia.l <mafia name>" },
			{ L_mafia_lista_title, "Mafia - Information" },

			{ L_antispam, "{FF0000}Do not spam and do not repeat yourself, {FFFFFF}You have been muted for an additional 1.0s" },
			{ L_chatmute, "{FF0000}You are muted, {FFFFFF}Mute expires in: %I64us" },
			{ L_cmdchatmute, "{FF0000}You are command-muted, {FFFFFF}CommandMute expires in: %I64us" },
			{ L_commandrestriction_game, "{FF0000}You cannot use this command at your current mini-game {FFFFFF}(if you want to leave the mini-game use {41BECC}/exit{FFFFFF})" },
			{ L_commandrestriction_onlyingame, "{FF0000}This command can only be used at certain mini-games" },
			{ L_commandrestriction_onlyonfoot, "This command can only be used when you are on foot" },
			{ L_commandrestriction_onlyasdriver, "This command can only be used when you are a vehicle driver" },
			{ L_commandrestriction_onlyinvehicle, "This command can only be used when you are a vehicle" },
			{ L_commandrestriction_onlyloggedin, "You need to be logged in to use this command" },
			{ L_commandrestriction_onlyinmafia, "This command can only be used when you are in a Mafia" },
			{ L_commandrestriction_notinmafia, "This command can only be used when you are not in a Mafia" },
			{ L_commandrestriction_onlyincar, "This command can only be used in a car" },
			{ L_commandrestriction_onlyinairplane,  "This command can only be used in an airplane" },
			{ L_commandrestriction_onlyinhelicopter, "This command can only be used in a helicopter" },
			{ L_commandrestriction_onlyregisteredanddataloaded, "You do not have an account or you need to wait for your account details to be loaded" },
			{ L_commandrestriction_afterfight, "You need to wait %I64u second after your last damage to be able to use this command" },
			{ L_commandrestriction_spawned, "You need to join the game before using this command" },
			{ L_commandrestriction_exe24plus_1, "You need to have eXe24+ installed to be able to use this command" },
			{ L_commandrestriction_exe24plus_2, "More information about eXe24+ can be found at {0088FF}http://plus.exe24.info/ {FFFFFF}or use {41BECC}/plus" },
			{ L_commandrestriction_vehorfoot, "You need to be onfoot or in a vehicle to use this command" },
			{ L_command_nopermission, "You do not have the required permission to use this command" },

			{ L_Yes, "Yes" },
			{ L_No, "No" },
			{ L_Cancel, "Cancel" },
			{ L_Login, "Login" },
			{ L_Confirm, "Confirm" },
			{ L_Help, "Help" },
			{ L_login_dialog_title, "Login" },
			{ L_login_dialog_text, "{FF0000}This account is registered and password protected.\r\n{FFFFFF}You need to login before continuing.\r\nEnter the account password, REACT-code or a one-time authentication code:" },
			{ L_registerq_title, "Question" },
			{ L_registerq_text, "{FFFFFF}This username does not have an account, do you want to create an account?" },
			{ L_register_text, "{FFFFFF}The minimum password length is five (5).\r\nEnter a password for your account:" },
			{ L_updatepass_title, "Choose a password"},
			{ L_loginhelp_askmail_title, "Verification" },
			{ L_loginhelp_askmail_text, "Enter the e-mail which is validated on this account:" },
			{ L_loginhelp_askmail_button_a, "Check" },
			{ L_loginhelp_askmail_button_b, "Exit" },
			{ L_loginhelp_antyspam, "To prevent spam, e-mails can be send once every 15 minutes." },
			{ L_loginhelp_checkmail, "Check your inbox, an e-mail with a one-time authentication code should arrive shortly" },
			{ L_login_noverifiedmail, "This account does not have a validated e-mail. No recovery options available. If you are unable to login to this account use F6 and /q" },
			{ L_login_welcomeback, "{FFFFFF}Welcome back.\r\nWe wish you a nice game. Tip: Remember to set and validate your /email (if you loose your password you will be able to recover this account)" },
			{ L_importantmessage, "You have a important notification!" },
			{ L_vip_upgrade, "You have been automatically advanced to VIP, thanks for playing with us!" },
			{ L_mm_vip_upgrade, "You have been automatically advanced to VIP, thanks for playing with us!" },
			{ L_information, "Information" },
			{ L_register_canceled, "{FFFFFF}You have chosen to not register an account.\r\nIf you change your mind you can use {00FF00}/register{FFFFFF} any time you want.\r\nWe wish you a nice game." },
			{ L_already_registered, "You already have an account" },
			{ L_register_success, "{FFFFFF}Registration is successful.\r\nYour password:\r\n\r\n%s\r\n\r\nWe wish you a nice game.\n\n\n\nTip: Remember to set and validate your /email (if you loose your password you will be able to recover this account)" },
			{ L_newpass_success_text, "You new password has been set: %s" },
			{ L_newpass_success_title, "New password set" },
			{ L_manageemail_text, "{%06X}Change E-Mail\n{%06X}Validate E-Mail\n{%06X}Send Validation E-Mail\n" },
			{ L_manageemail_title, "E-Mail management" },
			{ L_email_action_change_title, "Change E-Mail address" },
			{ L_email_action_change_text_a, "Invalid E-Mail address!\n\nEnter your new E-Mail:" },
			{ L_email_action_change_text_b, "You need to provide a new E-Mail address!\n\nEnter your new E-Mail:" },
			{ L_email_action_success_title, "New e-mail set" },
			{ L_email_action_success_text, "Your e-mail has been set: %s\n\nCheck your inbox for your unique REACT-code which will allow you to recover your account\n\nRemember to validate your /email! Accounts without a validated e-mail cannot be recovered" },
			{ L_email_change_newemail, "Enter your new E-Mail:" },
			{ L_email_change_verify_fail, "Invalid code!\n\nTo change your e-mail you need to provide a valid REACT-code (an additional message has been dispatched to the inbox registered with this account containing your REACT-code):" },
			{ L_email_verify_text, "Invalid code!\n\nExample REACT-code: A0A000A00A0A00A0AA000000000A0A00\n\nEnter your received REACT-code:" },
			{ L_email_verify_title, "E-Mail validation" },
			{ L_email_verify_success, "E-Mail validated successfully!" },
			{ L_email_verification_error, "Your E-Mail is already validated or you did not provide any E-Mail to this account" },
			{ L_email_activation_limit, "To prevent spam, the e-mail can be changed and/or activated once every 15 minutes." },
			{ L_email_change_verify_text_a, "Example REACT-code: A0A000A00A0A00A0AA000000000A0A00\n\nTo change your e-mail you need to provide a valid REACT-code (an additional message has been dispatched to the inbox registered with this account containing your REACT-code):" },
			{ L_email_change_verify_text_b, "Example REACT-code: A0A000A00A0A00A0AA000000000A0A00\n\nEnter your received REACT-code:" },

			{ L_mm_APTITT, "Player [%-3d]%s has accepter your Teleport request" },
			{ L_mm_RTTP_a, "Teleport request sent! It will be invalidated in 30 seconds" },
			{ L_mm_ITPTM_a, "Teleport invitation sent! It will be invalidated in 30 seconds" },
			{ L_mm_RTTP_b, "Player [%-3d]%s has requested to teleport to you - use /tphere %d to accept, request will be invalidated in 30 seconds" },
			{ L_mm_ITPTM_b, "Player [%-3d]%s invited you to teleport to him/her - use /tpto %d to accept - invitation will be invalidated in 30 seconds" },
			{ L_mm_invalid_playerid, "Invalid ID" },
			{ L_mm_player_in_minigame, "Player is in mini-game" },
			{ L_tpto_cannot_tp, "Teleport is not possible at the moment" },
			{ L_tpto_usage, "Usage: {41BECC}/tpto <id/nick>{FFFFFF} to teleport to a player (player must accept using {41BECC}/tphere %d{FFFFFF})" },
			{ L_mm_APTRTM, "Player [%-3d]%s accepted your teleport request" },
			{ L_tphere_usage, "Usage: {41BECC}/tphere <id/nick>{FFFFFF} to invite a player to teleport to you (the player has to accept using {41BECC}/tpto %d{FFFFFF})" },
			{ L_mm_cannot_tphere, "The player cannot be teleported at the moment" },

			{ L_pickupmessage_house, "Use {41BECC}/house{FFFFFF} to view information about this property" },
			{ L_house_nohouse, "You need to rent a house to use this command" },
			{ L_house_infostringbuilder_a, "{FFFFFF}This property {FF0000}is being rented{FFFFFF}, information:\n\t• Renting player: \n\t\t\t%s\n\n\t• Renting expires: %s" },
			{ L_house_infostringbuilder_locked, "\n\t• The doors to this house are locked" },
			{ L_house_infostringbuilder_b, "{FFFFFF}This property {00FF00}can be rented{FFFFFF}, information:\n" },
			{ L_house_infostringbuilder_c, "\n\n\t• Pricing for renting:\n" },
			{ L_house_infostringbuilder_d, "\
\n\nCertain groups of players have limits on how long they can rent a house.\n\
The following groups have the following renting limits (and need to refresh the renting each X days to keep the property):\n\n\
\t• Players:		5 days\n\
\t• VIP:			15 days\n\n\n\
Use {41BECC}/house k <amount of days>{FFFFFF} to extend the rent (if you are the owner) or rent this house\n\nAdditional commands:\n\
\t{41BECC}/house P{FFFFFF} - enter/exit house\n\
\t{41BECC}/house L{FFFFFF} - lock/unlock doors\n\
\t{41BECC}/house Z <id/nick>{FFFFFF} - invite/uninvite player to/from your house\n\
\t{41BECC}/house W <id/nick>{FFFFFF} - kick a player from your house\n\
\t{41BECC}/house Q {FFFFFF} - cancel renting (money and respect are {FF0000}NOT{FFFFFF} refunded)\n" 
			},
			{ L_house_infostringbuilder_e, "\
\t{41BECC}[HOUSE ID: %d]\n\
\t{41BECC}/house # <respect for renting 1 day>{FFFFFF} - Set the respect for renting 1 day\n\
\t{41BECC}/house $ <money for renting 1 day>{FFFFFF} - Set the money for renting one day\n\
\t{41BECC}/house :{FFFFFF} - Remove the current owner\n\
\t{41BECC}/house + <id/nick>{FFFFFF} - Set the player as the owner for 5 days\n\
\t{41BECC}/house > <amount of hours>{FFFFFF} - Set the renting expiry (0 == expires now, 1 in 1 H, etc.)\n\
\t{41BECC}/house * <type>{FFFFFF} - Set the new interior type (/houseadd)\n"
			},
			{ L_house_info_title, "House Information" },
			{ L_house_infostringbuilder_money, "\n\t\t(Money): 1 day: $%I64u, 7 days: $%I64u, 14 days: $%I64u, 21 days: $%I64u\n" },
			{ L_house_infostringbuilder_score, "\n\t\t(Respect): 1 day: R%I64u, 7 days: R%I64u, 14 days: R%I64u, 21 days: R%I64u\n" },
			{ L_house_enter_permissionerror, "You have no permission to enter this property" },
			{ L_house_rentlimit_reached, "You can only rent one house" },
			{ L_house_owneronly_action, "Only the owner can issue this command" },
			{ L_house_rent_mintime_error, "The minimum renting time is one day" },
			{ L_house_resources_error, "You do not have enough money and/or respect" },
			{ L_house_rent_success, "The renting period for this property has been extended" },
			{ L_house_rent_usage, "Usage: {41BECC}/house k <amount of days>" },
			{ L_house_door_open, "Doors opened" },
			{ L_house_door_close, "Doors closed" },
			{ L_house_kick_usage, "Usage: {41BECC}/house W <id/nick>" },
			{ L_house_kick_error, "Player is not in your house" },
			{ L_house_kick_success_1, "Player removed from your house" },
			{ L_house_kick_success_2, "The owner of the house has removed you from it" },
			{ L_house_range_error, "You need to be in range of a door to the property to issue this command" },
			{ L_house_action_executed, "Command executed" },
			{ L_house_invalid_interiorid, "Invalid interior type" },
			{ L_house_invalid_hours, "Invalid hours amount" },
			{ L_house_player_notonline, "Player has to be online" },
			{ L_house_player_supplyerror, "Invalid playerid/name" },
			{ L_house_uninvite_usage, "Usage: {41BECC}/house Z <id/nick>" },
			{ L_house_uninvite_success_1, "Permission to enter your house has been revoked for the selected player" },
			{ L_house_uninvite_success_2, "Permission to enter your house has been granted for the selected player" },
			{ L_house_score_error, "Invalid respect amount" },
			{ L_house_money_error, "Invalid money amount" },
			{ L_houseadd_usage_1, "Usage: {41BECC}/addhouse <type> <$/day> <R/day>" },
			{ L_houseadd_usage_2, "Types: 0 .. 9" },

			{ L_credits_scene_1, "{FF0000}Server founder & programmer\n{000000}Rafa³ 'Gamer_Z' Grasman\n{FF0000}Last GameMode update\n{000000}%s\n\n" },
			{ L_credits_scene_2, "\n{FF0000}Used Libraries{FFFFFF}\n\n\n{FF0000}AntiCheat V2.6; {000000}Gamer_Z\n{FF0000}Boost; {000000}boost.org\n{FF0000}ODB; {000000}Code Synthesis\n" },
			{ L_credits_scene_3, "{FF0000}MapAndreas Plugin; {000000}Kalcor\n{FF0000}MySQL; {000000}Oracle\n{FF0000}SampGDK; {000000}Zeex\n{FF0000}SampGDK Ext Mgr; {000000}Gamer_Z\n{FF0000}sqlite3; {000000}sqlite.org\n{FF0000}Streamer Plugin; {000000}Incognito\n{FF0000}Whirlpool Plugin; {000000}Y_Less\n" },
			{ L_credits_scene_4, "{FF0000}ZeroCMD++; {000000}Gamer_Z\n{FF0000}ZeroDLG; {000000}Gamer_Z\n\n\n{FF0000}SA-MP; {000000}sa-mp.com\n\n\n" },
			{ L_credits_scene_5, "{FF0000}Contact\n{000000}exe24.info / gz0.nl\n\n{FF0000}Dedicated Server Provider\n{000000}OVH.NL\n\n{FF0000}Forum & Domain Provider\n" },
			{ L_credits_scene_6, "{000000}STRATO.NL\n\n\n\n{FF0000}Special thanks to" },
			{ L_credits_scene_7, "{000000}IceCube, Oski20, C2A1\n{000000}West, BlackPow3R, Arcade\n" },

			{ L_weapons_dialog_text_a, "You can buy permanent weapons for {FF0000}R{FFFFFF}espect and {00FF00}$m{FFFFFF}oney{FFFFFF}\n" },
			{ L_weapons_dialog_text_dequip, "<{3FC8F2}Dequip (%d) %s{FFFFFF}" },
			{ L_weapons_dialog_text_equip, ">{73E067}Equip (%d) %s{FFFFFF}" },
			{ L_weapons_dialog_text_ammo, " | %d ammo: {00FF00}$%I64u{FFFFFF}" },
			{ L_weapons_dialog_text_magazinestatus, " | Clip State: %d" },
			{ L_weapons_dialog_text_buy, "${F25C5C}Buy (%d) %s {FFFFFF} for {FF0000}R%I64u{FFFFFF} " },
			{ L_weapons_dialog_text_level, " | Required level: %.0f" },
			{ L_weapons_dialog_text_b, "\nUse {FF0000}/ammo <weaponid> <amount>{FFFFFF} to buy ammo" },
			{ L_weapons_dialog_title, "Permanent Weapons" },

			{ L_weapons_dialog_text_weaponowned, "You already own this weapon" },
			{ L_weapons_dialog_text_lowlevel, "You do not have the required level" },
			{ L_weapons_dialog_text_lowscore, "You do not have enough respect" },
			{ L_weapons_dialog_text_invalidweaponid, "Invalid weaponid" },
			{ L_weapons_dialog_text_lowcash, "You do not have enough money" },
			{ L_weapons_dialog_text_notequiped, "This weapon is not equipped" },
			{ L_weapons_dialog_text_noammo, "Weapon is out of ammo" },
			{ L_weapons_dialog_text_needbuy, "You need to buy this weapon first" },

			{ L_buyammo_usage, "Usage: {41BECC}/ammo <weaponid> <amount>{FFFFFF}, Example: {41BECC}/ammo 25 100{FFFFFF} will buy 100 Shotgun shells" },
			{ L_buyammo_error_badbulletweapon, "You cannot buy ammunition for this weapon" },
			{ L_buyammo_error_0ammo, "The amount of ammunition you want to buy has to be greater than 0" },
			{ L_buyammo_error_maxammo, "You already own the maximum amount of ammunition for this weapon" },
			{ L_buyammo_success, "Ammo bought succesfully! Ammo in weapon %s: %d" },

			{ L_error_cant_usecmd_atm, "Can't use this command right now" },
			{ L_setcolor_removed, "Color removed!" },
			{ L_setcolor_usage, "Usage: {41BECC}/color <color>{FFFFFF}, Example: {41BECC}/color 77FFAA{FFFFFF}, pick a color at http://colorpicker.com | Current color: %06X" },
			{ L_say_usage, "Usage: {41BECC}/say <message>{FFFFFF} (max 111 characters)" },
			{ L_asay_usage, "Usage: {41BECC}/a <text>" },
			{ L_msay_usage, "Usage: {41BECC}/m <text>" },
			{ L_lsay_usage, "Usage: {41BECC}/l <text>" },
			{ L_savepos_success, "Your position has been saved, use {41BECC}/loadpos{FFFFFF} to teleport to this position" },
			{ L_givecash_success_1, "Transfer of $%I64u successful, balance: $%I64d" },
			{ L_givecash_success_2, "$%I64u has been transferred to you by %s from his own account, balance: $%I64d" },
			{ L_givecash_usage, "Usage: {41BECC}/givecash <id/nick> <amount of money>, balance: $%I64d" },
			{ L_givescore_success_1, "Transfer of %I64u respect successful, respect balance: %I64d" },
			{ L_givescore_success_2, "%I64u respect has been transferred to you by %s from his own account, respect balance: %I64u" },
			{ L_givescore_error, "You do not have enough respect" },
			{ L_givescore_usage, "Usage: {41BECC}/giverespect <id/nick> <amount of respect>, respect balance: %I64d" },
			{ L_agivecash_success_1, "Transfer of $%I64d successful, balance [id: %d]: $%I64d" },
			{ L_agivecash_success_2, "$%I64d has been transferred to you by %s, balance: $%I64d" },
			{ L_agivecash_usage, "Usage: {41BECC}/agivecash <id/nick> <amount of money>, balance: $%I64d" },
			{ L_agivescore_success_1, "Transfer of %I64d respect successful, respect balance [id: %d]: %I64d" },
			{ L_agivescore_success_2, "%I64d respect has been transferred to you by %s, respect balance: %I64d" },
			{ L_agivescore_usage, "Usage: {41BECC}/agiverespect <id/nick> <amount of respect>, respect balance: %I64d" },
			{ L_agiveexp_success_1, "Transfer of %I64d experience points successful, experience points [id: %d]: %I64u" },
			{ L_agiveexp_success_2, "%I64d experience points have been transferred to you by %s, experience points: %I64u" },
			{ L_agiveexp_usage, "Usage: {41BECC}/agiveexp <id/nick> <amount of exp points>, experience points: %I64u" },
			{ L_bank_withdraw_success, "Transfer from bank ($%I64d) successful, bank balance: $%I64d" },
			{ L_bank_nofunds_bank, "You do not have enough funds on your bank account" },
			{ L_bank_transfer_success, "Transfer to bank ($%I64d) successful, bank balance: $%I64d" },
			{ L_bank_nofunds_here, "You do not have enough cash" },
			{ L_bank_usage_1, "Usage: {41BECC}/bank <amount of money>{FFFFFF}, bank balance: $%I64d, use a negative amount to withdraw money from bank" },
			{ L_bank_usage_2, "Examples: {41BECC}/bank 100{FFFFFF} transfers $100 to your bank account and {41BECC}/bank -100{FFFFFF} withdraws $100" },

			{ L_race_gametxt, "Race" },
			{ L_race_joined_success, "You have joined the %s race" },
			{ L_race_staging, "The %s race begins in a moment (%s)" },
			{ L_race_not_enough_players, "Not enough players to start the race %s" },
			{ L_race_winner_text, "You have completed the %s race with position %d and a time of %.3f seconds, you were given $%I64u and %I64u respect" },
			
			{ L_announce_usage, "Usage: {41BECC}/ann <text>{FFFFFF}, text must be between 1 and 57 chars, allowed chars: [a-z][A-Z][0-9][no percent]" },

			{ L_minigame_inactivity, "You have been removed from the game, reason: Detected AFK or too big laggs" },

			{ L_goldcoin_foundnew, "Congratz! You have found a new golden coin! %d/%d | Use /zmhelp to get more information about this" },

			{ L_goldcoin_helpmsg, "Shoot the yellow magnet with a weapon to unloack it" },
		};

		struct SAddTranslation { SAddTranslation() { InternalLanguage::Add(translation); } } _SAddTranslation;
	};
};