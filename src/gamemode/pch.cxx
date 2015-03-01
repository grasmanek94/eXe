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

database * userdb = nullptr;
CMapAndreas * MapAndreas = new CMapAndreas;
AMX StreamerAMX;

void Report_Error(std::string error, std::string file, std::string function, int line)
{
	sampgdk::logprintf("Exception ('%s') in %s::%s::%d", error.c_str(), file.c_str(), function.c_str(), line);
	return;
}

bool DoPossibleUpgrade()
{
	if (userdb->schema_migration())
	{
		try
		{
			switch (userdb->schema_version())
			{
			case 1:
			case 2:
			{
				transaction t(userdb->begin());

				size_t n(0);
				for (user& p : userdb->query<user>())
				{
					userdb->update(p);

					if (n++ % 100 == 0)
					{
						t.commit();
						t.reset(userdb->begin());
					}
				}

				t.commit();
				break;
			}
			}
		}
		catch (odb::exception &zz)
		{
			REPORT_ERROR_IF_ANY(zz.what());
		}
		return true;
	}
	return false;
}
void SetupDatabase()
{
	if (DoPossibleUpgrade())
	{
		sampgdk::logprintf("Notice: Database updated");
	}
	else
	{
		try
		{
			odb::transaction t(userdb->begin());
			delete userdb->load<user>("server");
			t.commit();
		}
		catch (odb::exception &e)
		{
			try
			{
				if (e.what()[0] == '1' && e.what()[1] == ':')
				{
					odb::transaction CreateDB(userdb->begin());
#if defined DATABASE_MYSQL
					std::ifstream file("user.sql");
					std::string str((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());
					userdb->execute(str);
#else
					odb::schema_catalog::create_schema(*userdb);
#endif
					CreateDB.commit();

					odb::transaction t(userdb->begin());

					//just don't let anyone access these nicknames, sure? ye
					user server("server", "AXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
					user admin("admin", "AXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
					user administrator("administrator", "AXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

					userdb->persist(server);
					userdb->persist(admin);
					userdb->persist(administrator);

					t.commit();
				}
				else
				{
					REPORT_ERROR_IF_ANY(e.what());
				}
			}
			catch (odb::exception &zz)
			{
				REPORT_ERROR_IF_ANY(zz.what());
			}
		}
	}
}