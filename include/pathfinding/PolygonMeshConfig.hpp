#ifndef POLYGONMESHCONFIG_H_
#define POLYGONMESHCONFIG_H_

#include "detail/DebugSerializer.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace pathfinding
{

struct PolygonMeshConfig
{
	int width = 0;
	int height = 0;
//	int tileSize = 0;
	int tileSize = 48;
	int borderSize = 0;

	float cellSize = 1.0f;
	float cellHeight = 1.0f;

	float walkableSlopeAngle = 0.0f;
	int walkableHeight = 0;
	int walkableClimb = 0;
	int walkableRadius = 0;
	int maxEdgeLength = 0;

	float maxSimplificationError = 0.0f;

	int minRegionArea = 0;
	int mergeRegionArea = 0;
	int maxVertsPerPoly = 3;

	float detailSampleDist = 0.0f;

	float detailSampleMaxError = 0.0f;

	int maxObstacles = 128;
};

inline std::ostream& operator<<(std::ostream& os, const PolygonMeshConfig& data)
{
    os << "PolygonMeshConfig("
    PRINT_TO_STREAM(data, width)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, height)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, tileSize)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, borderSize)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, cellSize)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, cellHeight)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, walkableSlopeAngle)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, walkableHeight)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, walkableClimb)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, walkableRadius)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, maxEdgeLength)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, maxSimplificationError)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, minRegionArea)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, mergeRegionArea)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, maxVertsPerPoly)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, detailSampleDist)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, detailSampleMaxError)
    PRINT_DELIMITER() PRINT_TO_STREAM(data, maxObstacles)
            << ")";

    return os;
}

}
}

#endif /* POLYGONMESHCONFIG_H_ */
