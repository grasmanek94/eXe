#include "Random.hxx"

namespace GTAMath 
{
	boost::random::mt19937 rng;

	unsigned long long Random(unsigned long long inclusive_min, unsigned long long exclusive_max)
	{
		unsigned long long diff = exclusive_max - inclusive_min;

		//fix division by 0
		if (diff == 0)
		{
			return inclusive_min;
		}

		return (((unsigned long long)(rng() * 100000000.0)) % diff) + inclusive_min;
	}
}