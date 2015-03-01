/*
	Copyright (c) 2014 - 2015 Rafał "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafał Grasman <grasmanek94@gmail.com>
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
		Mateusz 'eider' Cichoń
*/
#pragma once
#include "LangMain.hxx"

/*
	• Translation Rules:
		► Make sure to replace {TABS} with {\t}
		► Try to avoid {NEWLINE}s, replace as much as you can with {\n}
		► Keep format variable order (!)
		► L_mm_* - ONLY a-z A-Z 0-9 :[] (TEXTDRAWS)
*/

namespace Language
{
	namespace Polish
	{
		const std::map<language_string_ids, std::string> translation =
		{
			{ L_language_name, "Polski [Polish-pl_PL]" },
			{ L_empty_underscore, "_" },
			{ L_empty, " " },
			{ L_invalid_playerid, "Nieprawidłowe ID" },
			{ L_eyecmd_player_enabled, "EyeCmd na graczu załączone dla ciebie" },
			{ L_eyecmd_player_disabled, "EyeCmd na graczu wyłączone dla ciebie" },
			{ L_eyecmd_player_watching_everyone, "Już podglądasz wszystkich. Kogo więcej byś jeszcze chciał(a)?" },
			{ L_eyecmd_player_usage, "Użycie: {b}/eye <id/nick> {/b} (ponownie użyj aby wyłączyć)" },
			{ L_eyecmdall_enabled, "Monitoring wszystkich komend załączony" },
			{ L_eyecmdall_disabled, "Monitoring wszystkich komend wyłączony" },
			{ L_global_chat_history, "Historia Czatu Globalnego" },
			{ L_no_history, "Brak Historii" },
			{ L_eyeip_enabled, "EyeIP na wszystkie nowe połączenia załączone" },
			{ L_eyeip_disabled, "EyeIP na wszystkie nowe połączenia wyłączone" },
			{ L_mm_achievement_newlevel, "Awansowales na nastepny poziom!" },
			{ L_local_message_too_long, "Za długa wiadomość, nie wysłano." },
			{ L_cheats_detected, "WYKRYTO NIEDOZWOLONE MODYFIKACJE GRY (mod_s0beit, CLEO lub inne)" },
			{ L_chat_history, "Historia Czatu" },
			{ L_lastchat_usage, "Użycie: {b}/apmh <id/nick>" },
			{ L_PM_history, "Historia PW" },
			{ L_lastpm_usage, "Użycie: {b}/apph <id/nick>" },
			{ L_CMD_history, "Historia Komend" },
			{ L_lastcmd_usage, "Użycie: {b}/apch <id/nick>" },
			{ L_spectate_usage, "/spec <id/nick>" },
			{ L_player_not_connected, "Gracz nie jest połączony" },
			{ L_player_not_spawned, "Gracz nie jest zrespawnowany" },
			{ L_spectate_generic_error, "Nie można tego gracza podglądać" },
			{ L_specoff_need_spectate_first, "Nie podglądasz kogoś" },

			{ L_games_stagecheck_msg, "Już się zapisałeś/aś na jakąś zabawę, odczekaj odliczenie" },
			{ L_games_exit_info, "Aby wyjść lub anulować wpisz {b}/exit" },
			{ L_games_exited, "Opuszczono zabawę" },
			{ L_games_all_list_stub, "{FFFFFF}Oto lista komend dostępnych zabaw:\n\n{FFFFFF}%s{FFFFFF}\n\nPamiętaj: Aby wyjść z jakiejkolwiek zabawy użyj {41BECC}/exit{FFFFFF}\nZabawy/Areny które mają czerwoną gwiazdkę obok siebie są tylko dostępne dla graczy którzy używają dodatek eXe24+ ({41BECC}/plus{FFFFFF})" },
			{ L_games_help, "Pomoc - Zabawy" },
			{ L_games_ArenaGranaty_name, "Arena Granatów" },
			{ L_games_ArenaMinigun_name, "Arena Minigunów" },
			{ L_games_ArenaOneDe_name, "Arena OneDe" },
			{ L_games_ArenaOneShot_name, "Arena OneShot" },
			{ L_games_ArenaPaintBall_name, "Arena PaintBall" },
			{ L_games_ArenaRPG_name, "Arena RPG" },
			{ L_games_ArenaSawnOff_name, "Arena SawnOff" },
			{ L_games_ArenaSniper_name, "Arena Sniperska" },
			{ L_games_ArenaTankBattle_name, "Arena Wojny Czołgów" },
			{ L_games_ZabawaBagno_name, "Zabawa Bagno" },
			{ L_games_ZabawaDerby_name, "Zabawa Derby" },
			{ L_games_ArenaFlyingTank_name, "Arena Latających Czołgów" },
			{ L_games_ZabawaHay_name, "Zabawa Sianko" },
			{ L_games_ZabawaTDM_name, "Zabawa Drużynowy DeathMatch" },
			{ L_games_ArenaWigilia_name, "Wigilia" },

			{ L_password_change, "Zmiana hasła" },
			{ L_setpass_usage, "Użycie: {b}/setpass <nick> <hasło>{/b} aby zmienić użytkownikowi hasło, użytkownik nie może być połączony z serwerem" },
			{ L_player_is_online, "Gracz jest online!" },
			{ L_unbanip_usage, "Użycie: {b}/unbanip <ip> (full){/b} podaj na koniec 'full' jeśli także chcesz odbanować konto które zostało zbanowane razem z tym IP" },
			{ L_unban_usage, "Użycie: {b}/unban <nick> (full){/b} podaj na koniec 'full' jeśli także chcesz odbanować ip które zostało zbanowane razem z tym Kontem" },
			{ L_PAP_LackOfPermissions, "(Brak uprawnień)" },
			{ L_PAP_DoesNotExist, "(Nie istnieje)" },
			{ L_Staff_CurrentlyOnline, "Personel serwera online: \n\n" },
			{ L_Staff_CurrentlyNone, "\tBrak" },
			{ L_Staff_DisplayTitle, "Personel Online" },
			{ L_report_reason_TooShortOrLong, "Powód musi być wypełniony lecz krótszy niż 64 znaki" },
			{ L_report_send, "Raport wysłany! Nie spamuj i czekaj aż Personel się skontaktuje" },
			{ L_report_usage, "Użycie: {b}/r <id/nick> <powód>{/b} aby zaraportować gracza" },
			{ L_promote_info_msg_1, "{A9C4E4}Informacje wykonania ustawień rang dla %s przez %s:\n" },
			{ L_promote_add_error, "\n\tNastępujące rangi nie udało się dodać:\n" },
			{ L_promote_add_success, "\n\tNastępujące rangi zostały dodane do konta:\n" },
			{ L_word_ranks, "Rangi" },
			{ L_promote_noaccess, "Nie masz uprawnień, id ma możliwie większe uprawnienia" },
			{ L_promote_params_error, "Nie można rozdzielić parametrów" },
			{ L_promote_params_invalid, "Nie podałeś dobrych parametrów" },
			{ L_promote_usage, "\
{A9C4E4}Dostępne rangi:\n\
\t• NONE      - Zabiera graczowi wszystkie rangi\n\
\t• USER      - Daje graczowi możliwości normalnego gracza\n\
\t• VIP       - Daje graczowi możliwości VIP-a\n\
\t• SPONSOR   - Daje graczowi możliwości Sponsora\n\
\t• MODERATOR - Daje graczowi możliwości Moderatora\n\
\t• ADMIN     - Daje graczowi możliwości Admina\n\
\t• VICEHEAD  - Daje graczowi możliwości Vice Head Admina\n\
\t• HEADADMIN - Daje graczowi możliwości Head Admina\n\
\t• WSPOLNIK  - Daje graczowi możliwości Wspólnika\n\
\t• GAMER     - Daje graczowi możliwości Gamera\n\
\n\
Użycie: {41BECC}/promote <id/nick> <RANGA1 RANGA2 RANGA3>{A9C4E4} aby dać użytkownikowi dane rangi.\n\
Przykład: '{FF0000}/promote Janek SPONSOR VIP{41BECC}'\n\
"
			},
			{ L_promote_take_error, "\r\n\tNastępujące rangi nie udało się zabrać:\r\n" },
			{ L_promote_take_success, "\r\n\tNastępujące rangi zostały zabrane z konta:\r\n" },
			{ L_depromote_usage, "\
{A9C4E4}Dostępne rangi:\
\t• USER		- Zabiera graczowi możliwości normalnego gracza\r\n	\
\t• VIP			- Zabiera graczowi możliwości VIP-a\r\n	\
\t• SPONSOR		- Zabiera graczowi możliwości Sponsora\r\n	\
\t• MODERATOR	- Zabiera graczowi możliwości Moderatora\r\n	\
\t• ADMIN		- Zabiera graczowi możliwości Admina\r\n	\
\t• VICEHEAD	- Zabiera graczowi możliwości Vice Head Admina\r\n	\
\t• HEADADMIN	- Zabiera graczowi możliwości Head Admina\r\n	\
\t• WSPOLNIK	- Zabiera graczowi możliwości Wspólnika\r\n	\
\t• GAMER		- Zabiera graczowi możliwości Gamera\r\n	\
\r\n\
Użycie: {41BECC}/depromote <id/nick> <RANGA1 RANGA2 RANGA3>{A9C4E4} aby zabrać użytkownikowi dane rangi.\r\n\
Przykład: '{FF0000}/depromote Janek SPONSOR VIP{41BECC}'\r\n\
"
			},

			{ L_kicked_player, "{b}%s{/b} wyrzucił gracza {b}%s{/b} z powódu {b}%s{/b}" },
			{ L_kicked_word, "brak" },
			{ L_kicked_dialoginfo, "Osoba wyrzucająca: %s\nKick ID: [%I64X] (zapisz to jeśli chcesz zgłosić incydent na forum!)\nOsoba wyrzucona: %s\nPowód: %s" },
			{ L_kicked_bye, "Wyrzucony/a" },

			{ L_banned_player, "{b}%s{/b} zbanował gracza {b}%s{/b} z powódu {b}%s{/b}" },
			{ L_banned_dialogdisplay, "{FFFFFF}Osoba banująca: %s\nBan ID: {FF0000}[%I64X]{FFFFFF} (zapisz to jeśli chcesz zgłosić incydent na forum!)\nOsoba zbanowana: %s\nZbanowane IP: %s\nPowód: %s\r\n\r\nForum: %s\r\n\r\nBan wygaśnie: %s\r\n\r\nBan wydany: %s" },
			{ L_banned_ipandaccount, "Zbanowany adres IP oraz Konto" },
			{ L_banned_ip, "Zbanowany adres IP" },
			{ L_banned_iphost, "Zbanowany HOST IP" },
			{ L_banned_account, "Zbanowane Konto" },

			{ L_kick_usage, "Użycie: {b}/kick <id/nick> (Opcjonalne:<powód>){/b} aby wyrzucić gracza z serwera" },
			{ L_ban_usage_1, "Użycie: {b}/ban <id/nick> (<ilość minut>{/b}(0 lub brak banuje na zawsze){41BECC} <powód>)" },
			{ L_ban_usage_2, "TIP| Godziny: Minuty | 1: 60 | 3: 180 | ~6: 360 | ~12: 700 | ~24: 1400 | ~7 Dni: 10,000 | ~31 Dni: 45,000" },

			{ L_mm_join_game, "%s dolaczyl(a) do gry" },
			{ L_mm_bag_startcd, "Zabawa Bagno wystartuje za chwile, uzyj {b}/bag{/b} aby dolaczyc"},
			{ L_mm_bag_staging, "Zapisales/as sie na {b}bagno{/b}" },
			{ L_mm_bag_not_enough_players, "Za malo graczy aby rozpoczac {b}bagno{/b}" },
			{ L_mm_bag_announce_winner, "Bagno wygral gracz {b}%s{/b}" },
			{ L_mm_drb_announce_winner, "Derby wygral gracz {b}%s{/b}" },
			{ L_mm_drb_startcd, "Zabawa Derby wystartuje za chwile, uzyj {b}/drb{/b} aby dolaczyc" },
			{ L_mm_drb_staging, "Zapisales/as sie na {b}debry{/b}" },
			{ L_mm_drb_not_enough_players, "Za malo graczy aby rozpoczac {b}derby{/b}" },
			{ L_mm_mute_player, "Zostales/as odciszony/a" },
			{ L_mm_mute_player_announce, "{b}%s{/b} uciszyl(a) gracza {b}%s{/b} z powodu {b}%s{/b}" },
			{ L_mute_player_title, "Wyciszenie na chat" },
			{ L_mute_player_info, "{FFFFFF}Zostałeś/aś uciszony/a na chat\n\nOsoba uciszająca: %s\nMute ID: {FF0000}[%I64X]{FFFFFF} (zapisz to jeśli chcesz zgłosić incydent na forum!)\nOsoba uciszona: %s\nPowód: %s\r\n\r\nForum: %s\r\n\r\nOdciszenie nastąpi za %I64u sekund" },
			{ L_mute_usage, "Użycie: {b}/mute <id/nick> <ilość sekund> (<powód>){/b}, 0 sekund aby odciszyć" },
			{ L_mm_tdm_team_red, "Czerwonych" },
			{ L_mm_tdm_team_blue, "Niebieskich" },
			{ L_mm_tdm_announce_winner, "TDM wygrala druzyna " },
			{ L_mm_tdm_startcd, "Zabawa TDM wystartuje za chwile, uzyj {b}/TDM{/b} aby dolaczyc" },
			{ L_mm_tdm_staging, "Zapisales/as sie na {b}TDM{/b}" },
			{ L_mm_tdm_not_enough_players, "Za malo graczy aby rozpoczac {b}TDM{/b}" },
			{ L_mm_hay_announce_winner, "Arene siana wygral gracz {b}%s{/b}" },
			{ L_mm_hay_startcd, "Arena siana wystartuje za chwile, uzyj {b}/hay{/b} aby dolaczyc" },
			{ L_mm_hay_staging, "Zapisales/as sie na {b}siano{/b}" },
			{ L_mm_hay_not_enough_players, "Za malo graczy aby rozpoczac {b}siano{/b}" },

			{ L_aexplode_executed, "Wykonano explode" },
			{ L_aexplode_usage, "Użycie: {b}/aexplode <id/nick>" },
			{ L_aslap_executed, "Wykonano slap" },
			{ L_aslap_reversed, "Zamiast odjąć zostało dodane {b}10 HP{/b} z powodu jej niskiej ilości" },
			{ L_aslap_usage, "Użycie: {b}/aslap <id/nick>" },
			{ L_apaddhp_executed, "Dodano podane HP" },
			{ L_apaddhp_usage, "Użycie: {b}/apaddhp <id/nick> <ilość>" },
			{ L_apsethp_executed, "Ustawiono podane HP" },
			{ L_apsethp_usage, "Użycie: {b}/apsethp <id/nick> <ilość>" },
			{ L_apaddarmor_executed, "Dodano podane Armour" },
			{ L_apaddarmor_usage, "Użycie: {b}/apaddarmor <id/nick> <ilość>" },
			{ L_apsetarmor_executed, "Ustawiono podany Pancerz" },
			{ L_apsetarmor_usage, "Użycie: {b}/apsetarmor <id/nick> <ilość>" },
			{ L_targetplayer_not_in_vehicle, "Gracz nie jest w pojeździe" },
			{ L_apvaddhp_executed, "Dodano podane HP Pojazdu w którym jest gracz" },
			{ L_apvaddhp_usage, "Użycie: {b}/apvaddhp <id/nick> <ilość>" },
			{ L_apvsethp_executed, "Ustawiono podane HP Pojazdu w którym jest gracz" },
			{ L_apvsethp_usage, "Użycie: {b}/apvsethp <id/nick> <ilość>" },
			{ L_atp_move_info, "{b}%s{/b} teleportował {b}%s{/b} do {b}%s{/b}" },
			{ L_atp_move_error, "Jednego z graczy nie można teraz teleportować" },
			{ L_atp_id_error, "Jedno z ID jest nieprawidłowe lub ID są takie same" },
			{ L_atp_usage, "Użycie: {b}/atp <id/nick od> <id/nick do>" },
			{ L_banip_success, "IP zostało zbanowane!" },
			{ L_banip_usage, "Użycie: {b}/banip <ip> <czas w minutach> <powód>" },
			{ L_banhost_success, "HOST został zbanowany!" },
			{ L_banhost_usage_1, "Użycie: {b}/banhost <ip> <czas w minutach> <powód>" },
			{ L_banhost_usage_2, "Przykład: {b}/banhost 2.3.4.5 0 cheat{/b} zbanuje host 2.3.*.* na nieskończoność (0) minut z powodem {b}cheat{/b}" },
			{ L_unbanhost_success, "HOST został odbanowany!" },
			{ L_unbanhost_failed, "Taki host nie jest zbanowany" },
			{ L_unbanhost_usage, "Użycie: {b}/unbanhost <ip>{/b} np. {b}/unbanhost 2.3.4.5 {/b}odbanuje host 2.3.*.*" },

			{ L_emulate_executed, "Polecenie wykonane" },
			{ L_emulate_usage, "Użycie: {b}/emulate <id/nick> <tekst lub /komenda>{/b}" },
			{ L_emulateall_usage, "Użycie: {b}/emulateall <id/nick> <tekst lub /komenda>{/b}" },

			{ L_dbworker_userpwchange_success, "Zmieniono hasło użytkownikowi %s" },
			{ L_dbworker_userpwchange_failure, "[SETPASS] Nie można znaleźć użytkownika %s" },
			{ L_dbworker_unban_usr_success_p, "[P] Odbanowano użytkownika %s" },
			{ L_dbworker_unban_usr_success_k, "[K] Odbanowano użytkownika %s" },
			{ L_dbworker_unban_usr_fail, "[UNBAN] Nie można znaleźć użytkownika %s" },
			{ L_dbworker_unban_ip_success_p, "[P] Odbanowano IP %s" },
			{ L_dbworker_unban_ip_success_k, "[K] Odbanowano IP %s" },
			{ L_dbworker_unban_ip_fail, "[UNBAN] Nie można znaleźć IP '%s' w liście banów" },

			{ L_v_usage, "Użycie: {b} / v[Nazwa / ModelID]" },
			{ L_v_slot_error, "Brak zapisanego pojazdu na podany slot" },
			{ L_v_slot_error_mafia, "Podany slot wymaga udziału w Mafii" },
			{ L_v_no_car_found_error, "Nie znaleziono takiego pojazdu!" },
			{ L_v_cannot_spawn_error, "Nie można zrespawnować tego pojazdu" },
			{ L_vehicles_carlist_head, "Pojazdy" },
			{ L_vehicles_carlist, "Samoloty\nHelikoptery\nDwukołowce\nKabriolety\nPrzemysłowe\nLowridery\nOff Road\nSerwis Publiczny\nSaloons\nSportowe\nKombi\nŁodzie\nPrzyczepy\nUnikatowe\nRC" },
			{ L_vehicles_planes, "Samoloty" },
			{ L_vehicles_helis, "Helikoptery" },
			{ L_vehicles_twowheeled, "Dwukołowce" },
			{ L_vehicles_cabrio, "Kabriolety" },
			{ L_vehicles_industrial, "Przemysłowe" },
			{ L_vehicles_lowriders, "Lowridery" },
			{ L_vehicles_public_service, "Serwis publiczny" },
			{ L_vehicles_sport, "Sportowe" },
			{ L_vehicles_wagons, "Kombi" },
			{ L_vehicles_boats, "Łodzie" },
			{ L_vehicles_trailers, "Przyczepy" },
			{ L_vehicles_unique, "Unikalne" },
			{ L_vehicles_offroad, "Off Road" },
			{ L_vehicles_saloons, "Saloons" },
			{ L_vehicles_remotecontrol, "Zdalnie Sterowane" },
			{ L_vcolor_usage, "Użycie: {b}/vkolor <kolor1> (<kolor2>){/b} - Kolory są od 0 do 256 | Obecne kolory: [%d, %d]" },
			{ L_vpaint_usage, "Użycie: {b} / vpaint <id>{/b} Możliwe paintjoby to 0, 1 lub 2, aby usunąć paintjob użyj id 3 | Obecny PaintJobID : %d" },
			{ L_tune_cannot_tune_error, "Tego pojazdu nie możesz stunigować" },

			{ L_countdown_counter, "Rozpoczeto odliczanie! Wpisz /cd aby Tobie tez odliczalo! Zostalo %d sek" },
			{ L_countdown_removed, "Usunieto Ciebie z odliczania" },
			{ L_countdown_joined, "Dolaczono do odliczania! Wpisz {b}/cd stop{/b} aby zatrzymac" },

			{ L_email_send_error, "Nastąpił błąd podczas wysyłania E-Maila do ciebie, powtórz polecenie później." },
			{ L_email_send_success, "E-Mail został wysłany poprawnie." },

			{ L_timed_score_add, "Dostales/as dodatkowe doswiadczenie, respekt oraz kase za obecnosc na serwerze!" },

			{ L_unknown_command, "Nieznana komenda: {b}" },

			{ L_lotto_win, "Trafiłeś/aś aż %d liczb w lotto, otrzymałeś/aś {b}$%I64u{/b}" },
			{ L_lotto_lose, "Niestety nie trafiłeś/aś żadnych liczb w lotto" },
			{ L_lotto_winannounce, "[LOTTO] Jackpot został rozdany między graczami, szczęśliwe liczby: %d, %d, %d, %d" },
			{ L_lotto_loseannounce, "[LOTTO] Nikt nie trafił na jakąkolwiek liczbę w lotto! Jackpot zostaje zwiększony do {b}$%I64u{/b}! Szczęśliwe liczby: %d, %d, %d, %d" },
			{ L_lotto_number_error, "Niektóre Twoje liczby się powtarzają lub nie są między 0 a 63, spróbuj jeszcze raz" },
			{ L_lotto_send_success, "Lotek został wysłany! Teraz czekaj na serwerze na losowanie, inaczej lotek przepadnie" },
			{ L_lotto_already_submitted, "Poczekaj na losowanie, już bieżesz udział" },
			{ L_lotto_usage_1, "Użycie: {b}/lotto <liczba 1> (nieobowiązkowe:  <liczba 2> ... <liczba max 8>){/b} aby wziąść udział w lotto z wybranymi liczbami" },
			{ L_lotto_usage_2, "Każda liczba kosztuje $750 za udział, liczby muszą się zaczynać na 0 (minimum) a maksymalnie kończyć na 63" },

			{ L_error_insufficient_funds, "Nie posiadasz dość funduszy" },
			{ L_invalid_funds_amount, "Niepoprawna suma pieniędzy (minimalnie $25,000)" },

			{ L_stats_totalgivenbounty, "Pełna suma którą dano za wszystkie głowy: $%I64u" },
			{ L_stats_mytotalbounty, "Podsumowana watrośc głowy: $%I64u" },
			{ L_stats_collectedbounty, "Podsumowana suma uzbierana z głów innych graczy: $%I64u" },
			{ L_stats_bountykillers, "Ilość zabitych graczy którzy mieli cenę za głowę: %I64u" },
			{ L_stats_vehicleflip, "Lubię Dachować! Aż %I64u razy!" },
			{ L_stats_pmspammer, "Spammer na PW! Aż %I64u wiadomości!" },
			{ L_stats_chatspammer, "Spammer na Czacie! Aż %I64u wiadomości!" },
			{ L_stats_lottowins, "Wygrano lotto %I64u razy" },
			{ L_stats_totallottowon, "Podsumowana suma wygrana w lotto: $%I64u" },
			{ L_stats_quizgood, "Poprawnych odpowiedzi na pytania w /quiz: %I64u" },
			{ L_stats_quizbad, "Niepoprawnych odpowiedzi na pytania w /quiz: %I64u" },
			{ L_stats_amountteleported, "Traveler! Ile razy wykonano teleport: %I64u" },
			{ L_stats_rampscount, "Rampownik! Postawiono aż %I64u ramp!" },
			{ L_stats_vehiclespawncount, "Automatyczny Spawner Pojazdów! Postawiono ich aż %I64u!" },
			{ L_stats_gamejoiner, "Imprezowicz! Dołączono aż do %I64u zabaw!" },
			{ L_stats_mng_kills, "Ilość zabójstw na arenie Minigunów: %I64u" },
			{ L_stats_mng_deaths, "Ilość śmierci na arenie Minigunów: %I64u" },
			{ L_stats_hay_wins, "Ilość wygranych rozgrywek na Sianko: %I64u" },
			{ L_stats_hay_loses, "Ilość przegranych rozgrywek na Sianko: %I64u" },
			{ L_stats_ode_kills, "Ilość zabójstw na arenie OneDe: %I64u" },
			{ L_stats_ode_deaths, "Ilość śmierci na arenie OneDe: %I64u" },
			{ L_stats_snp_kills, "Ilość zabójstw na arenie Sniper: %I64u" },
			{ L_stats_snp_deaths, "Ilość śmierci na arenie Sniper: %I64u" },
			{ L_stats_sof_kills, "Ilość zabójstw na arenie SawnOff: %I64u" },
			{ L_stats_sof_deaths, "Ilość śmierci na arenie SawnOff: %I64u" },
			{ L_stats_mafispammer, "Spammer na czacie Mafii! Aż %I64u wiadomości!" },
			{ L_stats_vipspammer, "Spammer na czacie Vipów! Aż %I64u wiadomości!" },
			{ L_stats_sponsorspammer, "Spammer na czacie Sponsorów! Aż %I64u wiadomości!" },
			{ L_stats_modspammer, "Spammer na czacie Moderatorów! Aż %I64u wiadomości!" },
			{ L_stats_adminspam, "Spammer na czacie Adminów! Aż %I64u wiadomości!" },
			{ L_stats_cheater, "CHEATER!!@@@$@^!! Aż %I64u zawiadomień!" },
			{ L_stats_helpme, "NA POMOC! HALPPPP! Aż %I64u razy przeglądnięto pomoc serwera!" },
			{ L_stats_privatevehicles, "Tylko Moje\3 ! Aż %I64u zrespawnowanych prywatnych pojazdów!" },
			{ L_stats_mafiavehicles, "MAFIA \3 4 Life \3 ! Aż %I64u zrespawnowanych pojazdów Mafii!" },
			{ L_stats_amounthealed, "Nie chcę umrzeć!! Aż %I64u się uleczono!" },
			{ L_stats_mammothtank, "CZOŁG!! Aż %I64u razy się opanczerzono!" },
			{ L_stats_acsuspect, "PODEJRZANY!! :P Podejrzenie przez AntiCheat padło aż %I64u razy!" },
			{ L_stats_roleplayer, "RolePlay bitchez! Aż %I64u razy użyto komendę /anim!" },
			{ L_stats_gotmafiascore, "Mafia \3 my \3 momma! Aż $%I64u dostano od Mafii!" },
			{ L_stats_givenmafiascore, "Mafia \3 my \3 momma! Aż %I64u respektu dostano od Mafii!" },
			{ L_stats_biggestkillstreak, "MEGGAAAA KILLAAA! Aż %I64u killstreak!" },
			{ L_stats_currentkillstreak, "Obecny killstreak: %I64u!" },
			{ L_stats_headshots, "HEADSHOT! Ilość headshotów: %I64u!" },
			{ L_stats_waitforxp, "Sęp na exp! Aż %I64u expa otrzymano za spędzanie czasu na serwerze!" },
			{ L_stats_agr_kills, "Ilość zabójstw na arenie Granatów: %I64u" },
			{ L_stats_agr_deaths, "Ilość śmierci na arenie Granatów: %I64u" },
			{ L_stats_arpg_kills, "Ilość zabójstw na arenie RPG: %I64u" },
			{ L_stats_arpg_deaths, "Ilość śmierci na arenie RPG: %I64u" },
			{ L_stats_osh_kills, "Ilość zabójstw na arenie OneShot: %I64u" },
			{ L_stats_osh_deaths, "Ilość śmierci na arenie OneShot: %I64u" },
			{ L_stats_apb_kills, "Ilość zabójstw na arenie PaintBall: %I64u" },
			{ L_stats_apb_deaths, "Ilość śmierci na arenie PaintBall: %I64u" },
			{ L_stats_ctf_kills, "Ilość zabójstw na CTF: %I64u" },
			{ L_stats_ctf_deaths, "Ilość śmierci na CTF: %I64u" },
			{ L_stats_ctf_pickedupflags, "Ilość podniesionych flag na CTF: %I64u" },
			{ L_stats_ctf_capturedflags, "Ilość dostarczonych flag na CTF: %I64u" },
			{ L_stats_wns_kills, "Ilość zabójstw na Wojnie na Statki: %I64u" },
			{ L_stats_wns_deaths, "Ilość śmierci na Wojnie na Statki: %I64u" },
			{ L_stats_ww3_kills, "Ilość zabójstw na WW3: %I64u" },
			{ L_stats_ww3_deaths, "Ilość śmierci na WW3: %I64u" },
			{ L_stats_seeknhide_finds, "Ilość znalezionych osób na Chowanym: %I64u" },
			{ L_stats_seeknhide_besttime, "Najdłuższy czas bycia schowanym: %I64u minut" },
			{ L_stats_drb_won, "Ilość wygranych rozgrywek Derby: %I64u" },
			{ L_stats_drb_lost, "Ilość przegranych rozgrywek Derby: %I64u" },
			{ L_stats_maze_fastesttime, "Najszybszy czas w labiryncie: %I64u sekund" },
			{ L_stats_tdm_kills, "Ilość zabójstw na TDM: %I64u" },
			{ L_stats_tdm_deaths, "Ilość śmierci na TDM: %I64u" },
			{ L_stats_tdm_wins, "Ilość wygranych rozgrywek TDM: %I64u" },
			{ L_stats_tdm_loses, "Ilość przegranych rozgrywek TDM: %I64u" },
			{ L_stats_bmxparkourft, "Najszybszy czas na parkour BMX: %I64u sekund" },
			{ L_stats_nrgparkourft, "Najszybszy czas na parkour NRG: %I64u sekund" },
			{ L_stats_parkourft, "Najszybszy czas na Parkour: %I64u sekund" },
			{ L_stats_spm_kills, "Ilość zabójstw na Sparingu Mafii: %I64u" },
			{ L_stats_spm_deaths, "Ilość śmierci na Sparingu Mafii: %I64u" },
			{ L_stats_spm_wins, "Ilość wygranych rozgrywek Sparingu Mafii: %I64u" },
			{ L_stats_spm_loses, "Ilość przegranych rozgrywek Sparingu Mafii: %I64u" },
			{ L_stats_mma_wins, "Ilość wygranych rozgrywek MMA: %I64u" },
			{ L_stats_mma_loses, "Ilość przegranych rozgrywek MMA: %I64u" },
			{ L_stats_wjg_kills, "Ilość zabójstw na Wojnie Gangów: %I64u" },
			{ L_stats_wjg_deaths, "Ilość śmierci na Wojnie Gangów: %I64u" },
			{ L_stats_wjg_wins, "Ilość wygranych rozgrywek Wojnie Gangów: %I64u" },
			{ L_stats_wjg_loses, "Ilość przegranych rozgrywek Wojnie Gangów: %I64u" },
			{ L_stats_racekartft, "Najszybszy czas na torze Kart: %I64u sekund" },
			{ L_stats_bag_kills, "Ilość zabójstw na BAG: %I64u" },
			{ L_stats_bag_deaths, "Ilość śmierci na BAG: %I64u" },
			{ L_stats_bag_wins, "Ilość wygranych rozgrywek BAG: %I64u" },
			{ L_stats_bag_loses, "Ilość przegranych rozgrywek BAG: %I64u" },
			{ L_stats_new_achievement, "Gratulacje! Zdobyles/as nowe osiagniecie! Wpisz /osg aby je zobaczyc" },
			{ L_stats_achievement_info, "Osiągnięcia\nStatystyki\nDodatkowe Statystyki (1)\nDodatkowe Statystyki (2)\nZabójstwa z broni\nŚmierci z broni\nKillstreak z broni\nZabójstwa przeciwników z tą samą bronią\nŚmierci przez przeciwników z tą samą bronią\nPoziomy broni\nUżyj /osg <id/nick> aby zobaczyć dane innego gracza" },
			{ L_stats_achievement_title, "Statystyki & Osiągnięcia gracza %s" },
			{ L_stats_txt_none, "Brak osiagnięć" },
			{ L_stats_amount_not_achieved, "\n\nIlość niezdobytych osiągnięć: %d" },
			{ L_stats_additional_none, "Brak dodatkowych statystyk" },
			{ L_stats_additional_info_1, "Dodatkowe statystyki (1) dla gracza %s" },
			{ L_stats_additional_info_2, "Dodatkowe statystyki (2) dla gracza %s" },
			{ L_stats_killsfromweapons, "Zabójstwa z broni dla gracza %s" },
			{ L_stats_killedbyweapons, "Śmierci z broni dla gracza %s" },
			{ L_stats_biggestkillstreaks, "Największy killstreak z broni dla gracza %s" },
			{ L_stats_killswithsameweapons, "Zabójstwa przeciwników z tą samą bronią dla gracza %s" },
			{ L_stats_deathswithsameweapons, "Śmierci przez przeciwników z tą samą bronią dla gracza %s" },
			{ L_stats_weaponlevels, "Poziomy broni dla gracza %s" },

			{ L_anim_invalid_category, "Nieprawidłowa kategoria" },
			{ L_anim_title, "Animacje" },
			{ L_anim_display_info, "\
{FFFFFF}Info: Lista jest w formacie {41BECC}[nazwa kategorii animacji - ilość animacji w kategorii]{FFFFFF}\n\
Aby użyć animację wpisz {41BECC}/anim <nazwa> <id>{FFFFFF},\n\
jeśli ilość animacji w kategorii to 1, nie trzeba wpisywać id. Id są od 0 do (ilość animacji-1)\n\
Aby zakończyć animację użyj {41BECC}/animstop{FFFFFF}\n\
Lista dostępnych animacji:\n"
			},

			{ L_setskin_usage, "Użycie: {b}/skin <ModelID>{/b}, Obecny skin: {b}%d{/b}" },

			{ L_savecar_cannot_save, "Nie można zapisać tego pojazdu" },
			{ L_mm_savecar_success, "Pojazd zapisany" },
			{ L_savecar_usage_1, "Użycie: {b}/vsave <slot>{/b} (slot od 0 do 9)" },
			{ L_savecar_usage_2, "Możesz zrespawnować za pomocą: {b}/v <slot>" },
			{ L_ramp_succes, "Typ rampy ustawiony!" },

			{ L_ramp_usage, "Użycie: {b}/ramp <id>{/b} - id od 1 do 6, 0 aby wyłączyć rampy" },

			{ L_tip_01, "~b~~h~~h~TIP: Mozna uzyc komende /pomoc aby otworzyc centrum pomocy" },
			{ L_tip_02, "~b~~h~~h~TIP: Jesli chcesz zapisac tuning swojego pojazdu uzyj /vsave (slot)~n~~b~~h~~h~Mozesz ponownie zaladowac pojazd za pomoca /v (slot)" },
			{ L_tip_03, "~b~~h~~h~TIP: Chcesz miec VIP-a? Teraz mozesz! Wiecej info pod /vip" },
			{ L_tip_04, "~b~~h~~h~TIP: Pamietaj ze w kazdej chwili mozesz zobaczyc swoje osiagniecia oraz statystyki.~n~~b~~h~~h~Uzyj do tego komende /osg" },
			{ L_tip_05, "~b~~h~~h~TIP: Jestes stalym graczem? Pamietaj by ustawic swoj /email" },
			{ L_tip_06, "~b~~h~~h~TIP: Pamietaj iz masz dostepne dodatkowe menu pod klawiszem ~k~~CONVERSATION_YES~" },
			{ L_tip_07, "~b~~h~~h~TIP: Lista teleportow jest dostepna pod /teles" },
			{ L_tip_08, "~b~~h~~h~TIP: Mozesz stworzyc dowolny pojazd! Uzyj komende /v lub /pojazdy" },
			{ L_tip_09, "~b~~h~~h~TIP: Mozesz postawic pojazd swojej mafii za pomoca /v 10 lub /v 11" },
			{ L_tip_10, "~b~~h~~h~TIP: Jesli chcesz miec wlasna mafie to ja zaloz! Uzyj komendy /mafia.create" },
			{ L_tip_11, "~b~~h~~h~TIP: Pamietaj aby zwracac uwage na otoczenie oraz sprytnie szukac gdyz~n~~b~~h~~h~zlota moneta moze byc tuz za rogiem!" },
			{ L_tip_12, "~b~~h~~h~TIP: Chcesz aby za wszelka cene ktos umarl?~n~~b~~h~~h~Wystaw wlasna cene uzywajac /hitman" },
			{ L_tip_13, "~b~~h~~h~TIP: Biorac udzial na zabawach, arenach oraz eventach zdobywasz wiecej exp" },
			{ L_tip_14, "~b~~h~~h~TIP: Chcesz miec odliczanie? uzyj /odlicz~n~~b~~h~~h~Inni takze moga uzyc /odlicz aby dolaczyc do odliczania" },
			{ L_tip_15, "~b~~h~~h~TIP: Uzywajac /bronie mozesz kupic stale bronie dla siebie~n~~b~~h~~h~Uzyj /ammo aby dokupic amunicje do stalej broni" },
			{ L_tip_16, "~b~~h~~h~TIP: Za pomoca /pmblock oraz /pmunblock mozesz decydowac~n~~b~~h~~h~kto ma prawo wysylac prywatne wiadomosci do Ciebie!" },
			{ L_tip_17, "~b~~h~~h~TIP: Za pomoca /ramp (id) mozesz ustawic sobie skocznie~n~~b~~h~~h~ktora sie pojawia w pojezdzie kiedy sie naciska przycisk strzelania" },
			{ L_tip_18, "~b~~h~~h~TIP: Lista wszystkich zabaw jest dostepna pod /games" },
			{ L_tip_19, "~b~~h~~h~TIP: W wyscigach czasami mozna sie naprawiac/respawnowac/flipowac za pomoca:~n~~b~~h~~h~~k~~CONVERSATION_YES~/~k~~VEHICLE_HORN~/~k~~TOGGLE_SUBMISSIONS~" },
			{ L_tip_20, "~b~~h~~h~TIP: Uzyj ~k~~CONVERSATION_YES~ aby przywolac podreczne menu~n~~b~~h~~h~Steruj nim za pomoca ~k~~GROUP_CONTROL_BWD~ i ~k~~CONVERSATION_NO~" },
			{ L_bounty_noheadsonline, "\tBrak" },
			{ L_bounty_preinfo, "Oto lista graczy online za których jest cena za głowę:\n\n" },
			{ L_bounty_title, "Lista Zmarłych" },
			{ L_givebounty_usage, "Użycie: {b}/hitman <id/nick> <ilość kasy>" },
			{ L_givebounty_pricechange, "Wartosc glowy gracza {b}%s{/b} wzrosla do {b}$%I64u{/b}!" },
			{ L_givebounty_headtagged, "{b}$%I64u{/b} zostało wystawione na Twoją głowę przez {b}%s{/b} z własnego konta, stan: {b}$%I64u{/b}" },
			{ L_givebounty_sendtag, "Przelew w wysokości {b}$%I64u{/b} na głowę podanego gracza udał się sukcesem, stan ceny głowy: {b}$%I64u{/b}" },
			{ L_SGMI_title, "Informacje o Autorach / GameModzie oraz Kontakt" },
			{ L_SGMI_info, "\
{FF0000}Założyciel serwera & scripter		:		{FFFFFF}Rafał 'Gamer_Z' Grasman\n\
{FF0000}Ostatni update GameModa			:		{FFFFFF}%s\n\
\n\
{FF0000}Lista użytych bibliotek:		{FFFFFF}\n\
\n\
\t\t{65DB88}Nazwa Biblioteki				{D1BB2C}Autor\n\
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
{FF0000}Kontakt					:		{FFFFFF}exe24.info / gz0.nl\n\
{FF0000}Prowider serwera dedykowanego		:		{FFFFFF}OVH.NL\n\
{FF0000}Prowider hostingu forum & domeny		:		{FFFFFF}STRATO.NL\n\
\n\
\n\
\n\
Specjalne podziękowania dla:\n\
IceCube, Oski20, C2A1,\n\
West, BlackPow3R, Arcade\n"
			},

			{ L_sentrygun_created, "SentryGun stworzony" },
			{ L_sentrygun_usage, "Użycie {b}/sentryadd <zasięg> <id/nick (właściciel, 0 dla nikogo)>" },
			{ L_sentrygun_invalid_action, "Niepoprawna akcja" },
			{ L_sentrygun_notfound, "ID sentry nie znaleziono" },
			{ L_sentrygun_manage_usage, "Użycie {b}/sgm <turret ID> <akcja> <parametry>" },
			{ L_sentrygun_manage_nexuse, "Użycie {b}/sgm " },

			{ L_SendPM_ownID, "To Twoje ID!" },
			{ L_SendPM_pmAreOff, "Wyłączyłeś(aś) wiadomości PW, załącz je aby móc wysłać wiadomość" },
			{ L_SendPM_receiverIsBlockedInfo, "Podany gracz w tej chwili nie posiada możliwości prowadzenia rozmowy poprzez PW lub zablokowaliście się nawzajem" },
			{ L_SendPM_tooLong, "Za długa wiadomość, proszę skrócić wiadomość" },
			{ L_pm_wasMutedByAdmin, "Twoje PW zostały uciszone przez administratora" },
			{ L_pm_turnOn, "Załączyłeś(aś) wiadomości PW" },
			{ L_pm_turnOff, "Wyłączyłeś(aś) wiadomości PW" },
			{ L_pm_usage, "Użycie: {b}/pm <id/nick> <wiadomość>{/b} aby wysłać, {b}/pm W{/b} aby wyłączyć PW, {b}/pm Z{/b} aby załączyć PW" },
			{ L_pm_usage2, "           {b}/odp <wiadomość>{/b} (lub {41BECC}`<wiadomość>{/b})  aby odpowiedzieć na ostatnią wiadomość" },
			{ L_reply_cannotFoundLastID, "Nie można odnaleźć ostatniego gracza z którym prowadziłeś(aś) rozmowę" },
			{ L_reply_cannotBeEmpty, "Wiadomość nie może być pusta" },
			{ L_pmblock_playerBlockedInfo, "Gracz zablokowany!" },
			{ L_pmblock_usage, "Użycie: {b}/pmblock <id/nick>" },
			{ L_pmunblock_playerBlocked, "Gracz odblokowany!" },
			{ L_pmunblock_usage, "Użycie: {b}/pmunblock <id/nick>" },
			{ L_mm_tdm_teamselector_red, "Jestes w druzynie ~r~Czerwonych" },
			{ L_mm_tdm_teamselector_blue, "Jestes w druzynie ~b~Niebieskich" },

			{ L_help_timespan, "%02d dni %02d godz. %02d min. %02d sek." },
			{ L_help_stats_exe24mod_on, "{00FF00}Załączony" },
			{ L_help_stats_exe24mod_off, "{FF0000}Brak" },
			{ L_help_stats_infostringadmin,
"{FFFFFF}Nick						{FFA321}%s\n\
\n\
{FFFFFF}Kasa						{FFA321}$%I64d\n\
{FFFFFF}Respekt					{FFA321}%I64d\n\
{FFFFFF}Poziom					{FFA321}%d\n\
{FFFFFF}Postęp do poziomu %4d			{FFA321}%d%%\n\
{FFFFFF}Zgonów					{FFA321}%I64u\n\
{FFFFFF}Samobójstw					{FFA321}%I64u\n\
{FFFFFF}Zabójstw					{FFA321}%I64u\n\
{FFFFFF}Cena za głowę				{FFA321}$%I64u\n\
{FFFFFF}Znalezione złote monety			{FFA321}%3d/%3d\n\
{FFFFFF}Wyrzuceń przez personel			{FFA321}%I64u\n\
{FFFFFF}Ostrzeżeń					{FFA321}%I64u\n\
{FFFFFF}Czas aktualnej gry				{FFA321}%s\n\
{FFFFFF}Czas spędzony na serwerze			{FFA321}%s\n\
{FFFFFF}Skin model ID					{FFA321}%d\n\
{FFFFFF}Mafia						{FFA321}%s\n\
\n\
{FFFFFF}Konto premium do				{FFA321}%s\n\
{FFFFFF}Uwięziony do					{FFA321}%s\n\
{FFFFFF}Data rejestracji				{FFA321}%s\n\
{FFFFFF}Ostatni ban trwał do				{FFA321}%s\n\
{FFFFFF}Ostatni powód banu				{FFA321}%s\n\
{FFFFFF}Ostatni banujący				{FFA321}%s\n\
{FFFFFF}Stan dodatku  eXe24+				%s\n\
\n\
\n\
{FFFFFF}Rangi ({00FF00}Tak{FFFFFF}/{FF0000}Nie{FFFFFF}):\n\n\
\t\t\t\t\t\t{00FF00}Gracz\n\
\t\t\t\t\t\t{%06X}VIP (stały gracz)\n\
\t\t\t\t\t\t{%06X}Sponsor\n\
\t\t\t\t\t\t{%06X}Moderator\n\
\t\t\t\t\t\t{%06X}Admin\n\
\t\t\t\t\t\t{%06X}Wiceprezes\n\
\t\t\t\t\t\t{%06X}Prezes\n\
\t\t\t\t\t\t{%06X}Współwłaściciel serwera\n\
\t\t\t\t\t\t{%06X}Właściciel serwera\n\
\n\
\n\
{FFFFFF}Sprawdź innych graczy którzy są online za pomocą {41BECC}/stats <id/nick>{FFFFFF}! :)"
			},
			{ L_help_stats_infostringuser,
"{FFFFFF}Nick						{FFA321}%s\n\
\n\
{FFFFFF}Kasa						{FFA321}$%I64d\n\
{FFFFFF}Respekt					{FFA321}%I64d\n\
{FFFFFF}Poziom					{FFA321}%d\n\
{FFFFFF}Postęp do poziomu %4d			{FFA321}%d%%\n\
{FFFFFF}Zgonów					{FFA321}%I64u\n\
{FFFFFF}Samobójstw					{FFA321}%I64u\n\
{FFFFFF}Zabójstw					{FFA321}%I64u\n\
{FFFFFF}Cena za głowę				{FFA321}$%I64u\n\
{FFFFFF}Znalezione złote monety			{FFA321}%3d/%3d\n\
{FFFFFF}Wyrzuceń przez personel			{FFA321}%I64u\n\
{FFFFFF}Czas aktualnej gry				{FFA321}%s\n\
{FFFFFF}Czas spędzony na serwerze			{FFA321}%s\n\
{FFFFFF}Skin model ID					{FFA321}%d\n\
{FFFFFF}Mafia						{FFA321}%s\n\
\n\
{FFFFFF}Konto premium do				{FFA321}%s\n\
{FFFFFF}Data rejestracji				{FFA321}%s\n\
{FFFFFF}Stan dodatku  eXe24+				%s\n\
\n\
\n\
{FFFFFF}Rangi ({00FF00}Tak{FFFFFF}/{FF0000}Nie{FFFFFF}):\n\n\
\t\t\t\t\t\t{00FF00}Gracz\n\
\t\t\t\t\t\t{%06X}VIP (stały gracz)\n\
\t\t\t\t\t\t{%06X}Sponsor\n\
\t\t\t\t\t\t{%06X}Personel Serwera\n\
\n\
\n\
{FFFFFF}Sprawdź innych graczy którzy są online za pomocą {41BECC}/stats <id/nick>{FFFFFF}! :)"
			},
			{ L_help_stats_title, "Statystyki gracza %s" },
			{ L_help_account_title, "Pomoc - Konto" },
			{ L_help_account_text,
"{41BECC}/rejestracja{FFFFFF} - rejestrujesz konto\n\
{41BECC}/newpass{FFFFFF} - zmieniasz hasło\n\
{41BECC}/email{FFFFFF} - Ustawienia e-mail (tylko i wyłącznie dla odzyskiwania dostępu do konta)\n"
			},
			{ L_help_monety_title, "Pomoc - Złote monety" },
			{ L_help_monety_text,
"{FFFFFF}Złote Monety to duże żółte magnesy ukryte w różnych zakątkach serwera (tylko i wyłącznie na głównym świecie)\n\n\
Gdy zobaczysz jedną z Złotych Monet, Twoim celem będzie trafienie ją bronią palną (Sniper, Deser Eagle, itp).\n\
Jeśli trafisz - odkrywasz monetę na stałe oraz zdobywasz pieniądze, respekt oraz level.\n\
Zdobywanie monet jest jednym z wymogów aby zostać VIP-em na serwerze. Więcej informacji pod komendą {41BECC}/vip{FFFFFF}\n\n\n\n\
P.S. Większość Złotych Monet jest bardzo dobrze ukryta! Miej oko otwarte na detale :)\n\n\
ProTip: Chcesz w tej chwili zdobyć jedną Złotą Monetę? Wpisz {41BECC}/zmtp{FFFFFF} (O ile tą już nie postrzeliłeś/aś)\n\n\n\n\
Informacje dodatkowe: Podsumowane zarobki z zebrania wszystkich monet to ~$20,500,00 oraz ~2,500 Respektu" },
			{ L_help_plus_title, "Pomoc - Dodatek eXe24+" },
			{ L_help_plus_text,
"eXe24+ to całkowicie darmowy dodatek do gry Grand Theft Auto San Andreas Multiplayer stworzony przez ekipę exe24.info.\n\
Dodatek ten pozwala na dodatkowe możliwości dla graczy oraz serwera (TYLKO na zrobionych do tego arenach z gwiazdką) aby osiągnąc następny poziom zabawy!\n\
\n\
Dodatek pozwala między innymi na:\n\
\n\
\t• Latanie każdym pojazdem na serwerze (tryb samolot & helikopter)\n\
\t• Opcja aby nigdy nie spaść z rowera / motora itp.\n\
\t• Break Dance\n\
\t• Prawdziwy GodMode który serwer może dać\n\
\t• Prefect Handling można załączyć\n\
\t• Usunięcie limit prędkości samolotów oraz limity wysokości\n\
\t• Super prędkość(speed boost)!\n\
\t• Super hamulce\n\
\t• Prowadzenie pojazdów pod/nad wodą (tak samo i chodzenie)\n\
\t• Mega skakanie!\n\
\t• SpiderWheels!\n\
\t• I dużo więcej możliwości...\n\
\n\
\n\
\n\
eXe24+ wymaga klient SA-MP o wersji równej 0.3z-R1(!)\n\
Sciągnąć ten oto klient można z oficjalnej strony SA-MP: http://files.sa-mp.com/sa-mp-0.3z-R1-install.exe\n\
\n\
\n\
Jeśli spełniasz wszystkie wymagania wystarczy pobrać nasz dodatek i dać go do folderu z SA-MPem oraz GTA:\n\
Jeśli GTA znajduje się w C:\\Program Files(x86)\\Rockstar Games\\GTA San Andreas\\, to tam nalezy umieścić sciągnięty plik.\n\
\n\
\n\
Pamiętaj aby zrobić kopie zapasowe plików które zostają przepisane!\n\
\n\
Dodatek jest do ściągnięcia na http://plus.exe24.info/\n\
\n\
Życzymy miłej gry na serwerze!"
			},
			{ L_help_vip_title, "Pomoc - VIP" },
			{ L_help_vip_text,
"{EAF20A}VIP {FFFFFF}- czyli stały gracz na serwerze to ranga którą serwer przyznaje graczom {00FF00}automatycznie{FFFFFF}.\n\
Rangę dostaję każdy gracz, bez względu na jakiekolwiek okoliczności, o ile spełnia kolejne wymagania:\n\n\
\tSpędzony czas na serwerze wynosi 30 godzin lub więcej (Twój spędzony czas: %I64u godzin)\n\
\tPoziom jest większy lub równy 10 (Twój poziom: %.0f)\n\
\tZostał podany e-mail ({41BECC}/email{FFFFFF}) oraz jest zweryfikowany (Status: %s)\n\
\tKonto zarejestrowane jest ponad 21 dni (Twoje konto jest zarejestrowane %I64u dni)\n\
\tZnaleziono przynajmniej 30 złotych monet (Znaleziono dotąd %d złotych monet)\n\
\n\
\n\
W chwili w której spełnisz wymagania musisz się ponownie zalogować poprzez wyjscie z serwera oraz ponowne wejscie.\n\
Ranga VIP zostanie automatycznie przyznana przy zalogowaniu.\n\
\n\
\n\
Dodatki VIP-a:\n\
\t• Traci tylko 10%% pieniędzy jeśli umrze (normalny gracz 33%%)\n\
\t• Dostaje minimalnie $10,000 oraz 3 respekt za każde zabójstwo (normalny gracz $7,500 oraz 2 respekt)\n\
\t• Może wynajmować domki dłużej\n\
\t• Ma własny czat na którym może pisać z innymi VIPami"
			},
			{ L_help_vip_mailverified, "Zweryfikowany" },
			{ L_help_vip_mailnotverified, "Nie zweryfikowano e-maila" },
			{ L_help_vehicle_title, "Pomoc - Pojazdy" },
			{ L_help_vehicle_text,
"{41BECC}/v{FFFFFF} - Tworzysz własny pojazd\n\
{41BECC}/pojazdy{FFFFFF} - Lista pojazdów\n\
{41BECC}/vkolor{FFFFFF} - Zmieniasz kolor pojazdu\n\
{41BECC}/vpaint{FFFFFF} - Zmieniasz paintjob pojazdu\n\
{41BECC}/vsave{FFFFFF} - Zapisujesz pojazd w którym się znajdujesz (wraz z tuningiem, paintjobami oraz kolorami)\n\
{41BECC}/fix{FFFFFF} - Napawiasz swój pojazd\n\
{41BECC}/f{FFFFFF} - Odwracasz pojazd który leży na dachu na koła\n"
			},
			{ L_help_chat_title, "Pomoc - Chat" },
			{ L_help_chat_text,
"{41BECC}|<text>{FFFFFF} - Chat w pobliżu\n\
{41BECC}\\<text>{FFFFFF} - Chat drużynowy\n\
{41BECC}!<text>{FFFFFF} - Chat mafii\n\
{41BECC}@<text>{FFFFFF} - Chat adminów\n\
{41BECC}#<text>{FFFFFF} - Chat VIPów\n\
{41BECC}$<text>{FFFFFF} - Chat Sponsorów\n\
{41BECC}&<text>{FFFFFF} - Chat Moderatorów\n\
{41BECC}`<text>{FFFFFF} - Odpowiedz na ostatnią PW\n"
			},
			{ L_help_staff_title, "Pomoc - Personel" },
			{ L_help_staff_text,
"{41BECC}/report{FFFFFF} - zgłaszasz gracza personelu na serwerze\n\
{41BECC}/personel{FFFFFF} - Lista personelu online na serwerze\n"
			},
			{ L_help_global_title, "Pomoc - Gra" },
			{ L_help_global_text,
"{41BECC}/skin{FFFFFF} - ustawiasz sobie skin\n\
{41BECC}/givecash{FFFFFF} - dajesz innemu graczu fundusze\n\
{41BECC}/giverespect{FFFFFF} - dajesz innemu graczu respekt\n\
{41BECC}/hitman{FFFFFF} - wystawiasz cenę za głowę gracza\n\
{41BECC}/hitman.lista{FFFFFF} - Pokazuje graczy online ktrórzy mają wystawioną cenę za głowę\n\
{41BECC}/anim{FFFFFF} - pokazuję listę animacji\n\
{41BECC}/clearchat{FFFFFF} - czyści czat\n\
{41BECC}/savepos{FFFFFF} - zapisuje Twoją pozycję\n\
{41BECC}/loadpos{FFFFFF} - ładuje ostatnio zapisaną pozycję\n\
{41BECC}/kolor{FFFFFF} - ustawiasz Sobie stały kolor\n\
{41BECC}/ramp{FFFFFF} - ustawiasz jakią rampę chcesz na KEY_ACTION dla pojazdów\n\
{41BECC}/odlicz{FFFFFF} - rozpoczynasz odliczanie\n\
{41BECC}/tpto{FFFFFF} - teleportujesz się do gracza\n\
{41BECC}/odlicz{FFFFFF} - teleportujesz gracza do siebie\n\
{41BECC}/kill{FFFFFF} - zabijasz się\n\
{41BECC}/respawn{FFFFFF} - odnawiasz spawn\n\
{41BECC}/przelew{FFFFFF} - wpłacasz(& wypłacasz) bezpośrednio na konto bankowe\n\
{41BECC}/lotto{FFFFFF} - graj i wygrywaj!\n\
{41BECC}/osg{FFFFFF} -  Zobacz osiągnięcia oraz dodatkowe statystyki (swoje lub innego gracza)\n\
{41BECC}/stats{FFFFFF} - Zobacz statystyki (swoje lub innego gracza)\n\
{41BECC}/ghelp2{FFFFFF} -  Następna strona komend\n"
			},
			{ L_help_global2_title, "Pomoc - Gra" },
			{ L_help_global2_text,
"{41BECC}/all{FFFFFF} - ulecza, daje pancerz oraz naprawia pojazd\n\
{41BECC}/ulecz{FFFFFF} - uleczasz się\n\
{41BECC}/pancerz{FFFFFF} - dodajesz sobie pancerz\n\
{41BECC}/ck{FFFFFF} - wstajesz jeśli się zdarzy carkill na Tobie\n\
{41BECC}/interior{FFFFFF} - Pokazuje obecje interior id\n\
{41BECC}/virtualworld{FFFFFF} - Pokazuje obecny Virtual World\n\
{41BECC}/animid{FFFFFF} - Pokazuje obecne ID Animacji\n\
{41BECC}/language{FFFFFF} - Wybierasz nowy język wyświetlania się informacji\n\
"
			},
			{ L_help_virtualworld, "Twój virtual world: %d" },
			{ L_help_animationid, "Twóje obecne AnimID: %d" },
			{ L_help_acmd_title, "Pomoc - Komendy Admina" },
			{ L_help_acmd_text,
"{41BECC}/spec{FFFFFF} - podglądasz gracza\n\
{41BECC}/specoff{FFFFFF} - respawnujesz się\n\
{41BECC}/agiverespect{FFFFFF} - dajesz graczowi respekt\n\
{41BECC}/agivecash{FFFFFF} - dajesz graczowi kasę\n\
{41BECC}/agiveexp{FFFFFF} - dajesz graczowi exp\n\
{41BECC}/setpass{FFFFFF} -  Ustawiasz graczowi nowe hasło\n\
{41BECC}/promote{FFFFFF} - Dajesz graczowi rangi\n\
{41BECC}/depromote{FFFFFF} -  Zabierasz graczowi rangi\n\
{41BECC}/eye{FFFFFF} -  Podglądasz komendy gracza\n\
{41BECC}/eyecmdall{FFFFFF} -  Podglądasz komendy wszystkim graczom\n\
{41BECC}/agmh{FFFFFF} -  Ostatnie 32 wiadomości czatu wysłane przez wszystkich\n\
{41BECC}/apmh{FFFFFF} -  Ostatnie 24 wiadomości czatu wysłane przez gracza\n\
{41BECC}/apph{FFFFFF} -  Ostatnie 24 prywatne wiadomości gracza\n\
{41BECC}/apch{FFFFFF} -  Ostatnie 24 wpisane komendy gracza\n\
{41BECC}/players{FFFFFF} -  lista graczy wraz z ip\n\
{41BECC}/kick{FFFFFF} -  Wyrzucasz gracza\n\
{41BECC}/ban{FFFFFF} -  Banujesz gracza\n\
{41BECC}/unban{FFFFFF} -  Odbanowujesz gracza\n\
{41BECC}/unbanip{FFFFFF} -  Odbanowujesz ip\n\
{41BECC}/a{FFFFFF} -  piszesz na czacie adminów\n\
{41BECC}/m{FFFFFF} -  piszesz naczacie moderatorów\n\
{41BECC}/aclch{FFFFFF} -  czyści chat ogólny każdemu\n\
{41BECC}/eyeip{FFFFFF} -  załącza podgląd ip graczy którzy wchodzą\n\
{41BECC}/acmd2{FFFFFF} -  Ciąg dalszy komend admina\n\
"
			},
			{ L_help_acmd2_title, "Pomoc - Komendy Admina 2" },
			{ L_help_acmd2_text,
"{41BECC}/aexplode{FFFFFF} - powoduje ekspozję u gracza\n\
{41BECC}/aslap{FFFFFF} - odejmuje graczowi 10 hp oraz ustawia jego pozycję o 0.5 gta jednostek wyżej\n\
{41BECC}/apaddhp{FFFFFF} - dodaje/odejmuje graczowi hp\n\
{41BECC}/apsethp{FFFFFF} - ustawia graczowi hp\n\
{41BECC}/apaddarmor{FFFFFF} - dodaje/odejmuje graczowi pancerz\n\
{41BECC}/apsetarmor{FFFFFF} - ustawia graczowi pancerz\n\
{41BECC}/apvaddhp{FFFFFF} - dodaje/odejmuje hp pojazdu w którym jest gracz\n\
{41BECC}/apvsethp{FFFFFF} - ustawia hp pojazdu w którym jest gracz\n\
{41BECC}/atp{FFFFFF} - teleportujesz jednego gracza do drugiego\n\
{41BECC}/dl{FFFFFF} - Pokazuje dodatkowe info o pojazdach\n\
{41BECC}/banip{FFFFFF} - Banuje podane IP\n\
{41BECC}/banhost{FFFFFF} - Banuje hosta bazując na IP\n\
{41BECC}/unbanhost{FFFFFF} - Odbanowuje hosta bazując na IP\n\
{41BECC}/sentryadd{FFFFFF} - Tworzy Sentry Turret\n\
{41BECC}/sgm{FFFFFF} - Zarządzanie Sentry Turretami (range,exp,delete,setpos,owner,setarea,setareaex,ownermafia)\n\
{41BECC}/emulate{FFFFFF} - piszesz na czacie lub uzywasz komendę jako gracz\n\
{41BECC}/emulate-all{FFFFFF} - piszesz na czacie lub uzywasz komendę jako wszyscy\n\
{41BECC}/ips{FFFFFF} - zobacz liste IP które mają więcej niż jednego gracza\n\
{41BECC}/ann{FFFFFF} - pokazujesz globalny tekst na ekranie\n\
"
			},
			{ L_help_mafia_title, "Pomoc - Mafia" },
			{ L_help_mafia_text,
"{41BECC}/mafia.create{FFFFFF} - Tworzysz własną mafie\n\
{41BECC}/mafia.q{FFFFFF} - Opuszczasz mafie\n\
{41BECC}/mafia.delete{FFFFFF} - Usuwasz mafie\n\
{41BECC}/mafia.v{FFFFFF} - Zapisujesz pojazd jako pojazd mafii\n\
{41BECC}/mafia.i{FFFFFF} - Zapraszasz gracza do mafii\n\
{41BECC}/mafia.j{FFFFFF} - Akceptujesz zaproszenie do mafii\n\
{41BECC}/mafia.gc{FFFFFF} - Dajesz członku mafii kasę\n\
{41BECC}/mafia.gr{FFFFFF} - Dajesz członku mafii respekt\n\
{41BECC}/mafia.dc{FFFFFF} - Darujesz mafii kasę\n\
{41BECC}/mafia.dr{FFFFFF} - Darujesz mafii respekt\n\
{41BECC}/mafia.c{FFFFFF} - Zmieniasz kolor mafii\n\
{41BECC}/mafia.k{FFFFFF} - Wyrzucasz gracza z mafii\n\
{41BECC}/mafia.s{FFFFFF} - Ustawiasz graczowi rangę w mafii\n\
{41BECC}/mafia.l{FFFFFF} - Lista członków podanej mafii\n\
{41BECC}/mafia.p{FFFFFF} - Ustawiasz które rangi jakie komendy mogą używać\n\
{41BECC}/mafia.skin{FFFFFF} - ustawiasz skin mafii\n\
{41BECC}/mafia.spawn{FFFFFF} - ustawiasz spawn mafii\n\
{41BECC}/mafia.spawn.on{FFFFFF} - ustawiasz swój spawn na spawn mafii\n\
{41BECC}/mafia.spawn.off{FFFFFF} - przywracasz spawn dla normalnych graczów\n\
{41BECC}/mafia.skin.on{FFFFFF} - używasz skin mafii\n\
{41BECC}/mafia.skin.off{FFFFFF} - używasz własny skin\n\
{41BECC}/mafias{FFFFFF} - Lista mafii które są online na serwerze\n"
			},
			{ L_help_pm_title, "Pomoc - Prywatne Wiadomości" },
			{ L_help_pm_text,
"{41BECC}/pm{FFFFFF} - Wysyłasz prywatną wiadomość do gracza\n\
{41BECC}/pmblock{FFFFFF} - Blokujesz graczowi możliwość wysyłania PM do Ciebie\n\
{41BECC}/pmunblock{FFFFFF} - Oddajesz graczowi możliwość wysyłania PM do Ciebie\n\
{41BECC}/odp{FFFFFF} - Odpowiadasz na ostatnią prywatną wiadomość\n"
			},
			{ L_help_teleport_title, "Lista Teleportów" },
			{ L_help_serverinfo_title, "Opis Serwera" },
			{ L_help_serverinfo_text,
"\
{FFFFFF}Gamer_Z eXtreme Party 24/7 - czyli w skrócie {41BECC}eXe24{FFFFFF}\n\
\n\
Serwer ten jest typu {E88215}Modern DeathMatch{FFFFFF}. \n\
Oznacza to iż można zabijać wszystkich do woli oraz w dowolny sposób.\n\
\n\
Jako jeden z nielicznych serwerów zezwalamy na:\n\
\n\
\t* respawn pojazdów typu czołg, hunter oraz hydrę (żadnych haczyków, piszesz {41BECC}/v rhino{FFFFFF} i masz!)\n\
\t* zabijanie graczy z DB, CK, HK, itp jest {00FF00}dozwolone{FFFFFF}\n\
\t* prawdziwą zabawę w sposób jaki chcesz Ty!\n\
\n\
Naszym priorytetem jest zabawa oraz miła atmosfera podczas gry oraz poza nią.\n\
Dlatego też nie jesteśmy nastawieni na zdobywanie funduszy - wszystko na serwerze\n\
jest za darmo! Nie spotkasz tutaj przekrętów typu pay2play lub pay2win!\n\
Nie pozwolimy na takie coś!\n\
\n\
Dostępne są również dodatki oraz zabawy które odbiegają od trybu DeathMatch:\n\
\n\
\t* Wyścigi\n\
\t* Mini-misje\n\
\t* Osiągnięcia\n\
\t* Wyzwania\n\
\t* Oraz dużo więcej... ({41BECC}/pomoc {00FF00}:){FFFFFF} )\n\
\n\
Nie tylko oferujemy grę dla Ciebie jako osobie, ale również jest możliwość\n\
grania drużynowo. Dla tego celu zostały na serwer dodane Mafię. Są to stałe grupy\n\
na serwerze które się nie usuwają. Maksymalna ilość członków która się może znajdować\n\
w jednej mafii to 128! Z taką ilością gra drużynowa napewno nabierze znaczenia.\n\
\n\
Wykonanie GameMode serwera jest autorkie - kod gry jest strzeżony oraz prywatny, nigdzie indziej\n\
nie zobaczysz tego samego wykonania co tutaj. \n\
Pisany z myślą o optymalność dla trybu DeathMatch możemy jako jedyny serwer zagwarantować\n\
absolutnie zero lagów przy graniu! \n\
Postawiony na maszynie dedykowanej oraz zabezpieczonej sieci zagwarantuje stabilność grania.\n\
GameMod pisany w C++ zaś zagwarantuje jak najszybszą mozliwość wykonywania poleceń.\n\
\n\
Na tym serwerze o wszystko zostało jak najlepiej zadbane - co do najmniejszego szczegółu. \n\
\n\
Witamy u Mistrzów - bo my wiemy jak stworzyć prawdziwą zabawę!\n\
\n\
Cała ekipa eXe24 życzy wszystkim graczom miłej zabawy!\n\
"
			},
			{ L_help_rules_title, "Regulamin Serwera" },
			{ L_help_rules_text,
"\
Zasady obowiązujące na serwerze Gamer_Z eXtreme Party:\r\n\
\r\n\
Zakazane jest:\r\n\
\r\n\
§1. Używanie jakichkolwiek programów/dodatków/modułów/scriptów/modyfikacji ułatwiających rozgrywkę online (w tym mody CLEO!); (ban)\r\n\
§2. Wykorzystywanie bugów/błędów w grze GTA San Andreas; (kick lub ban)\r\n\
§3. Wyzywanie innych graczy, przejawy chamstwa; (uciszenie, kick, ban)\r\n\
§4. Uciekanie z walki za pomocą alt+tab lub pauzy; (kick)\r\n\
§5. Podszywanie się pod innych graczy lub personel serwera; (za obydwa grozi ban)\r\n\
§6. Ustawianie sobie obrażliwych/wulgarnych/reklamujących nicków/tagów/nazw mafii/opisów/itp; (kick, ban / usunięcie mafii)\r\n\
§7. Próby złamania hasła RCON; (ban na ip/sieć przez firewall, apelacja tego banu jest NIEMOŻLIWA)\r\n\
§8. Crashowanie serwera (jest to przestępstwo wg. prawa); (ban na ip/sieć przez firewall, apelacja tego banu jest NIEMOŻLIWA)\r\n\
§9. Reklamowanie konkurencyjnych serwerów, lub stron/forów konkurencyjnych serwerów SAMP (Nawet w Prywatnych Wiadomościach); (ban)\r\n\
§10. Przekazywanie informacji które pozwoliłyby innym graczom złamać punkt §1; (ban)\r\n\
§11. Współpracowanie z cheaterem poprzez np. niezgłaszanie raportów, bronienie go, stawianie się za nim/ją etc; (ban)\r\n\
§12. Namawianie do wpisywania komend, które mogą utrudnić grę graczowi (/q /kill /exit itp); (kick lub ban)\r\n\
§13. Spamowanie na chacie głównym, PM, chacie Mafii/Drużynowym, oraz spamowanie komendami; (uciszenie, kick lub ban)\r\n\
§14. Dawanie się zabić po to aby druga osoba zyskała respect lub exp; (ban dla obydwóch graczy)\r\n\
§15. Pisanie fałszywych lub bezsensownych reportów; (uciszenie na komendy lub/oraz czat)\r\n\
§16. Uniemożliwianie prowadzenia różnych imprez typu: WG, Zloty, Imprezy tuningowe itp. pod warunkiem że prowadzi je administrator oraz zostały ogłoszone przed rozpoczęciem; (kick)\r\n\
$17. Odchodzenie od komputera (afk) podczas pobytu na jednej z rozrywek (kick);\r\n\
§18. Przeszkadzanie w prowadzonych testach przez adminów po wcześniejszym poinformowaniu graczy o tym fakcie; (kick)\r\n\
§19. Wymuszanie danych do konta innych graczy lub administracji; (ban)\r\n\
"
			},
			{ L_help_house_title, "Pomoc - Domki na Serwerze" },
			{ L_help_house_text,
"{FFFFFF}Na serwerze można znaleść posiadłości o tematyce domków.\n\
Każdy gracz może wynająć Sobie jeden domek dla Siebie oraz nim zarządzać.\n\
\n\
Aby wynająć mieszkanie trzeba do drzwi jakiegokolwiek domu podejść który ma pickup oraz wpisać {41BECC}/dom{FFFFFF}\n\
Aby się teleportować do swojego domku wpisz {41BECC}/dom tp"
			},
			{ L_help_center_title, "Centrum Pomocy oraz Informacji (CPI)" },
			{ L_help_center_text,
"{41BECC}/vhelp{FFFFFF} - Komendy dotyczące pojazdów (tworzenie pojażdów, modyfikacja, itp)\n\
{41BECC}/pmhelp{FFFFFF} - Komendy dotyczące prywatne wiadomości (wysyłanie, blokady, itp)\n\
{41BECC}/khelp{FFFFFF} - Komendy dotyczące konta (rejestracja, hasło, itp)\n\
{41BECC}/tphelp{FFFFFF} - Lista teleportów\n\
{41BECC}/chhelp{FFFFFF} - Komendy interakcji z personelem serwera\n\
{41BECC}/ghelp{FFFFFF} - Komendy użyteczne podczas gry\n\
{41BECC}/mhelp{FFFFFF} - Komendy mafii\n\
{41BECC}/zhelp{FFFFFF} - Różne zabawy\n\
{41BECC}/vip{FFFFFF} - Informacje na temat kont VIP (np. jak zdobyć?)\n\
{41BECC}/zmhelp{FFFFFF} - Informacje na temat Złotych Monet\n\
{41BECC}/chathelp{FFFFFF} - Pomoc w pisaniu na chacie\n\
{41BECC}/domhelp{FFFFFF} - Pomoc z domkami do wynajęcia\n\
{41BECC}/opis-serwera{FFFFFF} - Opis/biografia/przedstawienie serwera (jak kto uważa :) )\n\
{41BECC}/napisy{FFFFFF} - Napisy 'końcowe'\n\
{41BECC}/plus{FFFFFF} - informacje o dodatku eXe24+\n\
{41BECC}/regulamin{FFFFFF} - Regulamin Serwera\n\
Informacje o Autorach / GameModzie, Kontakt\n"
			},
			{ L_help_center_admincmd, "{41BECC}/acmd{FFFFFF} - Komendy Personelu Serwera" },

			{ L_mafia_rank_worker, "Pracownik" },
			{ L_mafia_rank_vicepresident, "Vice-Prezes" },
			{ L_mafia_rank_president, "Prezes" },
			{ L_mafia_rank_owner, "Właściciel" },
			{ L_dbworker_mafia_creationerror, "Nie można stworzyć mafii o takiej nazwie, możliwie już istnieje" },
			{ L_mafia_create_invalidname, "Nazwa mafii posiada niedozwolone znaki." },
			{ L_mafia_create_alreadyexists, "Mafia o podanej nazwie już istnieje" },
			{ L_mafia_create_usage_1, "Użycie: {41BECC}/mafia.create <Nazwa>" },
			{ L_mafia_create_usage_2, "Aby móc założyć mafie Twój level musi wynosić minimalnie %.0f, zapłata wynosi %d respektu oraz $%d" },
			{ L_mafia_quit_ownererror, "Musisz przekazać komuś status właściciela zanim możesz odejść." },
			{ L_mafia_vsave_error, "Nie można zapisać tego pojazdu" },
			{ L_mm_mafia_vsave_success, "Pojazd zapisany" },
			{ L_mafia_vsave_usage_1, "Użycie: {b}/mvsave <slot>{/b} (slot 0 lub 1)" },
			{ L_mafia_vsave_usage_2, "Możesz zrespawnować za pomocą: {41BECC}/v 10 lub /v 11" },
			{ L_mafia_invite_sent, "Otrzymałeś/aś zaproszenie od mafii {b}%s{/b}, wpisz {b}/mafia.join %s{/b} aby dołączyć" },
			{ L_mafia_invite_success, "Zaproszenie wysłane!" },
			{ L_mafia_invite_alreadyinvited, "Gracz już otrzymał zaproszenie" },
			{ L_mafia_invite_alreadyinmafia, "Podany gracz już jest w mafii" },
			{ L_mafia_infive_usage, "Użycie: {b}/mafia.i <id/nick>{/b}, gracz musi być online" },
			{ L_mm_mafia_join_success, "{b}%s{/b} dolaczyl do mafii {b}%s{/b}" },
			{ L_mafia_join_error, "Żadnych członków mafii nie ma online, więc nie można dołączyć" },
			{ L_mafia_join_usage, "Użycie: {b}/mafia.j <nazwa mafii>{/b}, pamiętaj że możesz tylko dołączyć do mafii która wysłała do ciebie zaproszenie" },
			{ L_mafia_givecash_success_1, "Przelew w wysokości $%I64u udał się sukcesem, pozostało: $%I64d" },
			{ L_mafia_givecash_success_2, "$%I64u zostało Tobie zasilone przez %s z konta mafii, pozostało: $%I64d" },
			{ L_mafia_givecash_success_3, "$%I64u zostało Tobie zasilone z konta mafii, pozostało: $%I64u" },
			{ L_mafia_givecash_nocash, "Konto mafii nie posiada dość funduszy" },
			{ L_mafia_player_notinmafiaerror, "Gracz nie jest w Twojej mafii" },
			{ L_mafia_givecash_casherror, "Niepoprawna suma pieniędzy (minimalnie $25,000)" },
			{ L_mafia_givecash_usage, "Użycie: {b}/mafia.gc <id/nick> <ilość kasy>{/b}, stan konta: $%I64u" },
			{ L_mafia_givescore_success_1, "Przelew w wysokości %I64u respektu udał się sukcesem, pozostało: %I64u" },
			{ L_mafia_givescore_success_2, "%I64u respektu zostało Tobie zasilone przez %s z konta mafii, pozostało: %I64u" },
			{ L_mafia_givescore_success_3, "%I64u respektu zostało Tobie zasilone z konta mafii, pozostało: %I64u" },
			{ L_mafia_givescore_noscore, "Konto mafii nie posiada dość respektu" },
			{ L_mafia_givescore_scoreerror, "Niepoprawna suma respektu" },
			{ L_mafia_givescore_usage, "Użycie: {b}/mafia.gr <id/nick> <ilość respektu>{/b}, stan konta: %I64u respektu" },
			{ L_mafia_donatecash_success, "$%I64u zostało zasilone na konto mafii, suma: $%I64u" },
			{ L_mafia_donatecash_error, "Nie posiadasz tyle funduszy" },
			{ L_mafia_donatecash_usage, "Użycie: {b}/mafia.dc <ilość kasy>" },
			{ L_mafia_loaded_success, "Informacja: Z sukcesem załadowano twoją mafię {b}%s{/b} o ID {b}%d{/b}" },
			{ L_mafia_deleted, "Zostales usuniety z mafii" },
			{ L_mafia_kicked, "Mafia zostala usunieta" },
			{ L_mafia_permission_error, "Uprawnienia do tej komendy mafii ma tylko {b}%s{/b} oraz wyższe stopnie" },
			{ L_mafia_spawnoff, "Od teraz nie będziesz się spawnować na pozycji mafii" },
			{ L_mafia_spawnon, "Od teraz będziesz się spawnować na pozycji mafii" },
			{ L_mafia_skinoff, "Od teraz będziesz się spawnować z własnym skinem" },
			{ L_mafia_skinon, "Od teraz będziesz się spawnować z skinem Twojej mafii" },
			{ L_mafia_setspawn, "Spawn mafii został ustawiony na twóją obecną pozycję & kierunek" },
			{ L_mafia_setskin, "Skin mafii został ustawiony na twój obecny styl" },
			{ L_mafia_online_title, "Mafie które są online" },
			{ L_mafia_online_text, "Online mafie na serwerze:\n" },
			{ L_mafia_donatescore_success, "%I64u respektu zostało zasilone na konto mafii, suma: %I64u" },
			{ L_mafia_donatescore_error, "Nie posiadasz tyle respektu" },
			{ L_mafia_donatescore_usage, "Użycie: {b}/mafia.dr <ilość respektu>" },
			{ L_mafia_color_setsuccess, "Kolor ustawiony!" },
			{ L_color_too_dark_error, "Kolor jest za ciemny" },
			{ L_mafia_color_set_usage, "Użycie: {b}/mafia.c <kolor>{/b}, np. {b}/mafia.c 77FFAA{/b}, wybierz kolor na http://colorpicker.com | Obecny: %06X" },
			{ L_mafia_kick_cannotquit, "Jeśli chcesz opuścić mafie, użyj {b}/mafia.q" },
			{ L_mafia_playernotinmafia, "Nie ma takiego gracza w mafii" },
			{ L_mafia_kick_permissionerror, "Nie możesz wyrzucić gracza o wyższej randze" },
			{ L_mafia_kick_success, "Gracza wyrzucono z mafii" },
			{ L_mafia_kick_usage, "Użycie: {b}/mafia.k <id/nick>" },
			{ L_mafia_status_ownererror, "Nie można zmienić ostatniego właściciela mafii" },
			{ L_mafia_status_permissionerror_1, "Nie można zmienić rangi wyższej od siebie" },
			{ L_mafia_status_permissionerror_2, "Nie można dać wyższy level od samego siebie" },
			{ L_mafia_status_success_1, "Twoja ranga została zamieniona na '%s' przez '%s'" },
			{ L_mafia_status_success_2, "Zamieniłeś rangę graczowi '%s' na '%s'" },
			{ L_mafia_status_usage, "Użycie: {b}/mafia.s <id/nick> <ranga(0,1,2,3)>{/b}| 0 - Pracownik, 1 - WicePrezes, 2 - Prezes, 3 - Właściciel" },
			{ L_mafia_permissions_success, "Ustawiono pozwolenia" },
			{ L_mafia_permissions_error, "Nieznane pozwolenie" },
			{ L_mafia_permissions_usage,"\
{FFFFFF}Użycie: {41BECC}/mafia.p <pozwolenie> <ranga(0,1,2,3)>{FFFFFF}\n\
\n\
Pozwolenia:\n\
\tINVITE (Zapraszanie graczy)\n\
\tKICK (Wyrzucanie graczy)\n\
\tSAVECAR (Ustawianie pojazdu mafii)\n\
\tSPAWNCAR (zrespawnowanie pojazdu mafii z /v)\n\
\tCMLVL (kto może zmieniać innej osobie rangę)\n\
\tPERM (używanie tej komendy - zmiana pozwoleń)\n\
\tCOLOR (kto może zmieniać kolor mafii)\n\
\tDMONEY (kto może darować pieniądze)\n\
\tGMONEY (kto może wydawać pieniądze)\n\
\tDRESPCKT (kto może darować respekt)\n\
\tGRESPCKT (kto może wydawać respekt)\n\
\tDELETE (kto może usunąć mafie)\n\
\tSKIN (kto może zmieniać skin mafii)\n\
\tSPAWN (kto może zmieniać spawn mafii)\n\
\n\
Przykład: {41BECC}/mafia.p KICK 0{FFFFFF} pozwala wszystkim wyrzucanie innych członków mafii" 
			},
			{ L_dialog_title_info, "Info" },
			{ L_mafia_lista_name_error, "Mafia o podanej nazwie nie istnieje lub żaden z jej członków nie jest online" },
			{ L_mafia_lista_info, "Stan respektu mafii: %I64u\nStan funduszy mafii: $%I64u\nLista członków mafii '%s':\n" },
			{ L_mafia_lista_info_2, "\n\nMożesz sprawdzić inną mafie używając {41BECC}/mafia.l <nazwa mafii>" },
			{ L_mafia_lista_title, "Mafia - Informacje" },

			{ L_antispam, "Nie spamuj i się nie powtarzaj, zostałeś/aś uciszony(a) na dodatkowe {b}1.0s{/b}" },
			{ L_chatmute, "Jesteś ucziszony, {FFFFFF}jeszcze: %I64us" },
			{ L_cmdchatmute, "Tej komendy nie można użyć kiedy jest się uciszonym na komendy, jeszcze {b}%I64us{/b}" },
			{ L_commandrestriction_game, "Tej komendy nie można użyć podczas tej zabawy (jeśli chcesz wyjść użyj {b}/exit{/b})" },
			{ L_commandrestriction_onlyingame, "Tej komendy można użyć tylko podczas zabawy" },
			{ L_commandrestriction_onlyonfoot, "Komendę można tylko użyć kiedy jest się na piechotę" },
			{ L_commandrestriction_onlyasdriver, "Komendę można tylko użyć kiedy jest się kierowcą" },
			{ L_commandrestriction_onlyinvehicle, "Komendę można tylko użyć kiedy jest się w pojeździe" },
			{ L_commandrestriction_onlyloggedin, "Komendę można tylko użyć kiedy jest się zalogowanym użytkownikiem" },
			{ L_commandrestriction_onlyinmafia, "Komendę można tylko użyć kiedy jest się w mafii" },
			{ L_commandrestriction_notinmafia, "Komendę moża tylko użyć kiedy nie jest się w mafii" },
			{ L_commandrestriction_onlyincar, "Komendę moża tylko użyć kiedy jest się w samochodzie" },
			{ L_commandrestriction_onlyinairplane,  "Komendę moża tylko użyć kiedy jest się w samolocie" },
			{ L_commandrestriction_onlyinhelicopter, "Komendę moża tylko użyć kiedy jest się w helikopterze" },
			{ L_commandrestriction_onlyregisteredanddataloaded, "Nie posiadasz konta lub musisz poczekać na załadowanie danych aby użyć tej komendy" },
			{ L_commandrestriction_afterfight, "Musisz odczekać jeszcze %I64u sekund od ostatnich obrażeń zanim będziesz mógł użyć tę komende" },
			{ L_commandrestriction_spawned, "Musisz być w grze" },
			{ L_commandrestriction_exe24plus_1, "Aby móc wykonać to polecenie musisz posiadać eXe24+ (darmowy dodatek serwera)" },
			{ L_commandrestriction_exe24plus_2, "Więcej informacji jest dostępne na {b}http://plus.exe24.info/{/b} lub wpisz {b}/plus" },
			{ L_commandrestriction_vehorfoot, "Aby móc wykonać to polecenie musisz być na nogach lub w pojeździe" },
			{ L_command_nopermission, "Nie masz uprawnień aby użyć tej komendy" },

			{ L_Yes, "Tak" },
			{ L_No, "Nie" },
			{ L_Cancel, "Anuluj" },
			{ L_Login, "Zaloguj" },
			{ L_Confirm, "Potwierdź" },
			{ L_Help, "Pomoc" },
			{ L_login_dialog_title, "Logowanie" },
			{ L_login_dialog_text, "{FF0000}Konto jest zarejestrowane.\r\n{FFFFFF}Zaloguj się.\r\nWpisz hasło należące do konta, kod REACT konta lub jednorazowy kod do zalogowania:" },
			{ L_registerq_title, "Pytanko" },
			{ L_registerq_text, "{FFFFFF}Nie posiadasz jeszczę konta na serwerze, czy zechciałbyś/ałabyś stworzyć konto?" },
			{ L_register_text, "{FFFFFF}Wymagana minimalna ilość znaków to pięć (5).\r\nWpisz hasło które będzie należeć do konta:" },
			{ L_updatepass_title, "Wybierz hasło"},
			{ L_loginhelp_askmail_title, "Weryfikacja" },
			{ L_loginhelp_askmail_text, "Wpisz e-mail który jest zweryfikowany na tym koncie:" },
			{ L_loginhelp_askmail_button_a, "Sprawdź" },
			{ L_loginhelp_askmail_button_b, "Wyjdź z gry" },
			{ L_loginhelp_antyspam, "Aby zapobiec SPAM, e-mail można wysyłać co 15 minut." },
			{ L_loginhelp_checkmail, "Sprawdź swoją pocztę, powinna ona zawierać e-mail z kodem do jednorazowego zalogowania" },
			{ L_login_noverifiedmail, "To konto nie posiada zweryfikowanego e-maila, jeśli nie potrafisz się zalogować wcisnij F6 oraz użyj komendę /quit" },
			{ L_login_welcomeback, "{FFFFFF}Witamy ponownie.\r\nŻyczymy miłej gry.Tip: Pamiętaj aby ustawić oraz zweryfikować /email aby w razie potrzeby móc odzyskać dostęp do konta!" },
			{ L_importantmessage, "Masz ważne powiadomienie!" },
			{ L_vip_upgrade, "Zostałeś/aś awansowany na VIP-a jako podziękowanie za bycie stałym graczem!" },
			{ L_mm_vip_upgrade, "Zostales/as awansowany na VIP-a jako podziekowanie za bycie stalym graczem!" },
			{ L_information, "Informacja" },
			{ L_register_canceled, "{FFFFFF}Wybrałeś/aś aby nie dokonać rejestracji.\r\nJeśli zmienisz zdanie wpisz {00FF00}/rejestracja{FFFFFF} aby się zarejestrować.\r\nŻyczymy miłej gry." },
			{ L_already_registered, "Już posiadasz konto" },
			{ L_register_success, "{FFFFFF}Rejestracja się powiodła.\r\nTwoję hasło to:\r\n\r\n%s\r\n\r\nŻyczymy miłej gry.\n\n\n\nTip: Pamiętaj aby ustawić oraz zweryfikować /email aby w razie potrzeby móc odzyskać dostęp do konta!" },
			{ L_newpass_success_text, "Twoje nowe hasło zostało ustawione: %s" },
			{ L_newpass_success_title, "Ustawiono nowe hasło" },
			{ L_manageemail_text, "{%06X}Zmień E-Mail\n{%06X}Wykonaj weryfikację E-Maila\n{%06X}Wyślij nową weryfikację E-Maila\n" },
			{ L_manageemail_title, "Opcje zarządzania E-Mailem" },
			{ L_email_action_change_title, "Zmiana adresu E-Mail" },
			{ L_email_action_change_text_a, "Niepoprawny adres E-Mail!\n\nWpisz swój nowy E-Mail:" },
			{ L_email_action_change_text_b, "Musisz podać nowy adres E-Mail!\n\nWpisz swój nowy E-Mail:" },
			{ L_email_action_success_title, "Ustawiono nowy e-mail" },
			{ L_email_action_success_text, "Twój e-mail został ustawiony: %s\n\nSprawdź swoją pocztę na E-Mail zawierający kod do odzyskania konta (który także można użyć do weryfikacji e-maila) w razie problemów z zalogowaniem\n\nPamiętaj! Zweryfikuj /email ! Jeśli zgubisz hasło oraz konto nie jest zweryfikowane nie będziesz miał mozliwości aby odzyskać dostęp do konta!" },
			{ L_email_change_newemail, "Wpisz swój nowy E-Mail:" },
			{ L_email_change_verify_fail, "Niepoprawny kod!\n\nAby zmienić E-Mail musisz wpisać Kod REACT (przed chwilą został wysłany dodatkowy kod w razie zguby na znany nam E-Mail):" },
			{ L_email_verify_text, "Niepoprawny kod!\n\nPrzykładowy Kod REACT: A0A000A00A0A00A0AA000000000A0A00\n\nWpisz otrzymany Kod REACT:" },
			{ L_email_verify_title, "Weryfikacja adresu E-Mail" },
			{ L_email_verify_success, "E-Mail zweryfikowany!" },
			{ L_email_verification_error, "Twój e-mail już jest zweryfikowany lub nie podałeś żadnego e-maila który musi zostać zweryfikowany" },
			{ L_email_activation_limit, "Aby zapobiec SPAM, e-mail można zmieniać lub aktywować co 15 minut." },
			{ L_email_change_verify_text_a, "Przykładowy Kod REACT: A0A000A00A0A00A0AA000000000A0A00\n\nAby zmienić E-Mail musisz wpisać Kod REACT (przed chwilą został wysłany dodatkowy kod w razie zguby na znany nam E-Mail):" },
			{ L_email_change_verify_text_b, "Przykładowy Kod REACT: A0A000A00A0A00A0AA000000000A0A00\n\nWpisz otrzymany Kod REACT:" },

			{ L_mm_APTITT, "Gracz [%-3d]%s zaakceptowal Twoja prosbe o teleport" },
			{ L_mm_RTTP_a, "Zapytanie o teleport wyslane! - W ciagu 30 sekund zapytanie wygasa" },
			{ L_mm_ITPTM_a, "Zaproszenie na teleport wyslane! - W ciagu 30 sekund wygasa" },
			{ L_mm_RTTP_b, "Gracz [%-3d]%s prosi o teleport do Ciebie - wpisz /tphere %d aby zaakceptowac - W ciagu 30 sekund prosba wygasnie" },
			{ L_mm_ITPTM_b, "Gracz [%-3d]%s prosi o teleport do siebie - wpisz /tpto %d aby zaakceptowac - W ciagu 30 sekund zaproszenie wygasnie" },
			{ L_mm_invalid_playerid, "Nieprawidlowe ID" },
			{ L_mm_player_in_minigame, "Gracz jest teraz w zabawie" },
			{ L_tpto_cannot_tp, "W tej chwili teleport jest niemożliwy" },
			{ L_tpto_usage, "Użycie: {b}/tpto <id/nick>{/b} aby sie teleportowac do gracza (musi zaakceptować za pomocą {b}/tphere %d{/b})" },
			{ L_mm_APTRTM, "Gracz [%-3d]%s zaakceptowal Twoja prosbe o teleport" },
			{ L_tphere_usage, "Użycie: {b}/tphere <id/nick>{/b} aby spytać się gracza o pozwolenie o teleport (musi zaakceptować za pomocą {b}/tpto %d{/b})" },
			{ L_mm_cannot_tphere, "Gracz teraz nie może zostać przeteleportowany" },

			{ L_pickupmessage_house, "Wpisz {b}/dom{/b} aby zobaczyć informację dla tej posiadłości" },
			{ L_house_nohouse, "Musisz wynajmować dom aby móc użyć to polecenie" },
			{ L_house_infostringbuilder_a, "{FFFFFF}Posiadłość {FF0000}jest aktualnie wynajmowana{FFFFFF}, informacje:\n\t• Wynajmujący: \n\t\t\t%s\n\n\t• Wynajęte do: %s" },
			{ L_house_infostringbuilder_locked, "\n\t• Drzwi są zamknięte" },
			{ L_house_infostringbuilder_b, "{FFFFFF}Posiadłość {00FF00}można wynająć{FFFFFF}, informacje:\n" },
			{ L_house_infostringbuilder_c, "\n\n\t• Cennik aby wynająć/przedłużyć:\n" },
			{ L_house_infostringbuilder_d, "\
\n\nPoszczególne grupy na serwerze mogą tylko na pewną ilość dni wynająć (lub przedłużyć) posiadłość.\n\
Oto o ile dni kto może przedłużyć wynajem:\n\n\
\t• Gracz:		co 5 dni\n\
\t• VIP:			co 15 dni\n\n\n\
Użyj {41BECC}/dom k <ilość dni>{FFFFFF} aby przedłużyć jeśli jesteś właścicielem lub wynająć jeśli nim nie jesteś\n\nDodatkowe komendy:\n\
\t{41BECC}/dom P{FFFFFF} - wchodzisz do domu lub wychodzisz z niego\n\
\t{41BECC}/dom L{FFFFFF} - otwierasz/zamykasz drzwi\n\
\t{41BECC}/dom Z <id/nick>{FFFFFF} - daj/odbierz graczowi zezwolenie na wstęp do Twojego domu\n\
\t{41BECC}/dom W <id/nick>{FFFFFF} - wyrzuć gracza z Twojego domu\n\
\t{41BECC}/dom Q {FFFFFF} - anuluj wynajem (pieniądze oraz respekt {FF0000}nie{FFFFFF} zostaje zwracany)\n" 
			},
			{ L_house_infostringbuilder_e, "\
\t{41BECC}[DOM ID: %d]\n\
\t{41BECC}/dom # <respekt za dzień wynajmu>{FFFFFF} - Ustaw cene do wynajęcia za dzień (respekt)\n\
\t{41BECC}/dom $ <kasa za dzień wynajmu>{FFFFFF} - Ustaw cene do wynajęcia za dzień (fundusze)\n\
\t{41BECC}/dom :{FFFFFF} - Usuń obecnego wynajmującego\n\
\t{41BECC}/dom + <id/nick>{FFFFFF} - Ustaw gracza jako wynajmującego na 5 dni\n\
\t{41BECC}/dom > <ilość godzin>{FFFFFF} - Ustaw zakończenie wynajmu (0 == kończy się już, 1 za godzinę, itd.)\n\
\t{41BECC}/dom * <TYP>{FFFFFF} - Ustawia nowy typ interioru (/domadd)\n"
			},
			{ L_house_info_title, "Informacje Domu" },
			{ L_house_infostringbuilder_money, "\n\t\t(Fundusze): 1 dzień: $%I64u, 7 dni: $%I64u, 14 dni: $%I64u, 21 dni: $%I64u\n" },
			{ L_house_infostringbuilder_score, "\n\t\t(Respekt): 1 dzień: R%I64u, 7 dni: R%I64u, 14 dni: R%I64u, 21 dni: R%I64u\n" },
			{ L_house_enter_permissionerror, "Nie masz uprawnień aby wejść do tej posiadłości" },
			{ L_house_rentlimit_reached, "Już wynajmujesz dom, limit domów na gracza to 1" },
			{ L_house_owneronly_action, "Tylko właściciel może wykonać to polecenie" },
			{ L_house_rent_mintime_error, "Można minimalnie wynająć na jeden dzień" },
			{ L_house_resources_error, "Nie posiadasz dość funduszy oraz/lub respektu aby wykonać to polecenie" },
			{ L_house_rent_success, "Przedłużono wynajem posiadłości" },
			{ L_house_rent_usage, "Użycie: {41BECC}/dom k <ilość dni>" },
			{ L_house_door_open, "Drzwi zostały otwarte" },
			{ L_house_door_close, "Drzwi zostały zamknięte" },
			{ L_house_kick_usage, "Użycie: {41BECC}/dom W <id/nick>" },
			{ L_house_kick_error, "Gracz się nie znajduje w Twoim domu" },
			{ L_house_kick_success_1, "Gracz został wyrzucony" },
			{ L_house_kick_success_2, "Właściciel domu Cię wyrzucił" },
			{ L_house_range_error, "Musisz być blisko wejścia domu lub w domu aby móc użyć to polecenie" },
			{ L_house_action_executed, "Wykonano polecenie" },
			{ L_house_invalid_interiorid, "Niepoprawny typ interioru" },
			{ L_house_invalid_hours, "Niepoprawna ilość godzin" },
			{ L_house_player_notonline, "Gracz musi być online" },
			{ L_house_player_supplyerror, "Podaj gracza" },
			{ L_house_uninvite_usage, "Użycie: {b}/dom Z <id/nick>" },
			{ L_house_uninvite_success_1, "Zabrano graczowi zezwolenie na wejście do Twojego domu" },
			{ L_house_uninvite_success_2, "Dano graczowi zezwolenie na wejście do Twojego domu" },
			{ L_house_score_error, "Niepoprawna ilość respektu" },
			{ L_house_money_error, "Niepoprawna ilość kasy" },
			{ L_houseadd_usage_1, "Użycie: {b}/addhouse <TYP> <$/dzień> <R/dzień>" },
			{ L_houseadd_usage_2, "Typy: 0 .. 9" },

			{ L_credits_scene_1, "{FF0000}Założyciel serwera & scripter\n{000000}Rafał 'Gamer_Z' Grasman\n{FF0000}Ostatni update GameModa\n{000000}%s\n\n" },
			{ L_credits_scene_2, "\n{FF0000}Użyte biblioteki{FFFFFF}\n\n\n{FF0000}AntiCheat V2.6; {000000}Gamer_Z\n{FF0000}Boost; {000000}boost.org\n{FF0000}ODB; {000000}Code Synthesis\n" },
			{ L_credits_scene_3, "{FF0000}MapAndreas Plugin; {000000}Kalcor\n{FF0000}MySQL; {000000}Oracle\n{FF0000}SampGDK; {000000}Zeex\n{FF0000}SampGDK Ext Mgr; {000000}Gamer_Z\n{FF0000}sqlite3; {000000}sqlite.org\n{FF0000}Streamer Plugin; {000000}Incognito\n{FF0000}Whirlpool Plugin; {000000}Y_Less\n" },
			{ L_credits_scene_4, "{FF0000}ZeroCMD++; {000000}Gamer_Z\n{FF0000}ZeroDLG; {000000}Gamer_Z\n\n\n{FF0000}SA-MP; {000000}sa-mp.com\n\n\n" },
			{ L_credits_scene_5, "{FF0000}Kontakt\n{000000}exe24.info / gz0.nl\n\n{FF0000}Prowider serwera dedykowanego\n{000000}OVH.NL\n\n{FF0000}Prowider hostingu forum & domeny\n" },
			{ L_credits_scene_6, "{000000}STRATO.NL\n\n\n\n{FF0000}Specjalne podziękowania dla" },
			{ L_credits_scene_7, "{000000}IceCube, Oski20, C2A1\n{000000}West, BlackPow3R, Arcade\n" },

			{ L_weapons_dialog_text_a, "Stałe bronie kupuje się za {FF0000}R{FFFFFF}espekt oraz {00FF00}$f{FFFFFF}undusze{FFFFFF}\n" },
			{ L_weapons_dialog_text_dequip, "<{3FC8F2}Zdejmij (%d) %s{FFFFFF}" },
			{ L_weapons_dialog_text_equip, ">{73E067}Załóż (%d) %s{FFFFFF}" },
			{ L_weapons_dialog_text_ammo, " | %d ammo: {00FF00}$%I64u{FFFFFF}" },
			{ L_weapons_dialog_text_magazinestatus, " | Stan Magazynku: %d" },
			{ L_weapons_dialog_text_buy, "${F25C5C}Kup (%d) %s {FFFFFF} za {FF0000}R%I64u{FFFFFF} " },
			{ L_weapons_dialog_text_level, " | Wymagany poziom: %.0f" },
			{ L_weapons_dialog_text_b, "\nUżyj {b}/ammo <id broni> <ilosc>{/b} aby dokupić amunicję" },
			{ L_weapons_dialog_title, "Stałe bronie" },

			{ L_weapons_dialog_text_weaponowned, "Już posiadasz tą broń" },
			{ L_weapons_dialog_text_lowlevel, "Masz za niski poziom" },
			{ L_weapons_dialog_text_lowscore, "Masz za mało respektu" },
			{ L_weapons_dialog_text_invalidweaponid, "Niepoprawne id broni" },
			{ L_weapons_dialog_text_lowcash, "Nie posiadasz odpowiedniej ilości funduszy" },
			{ L_weapons_dialog_text_notequiped, "Nie masz tą broń założoną" },
			{ L_weapons_dialog_text_noammo, "Broń nie posiada amunicji" },
			{ L_weapons_dialog_text_needbuy, "Pierwsze musisz kupić tą broń" },

			{ L_buyammo_usage, "Użycie: {b}/ammo <id broni> <ilość>{/b}, np. {b}/ammo 25 100{/b} kupuje 100 amunicji do Shotgun" },
			{ L_buyammo_error_badbulletweapon, "Do tej broni nie można dokupić amunicję" },
			{ L_buyammo_error_0ammo, "Ilość amunicji musi być większa od 0" },
			{ L_buyammo_error_maxammo, "Posiadasz już maksymalną ilość amunicji dla tej broni" },
			{ L_buyammo_success, "Amunicję zakupiono! Stan magazynku %s: %d" },

			{ L_error_cant_usecmd_atm, "Nie można teraz użyć tej komendy" },
			{ L_setcolor_removed, "Kolor usunięty!" },
			{ L_setcolor_usage, "Użycie: {b}/kolor <kolor>{/b}, np. {b}/kolor 77FFAA{/b}, wybierz kolor na http://colorpicker.com | Obecny kolor: %06X" },
			{ L_say_usage, "Użycie: {b}/say <wiadomość>{/b} (max 111 znaków)" },
			{ L_asay_usage, "Użycie: {b}/a <tekst>" },
			{ L_msay_usage, "Użycie: {b}/m <tekst>" },
			{ L_lsay_usage, "Użycie: {b}/l <tekst>" },
			{ L_savepos_success, "Zapisano Twoją pozycję, użyj {b}/loadpos{/b} aby załadować" },
			{ L_givecash_success_1, "Przelew w wysokości {b}$%I64u{/b} udał się sukcesem, stan: {b}$%I64d{/b}" },
			{ L_givecash_success_2, "{b}$%I64u{/b} zostało Tobie zasilone przez {b}%s{/b} z własnego konta, stan: {b}$%I64d{/b}" },
			{ L_givecash_usage, "Użycie: {b}/givecash <id/nick> <ilość kasy>{/b}, stan konta: {b}$%I64d{/b}" },
			{ L_givescore_success_1, "Przelew w wysokości {b}%I64u{/b} respektu udał się sukcesem, stan: {b}%I64d{/b} respektu" },
			{ L_givescore_success_2, "{b}%I64u{/b} respektu zostało Tobie zasilone przez {b}%s{/b} z własnego konta, stan: {b}%I64u{/b} respektu" },
			{ L_givescore_error, "Nie posiadasz dość respektu" },
			{ L_givescore_usage, "Użycie: {b}/giverespect <id/nick> <ilość respektu>{/b}, stan respektu: {b}%I64d{/b}" },
			{ L_agivecash_success_1, "Przelew w wysokości {b}$%I64d{/b} udał się sukcesem, stan pieniędzy [id: %d]: {b}$%I64d{/b}" },
			{ L_agivecash_success_2, "{b}$%I64d{/b} zostało Tobie zasilone przez {b}%s{/b}, stan: {b}$%I64d{/b}" },
			{ L_agivecash_usage, "Użycie: {b}/agivecash <id/nick> <ilość kasy>{/b}, stan konta: {b}$%I64d{/b}" },
			{ L_agivescore_success_1, "Przelew w wysokości {b}%I64d{/b} respektu udał się sukcesem, stan respektu [id: %d]: {b}%I64d{/b}" },
			{ L_agivescore_success_2, "{b}%I64d{/b} respektu zostało Tobie zasilone przez {b}%s{/b}, stan: {b}%I64d{/b} respektu" },
			{ L_agivescore_usage, "Użycie: {b}/agiverespect <id/nick> <ilość respektu>{/b}, stan respektu: {b}%I64d{/b}" },
			{ L_agiveexp_success_1, "Przelew w wysokości {b}%I64d{/b} punktów doświadczenia udał się sukcesem, stan punktów doświadczenia [id: %d]: {b}%I64u{/b}" },
			{ L_agiveexp_success_2, "{b}%I64d{/b} punktów doświadczenia zostało Tobie zasilone przez {b}%s{/b}, stan: {b}%I64u{/b} punktów doświadczenia" },
			{ L_agiveexp_usage, "Użycie: {b}/agiveexp <id/nick> <ilość expa>{/b}, stan punktów doświadczenia: {b}%I64u{/b}" },
			{ L_bank_withdraw_success, "Przelew z <bank na Smartfon> w wysokości {b}$%I64d{/b} udał się sukcesem, stan banku: {b}$%I64d{/b}" },
			{ L_bank_nofunds_bank, "Nie posiadasz dość funduszy na koncie bankowym" },
			{ L_bank_transfer_success, "Przelew z <Smartfon na bank> w wysokości {b}$%I64d{/b} udał się sukcesem, stan banku: {b}$%I64d{/b}" },
			{ L_bank_nofunds_here, "Nie posiadasz dość funduszy przy sobie" },
			{ L_bank_usage_1, "Użycie: {b}/bank <ilość kasy>{/b}, stan konta: {b}$%I64d{/b}, użyj negatywną wartość aby wypłącić pieniądze" },
			{ L_bank_usage_2, "Przykłady: {b}/bank 100{/b} wpłaca $100 na konto bankowe a {b}/bank -100{/b} wypłaca $100 z konta bankowego" },

			{ L_race_gametxt, "Wyścig" },
			{ L_race_joined_success,	 "Dolaczyles/as do wyscigu %s"},
			{ L_race_staging,			 "Wyscig %s zaraz sie rozpocznie (%s)"},
			{ L_race_not_enough_players, "Za malo graczy aby rozpoczac wyscig %s" },
			{ L_race_winner_text,		"Ukonczyles/as wyscig %s na pozycji %d z czasem %.3f sekund oraz dostales/as $%I64u oraz %I64u respektu" },
			
			{ L_announce_usage, "Użycie: {b}/ann <tekst>{/b}, tekst musi zostać podany oraz mniejszy od 57 znaków, dozwolone znaki: [a-z][A-Z][0-9][bez procent]"},
		
			{ L_minigame_inactivity, "Zostałeś/aś wyrzucony z zabawy, powód: Wykryto AFK lub za duże lagi" },

			{ L_goldcoin_foundnew, "Gratulacje! Odkryles/as kolejna zlota monete! %d/%d | Wpisz /zmhelp jesli sie ciekawisz co to jest" },

			{ L_goldcoin_helpmsg, "Strzel bronia w zolty magnes aby zdobyc te zlota monete" },
		};

		struct SAddTranslation { SAddTranslation() { InternalLanguage::Add(translation); } } _SAddTranslation;
	};
};