#ifndef IGRAPHICSENGINE_H_
#define IGRAPHICSENGINE_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "IEventListener.hpp"
#include "RenderableId.hpp"
#include "MeshId.hpp"
#include "TextureId.hpp"
#include "SkeletonId.hpp"
#include "CameraId.hpp"

namespace hercules
{
namespace graphics
{

enum TransformSpace
{
	TS_LOCAL = 0,
	TS_WORLD
};



class IGraphicsEngine
{
public:
	virtual ~IGraphicsEngine()
	{
	}
	;
	
	virtual void setViewport(uint32 width, uint32 height) = 0;
	virtual void render(float32 delta) = 0;
	
	virtual CameraId createCamera(glm::vec3 position, glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;
	
	virtual MeshId createStaticMesh(
		std::vector<glm::vec3> vertices,
		std::vector<uint32> indices,
		std::vector<glm::vec4> colors,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> textureCoordinates
	) = 0;
	virtual MeshId createAnimatedMesh(
		std::vector<glm::vec3> vertices,
		std::vector<uint32> indices,
		std::vector<glm::vec4> colors,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> textureCoordinates,
		std::vector<glm::ivec4> boneIds,
		std::vector<glm::vec4> boneWeights
	) = 0;
	virtual MeshId createDynamicMesh(
		std::vector<glm::vec3> vertices,
		std::vector<uint32> indices,
		std::vector<glm::vec4> colors,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> textureCoordinates
	) = 0;
	
	virtual SkeletonId createSkeleton(uint32 numberOfBones) = 0;
	
	virtual TextureId createTexture2d(std::string uri) = 0;
	
	virtual RenderableId createRenderable(MeshId meshId, TextureId textureId) = 0;
	
	virtual void rotate(const CameraId cameraId, const glm::quat& quaternion, TransformSpace relativeTo = TransformSpace::TS_LOCAL) = 0;
	virtual void rotate(const RenderableId renderableId, const glm::quat& quaternion, TransformSpace relativeTo = TransformSpace::TS_LOCAL) = 0;
	virtual void rotate(const CameraId cameraId, const float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TransformSpace::TS_LOCAL) = 0;
	virtual void rotate(const RenderableId renderableId, const float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TransformSpace::TS_LOCAL) = 0;
	
	virtual void translate(const CameraId cameraId, const float32 x, const float32 y, const float32 z) = 0;
	virtual void translate(const RenderableId renderableId, const float32 x, const float32 y, const float32 z) = 0;
	virtual void translate(const CameraId cameraId, const glm::vec3& trans) = 0;
	virtual void translate(const RenderableId renderableId, const glm::vec3& trans) = 0;
	
	virtual void scale(const RenderableId renderableId, const float32 x, const float32 y, const float32 z) = 0;
	virtual void scale(const RenderableId renderableId, const glm::vec3& scale) = 0;
	virtual void scale(const RenderableId renderableId, const float32 scale) = 0;
	
	virtual void position(const RenderableId renderableId, const float32 x, const float32 y, const float32 z) = 0;
	virtual void position(const CameraId cameraId, const float32 x, const float32 y, const float32 z) = 0;
	virtual void position(const RenderableId renderableId, const glm::vec3& position) = 0;
	virtual void position(const CameraId cameraId, const glm::vec3& position) = 0;
	
	virtual void lookAt(const RenderableId renderableId, const glm::vec3& lookAt) = 0;
	virtual void lookAt(const CameraId cameraId, const glm::vec3& lookAt) = 0;
	
	virtual void assign(const RenderableId renderableId, const SkeletonId skeletonId) = 0;
	
	virtual void update(const SkeletonId skeletonId, const void* data, uint32 size) = 0;
	
	virtual void setMouseRelativeMode(bool enabled) = 0;
	virtual void setCursorVisible(bool visible) = 0;
	
	virtual void processEvents() = 0;
	virtual void addEventListener(IEventListener* eventListener) = 0;
	virtual void removeEventListener(IEventListener* eventListener) = 0;
};

}

}

#endif /* IGRAPHICSENGINE_H_ */
