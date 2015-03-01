#pragma once

#include <limits>
#include <string>
#include <functional>
#include <queue>
#include <vector>
#include <bitset>
#include <set>
#include <cmath>
#include <map>
#include <array>
#include <sstream>

#include "Random.hxx"

#include "../../boost_extract/boost/lexical_cast.hpp"

namespace GTACarColorGenerator
{
	const int SAMP_VEHICLE_PADDING = 400;

	const std::array<int, 2> RandomColor();
	const std::array<int, 2> GenerateColorByCarModel(int modelid);
}