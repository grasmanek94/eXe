#include "CarColorGenerator.hxx"

namespace GTACarColorGenerator
{
	using color_pair = std::array < int, 2 > ;

	template<typename T> bool is_number_signed(std::string s, T& val)
	{
		T _Val;
		if (boost::conversion::try_lexical_convert<T>(s, _Val))
		{
			val = _Val;
			return true;
		}
		return false;
	}

	const std::vector<color_pair> GetColorsFromSpacedLine(const std::string& line)
	{
		std::vector<std::string> split_part;
		//= Functions::string_split(line, ' ');
		std::stringstream ss(line);
		std::string item;
		while (std::getline(ss, item, ' '))
		{
			split_part.push_back(item);
		}
		std::vector<color_pair> return_value;
		assert(split_part.size() % 2 == 0);
		for (size_t i = 0; i < split_part.size(); i += 2)
		{
			std::array<int, 2> numbers;
			std::array<bool, 2> checks = { { is_number_signed(split_part[i], numbers[0]), is_number_signed(split_part[i + 1], numbers[1]) } };
			assert(checks[0] == true);
			assert(checks[1] == true);
			return_value.push_back(color_pair{{ numbers[0], numbers[1] }});
		}
		return return_value;
	}

	const std::array<std::vector<color_pair>, 212> VehicleRealColors =
	{ {
		{ GetColorsFromSpacedLine("4 1 123 1 113 1 101 1 75 1 62 1 40 1 36 1") },
		{ GetColorsFromSpacedLine("41 41 47 47 52 52 66 66 74 74 87 87 91 91 113 113") },
		{ GetColorsFromSpacedLine("10 10 13 13 22 22 30 30 39 39 90 90 98 98 110 110") },
		{ GetColorsFromSpacedLine("36 1 37 1 30 1 28 1 25 1 40 1 101 1 113 1") },
		{ GetColorsFromSpacedLine("113 39 119 50 123 92 109 100 101 101 95 105 83 110 66 25") },
		{ GetColorsFromSpacedLine("11 1 24 1 36 1 40 1 75 1 91 1 123 1 4 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("3 1") },
		{ GetColorsFromSpacedLine("26 26") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("4 1 9 1 10 1 25 1 36 1 40 1 45 1 84 1") },
		{ GetColorsFromSpacedLine("12 1 64 1 123 1 116 1 112 1 106 1 80 1 75 1") },
		{ GetColorsFromSpacedLine("9 1 10 8 11 1 25 8 27 1 29 8 30 1 37 8") },
		{ GetColorsFromSpacedLine("87 1 88 1 91 1 105 1 109 1 119 1 4 1 25 1") },
		{ GetColorsFromSpacedLine("25 1 28 1 43 1 67 1 72 1 9 1 95 1 24 1") },
		{ GetColorsFromSpacedLine("20 1 25 1 36 1 40 162 1 75 1 92 1 0 1 0") },
		{ GetColorsFromSpacedLine("1 3") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("119 119 117 227 114 114 108 108 95 95 81 81 61 61 41 41") },
		{ GetColorsFromSpacedLine("45 75 47 76 33 75 13 76 54 75 69 76 59 75 87 76") },
		{ GetColorsFromSpacedLine("6 1") },
		{ GetColorsFromSpacedLine("4 1 13 1 25 1 30 1 36 1 40 1 75 1 95 1") },
		{ GetColorsFromSpacedLine("96 25 97 25 101 25 111 31 113 36 83 57 67 59") },
		{ GetColorsFromSpacedLine("1 16 1 56 1 17 1 53 1 5 1 35") },
		{ GetColorsFromSpacedLine("1 0 2 2 3 2 3 6 6 16 15 30 24 53 35 61") },
		{ GetColorsFromSpacedLine("43 0") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("0 1") },
		{ GetColorsFromSpacedLine("4 75") },
		{ GetColorsFromSpacedLine("12 12 13 13 14 14 1 2 2 1 1 3 3 1 10 10") },
		{ GetColorsFromSpacedLine("46 26") },
		{ GetColorsFromSpacedLine("71 59 75 59 92 72 47 74 55 83 59 83 71 87 82 87") },
		{ GetColorsFromSpacedLine("43 0") },
		{ GetColorsFromSpacedLine("43 0") },
		{ GetColorsFromSpacedLine("1 1 12 12 2 2 6 6 4 4 46 46 53 53") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("83 1 87 1 92 1 95 1 109 1 119 45 11 1 ") },
		{ GetColorsFromSpacedLine("54 7 79 7 87 7 95 16 98 20 105 20 123 20 125 21") },
		{ GetColorsFromSpacedLine("6 76") },
		{ GetColorsFromSpacedLine("57 8 8 17 43 21 54 38 67 8 37 78 65 79 25 78") },
		{ GetColorsFromSpacedLine("34 34 32 32 20 20 110 110 66 66 84 84 118 118 121 121") },
		{ GetColorsFromSpacedLine("2 96 79 42 82 54 67 86 126 96 70 96 110 54 67 98") },
		{ GetColorsFromSpacedLine("0 0 11 105 25 109 36 0 40 36 75 36 0 36 0 109") },
		{ GetColorsFromSpacedLine("4 1 20 1 24 1 25 1 36 1 40 1 54 1 84 1") },
		{ GetColorsFromSpacedLine("32 36 32 42 32 53 32 66 32 14 32 32") },
		{ GetColorsFromSpacedLine("34 34 35 35 37 37 39 39 41 41 43 43 45 45 47 47") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("75 2") },
		{ GetColorsFromSpacedLine("3 6") },
		{ GetColorsFromSpacedLine("1 74") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("123 123 125 125 36 36 16 16 18 18 46 46 61 61 75 75") },
		{ GetColorsFromSpacedLine("1 3 1 5 1 16 1 22 1 35 1 44 1 53 1 57") },
		{ GetColorsFromSpacedLine("56 56") },
		{ GetColorsFromSpacedLine("26 26") },
		{ GetColorsFromSpacedLine("84 15 84 58 84 31 32 74 43 31 1 31 77 31 32 74") },
		{ GetColorsFromSpacedLine("84 63 91 63 102 65 105 72 110 93 121 93 12 95 23 1") },
		{ GetColorsFromSpacedLine("58 1 2 1 63 1 18 1 32 1 45 1 13 1 34 1") },
		{ GetColorsFromSpacedLine("91 1 101 1 109 1 113 1 4 1 25 1 30 1 36 1") },
		{ GetColorsFromSpacedLine("26 26 28 28 44 44 51 51 57 57 72 72 106 106 112 112") },
		{ GetColorsFromSpacedLine("1 3 1 9 1 18 1 30 17 23 46 23 46 32 57 34") },
		{ GetColorsFromSpacedLine("36 1 37 1 43 1 53 1 61 1 75 1 79 1 88 1") },
		{ GetColorsFromSpacedLine("12 12 13 13 14 14 1 2 2 1 1 3 3 1 10 10") },
		{ GetColorsFromSpacedLine("79 79 84 84 7 7 11 11 19 19 22 22 36 36 53 53") },
		{ GetColorsFromSpacedLine("14 75") },
		{ GetColorsFromSpacedLine("14 75") },
		{ GetColorsFromSpacedLine("67 76 68 76 78 76 2 76 16 76 18 76 25 76 45 88") },
		{ GetColorsFromSpacedLine("51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8") },
		{ GetColorsFromSpacedLine("6 6 46 46 53 53 3 3") },
		{ GetColorsFromSpacedLine("1 3") },
		{ GetColorsFromSpacedLine("43 0") },
		{ GetColorsFromSpacedLine("120 117 103 111 120 114 74 91 120 112 74 83 120 113 66 71") },
		{ GetColorsFromSpacedLine("56 15 56 53") },
		{ GetColorsFromSpacedLine("56 15 56 53") },
		{ GetColorsFromSpacedLine("97 1 81 1 105 1 110 1 91 1 74 1 84 1 83 1") },
		{ GetColorsFromSpacedLine("2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29") },
		{ GetColorsFromSpacedLine("6 7 7 6 1 6 89 91 119 117 103 102 77 87 71 77") },
		{ GetColorsFromSpacedLine("92 1 94 1 101 1 121 1 0 1 22 1 36 1 75 1") },
		{ GetColorsFromSpacedLine("72 1 66 1 59 1 45 1 40 1 39 1 35 1 20 1") },
		{ GetColorsFromSpacedLine("27 36 59 36 60 35 55 41 54 31 49 23 45 32 40 29") },
		{ GetColorsFromSpacedLine("73 45 12 12 2 2 6 6 4 4 46 46 53 53") },
		{ GetColorsFromSpacedLine("1 1 3 3 6 6 46 46 65 9 14 1 12 9 26 1") },
		{ GetColorsFromSpacedLine("41 41 48 48 52 52 64 64 71 71 85 85 10 10 62 62") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("12 35 50 32 40 26 66 36") },
		{ GetColorsFromSpacedLine("1 73 1 74 1 75 1 76 1 77 1 78 1 79") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("26 14 29 42 26 57 54 29 26 3 3 29 12 39 74 35") },
		{ GetColorsFromSpacedLine("2 26 2 29") },
		{ GetColorsFromSpacedLine("13 118 14 123 120 123 112 120 84 110 76 102") },
		{ GetColorsFromSpacedLine("0 0") },
		{ GetColorsFromSpacedLine("40 65 71 72 52 66 64 72 30 72 60 72") },
		{ GetColorsFromSpacedLine("30 26 77 26 81 27 24 55 28 56 49 59 52 69 71 107") },
		{ GetColorsFromSpacedLine("36 13") },
		{ GetColorsFromSpacedLine("36 117 36 13 42 30 42 33 54 36 75 79 92 101 98 109") },
		{ GetColorsFromSpacedLine("123 124 119 122 118 117 116 115 114 108 101 106 88 99 5 6") },
		{ GetColorsFromSpacedLine("74 72 66 72 53 56 37 19 22 22 20 20 9 14 0 0") },
		{ GetColorsFromSpacedLine("0 1") },
		{ GetColorsFromSpacedLine("11 123 13 120 20 117 24 112 27 107 36 105 37 107 43 93") },
		{ GetColorsFromSpacedLine("109 25 109 32 112 32 10 32 30 44 32 52 84 66 84 69") },
		{ GetColorsFromSpacedLine("75 84 40 84 40 110 28 119 25 119 21 119 13 119 4 119") },
		{ GetColorsFromSpacedLine("14 75") },
		{ GetColorsFromSpacedLine("7 94 36 88 51 75 53 75 58 67 75 67 75 61 79 62") },
		{ GetColorsFromSpacedLine("83 66 87 74 87 75 98 83 101 100 103 101 117 116 123 36") },
		{ GetColorsFromSpacedLine("51 39 57 38 45 29 34 9 65 9 14 1 12 9 26 1") },
		{ GetColorsFromSpacedLine("13 118 14 123 120 123 112 120 84 110 76 102") },
		{ GetColorsFromSpacedLine("3 3 6 6 7 7 52 52 76 76") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("7 1 74 1 61 1 16 1 25 1 30 1 36 1 53 1") },
		{ GetColorsFromSpacedLine("43 43 46 46 39 39 28 28 16 16 6 6 5 5 2 2") },
		{ GetColorsFromSpacedLine("3 90 4 90 7 68 8 66 12 60 27 97 34 51 37 51") },
		{ GetColorsFromSpacedLine("17 39 15 123 32 112 45 88 52 71 57 67 61 96 96 96") },
		{ GetColorsFromSpacedLine("38 51 21 36 21 34 30 34 54 34 55 20 48 18 51 6") },
		{ GetColorsFromSpacedLine("10 1 25 1 28 1 36 1 40 1 54 1 75 1 113 1") },
		{ GetColorsFromSpacedLine("13 76 24 77 63 78 42 76 54 77 39 78 11 76 62 77") },
		{ GetColorsFromSpacedLine("116 1 119 1 122 1 4 1 9 1 24 1 27 1 36 1") },
		{ GetColorsFromSpacedLine("37 36 36 36 40 36 43 41 47 41 51 72 54 75 55 84") },
		{ GetColorsFromSpacedLine("2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("74 74 75 13 87 118 92 3 115 118 25 118 36 0 118 118") },
		{ GetColorsFromSpacedLine("3 3 3 8 6 25 7 79 8 82 36 105 39 106 51 118") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("1 1 17 20 18 20 22 30 36 43 44 51 52 54") },
		{ GetColorsFromSpacedLine("2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29") },
		{ GetColorsFromSpacedLine("52 1 53 1 66 1 75 1 76 1 81 1 95 1 109 1") },
		{ GetColorsFromSpacedLine("0 1") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("110 1 111 1 112 1 114 1 119 1 122 1 4 1 13 1") },
		{ GetColorsFromSpacedLine("2 35 36 2 51 53 91 2 11 22 40 35") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("73 1 74 1 75 1 77 1 79 1 83 1 84 1 91 1") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("3 1 28 1 31 1 55 1 66 197 1 123 1 118 1 0") },
		{ GetColorsFromSpacedLine("9 1 12 1 26 96 30 96 32 1 37 1 57 96 71 96") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("96 67 86 70 79 74 70 86 61 98 75 75 75 91") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8") },
		{ GetColorsFromSpacedLine("13 118 24 118 31 93 32 92 45 92 113 92 119 113 122 113") },
		{ GetColorsFromSpacedLine("76 8 32 8 43 8 67 8 11 11 8 90 2 2 83 13") },
		{ GetColorsFromSpacedLine("3 1") },
		{ GetColorsFromSpacedLine("50 1 47 1 44 96 40 96 39 1 30 1 28 96 9 96") },
		{ GetColorsFromSpacedLine("62 37 78 38 2 62 3 87 2 78 113 78 119 62 7 78") },
		{ GetColorsFromSpacedLine("122 1 123 1 125 1 10 1 24 1 37 1 55 1 66 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("74 39 72 39 75 39 79 39 83 36 84 36 89 35 91 35") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("67 1 72 1 75 1 83 1 91 1 101 1 109 1 20 1") },
		{ GetColorsFromSpacedLine("56 56 49 49 26 124") },
		{ GetColorsFromSpacedLine("38 9 55 23 61 74 71 87 91 87 98 114 102 119 111 3") },
		{ GetColorsFromSpacedLine("53 32 15 32 45 32 34 30 65 32 14 32 12 32 43 32") },
		{ GetColorsFromSpacedLine("51 1 58 1 60 1 68 1 2 1 13 1 22 1 36 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("112 1 116 1 117 1 24 1 30 1 35 1 36 1 40 1") },
		{ GetColorsFromSpacedLine("51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8") },
		{ GetColorsFromSpacedLine("52 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29") },
		{ GetColorsFromSpacedLine("57 8 8 17 43 21 54 38 67 8 37 78 65 79 25 78") },
		{ GetColorsFromSpacedLine("36 1 35 1 17 1 11 1 116 1 113 1 101 1 92 1") },
		{ GetColorsFromSpacedLine("1 6") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("109 1 30 8 95 1 84 8 83 1 72 8 71 1 52 8") },
		{ GetColorsFromSpacedLine("97 96 88 64 90 96 93 64 97 96 99 81 102 114 114 1") },
		{ GetColorsFromSpacedLine("2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("2 35 36 2 51 53 91 2 11 22 40 35") },
		{ GetColorsFromSpacedLine("94 1 101 1 116 1 117 1 4 1 25 1 30 1 37 1") },
		{ GetColorsFromSpacedLine("91 38 115 43 85 6 79 7 78 8 77 18 79 18 86 24") },
		{ GetColorsFromSpacedLine("26 26") },
		{ GetColorsFromSpacedLine("12 1 19 96 31 64 25 96 38 1 51 96 57 1 66 96") },
		{ GetColorsFromSpacedLine("67 1 68 96 72 1 74 8 75 96 76 8 79 1 84 96") },
		{ GetColorsFromSpacedLine("1 3 8 7 8 10 8 16 23 31 40 44") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("92 92 81 81 67 67 66 66 61 61 53 53 51 51 47 47 43 43") },
		{ GetColorsFromSpacedLine("54 1 58 1 66 1 72 1 75 1 87 1 101 1 36 1") },
		{ GetColorsFromSpacedLine("41 10 41 20 49 11 56 123 110 113 112 116 114 118 119 101") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15") },
		{ GetColorsFromSpacedLine("119 1 122 1 8 1 10 1 13 1 25 1 27 1 32 1") },
		{ GetColorsFromSpacedLine("36 1 40 1 43 1 53 1 72 1 75 1 95 1 101 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("37 37 31 31 23 23 22 22 7 7 124 124 114 114 112 112") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("112 20") },
		{ GetColorsFromSpacedLine("0 1") },
		{ GetColorsFromSpacedLine("0 1") },
		{ GetColorsFromSpacedLine("0 1") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("81 8 32 8 43 8 67 8 11 11 8 90 2 2 83 13") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("58 1 69 1 75 77 18 1 32 1 45 45 13 1 34 1") },
		{ GetColorsFromSpacedLine("58 1 69 1 75 77 18 1 32 1 45 45 13 1 34 1") },
		{ GetColorsFromSpacedLine("67 76 68 76 78 76 2 76 16 76 18 76 25 76 45 88") },
		{ GetColorsFromSpacedLine("61 8 32 8 43 8 67 8 11 11 8 90 2 2 83 13") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("1 1") },
		{ GetColorsFromSpacedLine("36 36") },
		{ GetColorsFromSpacedLine("-1 -1") },
		{ GetColorsFromSpacedLine("-1 -1") }
	} };

	const std::array<int, 2> RandomColor()
	{
		int col1 = GTAMath::Random(0, 126);
		int col2 = GTAMath::Random(0, 126);

		return{ { col1, col2 } };
	}

	const std::array<int, 2> GenerateColorByCarModel(int modelid)
	{
		return 
			VehicleRealColors[modelid - SAMP_VEHICLE_PADDING][0][0] == -1 ?
			RandomColor() : 
			VehicleRealColors[modelid - SAMP_VEHICLE_PADDING][rand() % VehicleRealColors[modelid - SAMP_VEHICLE_PADDING].size()];
	}
}

//old generation extension used for generating the above color array
#if 0
#include "GameMode.hxx"
const std::vector<std::string> carcolsdat_parsed =
{
	{ "admiral 34 34 35 35 37 37 39 39 41 41 43 43 45 45 47 47" },
	{ "alpha 58 1 69 1 75 77 18 1 32 1 45 45 13 1 34 1" },
	{ "ambulan 1 3" },
	{ "androm 1 1" },
	{ "artict1 1 1" },
	{ "artict2 1 1" },
	{ "artict3 1 1" },
	{ "at400 1 3 8 7 8 10 8 16 23 31 40 44" },
	{ "baggage 1 73 1 74 1 75 1 76 1 77 1 78 1 79" },
	{ "bandito 2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29" },
	{ "banshee 12 12 13 13 14 14 1 2 2 1 1 3 3 1 10 10" },
	{ "barracks 43 0" },
	{ "beagle 3 90 4 90 7 68 8 66 12 60 27 97 34 51 37 51" },
	{ "benson 109 25 109 32 112 32 10 32 30 44 32 52 84 66 84 69" },
	{ "bf400 54 1 58 1 66 1 72 1 75 1 87 1 101 1 36 1" },
	{ "bfinject 1 0 2 2 3 2 3 6 6 16 15 30 24 53 35 61" },
	{ "bike 7 1 74 1 61 1 16 1 25 1 30 1 36 1 53 1" },
	{ "blade 9 1 12 1 26 96 30 96 32 1 37 1 57 96 71 96" },
	{ "blistac 74 72 66 72 53 56 37 19 22 22 20 20 9 14 0 0" },
	{ "bloodra 51 39 57 38 45 29 34 9 65 9 14 1 12 9 26 1" },
	{ "bmx 1 1 3 3 6 6 46 46 65 9 14 1 12 9 26 1" },
	{ "bobcat 96 25 97 25 101 25 111 31 113 36 83 57 67 59" },
	{ "boxburg 36 36" },
	{ "boxville 11 123 13 120 20 117 24 112 27 107 36 105 37 107 43 93" },
	{ "bravura 41 41 47 47 52 52 66 66 74 74 87 87 91 91 113 113" },
	{ "broadway 12 1 19 96 31 64 25 96 38 1 51 96 57 1 66 96" },
	{ "buccanee 2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29" },
	{ "buffalo 10 10 13 13 22 22 30 30 39 39 90 90 98 98 110 110" },
	{ "bullet 51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8" },
	{ "burrito 41 41 48 48 52 52 64 64 71 71 85 85 10 10 62 62" },
	{ "bus 71 59 75 59 92 72 47 74 55 83 59 83 71 87 82 87" },
	{ "cabbie 6 76" },
	{ "caddy 58 1 2 1 63 1 18 1 32 1 45 1 13 1 34 1" },
	{ "cadrona 52 1 53 1 66 1 75 1 76 1 81 1 95 1 109 1" },
	{ "cargobob 1 1" },
	{ "cheetah 20 1 25 1 36 1 40 162 1 75 1 92 1 0 1 0" },
	{ "clover 13 118 24 118 31 93 32 92 45 92 113 92 119 113 122 113" },
	{ "club 37 37 31 31 23 23 22 22 7 7 124 124 114 114 112 112" },
	{ "coach 54 7 79 7 87 7 95 16 98 20 105 20 123 20 125 21" },
	{ "coastg 56 15 56 53" },
	{ "comet 73 45 12 12 2 2 6 6 4 4 46 46 53 53" },
	{ "copcarla 0 1" },
	{ "copcarsf 0 1" },
	{ "copcarvg 0 1" },
	{ "copcarru 0 1" },
	{ "cropdust 17 39 15 123 32 112 45 88 52 71 57 67 61 96 96 96" },
	{ "dft30 1 1" },
	{ "dinghy 56 15 56 53" },
	{ "dodo 51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8" },
	{ "dozer 1 1" },
	{ "dumper 1 1" },
	{ "duneride 91 38 115 43 85 6 79 7 78 8 77 18 79 18 86 24" },
	{ "elegant 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "elegy 36 1 35 1 17 1 11 1 116 1 113 1 101 1 92 1" },
	{ "emperor 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "enforcer 0 1" },
	{ "esperant 45 75 47 76 33 75 13 76 54 75 69 76 59 75 87 76" },
	{ "euros 36 1 40 1 43 1 53 1 72 1 75 1 95 1 101 1" },
	{ "faggio 12 12 13 13 14 14 1 2 2 1 1 3 3 1 10 10" },
	{ "fbiranch 0 0" },
	{ "fcr900 74 74 75 13 87 118 92 3 115 118 25 118 36 0 118 118" },
	{ "feltzer 73 1 74 1 75 1 77 1 79 1 83 1 84 1 91 1" },
	{ "firela 3 1" },
	{ "firetruk 3 1" },
	{ "flash 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "flatbed 84 15 84 58 84 31 32 74 43 31 1 31 77 31 32 74" },
	{ "forklift 110 1 111 1 112 1 114 1 119 1 122 1 4 1 13 1" },
	{ "fortune 2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29" },
	{ "freeway 79 79 84 84 7 7 11 11 19 19 22 22 36 36 53 53" },
	{ "freight 1 1" },
	{ "glendale 67 76 68 76 78 76 2 76 16 76 18 76 25 76 45 88" },
	{ "glenshit 67 76 68 76 78 76 2 76 16 76 18 76 25 76 45 88" },
	{ "greenwoo 30 26 77 26 81 27 24 55 28 56 49 59 52 69 71 107" },
	{ "hermes 97 1 81 1 105 1 110 1 91 1 74 1 84 1 83 1" },
	{ "hotdog 1 1" },
	{ "hotknife 1 1 12 12 2 2 6 6 4 4 46 46 53 53" },
	{ "hotrina 7 94 36 88 51 75 53 75 58 67 75 67 75 61 79 62" },
	{ "hotrinb 83 66 87 74 87 75 98 83 101 100 103 101 117 116 123 36" },
	{ "hotring 36 117 36 13 42 30 42 33 54 36 75 79 92 101 98 109" },
	{ "hunter 43 0" },
	{ "huntley 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "hustler 50 1 47 1 44 96 40 96 39 1 30 1 28 96 9 96" },
	{ "infernus 12 1 64 1 123 1 116 1 112 1 106 1 80 1 75 1" },
	{ "intruder 62 37 78 38 2 62 3 87 2 78 113 78 119 62 7 78" },
	{ "jester 51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8" },
	{ "jetmax 36 13" },
	{ "journey 1 1" },
	{ "kart 2 35 36 2 51 53 91 2 11 22 40 35" },
	{ "landstal 4 1 123 1 113 1 101 1 75 1 62 1 40 1 36 1" },
	{ "launch 112 20" },
	{ "linerun 36 1 37 1 30 1 28 1 25 1 40 1 101 1 113 1" },
	{ "quad 120 117 103 111 120 114 74 91 120 112 74 83 120 113 66 71" },
	{ "majestic 37 36 36 36 40 36 43 41 47 41 51 72 54 75 55 84" },
	{ "mtbike 43 43 46 46 39 39 28 28 16 16 6 6 5 5 2 2" },
	{ "manana 4 1 9 1 10 1 25 1 36 1 40 1 45 1 84 1" },
	{ "marquis 12 35 50 32 40 26 66 36" },
	{ "maverick 26 14 29 42 26 57 54 29 26 3 3 29 12 39 74 35" },
	{ "merit 67 1 72 1 75 1 83 1 91 1 101 1 109 1 20 1" },
	{ "mesa 75 84 40 84 40 110 28 119 25 119 21 119 13 119 4 119" },
	{ "monster 32 36 32 42 32 53 32 66 32 14 32 32" },
	{ "monstera 1 1" },
	{ "monsterb 1 1" },
	{ "moonbeam 119 119 117 227 114 114 108 108 95 95 81 81 61 61 41 41" },
	{ "mower 94 1 101 1 116 1 117 1 4 1 25 1 30 1 37 1" },
	{ "mrwhoop 1 16 1 56 1 17 1 53 1 5 1 35" },
	{ "mule 25 1 28 1 43 1 67 1 72 1 9 1 95 1 24 1" },
	{ "nebula 116 1 119 1 122 1 4 1 9 1 24 1 27 1 36 1" },
	{ "nevada 38 9 55 23 61 74 71 87 91 87 98 114 102 119 111 3" },
	{ "newsvan 41 10 41 20 49 11 56 123 110 113 112 116 114 118 119 101" },
	{ "nrg500 3 3 3 8 6 25 7 79 8 82 36 105 39 106 51 118" },
	{ "oceanic 51 1 58 8 60 1 68 8 2 1 13 8 22 1 36 8" },
	{ "packer 4 1 20 1 24 1 25 1 36 1 40 1 54 1 84 1" },
	{ "patriot 43 0" },
	{ "pcj600 36 1 37 1 43 1 53 1 61 1 75 1 79 1 88 1" },
	{ "peren 113 39 119 50 123 92 109 100 101 101 95 105 83 110 66 25" },
	{ "petro 10 1 25 1 28 1 36 1 40 1 54 1 75 1 113 1" },
	{ "petrotr 1 1" },
	{ "phoenix 58 1 69 1 75 77 18 1 32 1 45 45 13 1 34 1" },
	{ "picador 81 8 32 8 43 8 67 8 11 11 8 90 2 2 83 13" },
	{ "pizzaboy 3 6" },
	{ "polmav 0 1" },
	{ "pony 87 1 88 1 91 1 105 1 109 1 119 1 4 1 25 1" },
	{ "predator 46 26" },
	{ "premier 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "previon 83 1 87 1 92 1 95 1 109 1 119 45 11 1 " },
	{ "primo 122 1 123 1 125 1 10 1 24 1 37 1 55 1 66 1" },
	{ "raindanc 1 6" },
	{ "rancher 13 118 14 123 120 123 112 120 84 110 76 102" },
	{ "rcbandit 2 96 79 42 82 54 67 86 126 96 70 96 110 54 67 98" },
	{ "rcbaron 14 75" },
	{ "rcraider 14 75" },
	{ "rcgoblin 14 75" },
	{ "rdtrain 13 76 24 77 63 78 42 76 54 77 39 78 11 76 62 77" },
	{ "reefer 56 56" },
	{ "regina 27 36 59 36 60 35 55 41 54 31 49 23 45 32 40 29" },
	{ "remingtn 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "rhino 43 0" },
	{ "rnchlure 13 118 14 123 120 123 112 120 84 110 76 102" },
	{ "romero 0 0 11 105 25 109 36 0 40 36 75 36 0 36 0 109" },
	{ "rumpo 34 34 32 32 20 20 110 110 66 66 84 84 118 118 121 121" },
	{ "rustler 6 7 7 6 1 6 89 91 119 117 103 102 77 87 71 77" },
	{ "sabre 2 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29" },
	{ "sadler 76 8 32 8 43 8 67 8 11 11 8 90 2 2 83 13" },
	{ "sadlshit 61 8 32 8 43 8 67 8 11 11 8 90 2 2 83 13" },
	{ "sanchez 6 6 46 46 53 53 3 3" },
	{ "sandking 123 124 119 122 118 117 116 115 114 108 101 106 88 99 5 6" },
	{ "savanna 97 96 88 64 90 96 93 64 97 96 99 81 102 114 114 1" },
	{ "seaspar 75 2" },
	{ "securica 4 75" },
	{ "sentinel 11 1 24 1 36 1 40 1 75 1 91 1 123 1 4 1" },
	{ "shamal 1 1" },
	{ "streak 1 1" },
	{ "streakc 1 1" },
	{ "skimmer 1 3 1 9 1 18 1 30 17 23 46 23 46 32 57 34" },
	{ "slamvan 3 1 28 1 31 1 55 1 66 197 1 123 1 118 1 0" },
	{ "solair 91 1 101 1 109 1 113 1 4 1 25 1 30 1 36 1" },
	{ "sparrow 1 3" },
	{ "speeder 1 3 1 5 1 16 1 22 1 35 1 44 1 53 1 57" },
	{ "stafford 92 92 81 81 67 67 66 66 61 61 53 53 51 51 47 47 43 43" },
	{ "stallion 57 8 8 17 43 21 54 38 67 8 37 78 65 79 25 78" },
	{ "stratum 57 8 8 17 43 21 54 38 67 8 37 78 65 79 25 78" },
	{ "stretch 1 1" },
	{ "stunt 38 51 21 36 21 34 30 34 54 34 55 20 48 18 51 6" },
	{ "sultan 52 39 9 39 17 1 21 1 33 0 37 0 41 29 56 29" },
	{ "sunrise 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "supergt 3 3 6 6 7 7 52 52 76 76" },
	{ "swatvan 1 1" },
	{ "sweeper 26 26" },
	{ "tahoma 109 1 30 8 95 1 84 8 83 1 72 8 71 1 52 8" },
	{ "tampa 74 39 72 39 75 39 79 39 83 36 84 36 89 35 91 35" },
	{ "taxi 6 1" },
	{ "topfun 26 26 28 28 44 44 51 51 57 57 72 72 106 106 112 112" },
	{ "tornado 67 1 68 96 72 1 74 8 75 96 76 8 79 1 84 96" },
	{ "towtruck 1 1 17 20 18 20 22 30 36 43 44 51 52 54" },
	{ "tractor 2 35 36 2 51 53 91 2 11 22 40 35" },
	{ "tram 1 74" },
	{ "trash 26 26" },
	{ "tropic 26 26" },
	{ "tug 1 1" },
	{ "tugstair 1 1" },
	{ "turismo 123 123 125 125 36 36 16 16 18 18 46 46 61 61 75 75" },
	{ "uranus 112 1 116 1 117 1 24 1 30 1 35 1 36 1 40 1" },
	{ "utility 56 56 49 49 26 124" },
	{ "vcnmav 2 26 2 29" },
	{ "vincent 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "virgo 40 65 71 72 52 66 64 72 30 72 60 72" },
	{ "voodoo 9 1 10 8 11 1 25 8 27 1 29 8 30 1 37 8" },
	{ "vortex 96 67 86 70 79 74 70 86 61 98 75 75 75 91" },
	{ "walton 72 1 66 1 59 1 45 1 40 1 39 1 35 1 20 1" },
	{ "washing 4 1 13 1 25 1 30 1 36 1 40 1 75 1 95 1" },
	{ "wayfarer 119 1 122 1 8 1 10 1 13 1 25 1 27 1 32 1" },
	{ "willard 37 37 42 42 53 53 62 62 7 7 10 10 11 11 15 15" },
	{ "windsor 51 1 58 1 60 1 68 1 2 1 13 1 22 1 36 1" },
	{ "yankee 84 63 91 63 102 65 105 72 110 93 121 93 12 95 23 1" },
	{ "yosemite 53 32 15 32 45 32 34 30 65 32 14 32 12 32 43 32" },
	{ "zr350 92 1 94 1 101 1 121 1 0 1 22 1 36 1 75 1" }
};

// templated version of my_equal so it could work with both char and wchar_t
template<typename charT>
struct my_equal {
	my_equal(const std::locale& loc) : loc_(loc) {}
	bool operator()(charT ch1, charT ch2) {
		return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
	}
private:
	const std::locale& loc_;
};

// find substring (case insensitive)
template<typename T>
int ci_find_substr(const T& str1, const T& str2, const std::locale& loc = std::locale())
{
	T::const_iterator it = std::search(str1.begin(), str1.end(),
		str2.begin(), str2.end(), my_equal<T::value_type>(loc));
	if (it != str1.end()) return it - str1.begin();
	else return -1; // not found
}

std::string	NazwyPojazdowCARCOL[612];

void ProcessVehicleNamesCARCOL(const std::vector<std::string> &names, const std::vector<int>& modelids)
{
	for (size_t i = 0; i < names.size(); ++i)
	{
		int modelid = modelids[i];
		std::string name = names[i];
		boost::replace_all(name, " ", "");
		boost::replace_all(name, "-", "");
		NazwyPojazdowCARCOL[modelid] = name;
	}
}

void ProcessVehicleNamesCARCOL()
{
	static const std::vector<std::string> Airplanes = { "Andromada", "AT-400", "Beagle", "Cropduster", "Dodo", "Hydra", "Nevada", "Rustler", "Shamal", "Skimmer", "Stuntplane" };
	static const std::vector<std::string> Helicopters = { "Cargobob", "Hunter", "Leviathan", "Maverick", "vcnMaverick", "PolMav", "Raindance", "Seasparrow", "Sparrow" };
	static const std::vector<std::string> Bikes = { "BF-400", "Bike", "BMX", "Faggio", "FCR-900", "Freeway", "MtBike", "NRG-500", "PCJ-600", "Pizzaboy", "Quad", "Sanchez", "Wayfarer" };
	static const std::vector<std::string> Kabriolets = { "Comet", "Feltzer", "Stallion", "Windsor" };
	static const std::vector<std::string> Industrial = { "Benson", "Bobcat", "Burrito", "Boxville", "Boxburg", "Cement Truck", "DFT-30", "Flatbed", "Linerunner", "Mule", "Newsvan", "Packer", "Petro", "Pony", "Rdtrain", "Rumpo", "Sadler", "SadlShit", "Topfun", "Tractor", "Trashmaster", "Utility Van", "Walton", "Yankee", "Yosemite" };
	static const std::vector<std::string> Lowriders = { "Blade", "Broadway", "Remingtn", "Savanna", "Slamvan", "Tahoma", "Tornado", "Voodoo" };
	static const std::vector<std::string> OffRoad = { "Bandito", "BF Injection", "Duneride", "Huntley", "Landstalker", "Mesa", "Monster", "Monster A", "Monster B", "Patriot", "Rancher A", "rnchlure", "Sandking" };
	static const std::vector<std::string> PublicService = { "Ambulance", "Barracks", "Bus", "Cabbie", "Coach", "Cop Bike (HPV-1000)", "Enforcer", "fbiranch", "copcarru", "Firetruk", "Firela", "copcarla", "copcarvg", "copcarsf", "Ranger", "Rhino", "swatvan", "Taxi" };
	static const std::vector<std::string> Saloons = { "Admiral", "Bloodra", "Bravura", "Buccaneer", "Cadrona", "Clover", "Elegant", "Elegy", "Emperor", "Esperanto", "Fortune", "GlenShit", "Glendale", "Greenwood", "Hermes", "Intruder", "Majestic", "Manana", "Merit", "Nebula", "Oceanic", "Picador", "Premier", "Previon", "Primo", "Sentinel", "Stafford", "Sultan", "Sunrise", "Tampa", "Vincent", "Virgo", "Willard", "Washington" };
	static const std::vector<std::string> Sports = { "Alpha", "Banshee", "Blista Compact", "Buffalo", "Bullet", "Cheetah", "Club", "Euros", "Flash", "Hotring Racer", "HotrinA", "HotrinB", "Infernus", "Jester", "Phoenix", "Sabre", "Super GT", "Turismo", "Uranus", "ZR-350" };
	static const std::vector<std::string> Combi = { "Moonbeam", "Perenniel", "Regina", "Solair", "Stratum" };
	static const std::vector<std::string> Boats = { "Coastguard", "Dinghy", "Jetmax", "Launch", "Marquis", "Predator", "Reefer", "Speeder", "Squallo", "Tropic" };
	static const std::vector<std::string> Trailers = { "ArticT1", "ArticT2", "ArticT3", "Baggage Trailer A", "Baggage Trailer B", "Farm Trailer", "Freight Flat Trailer (Train)", "Freight Box Trailer (Train)", "Petrotr", "streakc", "tugstair", "Utility Trailer" };
	static const std::vector<std::string> Trains = { "Brownstreak (Train)", "Freight (Train)" };
	static const std::vector<std::string> TrainTrailers = { "Freight Flat Trailer (Train)", "Freight Box Trailer (Train)", "streakc" };
	static const std::vector<std::string> Unique = { "Baggage", "Brownstreak (Train)", "Caddy", "Camper A", "journey", "Combine Harvester", "Dozer", "Dumper", "Forklift", "Freight (Train)", "Hotknife", "Hustler", "Hotdog", "Kart", "Mower", "Mr Whoopee", "Romero", "Securicar", "Stretch", "Sweeper", "Tram", "Towtruck", "Tug", "Vortex" };
	static const std::vector<std::string> RemoteControl = { "RC Bandit", "RC Baron", "RC Raider", "RC Goblin", "RC Tiger", "RC Cam" };

	static const std::vector<int> modelid_Airplanes = { 592, 577, 511, 512, 593, 520, 553, 476, 519, 460, 513 };
	static const std::vector<int> modelid_Helicopters = { 548, 425, 417, 487, 488, 497, 563, 447, 469 };
	static const std::vector<int> modelid_Bikes = { 581, 509, 481, 462, 521, 463, 510, 522, 461, 448, 471, 468, 586 };
	static const std::vector<int> modelid_Kabriolets = { 480, 533, 439, 555 };
	static const std::vector<int> modelid_Industrial = { 499, 422, 482, 498, 609, 524, 578, 455, 403, 414, 582, 443, 514, 413, 515, 440, 543, 605, 459, 531, 408, 552, 478, 456, 554 };
	static const std::vector<int> modelid_Lowriders = { 536, 575, 534, 567, 535, 566, 576, 412 };
	static const std::vector<int> modelid_OffRoad = { 568, 424, 573, 579, 400, 500, 444, 556, 557, 470, 489, 505, 495 };
	static const std::vector<int> modelid_PublicService = { 416, 433, 431, 438, 437, 523, 427, 490, 528, 407, 544, 596, 598, 597, 599, 432, 601, 420 };
	static const std::vector<int> modelid_Saloons = { 445, 504, 401, 518, 527, 542, 507, 562, 585, 419, 526, 604, 466, 492, 474, 546, 517, 410, 551, 516, 467, 600, 426, 436, 547, 405, 580, 560, 550, 549, 540, 491, 529, 421 };
	static const std::vector<int> modelid_Sports = { 602, 429, 496, 402, 541, 415, 589, 587, 565, 494, 502, 503, 411, 559, 603, 475, 506, 451, 558, 477 };
	static const std::vector<int> modelid_Combi = { 418, 404, 479, 458, 561 };
	static const std::vector<int> modelid_Boats = { 472, 473, 493, 595, 484, 430, 453, 452, 446, 454 };
	static const std::vector<int> modelid_Trailers = { 435, 450, 591, 606, 607, 610, 569, 590, 584, 570, 608, 611 };
	static const std::vector<int> modelid_Unique = { 485, 537, 457, 483, 508, 532, 486, 406, 530, 538, 434, 545, 588, 571, 572, 423, 442, 428, 409, 574, 449, 525, 583, 539 };
	static const std::vector<int> modelid_Trains = { 537, 538 };
	static const std::vector<int> modelid_TrainTrailers = { 569, 590, 570 };
	static const std::vector<int> modelid_RemoteControl = { 441, 464, 465, 501, 564, 594 };
	static const std::vector<int> modelid_Armed = { 425, 430, 432, 447, 464, 476, 520 };

	ProcessVehicleNamesCARCOL(Airplanes, modelid_Airplanes);
	ProcessVehicleNamesCARCOL(Helicopters, modelid_Helicopters);
	ProcessVehicleNamesCARCOL(Bikes, modelid_Bikes);
	ProcessVehicleNamesCARCOL(Kabriolets, modelid_Kabriolets);
	ProcessVehicleNamesCARCOL(Industrial, modelid_Industrial);
	ProcessVehicleNamesCARCOL(Lowriders, modelid_Lowriders);
	ProcessVehicleNamesCARCOL(OffRoad, modelid_OffRoad);
	ProcessVehicleNamesCARCOL(PublicService, modelid_PublicService);
	ProcessVehicleNamesCARCOL(Saloons, modelid_Saloons);
	ProcessVehicleNamesCARCOL(Sports, modelid_Sports);
	ProcessVehicleNamesCARCOL(Combi, modelid_Combi);
	ProcessVehicleNamesCARCOL(Boats, modelid_Boats);
	ProcessVehicleNamesCARCOL(Trailers, modelid_Trailers);
	ProcessVehicleNamesCARCOL(Unique, modelid_Unique);
	ProcessVehicleNamesCARCOL(RemoteControl, modelid_RemoteControl);
}

int GetVehicleModel(const std::string& substr)
{
	int ModelID = 0xFFFF;

	for (int i = 400; i < 612; i++)
	{
		if (ci_find_substr(boost::replace_all_copy(NazwyPojazdowCARCOL[i], "-", ""), substr) != -1)
		{
			ModelID = i;
			break;
		}
	}
	return ModelID;
}

std::array<std::string, 612> strings_arr;
void VehiclesInitedProbably(int timerid, void * param)
{
	ProcessVehicleNamesCARCOL();
	ParseInput Input;
	Input.SetFormat("ws");
	bool hadSeaSparrow = false;
	std::set<int> FoundCarIDs;
	for (auto& i : carcolsdat_parsed)
	{
		Input.ExecuteManualParse(i);
		int model = GetVehicleModel(Input.Get<std::string>(0));
		if (model != 0xFFFF)
		{
			if (hadSeaSparrow && model == 447)
				model = 469;
			if (model == 447)
				hadSeaSparrow = true;
			FoundCarIDs.insert(model);
			strings_arr[model - 400] = "\t{ GetColorsFromSpacedLine(\"" + Input.Get<std::string>(1) + "\") },";
			strings_arr[model] = strings_arr[model - 400];
		}
	}
	for (int i = 400; i < 612; ++i)
	{
		if (FoundCarIDs.find(i) == FoundCarIDs.end())
		{
			std::cout << "Missing id " << i << std::endl;
		}
	}
	for (auto& i : strings_arr)
	{
		if (!i.size())
			std::cout << "\t{ GetColorsFromSpacedLine(\"-1 -1\") }," << std::endl;
		else
			std::cout << i << std::endl;
	}
}

class Converter : public Extension::Base
{
public:
	bool OnGameModeInit()
	{
		sampgdk_SetTimerEx(2000, false, VehiclesInitedProbably, nullptr, nullptr);
		return true;
	}
} _Converter;
#endif