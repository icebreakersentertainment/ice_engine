#ifndef GAMENOISE_H_
#define GAMENOISE_H_

#include <memory>

#include "Types.hpp"

//#include "terrain/IFieldFunction.hpp"

namespace hercules
{
namespace noise
{

class GameNoise// : public glr::terrain::IFieldFunction
{
public:
	GameNoise(float32 amplitude = 1.0f, float32 octaves = 1.0f, float32 frequency = 1.0f, float32 zoom = 1.0f);
	//GameNoise(const GameNoise& other);
	virtual ~GameNoise();

	float32 getNoise(float32 x);
	float32 getNoise(float32 x, float32 y);
	virtual float32 getNoise(float32 x, float32 y, float32 z);

private:
	float32 amplitude_;
	float32 octaves_;
	float32 frequency_;
	float32 zoom_;
	
	static float32 maxX, maxY, maxZ;
	
	float32 getSmoothNoise(float32 x, float32 y, float32 zoom, int octaves, float32 lacunarity, float32 gain);
	float32 getSmoothNoise(float32 x, float32 y, float32 z, float32 zoom = 1.0f, int octaves = 8, float32 lacunarity = 2.0, float32 gain = 0.5);
};

}

}

#endif /* GAMENOISE_H_ */
