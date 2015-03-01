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

//tutaj nic sie nie zapisuje do konta gdy¿ to itak bêdzie na serwerze jeden dzieñ w roku 
//(jedynie prezenty ale to ammo respekt itp to normalne ¿e siê zapisuje)

//nie ma wigilii to mo¿na j¹ odkomentowaæ
namespace Wigilia
{
	/*

	struct WigiliaData
	{
		int LastPickup;
		std::set<int> OtwartePrezenty;
		std::map<std::string, std::string, ci_less> OtrzymaneZyczenia;
	};

	std::string url;
	std::map<std::string, WigiliaData, ci_less> PlayerWigilia;

	class Arena : public Base
	{
	public:
		std::vector<SObject> ArenaObjects;

		static const size_t routes = 2;

		std::vector<SSpawnPoint> StatkiPath[routes];

		Arena() :
		Base(L_games_ArenaWigilia_name, "/wigilia"),
		ArenaObjects(
		{
			{ 19076, 836.4003906, -2059.5000000, 11.8000002, 0.0000000, 0.0000000, 0.0000000 }, //Object number 0
			{ 1279, 836.2999900, -2058.7000000, 12.0000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 1
			{ 1279, 835.5000000, -2058.7002000, 12.2000000, 0.0000000, 334.9950000, 0.0000000 }, //Object number 2
			{ 1212, 835.5999800, -2058.5000000, 11.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 3
			{ 1550, 837.0000000, -2059.5000000, 12.3000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 4
			{ 1550, 835.7999900, -2059.5000000, 12.3000000, 0.0000000, 27.5000000, 0.0000000 }, //Object number 5
			{ 1575, 836.2999900, -2060.0000000, 11.9000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 6
			{ 1576, 836.2999900, -2060.0000000, 12.1000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 7
			{ 1579, 836.7000100, -2060.0000000, 12.0000000, 0.0000000, 38.0000000, 334.0000000 }, //Object number 8
			{ 1580, 835.2998000, -2059.2998000, 11.9000000, 2.0000000, 0.0000000, 265.9950000 }, //Object number 9
			{ 3399, 836.4003900, -2073.5000000, 9.8000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 12
			{ 969, 845.0000000, -2096.6001000, 6.2000000, 89.6460000, 134.9990000, 45.0010000 }, //Object number 13
			{ 1279, 835.2999900, -2060.1001000, 11.9000000, 0.0000000, 358.2450000, 0.0000000 }, //Object number 14
			{ 969, 828.0000000, -2093.3999000, 6.2000000, 89.6460000, 135.5670000, 224.1820000 }, //Object number 15
			{ 969, 838.2000100, -2087.8000000, 6.2000000, 89.6430000, 134.9950000, 135.0000000 }, //Object number 16
			{ 969, 838.2002000, -2079.2998000, 6.2000000, 89.6430000, 134.9840000, 135.0000000 }, //Object number 17
			{ 969, 838.2002000, -2073.5000000, 8.8000000, 71.4940000, 90.7530000, 179.1930000 }, //Object number 18
			{ 970, 832.7999900, -2093.5000000, 6.7000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 19
			{ 970, 835.0996100, -2091.3994000, 6.7000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 20
			{ 970, 835.0999800, -2087.3999000, 6.7000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 21
			{ 970, 835.0999800, -2083.3000000, 6.7000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 22
			{ 970, 832.7999900, -2096.5000000, 6.7000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 23
			{ 970, 836.7999900, -2096.5000000, 6.7000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 24
			{ 970, 840.2999900, -2096.5000000, 6.7000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 25
			{ 970, 840.2998000, -2093.5000000, 6.7000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 26
			{ 970, 838.2000100, -2091.3999000, 6.7000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 27
			{ 970, 838.2000100, -2087.3000000, 6.7000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 28
			{ 970, 838.2000100, -2083.3000000, 6.7000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 29
			{ 970, 835.0999800, -2079.5000000, 7.3000000, 1.1870000, 341.7460000, 90.1420000 }, //Object number 30
			{ 970, 838.2000100, -2079.3999000, 7.3000000, 1.1870000, 341.7410000, 90.1370000 }, //Object number 31
			{ 971, 835.2999900, -2072.3999000, 11.4000000, 0.0000000, 0.0000000, 90.0000000 }, //Object number 32
			{ 971, 837.7841797, -2072.9472656, 11.3999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 33
			{ 983, 849.7000100, -2065.0000000, 12.6000000, 0.0000000, 0.0000000, 88.7500000 }, //Object number 34
			{ 983, 845.9000200, -2064.8999000, 12.6000000, 0.0000000, 0.0000000, 88.7480000 }, //Object number 35
			{ 983, 840.2999900, -2066.7000000, 12.6000000, 0.0000000, 0.0000000, 126.7480000 }, //Object number 36
			{ 983, 847.5996100, -2062.0996000, 12.6000000, 0.0000000, 0.0000000, 88.7480000 }, //Object number 37
			{ 983, 839.2000100, -2064.0000000, 12.6000000, 0.0000000, 0.0000000, 128.7480000 }, //Object number 38
			{ 983, 844.7999900, -2062.1001000, 12.6000000, 0.0000000, 0.0000000, 88.7480000 }, //Object number 39
			{ 983, 834.0000000, -2064.3999000, 12.6000000, 0.0000000, 0.0000000, 60.0000000 }, //Object number 40
			{ 983, 829.2000100, -2060.3999000, 12.6000000, 0.0000000, 0.0000000, 39.9960000 }, //Object number 41
			{ 983, 832.5000000, -2066.5000000, 12.6000000, 0.0000000, 0.0000000, 60.0000000 }, //Object number 42
			{ 983, 827.0999800, -2063.3999000, 12.6000000, 0.0000000, 0.0000000, 59.9960000 }, //Object number 43
			{ 983, 823.2000100, -2059.0000000, 12.6000000, 0.0000000, 0.0000000, 21.9960000 }, //Object number 44
			{ 2780, 852.4000200, -2048.6001000, 11.1000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 45
			{ 2780, 820.2000100, -2046.8000000, 11.1000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 46
			{ 1318, 850.2000100, -2059.8000000, 11.9000000, 0.0000000, 88.7500000, 128.0000000 }, //Object number 47
			{ 1318, 851.5000000, -2061.5000000, 11.9000000, 0.0000000, 88.7480000, 109.9960000 }, //Object number 48
			{ 1318, 850.7999900, -2063.3999000, 11.9000000, 0.0000000, 88.7480000, 1.9950000 }, //Object number 49
			{ 1318, 848.5999800, -2063.3999000, 11.9000000, 0.0000000, 88.7480000, 1.9940000 }, //Object number 50
			{ 1318, 846.0999800, -2063.3999000, 11.9000000, 0.0000000, 88.7480000, 1.9940000 }, //Object number 51
			{ 1318, 843.5999800, -2063.3999000, 11.9000000, 0.0000000, 88.7480000, 1.9940000 }, //Object number 52
			{ 1318, 841.5999800, -2064.3999000, 11.9000000, 0.0000000, 88.7480000, 31.9940000 }, //Object number 53
			{ 1318, 839.9000200, -2065.5000000, 11.9000000, 0.0000000, 88.7480000, 31.9920000 }, //Object number 54
			{ 1318, 838.2999900, -2066.6001000, 11.9000000, 0.0000000, 88.7480000, 51.9920000 }, //Object number 55
			{ 1318, 834.9000200, -2066.2000000, 11.9000000, 0.0000000, 88.7480000, 329.9870000 }, //Object number 56
			{ 1318, 832.9000200, -2065.0000000, 11.9000000, 0.0000000, 88.7480000, 329.9850000 }, //Object number 57
			{ 1318, 830.2000100, -2063.3999000, 11.9000000, 0.0000000, 88.7480000, 329.9850000 }, //Object number 58
			{ 1318, 828.5000000, -2062.0000000, 11.9000000, 0.0000000, 88.7480000, 309.9850000 }, //Object number 59
			{ 1318, 826.5000000, -2060.2000000, 11.9000000, 0.0000000, 88.7480000, 279.9850000 }, //Object number 60
			{ 2480, 837.5000000, -2058.5000000, 12.3999996, 0.0000000, 0.0000000, 137.9992676 }, //Object number 61
			{ 2480, 837.3994141, -2058.5996094, 12.3999996, 0.0000000, 0.0000000, 318.4936523 }, //Object number 62
			{ 8355, 836.4003906, -3509.7001953, 11.8999996, 0.0000000, 0.0000000, 0.0000000 }, //Object number 63
			{ 8355, 876.2998047, -3509.7001953, 11.8999996, 0.0000000, 0.0000000, 0.0000000 }, //Object number 66
			{ 8355, 798.6992188, -3509.6992188, 11.8999996, 0.0000000, 0.0000000, 0.0000000 }, //Object number 67
			{ 3361, 836.4003906, -3435.6992188, 9.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 68
			{ 971, 837.1992188, -3398.0996094, 6.0999999, 90.0000000, 0.2526855, 357.9895020 }, //Object number 69
			{ 969, 829.0000000, -3391.5000000, 6.1999998, 89.7473145, 179.9945068, 178.2476807 }, //Object number 70
			{ 969, 845.2000100, -3395.1001000, 6.2000000, 89.7470000, 179.9950000, 358.7480000 }, //Object number 71
			{ 971, 837.0000000, -3405.2000000, 6.1000000, 90.0000000, 0.2530000, 357.9900000 }, //Object number 72
			{ 971, 836.7998047, -3412.1992188, 6.0999999, 90.0000000, 0.2526855, 357.9895020 }, //Object number 73
			{ 971, 836.5996094, -3419.2001953, 6.0999999, 90.0000000, 0.2526855, 357.9895020 }, //Object number 74
			{ 971, 836.3994141, -3426.2998047, 6.0999999, 90.0000000, 0.2526855, 357.9895020 }, //Object number 75
			{ 971, 836.2998047, -3429.5996094, 6.5000000, 65.7366943, 1.2139893, 357.3797607 }, //Object number 76
			{ 16115, 855.5000000, -3560.8999000, 14.7000000, 0.0000000, 0.0000000, 170.0000000 }, //Object number 77
			{ 16118, 784.2998047, -3546.3994141, 9.3999996, 0.0000000, 0.0000000, 0.0000000 }, //Object number 78
			{ 16118, 783.2999900, -3506.6001000, 9.4000000, 0.0000000, 0.0000000, 0.0000000 }, //Object number 79
			{ 16118, 783.0996094, -3472.5996094, 9.3999996, 0.0000000, 0.0000000, 0.0000000 }, //Object number 80
			{ 16118, 812.5999800, -3449.1001000, 10.0000000, 354.0000000, 0.0000000, 90.0000000 }, //Object number 81
			{ 16118, 877.0996094, -3447.5000000, 10.9000034, 0.0000000, 0.0000000, 90.0000000 }, //Object number 82
			{ 16118, 897.5999800, -3460.7000000, 11.3000000, 0.0000000, 0.0000000, 174.0000000 }, //Object number 83
			{ 16118, 898.9000200, -3504.8999000, 11.1000000, 0.0000000, 0.0000000, 173.9960000 }, //Object number 84
			{ 16118, 894.0996094, -3549.2998047, 9.6999998, 0.0000000, 0.0000000, 173.9959717 }, //Object number 85
			{ 971, 835.0999800, -3436.7000000, 10.7000000, 0.0000000, 182.0000000, 87.4920000 }, //Object number 86
			{ 971, 837.7000100, -3436.8999000, 10.7000000, 0.0000000, 182.0000000, 89.4900000 }, //Object number 87
			{ 971, 842.2747803, -3442.0000000, 13.5000000, 0.0000000, 179.9945068, 170.2386475 }, //Object number 88
			{ 971, 830.5991211, -3442.5996094, 13.5000000, 0.0000000, 179.9945068, 200.2368164 }, //Object number 89
			{ 970, 840.2999900, -3391.8999000, 6.8000000, 0.0000000, 0.0000000, 358.5000000 }, //Object number 90
			{ 970, 836.0999800, -3391.8000000, 6.8000000, 0.0000000, 0.0000000, 358.4950000 }, //Object number 91
			{ 970, 832.9000200, -3391.7000000, 6.8000000, 0.0000000, 0.0000000, 358.4950000 }, //Object number 92
			{ 970, 831.9000200, -3396.2000000, 6.8000000, 0.0000000, 0.0000000, 297.9950000 }, //Object number 93
			{ 970, 841.9000200, -3397.0000000, 6.8000000, 0.0000000, 0.0000000, 253.9930000 }, //Object number 94
			{ 970, 832.4000200, -3396.3999000, 6.8000000, 86.7500000, 0.0000000, 266.2430000 }, //Object number 95
			{ 970, 831.7999900, -3396.3999000, 6.8000000, 86.7480000, 0.0000000, 266.2430000 }, //Object number 96
			{ 970, 841.7999900, -3396.6001000, 6.8000000, 86.7480000, 0.0000000, 266.2430000 }, //Object number 97
			{ 970, 832.7998047, -3400.0996094, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 98
			{ 970, 832.7000100, -3404.2000000, 6.8000000, 0.0000000, 0.0000000, 268.7480000 }, //Object number 99
			{ 970, 832.5999800, -3408.2000000, 6.8000000, 0.0000000, 0.0000000, 268.7480000 }, //Object number 100
			{ 970, 832.5000000, -3412.3000000, 6.8000000, 0.0000000, 0.0000000, 268.7480000 }, //Object number 101
			{ 970, 832.4000200, -3416.3999000, 6.8000000, 0.0000000, 0.0000000, 268.7480000 }, //Object number 102
			{ 970, 832.2999900, -3420.5000000, 6.8000000, 0.0000000, 0.0000000, 268.7480000 }, //Object number 103
			{ 970, 832.2000100, -3424.6001000, 6.8000000, 0.0000000, 0.0000000, 268.7480000 }, //Object number 104
			{ 970, 841.3085938, -3400.9179688, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 105
			{ 19076, 871.7080078, -3475.4658203, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 106
			{ 19076, 871.8095703, -3544.5488281, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 107
			{ 19076, 871.7792969, -3524.0273438, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 108
			{ 19076, 870.7343750, -3494.0175781, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 109
			{ 19076, 807.2353516, -3478.6220703, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 110
			{ 19076, 801.6992188, -3506.9345703, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 111
			{ 19076, 801.5429688, -3530.6093750, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 112
			{ 19076, 803.4365234, -3546.1982422, 11.8000011, 0.0000000, 0.0000000, 0.0000000 }, //Object number 113
			{ 2208, 835.8046875, -3489.8662109, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 114
			{ 2208, 836.6784058, -3489.9260254, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 115
			{ 2208, 836.6616821, -3492.8361816, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 116
			{ 2208, 835.8171997, -3492.6928711, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 117
			{ 2208, 836.6542969, -3495.5498047, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 118
			{ 2208, 835.8034668, -3495.5498047, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 119
			{ 2208, 835.7960815, -3498.2641602, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 120
			{ 2208, 836.6467285, -3498.2636719, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 121
			{ 2208, 836.6326904, -3500.9038086, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 122
			{ 2208, 835.7810059, -3500.8867188, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 123
			{ 2208, 835.7960815, -3503.6025391, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 124
			{ 2208, 836.6196899, -3503.6347656, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 125
			{ 2208, 836.6163940, -3506.0078125, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 126
			{ 2208, 835.8173828, -3505.9609375, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 127
			{ 2208, 835.8054810, -3508.7470703, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 128
			{ 2208, 836.6328735, -3508.7492676, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 129
			{ 2208, 836.6242065, -3511.5229492, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 130
			{ 2208, 835.8234863, -3511.5217285, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 131
			{ 2208, 835.8145142, -3514.2714844, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 132
			{ 2208, 836.6152344, -3514.2714844, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 133
			{ 2208, 836.6293335, -3516.8334961, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 134
			{ 2208, 835.8072510, -3516.7585449, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 135
			{ 2208, 835.8098755, -3519.5483398, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 136
			{ 2208, 836.6064453, -3519.6103516, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 137
			{ 2208, 836.5738525, -3521.8266602, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 138
			{ 2208, 835.8222656, -3521.8115234, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 139
			{ 970, 841.2164917, -3404.9882812, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 140
			{ 970, 841.0769653, -3409.0163574, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 141
			{ 970, 840.9490356, -3413.1594238, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 142
			{ 970, 840.8032837, -3417.2502441, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 143
			{ 970, 840.6825562, -3421.3444824, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 144
			{ 970, 840.5860596, -3425.4455566, 6.8000002, 0.0000000, 0.0000000, 268.7475586 }, //Object number 145
			{ 971, 839.8955688, -3429.9389648, 6.5000000, 4.2527466, 179.4998779, 242.5236206 }, //Object number 146
			{ 971, 833.5072021, -3430.3647461, 6.5000000, 4.2517090, 179.4946289, 292.2731934 }, //Object number 147
			{ 1724, 835.7628784, -3488.2363281, 11.8999996, 0.0000000, 0.0000000, 0.0000000 }, //Object number 148
			{ 1723, 833.9857788, -3492.0441895, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 149
			{ 1724, 836.6820068, -3525.9655762, 11.8999996, 0.0000000, 0.0000000, 180.0000000 }, //Object number 150
			{ 1723, 833.9869385, -3494.9106445, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 151
			{ 1723, 833.9879150, -3497.7768555, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 152
			{ 1723, 833.9863281, -3500.7177734, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 153
			{ 1723, 833.9853516, -3503.6591797, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 154
			{ 1723, 833.9854736, -3506.5786133, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 155
			{ 1723, 833.9864502, -3509.4741211, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 156
			{ 1723, 833.9861450, -3512.4245605, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 157
			{ 1723, 833.9871216, -3515.3278809, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 158
			{ 1723, 833.9856567, -3518.2722168, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 159
			{ 1723, 833.9866943, -3521.1704102, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 160
			{ 1723, 833.9868164, -3524.0937500, 11.8999996, 0.0000000, 0.0000000, 90.0000000 }, //Object number 161
			{ 1723, 838.3532104, -3522.1584473, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 162
			{ 1723, 838.3638916, -3519.2680664, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 163
			{ 1723, 838.3739014, -3516.3217773, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 164
			{ 1723, 838.3836060, -3513.3710938, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 165
			{ 1723, 838.3784180, -3510.4272461, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 166
			{ 1723, 838.3760986, -3507.5300293, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 167
			{ 1723, 838.3743286, -3504.6315918, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 168
			{ 1723, 838.3719482, -3501.7092285, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 169
			{ 1723, 838.3697510, -3498.7834473, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 170
			{ 1723, 838.3764648, -3495.8427734, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 171
			{ 1723, 838.3714600, -3492.9121094, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 172
			{ 1723, 838.3663940, -3489.9807129, 11.8999996, 0.0000000, 0.0000000, 270.0000000 }, //Object number 173
			{ 2857, 836.2051392, -3490.3984375, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 174
			{ 2215, 836.1203003, -3491.9074707, 12.8402920, 334.6933594, 23.3539124, 12.7074890 }, //Object number 175
			{ 2453, 836.2333374, -3492.7600098, 13.0739841, 0.0000000, 0.0000000, 0.0000000 }, //Object number 176
			{ 2867, 836.3936157, -3494.7290039, 12.8163471, 0.0000000, 0.0000000, 0.0000000 }, //Object number 177
			{ 2858, 836.4409180, -3495.8483887, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 178
			{ 1950, 836.2875977, -3496.9155273, 12.9534607, 0.0000000, 0.0000000, 0.0000000 }, //Object number 179
			{ 1668, 836.4050903, -3498.1176758, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 180
			{ 1510, 836.1570435, -3497.6213379, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 181
			{ 1485, 835.9754639, -3497.5810547, 12.8152924, 0.0000000, 0.0000000, 0.0000000 }, //Object number 182
			{ 1484, 836.6802368, -3497.5559082, 12.7430668, 0.0000000, 296.0000000, 0.0000000 }, //Object number 183
			{ 1517, 836.2968140, -3523.7941895, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 184
			{ 1665, 836.3906250, -3498.5534668, 12.8507452, 0.0000000, 0.0000000, 0.0000000 }, //Object number 185
			{ 1517, 836.2886353, -3522.3144531, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 186
			{ 1517, 836.2153320, -3521.3173828, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 187
			{ 1517, 836.1057129, -3519.8217773, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 188
			{ 1517, 836.2092896, -3517.8090820, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 189
			{ 1517, 836.3491821, -3516.2949219, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 190
			{ 1517, 836.2395020, -3514.7993164, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 191
			{ 1517, 836.1483154, -3513.5524902, 12.9650478, 0.0000000, 0.0000000, 0.0000000 }, //Object number 192
			{ 1665, 836.4324951, -3501.1789551, 12.8507452, 0.0000000, 0.0000000, 0.0000000 }, //Object number 193
			{ 1665, 836.2922974, -3523.3156738, 12.8507452, 0.0000000, 0.0000000, 0.0000000 }, //Object number 194
			{ 1665, 836.1810303, -3521.7949219, 12.8507452, 0.0000000, 0.0000000, 0.0000000 }, //Object number 195
			{ 1668, 836.3222656, -3499.6142578, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 196
			{ 1668, 836.2675781, -3500.6123047, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 197
			{ 1668, 836.1718750, -3502.3588867, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 198
			{ 1668, 836.1035156, -3503.6059570, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 199
			{ 1668, 836.0214844, -3505.1025391, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 200
			{ 1668, 836.2027588, -3506.3637695, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 201
			{ 1668, 836.1201172, -3507.8603516, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 202
			{ 1668, 836.0517578, -3509.1079102, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 203
			{ 1668, 836.2056885, -3510.8676758, 12.9563303, 0.0000000, 0.0000000, 0.0000000 }, //Object number 204
			{ 2858, 836.3433838, -3499.1333008, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 205
			{ 2858, 836.2061768, -3507.1442871, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 206
			{ 2858, 836.2668457, -3509.8430176, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 207
			{ 2858, 836.2061768, -3511.9545898, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 208
			{ 2858, 836.2728271, -3515.4670410, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 209
			{ 2867, 836.1794434, -3518.7421875, 12.8163471, 0.0000000, 0.0000000, 0.0000000 }, //Object number 210
			{ 2867, 836.2399902, -3508.4848633, 12.8163471, 0.0000000, 0.0000000, 0.0000000 }, //Object number 211
			{ 2453, 836.2166748, -3500.0839844, 13.0739841, 0.0000000, 0.0000000, 0.0000000 }, //Object number 212
			{ 2453, 836.1344604, -3504.4150391, 13.0739841, 0.0000000, 0.0000000, 0.0000000 }, //Object number 213
			{ 2453, 836.0963135, -3514.1943359, 13.0739841, 0.0000000, 0.0000000, 0.0000000 }, //Object number 214
			{ 2215, 835.9970703, -3494.1528320, 12.8402920, 334.6929932, 23.3514404, 12.7056885 }, //Object number 215
			{ 2215, 836.0894165, -3502.9921875, 12.8402920, 334.6929932, 23.3514404, 12.7056885 }, //Object number 216
			{ 2215, 836.0399780, -3506.1684570, 12.8402920, 334.6929932, 23.3514404, 12.7056885 }, //Object number 217
			{ 2215, 836.7369995, -3515.5019531, 12.8402920, 334.6929932, 23.3459473, 12.7001953 }, //Object number 218
			{ 2857, 836.4150391, -3517.0156250, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 219
			{ 2857, 836.2773438, -3520.3105469, 12.7652931, 0.0000000, 0.0000000, 0.0000000 }, //Object number 220
			{ 14608, 839.5703125, -3546.2470703, 13.5460243, 0.0000000, 0.0000000, 319.9987793 }, //Object number 221
			{ 9833, 844.4716797, -3545.5664062, 8.4775524, 0.0000000, 0.0000000, 0.0000000 }, //Object number 222
			{ 9833, 833.9121094, -3546.1181641, 8.4775524, 0.0000000, 0.0000000, 0.0000000 }, //Object number 223
			{ 3524, 845.1171875, -3460.0986328, 14.7853394, 0.0000000, 0.0000000, 0.0000000 }, //Object number 224
			{ 3524, 831.0810547, -3460.8291016, 14.7853394, 0.0000000, 0.0000000, 0.0000000 }, //Object number 225
			{ 3472, 852.1141357, -3505.1638184, 11.6250038, 0.0000000, 0.0000000, 0.0000000 }, //Object number 226
			{ 3472, 821.5292969, -3526.5126953, 11.6250038, 0.0000000, 0.0000000, 0.0000000 }, //Object number 227
			{ 3472, 820.4375000, -3476.6347656, 11.6250038, 0.0000000, 0.0000000, 0.0000000 }, //Object number 228
			{ 1439, 852.7277832, -3463.9729004, 11.9249992, 0.0000000, 0.0000000, 0.0000000 }, //Object number 229
			{ 1439, 854.5849609, -3464.0034180, 11.9249992, 0.0000000, 0.0000000, 0.0000000 }, //Object number 230
			{ 1439, 856.3889160, -3463.9880371, 11.9249992, 0.0000000, 0.0000000, 0.0000000 }, //Object number 231
			{ 1439, 858.1901855, -3463.9470215, 11.9249992, 0.0000000, 0.0000000, 0.0000000 }, //Object number 232
			{ 1978, 861.3777466, -3499.5566406, 12.9256239, 0.0000000, 0.0000000, 0.0000000 }, //Object number 233
			{ 1978, 861.5664062, -3506.5712891, 12.9256239, 0.0000000, 0.0000000, 0.0000000 }, //Object number 234
			{ 1978, 861.5803223, -3512.8254395, 12.9256239, 0.0000000, 0.0000000, 0.0000000 }, //Object number 235
		})
		{
			StatkiPath[0] = std::initializer_list<SSpawnPoint>({//L
				{ 846.7998, -2107.2001, 18.4, 180.0 },//begin
				{ 846.7998, -3404.0996, 18.4, 180.0 },//destination

				//travel to begin from destination
				{ 897.5137, -3428.1580, 18.4, 270.0 },
				{ 959.9480, -3294.0354, 18.4, 0.0 },
				{ 930.4230, -2050.6565, 18.4, 0.0 },
				{ 906.9404, -2023.3687, 18.4, 90.0 },
				{ 872.0658, -2051.2031, 18.4, 145.0 }
			});

			StatkiPath[1] = std::initializer_list<SSpawnPoint>({//R
				{ 825.2998, -2107.2001, 18.4, 180.0 },//begin
				{ 825.2998, -3404.0996, 18.4, 180.0 },//destination

				//travel to begin from destination
				{ 792.2733, -3423.2170, 18.4, 95.0 },
				{ 722.2722, -3314.0747, 18.4, 0.0 },
				{ 686.3477, -2045.1881, 18.4, 0.0 },
				{ 719.0346, -2008.9291, 18.4, 270.0 },
				{ 777.0340, -2038.6691, 18.4, 235.0 },
			});
		}

		//statek, route, current/"last"
		std::map<int,std::pair<int,int>> statki;

		float SPEEDMULT[routes];
		const float SPEED = 10.0f;
		int waiting;

		std::map<int, SSpawnPoint> Prezenty;

		std::pair<int,float> GetNext(int which, int current)
		{
			int next = current + 1;
			float next_speed = SPEED;

			if (next >= StatkiPath[which].size())
				next = 0;

			if (next != 1)
				next_speed *= SPEEDMULT[which];

			//debug_fixSendClientMessageToAll(Functions::string_format("Wigilia::GetNext(%d,%d)->(%d,%f);", which, current, next, next_speed));
			return std::pair<int, float>(next,next_speed);
		}

		void TimerLaunch(int timerid, ZabawyTimerVector& vector)
		{
			//debug_fixSendClientMessageToAll("Wigilia::TimerLaunch");
			for (auto&i : statki)
			{				
				std::pair<int, float> nextdata = GetNext(i.second.first, i.second.second);
				i.second.second = nextdata.first;
				StreamerLibrary::MoveDynamicObject(i.first, StatkiPath[i.second.first][i.second.second].x, StatkiPath[i.second.first][i.second.second].y, StatkiPath[i.second.first][i.second.second].z, nextdata.second, 0.0, 0.0, StatkiPath[i.second.first][i.second.second].a);
				//debug_fixSendClientMessageToAll(Functions::string_format("Wigilia::TimerLaunch->Launch(%d,%d,%d);", i.first, i.second.first, i.second.second));
			}
		}

		bool OnGameModeInit()
		{
			MakeAllDynamicObjects(ArenaObjects, CurrentGameVW);

			statki[StreamerLibrary::CreateDynamicObject(8493, 846.7998, -2107.2001, 18.4, 0.0000000, 0.0000000, 180.0, CurrentGameVW)] = std::pair<int, int>(0, 0); //object(pirtshp01_lvs) (1)  L DEPART
			statki[StreamerLibrary::CreateDynamicObject(8493, 846.7998, -3404.0996, 18.4, 0.0000000, 0.0000000, 180.0, CurrentGameVW)] = std::pair<int, int>(0, 1); //object(pirtshp01_lvs) (4)  L DEST
																																
			statki[StreamerLibrary::CreateDynamicObject(8493, 825.2998, -2107.2001, 18.4, 0.0000000, 0.0000000, 180.0, CurrentGameVW)] = std::pair<int, int>(1, 0); //object(pirtshp01_lvs) (2)  R DEPART
			statki[StreamerLibrary::CreateDynamicObject(8493, 825.2998, -3404.0996, 18.4, 0.0000000, 0.0000000, 180.0, CurrentGameVW)] = std::pair<int, int>(1, 1); //object(pirtshp01_lvs) (3)  R DEST

			for (size_t j = 0; j < routes; ++j)
			{
				SPEEDMULT[j] = 0.0f;
				for (size_t i = 2; i < (StatkiPath[j].size() - 2); ++i)
					SPEEDMULT[j] += glm::distance(glm::vec2(StatkiPath[j][i].x, StatkiPath[j][i].y), glm::vec2(StatkiPath[j][i+1].x, StatkiPath[j][i+1].y));
				SPEEDMULT[j] /= glm::distance(glm::vec2(StatkiPath[j][0].x, StatkiPath[j][0].y), glm::vec2(StatkiPath[j][1].x, StatkiPath[j][1].y));
			}

			waiting = 0;
			CreateTimer<Arena>(20000, false, &Arena::TimerLaunch);

			Prezenty[StreamerLibrary::CreateDynamicPickup(19054, 1, 868.9197, -3542.6719, 12.9000, CurrentGameVW, 0)] = { 868.9197, -3542.6719, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19057, 1, 839.5009, -3544.2490, 12.9000, CurrentGameVW, 0)] = { 839.5009, -3544.2490, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19055, 1, 804.8654, -3546.2144, 12.9000, CurrentGameVW, 0)] = { 804.8654, -3546.2144, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19056, 1, 803.7242, -3531.0906, 12.9000, CurrentGameVW, 0)] = { 803.7242, -3531.0906, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19055, 1, 804.0604, -3506.7988, 12.9000, CurrentGameVW, 0)] = { 804.0604, -3506.7988, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19054, 1, 808.5134, -3480.5779, 12.9000, CurrentGameVW, 0)] = { 808.5134, -3480.5779, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19055, 1, 869.4737, -3475.9109, 12.9000, CurrentGameVW, 0)] = { 869.4737, -3475.9109, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19056, 1, 867.8204, -3494.0415, 12.9000, CurrentGameVW, 0)] = { 867.8204, -3494.0415, 12.9000, 0.0 };
			Prezenty[StreamerLibrary::CreateDynamicPickup(19057, 1, 869.5078, -3523.1841, 12.9000, CurrentGameVW, 0)] = { 869.5078, -3523.1841, 12.9000, 0.0 };
			
			return true;
		}

		bool OnDynamicObjectMoved(int objectid) 
		{ 
			auto i = statki.find(objectid);
			if (i != statki.end())
			{
				if (i->second.second == 0 || i->second.second == 1)
				{
					++waiting;
					//debug_fixSendClientMessageToAll(Functions::string_format("Wigilia::OnDynamicObjectMoved::waiting++(%d)[%d,%d]", waiting, i->second.second, objectid));
				}
				else
				{
					std::pair<int, float> nextdata = GetNext(i->second.first, i->second.second);
					i->second.second = nextdata.first;
					StreamerLibrary::MoveDynamicObject(i->first, StatkiPath[i->second.first][i->second.second].x, StatkiPath[i->second.first][i->second.second].y, StatkiPath[i->second.first][i->second.second].z, nextdata.second, 0.0, 0.0, StatkiPath[i->second.first][i->second.second].a);
					//debug_fixSendClientMessageToAll(Functions::string_format("Wigilia::OnDynamicObjectMoved->Launch(%d,%d,%d);", i->first, i->second.first, i->second.second));
				}
			}
			if (waiting >= 4)
			{
				waiting = 0;
				CreateTimer<Arena>(20000, false, &Arena::TimerLaunch);
				//debug_fixSendClientMessageToAll("Wigilia::TimerLaunch::ActivateTimer");
			}
			return true; 
		}

		void PutPlayerIntoGame(int playerid, bool respawn = false)
		{
			fixSetPlayerHealth(playerid, 256000.0f);
			fixSetPlayerArmour(playerid, 256000.0f);

			TeleportPlayer(playerid, 838.1479f, -2012.3489f, 12.8672f, 180.0f, false, 0, CurrentGameVW, "", 0, 0, 2.0, 2.0, true);

			if (!respawn && url.size() >= 16)
				PlayAudioStreamForPlayer(playerid, url.c_str(), 0.0, 0.0, 0.0, 0.0, false);
		}

		bool OnKeepInGameSpawn(int playerid)
		{
			AddPlayer(playerid, true, -1);
			PutPlayerIntoGame(playerid, true);
			return true;
		}

		bool OnGameCommandExecute(int playerid, std::string params)
		{
			if (UniversalModeDate.date().year_month_day().day == 24 && UniversalModeDate.date().year_month_day().month == 12)
			{
				Player[playerid].KeepInGame = true;
				PutPlayerIntoGame(playerid);
				return true;
			}
			return false;
		}

		bool PlayerRequestGameExit(int playerid, int reason)
		{
			switch (reason)
			{
			case PlayerRequestExitTypeDisconnect:
				PlayerExitGame(playerid, false, false, false, true);
				break;
			case PlayerRequestExitTypeExit:
				StopAudioStreamForPlayer(playerid);
				PlayerExitGame(playerid, true, false, true, true);
				break;
			}
			return true;
		}

		bool OnPlayerPickUpDynamicPickup(int playerid, int pickupid)
		{
			auto &i = PlayerWigilia[Player[playerid].PlayerName];

			auto found = Prezenty.find(pickupid);
			if (found != Prezenty.end())
			{
				if (i.LastPickup != pickupid)
				{
					i.LastPickup = pickupid;
					if (i.OtwartePrezenty.find(pickupid) == i.OtwartePrezenty.end())
						fixSendClientMessage(playerid, -1, "Wpisz {41BECC}/prezent{FFFFFF} aby otworzyæ");
					else
						fixSendClientMessage(playerid, -1, "Ten prezent ju¿ zosta³ otwarty przez Ciebie");
				}
			}
			return true;
		}
	} 
	//_Arena
		;
		/*
	ZCMDF(oplatek, PERMISSION_NONE, SetCommandPermissionGameID(RESTRICTION_ONLY_IN_A_GAME, _Arena.CurrentGameID), cmd_alias({}), "ps")
	{
		auto &i = PlayerWigilia[Player[playerid].PlayerName];
		if (parser.Good() == 2)
		{
			int targetid = parser.Get<ParsePlayer>(0).playerid;
			std::string ¿yczenia = parser.Get<std::string>(1);
			if (targetid != INVALID_PLAYER_ID && Player[targetid].CurrentGameID == _Arena.CurrentGameID && targetid != playerid)
			{
				auto& t = PlayerWigilia[Player[targetid].PlayerName];
				if (t.OtrzymaneZyczenia.find(Player[playerid].PlayerName) == t.OtrzymaneZyczenia.end())
				{				
					Player[playerid].GiveExperience((unsigned long long)7);
					Player[playerid].GiveMoney(7000);
					Player[playerid].GiveScore(1);

					std::string toshow;
					t.OtrzymaneZyczenia[Player[playerid].PlayerName] = ¿yczenia;

					for (auto z : t.OtrzymaneZyczenia)
					{
						if (boost::iequals(Player[playerid].PlayerName, z.first))
							toshow.append("{9EDBF9}" + z.first + " podzieli³(a) siê op³atkiem oraz ¿yczy: " + z.second + "\n");
						else
							toshow.append("{FFFFFF}" + z.first + " podzieli³(a) siê op³atkiem oraz ¿yczy: " + z.second + "\n");
					}

					ShowPlayerCustomDialog(targetid, DLG_DUMMY, DIALOG_STYLE_MSGBOX, "Otrzymano nowe ¿yczenia od " + Player[playerid].PlayerName, toshow, "V", "X");
					fixSendClientMessage(playerid, -1, "¯yczenia wys³ane oraz podzielono siê op³atkiem");
					fixSendClientMessage(playerid, -1, Player[playerid].PlayerName + " podzieli³(a) siê z Tob¹ op³atkiem oraz sk³ada Tobie ¿yczenia: ");
					fixSendClientMessage(playerid, -1, "____ " + ¿yczenia);

					return true;
				}
				fixSendClientMessage(playerid, -1, "Gracz ju¿ otrzyma³ od Ciebie ¿yczenia");
				return true;
			}
			fixSendClientMessage(playerid, -1, "Podane ID nie uczêszcza na tê Wigilie");
			return true;
		}
		fixSendClientMessage(playerid, -1, "Wpisz {41BECC}/oplatek <id/nick> <¿yczenia>{FFFFFF} aby podzieliæ siê Swoim op³atkiem z graczem");
		return true;
	}

	typedef std::pair<int, int> ipair;
	typedef std::pair<int, ipair> vpair;

	const std::vector<int> losowebronie_noammo = 
	{ 
		1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 1 
	};

	const std::vector<vpair> losowebronie_yesammo = 
	{ 
		vpair(16, ipair(10,  20)),
		vpair(17, ipair(500, 1000)),
		vpair(18, ipair(10,  20)),
		vpair(22, ipair(100, 200)),
		vpair(23, ipair(100, 200)),
		vpair(25, ipair(100, 200)),
		vpair(26, ipair(100, 200)),
		vpair(27, ipair(100, 200)),
		vpair(28, ipair(300, 600)),
		vpair(29, ipair(100, 600)),
		vpair(30, ipair(50,  200)),
		vpair(31, ipair(50,  200)),
		vpair(33, ipair(100, 200)),
		vpair(34, ipair(100, 200)),
		vpair(35, ipair(2, 10)),
		vpair(36, ipair(2, 10)),
		vpair(37, ipair(100, 1000)),
		vpair(39, ipair(5, 50)),
		vpair(16, ipair(10, 20))
	};

	ZCMD3(prezent, PERMISSION_NONE, SetCommandPermissionGameID(RESTRICTION_ONLY_IN_A_GAME, _Arena.CurrentGameID))
	{
		auto &i = PlayerWigilia[Player[playerid].PlayerName];

		auto found = _Arena.Prezenty.find(i.LastPickup);
		if (found != _Arena.Prezenty.end())
		{
			if (GetPlayerDistanceFromPoint(playerid, found->second.x, found->second.y, found->second.z) < 5.0f)
			{
				if (i.OtwartePrezenty.find(found->first) == i.OtwartePrezenty.end())
				{
					i.OtwartePrezenty.insert(found->first);

					std::string what;
					int pseudorandnum = Functions::RandomGenerator->Random(0, 6);

					switch (pseudorandnum)
					{
						case 0:
						{
							int weaponid = losowebronie_noammo[Functions::RandomGenerator->Random(0, losowebronie_noammo.size() - 1)];
							
							GivePlayerBuyWeapon(playerid, weaponid, 0, true);

							what = "Broñ " + WeaponInfo[weaponid].weaponName;
						}
						break;

						case 1:
						{
							vpair weaponid = losowebronie_yesammo[Functions::RandomGenerator->Random(0, losowebronie_yesammo.size() - 1)];

							int ammo = Functions::RandomGenerator->Random(weaponid.second.first, weaponid.second.second);

							GivePlayerBuyWeapon(playerid, weaponid.first, ammo, true);

							what = Functions::string_format("Broñ " + WeaponInfo[weaponid.first].weaponName + " oraz %d amunicji", ammo);
						}
						break;

						case 2:
						{
							unsigned long long respect = Functions::RandomGenerator->Random(1, 5);
							Player[playerid].GiveScore(respect);

							what = Functions::string_format("%I64u respektu", respect);
						}
						break;

						case 3:
						{
							unsigned long long exp = Functions::RandomGenerator->Random(10, 50);
							Player[playerid].GiveExperience(exp);

							what = Functions::string_format("%I64u punktów doœwiadczenia", exp);
						}
						break;

						default:
						{
							unsigned long long money = Functions::RandomGenerator->Random(10000, 100000);
							Player[playerid].GiveMoney(money);

							what = Functions::string_format("$%I64u", money);
						}
						break;
					}
					fixSendClientMessageToAll(-1, Player[playerid].PlayerName + " otworzy³(a) prezent z zawartoœci¹: " + what);
					return true;
				}
				fixSendClientMessage(playerid, -1, "Ten prezent ju¿ zosta³ otwarty przez Ciebie");
				return true;
			}
		}
		
		fixSendClientMessage(playerid, -1, "PodejdŸ do jakiejkolwiek choinki z prezentem gdy¿ jesteœ za daleko od jakiegokolwiek prezentu");
		return true;
	}

	ZCMD3(wigiliaplay, PERMISSION_MODERATOR, SetCommandPermissionGameID(RESTRICTION_ONLY_IN_A_GAME, _Arena.CurrentGameID))
	{
		if (params.size() >= 16)
		{
			url.assign(params);
			for(auto i: _Arena.PlayersInGame)
				PlayAudioStreamForPlayer(i, url.c_str(), 0.0, 0.0, 0.0, 0.0, false);
			fixSendClientMessage(playerid, -1, "Za³¹czono utwór dla wszystkich na Wigilii");
			return true;
		}
		fixSendClientMessage(playerid, -1, "U¿ycie: {41BECC}/wigiliaplay <link>{FFFFFF} np. {41BECC}/wigiliaplay http://music.com/dance-festival.mp3");
		return true;
	}
	*/
};