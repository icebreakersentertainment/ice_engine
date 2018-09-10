#ifndef POLYGONMESHCONFIG_H_
#define POLYGONMESHCONFIG_H_

#include "Types.hpp"

namespace ice_engine
{
namespace pathfinding
{

struct PolygonMeshConfig
{
	int width = 0;
	int height = 0;
	int tileSize = 0;
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
};

}
}

#endif /* POLYGONMESHCONFIG_H_ */
