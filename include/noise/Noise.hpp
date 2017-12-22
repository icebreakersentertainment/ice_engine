#ifndef NOISE_H_
#define NOISE_H_

#include "FastNoise.h"
#include "Types.hpp"

namespace ice_engine
{
namespace noise
{

class Noise
{
public:
	Noise() = default;

	Noise(const uint32 seed) : noise_(seed)
	{
	}

	void setSeed(const uint32 seed)
	{
		noise_.SetSeed(seed);
	}

	uint32 getSeed() const
	{
		return noise_.GetSeed();
	}
	
	float32 getValue(const float32 x, const float32 y) const
	{
		return noise_.GetValue(x, y);
	}

private:
	FastNoise noise_;
};

}
}

#endif /* NOISE_H_ */

