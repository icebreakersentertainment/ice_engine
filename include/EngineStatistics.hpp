#ifndef ENGINESTATISTICS_H_
#define ENGINESTATISTICS_H_

#include <chrono>

#include "Types.hpp"

namespace ice_engine
{

struct EngineStatistics
{
	float32 fps;
    std::chrono::duration<float32> renderTime;
};

}

#endif /* ENGINESTATISTICS_H_ */
