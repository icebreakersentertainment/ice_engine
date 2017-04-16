class RenderSceneHandle
{
};

virtual void setViewport(const uint32 width, const uint32 height) = 0;
virtual void render(const float32 delta, const RenderSceneHandle& renderSceneHandle) = 0;



/* CAMERA STUFF */
virtual CameraHandle createCamera(const glm::vec3& position, const glm::vec3& lookAt = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;



/* MESH STUFF */
virtual MeshHandle createStaticMesh(
	const std::vector<glm::vec3>& vertices,
	const std::vector<uint32>& indices,
	const std::vector<glm::vec4>& colors,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& textureCoordinates
) = 0;
virtual MeshHandle createAnimatedMesh(
	const std::vector<glm::vec3>& vertices,
	const std::vector<uint32>& indices,
	const std::vector<glm::vec4>& colors,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& textureCoordinates,
	const std::vector<glm::ivec4>& boneIds,
	const std::vector<glm::vec4>& boneWeights
) = 0;
virtual MeshHandle createDynamicMesh(
	const std::vector<glm::vec3>& vertices,
	const std::vector<uint32>& indices,
	const std::vector<glm::vec4>& colors,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& textureCoordinates
) = 0;



/* SKELETON STUFF */
virtual SkeletonHandle createSkeleton(const uint32 numberOfBones) = 0;



/* TEXTURE STUFF */
virtual TextureHandle createTexture2d(const utilities::Image& image) = 0;



/* RENDER SCENE STUFF */
virtual RenderSceneHandle createRenderScene() = 0;
virtual void destroyRenderScene(const RenderSceneHandle& renderSceneHandle) = 0;



/* RENDERABLE STUFF */
virtual RenderableHandle createRenderable(const RenderSceneHandle& renderSceneHandle, const MeshHandle& meshHandle, const TextureHandle& textureHandle) = 0;
virtual RenderableHandle createRenderable(const MeshHandle& meshHandle, const TextureHandle& textureHandle) = 0;



/* MANIPULATE RENDERABLES */
virtual void rotate(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const glm::quat& quaternion, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::quat& quaternion, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const CameraHandle& cameraHandle, const glm::quat& quaternion, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const RenderableHandle& renderableHandle, const glm::quat& quaternion, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo = TransformSpace::TS_LOCAL) = 0;

virtual void rotation(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const glm::quat& quaternion) = 0;
virtual void rotation(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::quat& quaternion) = 0;
virtual void rotation(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis) = 0;
virtual void rotation(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis) = 0;
virtual void rotation(const CameraHandle& cameraHandle, const glm::quat& quaternion) = 0;
virtual void rotation(const RenderableHandle& renderableHandle, const glm::quat& quaternion) = 0;
virtual void rotation(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis) = 0;
virtual void rotation(const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis) = 0;

virtual void translate(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void translate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void translate(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const glm::vec3& trans) = 0;
virtual void translate(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& trans) = 0;
virtual void translate(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void translate(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void translate(const CameraHandle& cameraHandle, const glm::vec3& trans) = 0;
virtual void translate(const RenderableHandle& renderableHandle, const glm::vec3& trans) = 0;

virtual void scale(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void scale(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& scale) = 0;
virtual void scale(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 scale) = 0;
virtual void scale(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void scale(const RenderableHandle& renderableHandle, const glm::vec3& scale) = 0;
virtual void scale(const RenderableHandle& renderableHandle, const float32 scale) = 0;

virtual void position(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void position(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void position(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& position) = 0;
virtual void position(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const glm::vec3& position) = 0;
virtual void position(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void position(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void position(const RenderableHandle& renderableHandle, const glm::vec3& position) = 0;
virtual void position(const CameraHandle& cameraHandle, const glm::vec3& position) = 0;

virtual void lookAt(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const glm::vec3& lookAt) = 0;
virtual void lookAt(const RenderSceneHandle& renderSceneHandle, const CameraHandle& cameraHandle, const glm::vec3& lookAt) = 0;
virtual void lookAt(const RenderableHandle& renderableHandle, const glm::vec3& lookAt) = 0;
virtual void lookAt(const CameraHandle& cameraHandle, const glm::vec3& lookAt) = 0;



/* ASSIGNMENT STUFF */
virtual void assign(const RenderSceneHandle& renderSceneHandle, const RenderableHandle& renderableHandle, const SkeletonHandle& skeletonHandle) = 0;
virtual void assign(const RenderableHandle& renderableHandle, const SkeletonHandle& skeletonHandle) = 0;



/* UPDATE DATA */
virtual void update(const SkeletonHandle& skeletonHandle, const void* data, const uint32 size) = 0;



/* MOUSE STUFF */
virtual void setMouseRelativeMode(const bool enabled) = 0;
virtual void setCursorVisible(const bool visible) = 0;



/* MISC STUFF */
virtual void processEvents() = 0;
virtual void addEventListener(IEventListener* eventListener) = 0;
virtual void removeEventListener(IEventListener* eventListener) = 0;
