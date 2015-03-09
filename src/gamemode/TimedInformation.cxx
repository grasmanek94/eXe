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

namespace TimedInformation
{
#if defined _LOCALHOST_DEBUG
	static const int TimeBetweenShows = 12000;
#else
	static const int TimeBetweenShows = 60000;
#endif
	void* timer_owner;

	CLanguageTextDraw TimedInformationTextDraw;

	static const std::vector<language_string_ids> Informations =
	{
		L_tip_01,
		L_tip_02,
		L_tip_03,
		L_tip_04,
		L_tip_05,
		L_tip_06,
		L_tip_07,
		L_tip_08,
		L_tip_09,
		L_tip_10,
		L_tip_11,
		L_tip_12,
		L_tip_13,
		L_tip_14,
		L_tip_15,
		L_tip_16,
		L_tip_17,
		L_tip_18,
		L_tip_19,
		L_tip_20,
	};

	unsigned long currentinformation = 0;
	void EndShowRandomInformation(int timerid, void * param);
	void StartShowRandomInformation(int timerid, void * param)
	{
		TimedInformationTextDraw.SetString(Informations[currentinformation]);
		TimedInformationTextDraw.Show();

		if (++currentinformation == Informations.size())
		{
			currentinformation = 0;
		}
		sampgdk_SetTimerEx(6000, false, EndShowRandomInformation, nullptr, timer_owner);
	}

	void EndShowRandomInformation(int timerid, void * param)
	{
		TimedInformationTextDraw.Hide();
		sampgdk_SetTimerEx(TimeBetweenShows, false, StartShowRandomInformation, nullptr, timer_owner);
	}

	class TimedInformationProcessor : public Extension::Base
	{
	public:
		bool OnGameModeInit() override
		{
			TimedInformationTextDraw.Create(320.0, 415.0, L_tip_01, 2, 255, 1, 0.24, 1.1, -1, 1, 1, 0, 0);

			timer_owner = this;
			sampgdk_SetTimerEx(TimeBetweenShows, false, StartShowRandomInformation, nullptr, timer_owner);
			return true;
		}
	} _TimedInformationProcessor;
};