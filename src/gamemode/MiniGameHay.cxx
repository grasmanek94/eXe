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

using namespace Zabawy;

namespace TerrainGenerator
{
	const glm::vec3 hay_size = { 4.0f, 4.0f, 3.0f };
	typedef std::pair<glm::vec3, glm::vec3> PosRotDef;

	std::vector<PosRotDef> GeneratedObjectList;
	std::atomic<bool> Generating = false;
	std::thread * generator = nullptr;
	std::unordered_set<int> CreatedObjectList;

	glm::vec3 GetOffsetAtAngleFromPos(float distance, float angle)
	{
		float x = distance * sin(-angle / (180.0f / M_PI));
		float y = distance * cos(-angle / (180.0f / M_PI));
		return glm::vec3(x, y, 0.0);
	}

	void GetLine(glm::vec3 from, glm::vec3 to, std::vector<PosRotDef> &objects, bool yrot = false, float startbias = 0.0f, float maxbias = 0.0f, bool bias = false, bool second = false)
	{
		glm::vec3 DirectionVector = to - from;
		float ry = yrot ? -atan2f(DirectionVector.z, sqrt(pow(DirectionVector.x, 2.0f) + pow(DirectionVector.y, 2.0f))) * (180.0f / M_PI) : 0.0f;
		float rz = atan2f(DirectionVector.y, DirectionVector.x) * (180.0f / M_PI);
		float dist = glm::length(DirectionVector);
		float currentdistance = hay_size.x / 2.0f;
		int i = 0;
		while (currentdistance < dist)
		{
			if (!second || (second && i++ % 2 == 0))
			{
				float percentage = currentdistance / dist;
				glm::vec3 NewPos = from + (DirectionVector*(percentage));
				objects.push_back(PosRotDef(glm::vec3(NewPos.x, NewPos.y, NewPos.z), glm::vec3((bias ? (startbias + (maxbias*sin(percentage*2.0f*M_PI))) : (startbias)), ry, rz)));
			}
			currentdistance += hay_size.x;
		}
	}

	void GetSquare(glm::vec3 from, glm::vec3 to, std::vector<PosRotDef> &objects, size_t width = 0, bool yrot = false, float startbias = 0.0f, float maxbias = 0.0f, bool bias = false, bool second = false)
	{
		glm::vec3 DirectionVector = to - from;

		float rz = atan2f(DirectionVector.y, DirectionVector.x) * (180.0f / M_PI);
		float dist = glm::length(DirectionVector);
		float currentdistance = hay_size.x / 2.0f;

		if (width == 0)//square approximation
		{
			width = (size_t)((dist - currentdistance) / hay_size.y);
		}

		for (size_t i = 0; i < width; ++i)
		{
			glm::vec3 offset(GetOffsetAtAngleFromPos(4.0f*(float)i, rz));
			GetLine(from + offset, to + offset, objects, yrot, startbias, maxbias, bias, second);
		}
	}

	void GetCircle(glm::vec3 center, float radius, std::vector<PosRotDef> &objects, float fromangle = 0.0f, float toangle = 360.0f, float maxz = 0.0f, bool xrot = false, float startbias = 0.0f, float maxbias = 0.0f, bool bias = false, bool second = false, bool sinheight = false)
	{
		float dist = 2.0*M_PI*radius;
		float currentdistance = 0.0f;
		float rx = 1000.0f;
		if (!xrot)
		{
			rx = 0.0f;
		}
		int i = 0;
		while (currentdistance < dist)
		{
			float percentage = currentdistance / dist;
			float degrees = (currentdistance / radius) * (180.0 / M_PI);
			if (degrees >= fromangle && degrees <= toangle)
			{
				if (!second || (second && i++ % 2 == 0))
				{
					float newz = sinheight ? maxz*sin(percentage*2.0f*M_PI) : maxz*percentage;
					glm::vec3 NewPos = center + GetOffsetAtAngleFromPos(radius, degrees) + glm::vec3(0.0f, 0.0f, newz);
					float rz = atan2f(NewPos.y - center.y, NewPos.x - center.x) * (180.0f / M_PI);

					if ((xrot && rx == 1000.0f) || (sinheight && xrot))
					{
						float newdistance = currentdistance + hay_size.x;
						float newpercentage = newdistance / dist;
						float newdegrees = (newdistance / radius) * (180.0 / M_PI);
						float newnewz = sinheight ? maxz*sin(newpercentage*2.0f*M_PI) : maxz*newpercentage;
						glm::vec3 newNewPos = center + GetOffsetAtAngleFromPos(radius, newdegrees) + glm::vec3(0.0f, 0.0f, newnewz);
						glm::vec3 DirectionVector = newNewPos - NewPos;
						rx = xrot ? -atan2f(DirectionVector.z, sqrt(pow(DirectionVector.x, 2.0f) + pow(DirectionVector.y, 2.0f))) * (180.0f / M_PI) : 0.0f;
					}

					objects.push_back(PosRotDef(glm::vec3(NewPos.x, NewPos.y, NewPos.z), glm::vec3(-rx, (bias ? (startbias + (maxbias*sin(percentage*2.0f*M_PI))) : (startbias)), rz)));
				}
			}
			currentdistance += hay_size.x;
		}
	}

	float GetRandomElevation()
	{
		return Functions::gen_random_float(-30.0f, 30.0f);
	}

	float GetRandomDirection()
	{
		return Functions::gen_random_float(-180.0f, 180.0f);
	}

	float GetRandomDistance()
	{
		return Functions::gen_random_float(5.0f, 30.0f);
	}

	float GetRandomCircleRadius()
	{
		return Functions::gen_random_float(7.0f, 15.0f);
	}

	float GetRandomCircleMaxAngle()
	{
		return Functions::gen_random_float(30.0f, 360.0f);
	}

	bool GetRandomBool()
	{
		return Functions::RandomGenerator->Random() % 2 == 0;
	}

	int GetRandom(int min, int max)
	{
		return Functions::RandomGenerator->Random(min, max);
	}

	float GetRandomMinus()
	{
		if (GetRandomBool())
		{
			return -1.0f;
		}
		return 1.0f;
	}

	glm::vec3 GetRandomVectorDirection(float factor = 1.0f)
	{
		return glm::vec3(GetRandomMinus()*GetRandomDistance() * factor, GetRandomMinus()*GetRandomDistance() * factor, (GetRandomMinus()*((GetRandomDistance() / 5.0f) - 1.0f)) * factor);
	}

	glm::vec3 GetTargetPos(glm::vec3 from, glm::vec3 direction, float distance)
	{
		return from + direction * distance;
	}

	bool TooClose(std::vector<PosRotDef>& a, std::vector<PosRotDef>& b, float distance)
	{
		for (auto& i : a)
		{
			for (auto& j : b)
			{
				if (glm::distance(i.first, j.first) < distance)
				{
					return true;
				}
			}
		}
		return false;
	}

	void Generate(float x, float y, float z)
	{
		GeneratedObjectList.clear();
		glm::vec3 from(x, y, z);
		std::vector<PosRotDef> temp;
		while (GeneratedObjectList.size() < 133)
		{
			temp.clear();
			glm::vec3 rfrom(from);
			glm::vec3 rto(from);
			int type = GetRandom(0, 3);
			rfrom = from + GetRandomVectorDirection(type == 1 ? 0.33f : 1.0f);

			while (glm::distance(rfrom, rto) < 4.0f)
			{
				rto = (GetRandomBool() ? from : from) + GetRandomVectorDirection(type == 1 ? 1.5f : 1.0f);
			}

			switch (type)
			{
			case 0:
				GetLine(rfrom, rto, temp, GetRandomBool(), 0.0, 0.0, false, GetRandomBool());
				break;
			case 1:
				GetSquare(rfrom, rto, temp, 0, GetRandomBool(), 0.0, 0.0, false, GetRandomBool());
				break;
			case 2:
				GetCircle(rfrom, GetRandomCircleRadius(), temp, 0.0, GetRandomCircleMaxAngle(), GetRandomDistance() / 6.0f, GetRandomBool(), 0.0, 0.0, false, GetRandomBool(), GetRandomBool());
				break;
			}

			if (!TooClose(GeneratedObjectList, temp, 5.0f))
			{
				GeneratedObjectList.insert(GeneratedObjectList.end(), temp.begin(), temp.end());
			}
		}
		Generating = false;
	}

	int CurrentGameVW = 0;
	bool StartGenerator(int _CurrentGameVW, float x = 0.0, float y = 0.0, float z = 400.0)
	{
		if (generator != nullptr)
		{
			return false;
		}

		Generating = true;
		CurrentGameVW = _CurrentGameVW;

		for (auto& i : CreatedObjectList)
		{
			StreamerLibrary::DestroyDynamicObject(i);
		}

		CreatedObjectList.clear();
		generator = new std::thread(Generate, x, y, z);
		return true;
	}

	void CreateObjects()
	{
		for (auto& i : GeneratedObjectList)
		{
			CreatedObjectList.insert(StreamerLibrary::CreateDynamicObject(3374, i.first.x, i.first.y, i.first.z, i.second.x, i.second.y, i.second.z, CurrentGameVW));
		}
	}

	bool Process()
	{
		if (generator != nullptr && Generating == false)
		{
			generator = nullptr;
			CreateObjects();
			return true;//done generating
		}
		return false;
	}
};
using namespace TerrainGenerator;

namespace Sianko
{
	class Arena : public Base
	{
	public:
		Arena() :
			Base(L_games_ZabawaHay_name, "/hay", eST_Zabawa)
		{}

		int AreaID;
		int CurrentGameState;
		int GameTimer;

		const int ARENA_MIN_PLAYERS = 2;

		void SetGameState(int state)
		{
			if (CurrentGameState != state)
			{
				CurrentGameState = state;
				NeedUpdate();
			}
		}

		void ResetGame()
		{
			DestroyTimerValidate(GameTimer);
			SetGameState(0);
		}

		void ResetGameFull(bool PlayersAlreadyInGame = false)
		{
			if (PlayersAlreadyInGame)
			{
				while (PlayersInGame.size())
				{
					CheckWinner(*PlayersInGame.begin(), true);
				}
			}
			else
			{
				while (PlayersInGame.size())
				{
					PlayerExitGame(*PlayersInGame.begin());
				}
			}

			if (CurrentGameState)
			{
				ResetGame();
			}
		}

		bool OnGameModeInit() override
		{
			AreaID = StreamerLibrary::CreateDynamicCube(-100.0, -100.0, 300.0, 100.0, 100.0, 500.0, CurrentGameVW);
			SetGameState(0);
			GameTimer = INVALID_TIMER_ID;
			return true;
		}

		void CheckWinner(int playerid, bool nowinner = false)
		{
			if (CurrentGameState == 3 && PlayersInGame.size() > 1)
			{
				GivePlayerAchievement(playerid, EAM_ZabawySiankoPrzegrane, 1);
			}

			PlayerExitGame(playerid);

			if (PlayersInGame.size() > 1)
			{
				return;
			}

			if (PlayersInGame.size() == 1)
			{
				playerid = *PlayersInGame.begin();
				if (!nowinner)
				{
					fixSendClientMessageToAllF(Color::COLOR_INFO3, L_mm_hay_announce_winner, true, true, Player[playerid].PlayerName.c_str());
					Player[playerid].GiveExperienceKill(playerid);
					Player[playerid].GiveScore(3);
					GivePlayerAchievement(playerid, EAM_ZabawySiankoWygrane, 1);
				}
				PlayerExitGame(playerid);			
			}

			ResetGame();
		}

		bool PlayerRequestGameExit(int playerid, int reason) override
		{
			switch (reason)
			{
			case PlayerRequestExitTypeDeath:
				CheckWinner(playerid);
				break;
			case PlayerRequestExitTypeDisconnect:
				CheckWinner(playerid);
				break;
			case PlayerRequestExitTypeExit:
				CheckWinner(playerid);
				break;
			}
			return true;
		}

		bool PlayerRequestCencelStaging(int playerid, int reason) override
		{
			PlayerExitGame(playerid);
			return true;
		}

		bool OnPlayerShootDynamicObject(int playerid, int weaponid, int objectid, float x, float y, float z) override
		{
			auto it = CreatedObjectList.find(objectid);
			if (it != CreatedObjectList.end())
			{
				StreamerLibrary::DestroyDynamicObject(*it);
				CreatedObjectList.erase(it);
			}
			return true;
		}

		bool OnPlayerLeaveDynamicArea(int playerid, int areaid) override
		{
			if (AreaID == areaid)
			{
				CheckWinner(playerid);
			}
			return true;
		}

		void PutPlayerIntoGame(int playerid)
		{
			AddStagingPlayer(playerid, 0xFFFFFF00);
			fixSetPlayerHealth(playerid, 10000000.0);
			auto j = GeneratedObjectList[Functions::RandomGenerator->Random(0, GeneratedObjectList.size() - 1)];
			TeleportPlayer(playerid, j.first.x, j.first.y, j.first.z + hay_size.z + 0.5, j.second.z, false, 0, CurrentGameVW, "", 0, 0, 0.0, 0.0, true);
			fixTogglePlayerControllable(playerid, false);
		}

		void TArenaStart(int timerid, ZabawyTimerVector& vector)
		{
			static int counter = 0;
			if (CurrentGameState == 2)
			{
				if (PlayersInGame.size() < ARENA_MIN_PLAYERS)
				{
					ResetGameFull();
				}
				else
				{					
					counter = 0;
					SetGameState(3);

					for (auto i : GetPlayersCopy())
					{
						PutPlayerIntoGame(i);
					}

					SwitchGameTimer(GameTimer, CreateTimer<Arena>(1000, true, &Arena::TArenaStart));
				}
			}
			else if (CurrentGameState == 3)
			{
				if (PlayersInGame.size() == 0)
				{
					ResetGameFull();
				}
				else
				{
					if (++counter < 6)
					{
						std::string CountDown(Functions::string_format("%d", 6 - counter));
						for (auto i : PlayersInGame)
						{
							if (counter == 1)
							{
								SetCameraBehindPlayer(i);
							}
							GameTextForPlayer(i, CountDown.c_str(), 1000, 6);
						}
					}
					else if (counter == 6)
					{
						for (auto i : GetPlayersCopy())
						{
							if (!StreamerLibrary::IsPlayerInDynamicArea(i, AreaID, 1))
							{
								CheckWinner(i);//PlayersInGame.erase
							}
							else
							{
								GameTextForPlayer(i, "GO!", 500, 6);
								safeGivePlayerWeapon(i, 24, 999999);
								safeGivePlayerWeapon(i, 34, 999999);
								fixTogglePlayerControllable(i, true);
							}
						}
						if (PlayersInGame.size() < ARENA_MIN_PLAYERS)
						{
							ResetGameFull(true);
						}
					}
					else if (counter > 120)
					{
						if (CreatedObjectList.size())
						{
							StreamerLibrary::DestroyDynamicObject(*CreatedObjectList.begin());
							CreatedObjectList.erase(CreatedObjectList.begin());
						}
						else
						{
							ResetGameFull(true);
						}
					}
				}
			}
			else
			{
				ResetGameFull();
			}
		}

		void ProcessTick() override
		{
			if (Process())
			{
				SetGameState(2);
				fixSendClientMessageToAll(Color::COLOR_INFO3, L_mm_hay_startcd);
				SwitchGameTimer(GameTimer, CreateTimer<Arena>(15000, false, &Arena::TArenaStart));
			}
		}

		bool Joinable() override
		{
			return CurrentGameState != 3 && PlayersInGame.size() < 30;
		}

		bool Staging() override
		{
			return CurrentGameState == 1 || CurrentGameState == 2;
		}

		bool OnGameCommandExecute(int playerid, std::string params) override
		{
			if (!Generating && CurrentGameState == 0)
			{
				SetGameState(1);
				StartGenerator(CurrentGameVW);
			}
			
			SetPlayerStaging(playerid);

			SendClientMessage(playerid, Color::COLOR_INFO3, L_mm_hay_staging);
			return true;
		}

		int GetCommandRestrictions() override
		{
			return RESTRICTION_IN_VEHICLE_OR_ONFOOT | RESTRICTION_NOT_IN_A_GAME;
		}
	} _ArenaSianko;
}