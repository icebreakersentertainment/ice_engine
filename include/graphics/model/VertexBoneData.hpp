#ifndef VERTEXBONEDATA_H_
#define VERTEXBONEDATA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{

struct VertexBoneData
{
	glm::ivec4 boneIds;
	glm::vec4 weights;
	
	VertexBoneData()
	{
		boneIds = glm::ivec4(0);
		weights = glm::vec4(0.0f);
	}
	
	void addBoneWeight( uint32 id, float32 weight )
	{
		for (uint32 i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				boneIds[i] = id;
				weights[i] = weight;
				
				assert( weights[0] + weights[1] + weights[2] + weights[3] < 1.05f );
				
				return;
			} 
		}
		
		// If we get here, we have too many bones per vertex
		assert(0);
	}
};

}
}
}

#endif /* VERTEXBONEDATA_H_ */
