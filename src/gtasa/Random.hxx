#pragma once

#include "../../boost_extract/boost/random.hpp"
#include "../../boost_extract/boost/variant.hpp"

namespace GTAMath
{
	unsigned long long Random(unsigned long long inclusive_min = 0, unsigned long long exclusive_max = (0x7FFFFFFFFFFFFFFF - 1));
}