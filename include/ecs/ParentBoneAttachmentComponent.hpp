#ifndef PARENTBONEATTACHMENTCOMPONENT_H_
#define PARENTBONEATTACHMENTCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Entity.hpp"

#include "serialization/glm/IVec4.hpp"
#include "serialization/glm/Vec4.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct ParentBoneAttachmentComponent
{
	ParentBoneAttachmentComponent() = default;

	ParentBoneAttachmentComponent(std::string boneName) : boneName(boneName)
	{
	};

	ParentBoneAttachmentComponent(
		std::string boneName,
		glm::ivec4 boneIds,
		glm::vec4 boneWeights
	)
	:
		boneName(boneName),
		boneIds(boneIds),
		boneWeights(boneWeights)
	{
	};
	
	static uint8 id()  { return 15; }

	std::string boneName;
	glm::ivec4 boneIds;
	glm::vec4 boneWeights;
//	float32 runningTime = 0.0f;
//	std::vector<glm::mat4> transformations;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::ParentBoneAttachmentComponent& c, const unsigned int version)
{
	ar & c.boneName & c.boneIds & c.boneWeights;
}

}
}

#endif /* PARENTBONEATTACHMENTCOMPONENT_H_ */
