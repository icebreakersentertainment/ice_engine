#ifndef IGRAPHICSENGINE_H_
#define IGRAPHICSENGINE_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "IEventListener.hpp"
#include "TransformSpace.hpp"
#include "RenderSceneHandle.hpp"
#include "RenderableHandle.hpp"
#include "TerrainRenderableHandle.hpp"
#include "TerrainHandle.hpp"
#include "MeshHandle.hpp"
#include "MaterialHandle.hpp"
#include "TextureHandle.hpp"
#include "SkeletonHandle.hpp"
#include "BonesHandle.hpp"
#include "PointLightHandle.hpp"
#include "CameraHandle.hpp"
#include "VertexShaderHandle.hpp"
#include "FragmentShaderHandle.hpp"
#include "TessellationControlShaderHandle.hpp"
#include "TessellationEvaluationShaderHandle.hpp"
#include "ShaderProgramHandle.hpp"
#include "IHeightMap.hpp"
#include "ISplatMap.hpp"
#include "IDisplacementMap.hpp"
#include "IMesh.hpp"
#include "ITexture.hpp"
#include "ISkeleton.hpp"
#include "IImage.hpp"

//#include "model/Model.hpp"

namespace ice_engine
{
namespace graphics
{

struct DisplacementMap
{
	DisplacementMap() = default;
	DisplacementMap(IImage* displacementMap) : displacementMap(displacementMap) {}
	
	IImage* displacementMap = nullptr;
};

class IGraphicsEngine
{
public:
	virtual ~IGraphicsEngine()
	{
	}
	;
	
	virtual void setViewport(const uint32 width, const uint32 height) = 0;
	virtual glm::uvec2 getViewport() const = 0;
	
	virtual glm::mat4 getModelMatrix() const = 0;
	virtual glm::mat4 getViewMatrix() const = 0;
	virtual glm::mat4 getProjectionMatrix() const = 0;
	
	virtual void beginRender() = 0;
	virtual void render(const RenderSceneHandle& renderSceneHandle) = 0;
	virtual void renderLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) = 0;
	virtual void renderLines(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& lineData) = 0;
	virtual void endRender() = 0;
	
	virtual RenderSceneHandle createRenderScene() = 0;
	virtual void destroyRenderScene(const RenderSceneHandle& renderSceneHandle) = 0;
	
	virtual CameraHandle createCamera(const glm::vec3& position, const glm::vec3& lookAt = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;
	
	virtual PointLightHandle createPointLight(const RenderSceneHandle& renderSceneHandle, const glm::vec3& position) = 0;
	virtual void destroy(const RenderSceneHandle& renderSceneHandle, const PointLightHandle& pointLightHandle) = 0;
	
	virtual MeshHandle createStaticMesh(const IMesh* mesh) = 0;
	virtual MeshHandle createDynamicMesh(const IMesh* mesh) = 0;
	
	virtual SkeletonHandle createSkeleton(const MeshHandle& meshHandle, const ISkeleton* skelton) = 0;
	virtual void destroy(const SkeletonHandle& skeletonHandle) = 0;

	virtual BonesHandle createBones(const uint32 maxNumberOfBones) = 0;
	virtual void destroy(const BonesHandle& bonesHandle) = 0;

	virtual void attach(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const BonesHandle& bonesHandle) = 0;
	virtual void detach(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const BonesHandle& bonesHandle) = 0;

	virtual void attachBoneAttachment(
		const RenderSceneHandle& renderSceneHandle,
		const RenderableHandle& renderableHandle,
		const BonesHandle& bonesHandle,
		const glm::ivec4& boneIds,
		const glm::vec4& boneWeights
	) = 0;
	virtual void detachBoneAttachment(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle) = 0;
	
	virtual TextureHandle createTexture2d(const ITexture* texture) = 0;
	
	virtual MaterialHandle createMaterial(const IPbrMaterial* pbrMaterial) = 0;
	
	virtual TerrainHandle createStaticTerrain(
			const IHeightMap* heightMap,
			const ISplatMap* splatMap,
			const IDisplacementMap* displacementMap
		) = 0;
		virtual void destroy(const TerrainHandle& terrainHandle) = 0;

	virtual VertexShaderHandle createVertexShader(const std::string& data) = 0;
	virtual FragmentShaderHandle createFragmentShader(const std::string& data) = 0;
	virtual TessellationControlShaderHandle createTessellationControlShader(const std::string& data) = 0;
	virtual TessellationEvaluationShaderHandle createTessellationEvaluationShader(const std::string& data) = 0;
	virtual bool valid(const VertexShaderHandle& shaderHandle) const = 0;
	virtual bool valid(const FragmentShaderHandle& shaderHandle) const = 0;
	virtual bool valid(const TessellationControlShaderHandle& shaderHandle) const = 0;
	virtual bool valid(const TessellationEvaluationShaderHandle& shaderHandle) const = 0;
	virtual void destroyShader(const VertexShaderHandle& shaderHandle) = 0;
	virtual void destroyShader(const FragmentShaderHandle& shaderHandle) = 0;
	virtual void destroyShader(const TessellationControlShaderHandle& shaderHandle) = 0;
	virtual void destroyShader(const TessellationEvaluationShaderHandle& shaderHandle) = 0;
	virtual ShaderProgramHandle createShaderProgram(const VertexShaderHandle& vertexShaderHandle, const FragmentShaderHandle& fragmentShaderHandle) = 0;
	virtual ShaderProgramHandle createShaderProgram(
		const VertexShaderHandle& vertexShaderHandle,
		const TessellationControlShaderHandle& tessellationControlShaderHandle,
		const TessellationEvaluationShaderHandle& tessellationEvaluationShaderHandle,
		const FragmentShaderHandle& fragmentShaderHandle
	) = 0;
	virtual bool valid(const ShaderProgramHandle& shaderProgramHandle) const = 0;
	virtual void destroyShaderProgram(const ShaderProgramHandle& shaderProgramHandle) = 0;
	
	virtual RenderableHandle createRenderable(
		const RenderSceneHandle& renderSceneHandle,
		const MeshHandle& meshHandle,
		const TextureHandle& textureHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const glm::vec3& scale = glm::vec3(1.0f),
		const ShaderProgramHandle& shaderProgramHandle = ShaderProgramHandle()
	) = 0;
	virtual RenderableHandle createRenderable(
		const RenderSceneHandle& renderSceneHandle,
		const MeshHandle& meshHandle,
		const MaterialHandle& materialHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const glm::vec3& scale = glm::vec3(1.0f)
	) = 0;
	virtual void destroy(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle) = 0;
	
	virtual TerrainRenderableHandle createTerrainRenderable(
		const RenderSceneHandle& renderSceneHandle,
		const TerrainHandle& terrainHandle
	) = 0;
	virtual void destroy(const RenderSceneHandle& renderSceneHandle, const TerrainRenderableHandle& terrainRenderableHandle) = 0;
	
	virtual void rotate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::quat& quaternion, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
	virtual void rotate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
	virtual void rotate(const CameraHandle& cameraHandle, const glm::quat& quaternion, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
	virtual void rotate(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
	
	virtual void rotation(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::quat& quaternion) = 0;
	virtual void rotation(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis) = 0;
	virtual glm::quat rotation(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle) const = 0;
	virtual void rotation(const CameraHandle& cameraHandle, const glm::quat& quaternion) = 0;
	virtual void rotation(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis) = 0;
	virtual glm::quat rotation(const CameraHandle& cameraHandle) const = 0;
	
	virtual void translate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void translate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& trans) = 0;
	virtual void translate(const RenderSceneHandle& renderSceneHandle, const PointLightHandle& pointLightHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void translate(const RenderSceneHandle& renderSceneHandle, const PointLightHandle& pointLightHandle, const glm::vec3& trans) = 0;
	virtual void translate(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void translate(const CameraHandle& cameraHandle, const glm::vec3& trans) = 0;
	
	virtual void scale(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void scale(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& scale) = 0;
	virtual void scale(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 scale) = 0;
	virtual glm::vec3 scale(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle) const = 0;
	
	virtual void position(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void position(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& position) = 0;
	virtual glm::vec3 position(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle) const = 0;
	virtual void position(const RenderSceneHandle& renderSceneHandle, const PointLightHandle& pointLightHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void position(const RenderSceneHandle& renderSceneHandle, const PointLightHandle& pointLightHandle, const glm::vec3& position) = 0;
	virtual glm::vec3 position(const RenderSceneHandle& renderSceneHandle, const PointLightHandle& pointLightHandle) const = 0;
	virtual void position(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void position(const CameraHandle& cameraHandle, const glm::vec3& position) = 0;
	virtual glm::vec3 position(const CameraHandle& cameraHandle) const = 0;
	
	virtual void lookAt(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& lookAt) = 0;
	virtual void lookAt(const CameraHandle& cameraHandle, const glm::vec3& lookAt) = 0;
	
	virtual void assign(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const SkeletonHandle& skeletonHandle) = 0;
	
	virtual void update(
		const RenderSceneHandle& renderSceneHandle,
		const RenderableHandle& renderableHandle,
		const BonesHandle& bonesHandle,
		const std::vector<glm::mat4>& transformations
	) = 0;
	
	virtual void setMouseRelativeMode(const bool enabled) = 0;
	virtual void setWindowGrab(const bool enabled) = 0;
	virtual void setCursorVisible(const bool visible) = 0;
	
	virtual void processEvents() = 0;
	virtual void addEventListener(IEventListener* eventListener) = 0;
	virtual void removeEventListener(IEventListener* eventListener) = 0;
};

}

}

#endif /* IGRAPHICSENGINE_H_ */
