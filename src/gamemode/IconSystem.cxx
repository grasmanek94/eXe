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

class Icons : public Extension::Base
{
	bool OnGameModeInit() override
	{
		StreamerLibrary::CreateDynamicMapIcon(1980.9590, 1025.1013, 10.8203, 44, 0, 0, -1, -1, 200); //4smoczki
		StreamerLibrary::CreateDynamicMapIcon(2185.7031, 1615.0219, 9.1074, 25, 0, 0, -1, -1, 200); //Caligula
		StreamerLibrary::CreateDynamicMapIcon(2251.6206, 2453.1201, 10.6719, 30, 0, 0, -1, -1, 200); //milicjalv
		StreamerLibrary::CreateDynamicMapIcon(-1614.7430, 721.3600, 13.6308, 30, 0, 0, -1, -1, 200); //milicjasf
		StreamerLibrary::CreateDynamicMapIcon(1554.6021, -1675.6003, 16.1953, 30, 0, 0, -1, -1, 200); //milicjasf
		StreamerLibrary::CreateDynamicMapIcon(1673.8817, 1447.6927, 10.7842, 5, 0, 0, -1, -1, 200); //lvlot
		StreamerLibrary::CreateDynamicMapIcon(-1420.4910, -286.3807, 14.5808, 5, 0, 0, -1, -1, 200); //sflot
		StreamerLibrary::CreateDynamicMapIcon(1687.8898, -2238.8435, 13.5469, 5, 0, 0, -1, -1, 200); //lslot
		StreamerLibrary::CreateDynamicMapIcon(2003.2103, 1544.1025, 13.5859, 9, 0, 0, -1, -1, 200); //piraty
		StreamerLibrary::CreateDynamicMapIcon(-2627.2065, 1417.9973, -0.7613, 23, 0, 0, -1, -1, 200); //jizzy
		StreamerLibrary::CreateDynamicMapIcon(414.3109, 2534.0222, 19.1484, 36, 0, 0, -1, -1, 200); //szkola
		StreamerLibrary::CreateDynamicMapIcon(-2077.3899, -121.6180, 35.1641, 36, 0, 0, -1, -1, 200); //szkola
		StreamerLibrary::CreateDynamicMapIcon(-2186.6924, 2415.5525, 5.1563, 36, 0, 0, -1, -1, 200); //szkola
		StreamerLibrary::CreateDynamicMapIcon(1607.4534, 1817.1342, 10.8203, 22, 0, 0, -1, -1, 200); //szpital lv
		StreamerLibrary::CreateDynamicMapIcon(2386.4814, 1041.2076, 10.8203, 27, 0, 0, -1, -1, 200); //tunelv
		StreamerLibrary::CreateDynamicMapIcon(-2734.2117, 224.5040, 8.6784, 27, 0, 0, -1, -1, 200); //tunesf
		StreamerLibrary::CreateDynamicMapIcon(1048.1530, -1019.6749, 32.1016, 27, 0, 0, -1, -1, 200); //tunels
		StreamerLibrary::CreateDynamicMapIcon(2642.5098, -2047.3104, 13.5525, 27, 0, 0, -1, -1, 200); //lowrider
		StreamerLibrary::CreateDynamicMapIcon(795.5688, 830.9290, 7.3306, 11, 0, 0, -1, -1, 200); //kopalnia

		//Pay'n'Spray - Nie moje (forum.sa-mp.com)
		StreamerLibrary::CreateDynamicMapIcon(2067.4, -1831.2, 13.5, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(488.0, -1734.0, 34.4, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(720.016, -454.625, 15.328, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(-1420.547, 2583.945, 58.031, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(1966.532, 2162.65, 10.995, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(-2425.46, 1020.83, 49.39, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(1021.8, -1018.7, 30.9, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(-1908.9, 292.3, 40.0, 63, 0, 0, -1, -1, 200);
		StreamerLibrary::CreateDynamicMapIcon(-103.6, 1112.4, 18.7, 63, 0, 0, -1, -1, 200);
		return true;
	}
} _Icons;

