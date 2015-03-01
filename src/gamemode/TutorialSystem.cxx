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

#if defined USE_TUTORIAL
std::vector<int> TutorialTextDraws;

int CreateTutorialTextDraw(std::string text)
{
	int temp = TextDrawCreate(18.000000, 120.000000, text.c_str());
	TextDrawBackgroundColor(temp, 255);
	TextDrawFont(temp, 1);
	TextDrawLetterSize(temp, 0.290000, 1.399999);
	TextDrawColor(temp, -1);
	TextDrawSetOutline(temp, 1);
	TextDrawSetProportional(temp, 1);
	TextDrawUseBox(temp, 1);
	TextDrawBoxColor(temp, 85);
	TextDrawTextSize(temp, 224.000000, 0.000000);
	TextDrawSetSelectable(temp, 0);
	return temp;
}

ZCMDF(poradnik, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/tutorial" }), "D")
{
	unsigned long which = 0;
	if (parser.Good())
		which = parser.Get<unsigned long>();
	if (which >= TutorialTextDraws.size())
		which = TutorialTextDraws.size() - 1;
	if (Player[playerid].CurrentTutorialID != -1)
	{
		TextDrawHideForPlayer(playerid, Player[playerid].CurrentTutorial);
	}
	Player[playerid].CurrentTutorialID = which;
	Player[playerid].CurrentTutorial = TutorialTextDraws[which];
	TextDrawShowForPlayer(playerid, Player[playerid].CurrentTutorial);
	return true;
}

ZCMD(poradnikhide, PERMISSION_NONE, RESTRICTION_NOT_IN_A_GAME, cmd_alias({ "/poradnikoff", "/tutorialhide", "/tutorialoff" }))
{
	if (Player[playerid].CurrentTutorialID != -1)
	{
		TextDrawHideForPlayer(playerid, Player[playerid].CurrentTutorial);
		Player[playerid].CurrentTutorialID = -1;
		Player[playerid].CurrentTutorial = INVALID_TEXT_DRAW;
	}
	return true;
}

class TutorialSystemProcessor : public Extension::Base
{
public:
	bool OnGameModeInit()
	{
		for (size_t i = 0; i < Tutorial::Text.size(); ++i)
		{
			TutorialTextDraws.push_back(CreateTutorialTextDraw(Tutorial::Text[i]));
		}
		return true;
	}
} _TutorialSystemProcessor;

namespace Tutorial
{
	const std::vector<std::string> Text =
	{
		MakeDesc("~r~Spis Poradnikow") +
		MakeDesc("/Poradnik 1 - Kupno Stalych Broni") +
		MakeDesc("/Poradnik 2 - Kupno Amunicji do Broni") +
		MakeDesc("/Poradnik 3 - Zakladanie broni") +
		MakeDesc("/Poradnik 4 - Zdejmowanie broni") +
		MakeDesc("/Poradnik 5 - Tworzenie Pojazdow") +
		MakeDesc("/Poradnik 6 - Zapisywanie Pojazdow") +
		MakeDesc("/Poradnik 7 - Tworzenie zapisanych Pojazdow") +
		MakeDesc("/Poradnik 8 - Animacje") +
		MakeDesc("/Poradnik 9 - Zlote Monety") +
		MakeDesc("/Poradnik 10 - QuickMenu") +
		MakeDesc("/Poradnik 11 - Teleporty") +
		MakeDesc("/Poradnik 12 - Odliczanie") +
		MakeDesc("/Poradnik 13 - Rampy (czyli skocznie)") +
		MakeDesc("/Poradnik 14 - Zabawy, Eventy, Areny") +
		MakeDesc("/Poradnikoff - Zamyka poradnik"),

		TutrName("1. Kupno Stalych Broni") +
		MakeDesc("Posiadanie wlasnych broni jest bardzo wazne w pokonywaniu przeciwnikow - dlatego sie je kupuje") +
		PressKey("1", "'T'") +
		AskInput("2", "/bronie") +
		AskInput("3", "'$ 25' ~w~(na samym dole - pamietaj o spacji miedzy $ a 25)") +
		PressKey("4", "'ENTER'") +
		MakeDesc("Wtedy bron bedzie Twoja, zanim bedzie mozna bron zalozyc trzeba kupic amunicje - ~b~~h~/Poradnik 2"),

		TutrName("2. Kupno Amunicji do Broni") +
		MakeDesc("Moze i posiadasz bron - lecz kazda bron potrzebuje amunicji z ktorej moze strzelac") +
		PressKey("1", "'T'") +
		AskInput("2", "/bronie") +
		AskInput("3", "'A 25 40' ~w~(na samym dole - pamietaj o spacjach)") +
		PressKey("4", "'ENTER'") +
		MakeDesc("Wtedy shotgun bedzie mial o 40 amunicji wiecej niz poprzednio - jednak jeszcze nie mozesz uzywac tej broni poniewaz ja musisz zalozyc - ~b~~h~/Poradnik 3"),

		TutrName("3. Zakladanie broni") +
		MakeDesc("Moze i posiadasz bron z amunicja - lecz kazda bron trzeba zalozyc zanim mozna z niej strzelac") +
		PressKey("1", "'T'") +
		AskInput("2", "/bronie") +
		AskInput("3", "'> 25' ~w~(na samym dole - pamietaj o spacji miedzy > a 25)") +
		PressKey("4", "'ENTER'") +
		MakeDesc("Wtedy shotgun bedzie w Twoich zalozonych broniach - mozna bron takze schowac - ~b~~h~/Poradnik 4"),

		TutrName("4. Zdejmowanie broni") +
		MakeDesc("Nie chcesz miec pewnej broni przy sobie? Zaden problem") +
		PressKey("1", "'T'") +
		AskInput("2", "/bronie") +
		AskInput("3", "'< 25' ~w~(na samym dole - pamietaj o spacji miedzy > a 25)") +
		PressKey("4", "'ENTER'") +
		MakeDesc("Wtedy shotgun bedzie w Twoich zapisanych broniach - Nie tylko bedziesz potrzebowal(a) bronie - takze pojazdy - ~b~~h~/Poradnik 5"),

		TutrName("5. Tworzenie Pojazdow") +
		MakeDesc("Serwer ulatwia przemieszczanie sie - zawsze mozesz stworzyc wlasny pojazd") +
		PressKey("1", "T") +
		AskInput("2", "'/v infernus'") +
		PressKey("3", "'ENTER'") +
		MakeDesc("TIP: Komenda ~b~~h~/pojazdy~w~ ma cala liste pojazdow~n~Nie tylko prostep ojazdy sa dozwolone - Mozesz takze zapisac tuning pojazdow - ~b~~h~/Poradnik 6"),

		TutrName("6. Zapisywanie Pojazdow") +
		MakeDesc("Fajnie stuningowany samochod? Nie chcesz go stracic? Zapisz go!") +
		DoAction("1", "Wsiadz do pojazdu ktory chcesz zapisac") +
		PressKey("2", "T") +
		AskInput("3", "'/vsave 3'") +
		PressKey("4", "'ENTER'") +
		MakeDesc("Pojazd zostanie wtedy zapisanyna slocie '3'~n~(mozliwe sloty: 0,1,2,3,4,5,6,7,8,9, sloty 10 oraz 11 sa dla pojazdow mafii)~n~lecz napewno chcesz go ponownie stworzyc - ~b~~h~/Poradnik 7"),

		TutrName("7. Tworzenie zapisanych Pojazdow") +
		MakeDesc("Gdzie sie podzial Twoj samochod? Tutaj!") +
		PressKey("1", "T") +
		AskInput("2", "'/v 3'") +
		PressKey("3", "'ENTER'") +
		MakeDesc("Pojazd zapisany za pomoca '/vsave 3' zostanie wtedy zaladowany - moze chcesz pokosztowac animacji? - ~b~~h~/Poradnik 8"),

		TutrName("8. Animacje") +
		MakeDesc("Serwer oferuje Tobie ponad 1500 animacji!") +
		PressKey("1", "T") +
		AskInput("2", "'/anim cellphone 1'") +
		PressKey("3", "'ENTER'") +
		DoAction("4", "Poczekaj pare sekund") +
		PressKey("5", "T") +
		AskInput("6", "'/anim cellphone 0'") +
		PressKey("7", "'ENTER'") +
		MakeDesc("Uzyj wyobraznii i baw sie - moze chcesz zlote monety? - ~b~~h~/Poradnik 9"),

		TutrName("9. Zlote Monety") +
		MakeDesc("Luksus! Zbieraj je ile sie da :)") +
		PressKey("1", "T") +
		AskInput("2", "'/zmtp'") +
		PressKey("3", "'ENTER'") +
		DoAction("4", "Strzel w zlota monete przed Siebie za pomoca broni") +
		MakeDesc("Monety sa dobrze ukryte - wiec dobrze szukaj! Chcesz szybkie uleczanie? - ~b~~h~/Poradnik 10"),

		TutrName("10. QuickMenu") +
		MakeDesc("Ulatwnienie dla kazdego DM-owca") +
		PressKey("1", "~k~~CONVERSATION_YES~") +
		DoAction("2", "najedz na opcje 'Ulecz' za pomoca ~k~~CONVERSATION_NO~/~k~~GROUP_CONTROL_BWD~") +
		PressKey("3", "~k~~CONVERSATION_YES~") +
		MakeDesc("To ulatwienie moze Tobie nieraz uratowac zycie! Czy wiedziales/as ze moszesz sie przeniesc gdzie chcesz? - ~b~~h~/Poradnik 11"),

		TutrName("11. Teleporty") +
		MakeDesc("Serwer ulatwia przemieszczanie sie - takze za pomoca komend") +
		PressKey("1", "T") +
		AskInput("2", "'/teles'") +
		PressKey("3", "'ENTER'") +
		MakeDesc("Z tej listy mozesz wpisac dowolna komende do teleportu.~n~TIP: /loadpos oraz /savepos pozwalaja tworzyc wlasne lokacje teleportu!~n~Jesli chcesz sie przeniesc do gracza wpisz /tpto~n~Moze on zaakceptowac Twoja prosbe za pomoca /tphere~n~ Czy wiedziales/as ze jest odliczanie na serwerze? - ~b~~h~/Poradnik 12"),

		TutrName("12. Odliczanie") +
		MakeDesc("Wyscig? Solowka? Cokolwiek co wymaga zamrozenia na pare sekund? Odliczanie!") +
		PressKey("1", "T") +
		AskInput("2", "'/odlicz'") +
		PressKey("3", "'ENTER'") +
		MakeDesc("Kiedy jakikolwiek gracz wpisze /odlicz pojawi sie zielony napis nad jego glowa: ~r~[odliczanie]~w~~n~Chcesz stawiac skocznie? - ~b~~h~/Poradnik 13"),

		TutrName("13. Rampy (czyli skocznie)") +
		MakeDesc("Fly baby, fly! Heheh.") +
		PressKey("1", "T") +
		AskInput("2", "'/ramp 3'") +
		PressKey("3", "'ENTER'") +
		DoAction("3", "Wsiadz do jakiegokolwiek pojazdy") +
		DoAction("4", "Rozpedz sie") +
		PressKey("5", "~k~~VEHICLE_FIREWEAPON_ALT~") +
		DoAction("6", "Skacz i lataj!") +
		MakeDesc("Tylko ty widzisz swoje rampy, mozesz ustawic pare roznych ramp, np. ~b~~h~/ramp 5~w~~n~Rampy mozesz wylaczyc za pomoca ~b~~h~/ramp 0~w~ Lubisz rozne zabawy? - ~b~~h~/Poradnik 14"),

		TutrName("14. Zabawy, Eventy, Areny") +
		MakeDesc("Czyli rozgrywka na wyzszym poziomie!") +
		MakeDesc("Po prawej stronie widac liste roznych komend - sa to zabawy oraz eventy") +
		PressKey("1", "T") +
		AskInput("2", "'/rpg'") +
		PressKey("3", "'ENTER'") +
		DoAction("4", "Postrzelaj, umieraj, zabijaj, baw sie") +
		PressKey("5", "T") +
		AskInput("6", "'/exit'") +
		PressKey("7", "'ENTER'") +
		MakeDesc("To byl ostatni poradnik, milej zabawy na serwerze!")

	};
};
#endif