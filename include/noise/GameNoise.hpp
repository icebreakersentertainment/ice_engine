#ifndef GAMENOISE_H_
#define GAMENOISE_H_

#include <memory>

//#include "terrain/IFieldFunction.hpp"

namespace gamenoise
{

class GameNoise// : public glr::terrain::IFieldFunction
{
public:
	GameNoise(float amplitude = 1.0f, float octaves = 1.0f, float frequency = 1.0f, float zoom = 1.0f);
	//GameNoise(const GameNoise& other);
	virtual ~GameNoise();

	float getNoise(float x);
	float getNoise(float x, float y);
	virtual glm::detail::float32 getNoise(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);

private:
	float amplitude_;
	float octaves_;
	float frequency_;
	float zoom_;
	
	static float maxX, maxY, maxZ;
	
	float getSmoothNoise(float x, float y, float zoom, int octaves, float lacunarity, float gain);
	float getSmoothNoise(float x, float y, float z, float zoom = 1.0f, int octaves = 8, float lacunarity = 2.0, float gain = 0.5);
};

}

#endif /* GAMENOISE_H_ */
