#include <iostream>
#include <algorithm>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "noise/GameNoise.hpp"

#include "simplexnoise/simplexnoise1234.h"


namespace herculesnoise
{

float GameNoise::maxX = 256.0f;
float GameNoise::maxY = 256.0f;
float GameNoise::maxZ = 256.0f;

GameNoise::GameNoise(float amplitude, float octaves, float frequency, float zoom)
	: amplitude_(amplitude), octaves_(octaves), frequency_(frequency), zoom_(zoom)
{
}

/*
GameNoise::GameNoise(const GameNoise& other)
{
	
}
*/

GameNoise::~GameNoise()
{
}

float GameNoise::getNoise(float x)
{
	//return SimplexNoise1234::noise(x);
	return 0.0f;
}

float GameNoise::getNoise(float x, float y)
{
	return getSmoothNoise(x, y, 10.0f, 16, 6.0f, 0.05f);
}
	
float GameNoise::getSmoothNoise(float x, float y, float zoom, int octaves, float lacunarity, float gain)
{
	float amplitude = 1.0;
	float frequency = 1.0;
	float sum = 0.0;
	
	//std::cout << x << ", " << y << ", " << z << std::endl;
	
	x = x / zoom;
	y = y / zoom;
	
	
	for ( int i = 0; i < octaves; ++i )
	{
		sum += amplitude * SimplexNoise1234::noise(x * frequency, y * frequency);
		amplitude *= gain;
		frequency *= lacunarity;
	}
	
	return sum;
}

glm::detail::float32 GameNoise::getNoise(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	assert(x < maxX && y < maxY && z < maxZ);
	
	// Sphere
	/*
	x -= 4.0f;
	y -= 4.0f;
	z -= 4.0f;
	float r = 2.0f;
	float result = x*x + y*y + z*z - r*r;
	
	if (result < 0.0f)
		return result;
	
	if (y > -1.0f)
		return 1.0f;
	
	return -1.0f;
	*/
	
	//return SimplexNoise1234::noise(x, y, z);
	
	// Plane
	/*
	glm::vec3 n = glm::vec3(1.0f, 1.0f, 0.0f);
	n = glm::normalize(n);
	float r = n.x*x + n.y*y + n.z*z;
	
	return r;
	*/
	
	// Noise
	
	// Why is (0.0f, 0.0f, 0.0f) out of bounds?
	//if (x==0.0f && y==0.0f && z==0.0f)
	//	return 0.0f;

	//float r = groundCaveMult->get(x, y, z);
	//std::cout << x << ", " << y << ", " << z << ": " << r << std::endl;
	
	//r = r - 6.0f;
	
	//return r;
	
	return getSmoothNoise(x, y, z, 10.0f, 16, 6.0f, 0.05f);
}

float GameNoise::getSmoothNoise(float x, float y, float z, float zoom, int octaves, float lacunarity, float gain)
{
	/*
	const float yMin = getSmoothNoise(x, z, 135.0f, 16, 6.0f, 0.05f) * 10.0f;
	
	const float diff = yMin - y;
	const float modVal = diff / 5.0f;
	const float retVal = -1.0f * modVal;
	return std::max(-1.0f, retVal);
	*/
	
	float amplitude = 1.0;
	float frequency = 1.0;
	float sum = 0.0;
	
	// Put a lower bound on y values
	if (y < 0.3f)
	{
		const float yMin = getSmoothNoise(x, z, 135.0f, 16, 6.0f, 0.05f) * 10.0f;
		
		if (y < yMin)
		{
			const float diff = yMin - y;
			const float modVal = diff / 5.0f;
			const float retVal = -1.0f * modVal;
			return std::max(-1.0f, retVal);
		}
	}
	
	// Put an upper bound on y values
	const float yMax = getSmoothNoise(x, z, 150.0f, 16, 6.0f, 0.05f) * 50.0f;
	//std::cout << y << " " << yMax << std::endl;
	if (y > yMax)
		return 1.0f;
	
	
	//std::cout << x << ", " << y << ", " << z << std::endl;
	
	x = x / zoom;
	y = y / zoom;
	z = z / zoom;
	
	
	for ( int i = 0; i < octaves; ++i )
	{
		sum += amplitude * SimplexNoise1234::noise(x * frequency, y * frequency, z * frequency);
		amplitude *= gain;
		frequency *= lacunarity;
	}
	
	return sum;
}

}
