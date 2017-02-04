#ifndef GRAPHICSENGINE_H_
#define GRAPHICSENGINE_H_

#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "graphics/IGraphicsEngine.hpp"
#include "graphics/Event.hpp"

#include "fs/IFileSystem.hpp"

namespace graphics
{

struct Vbo
{
	GLuint id;
};

struct Ebo
{
	GLuint id;
	GLenum mode;
  	GLsizei count;
	GLenum type;
};

struct Ubo
{
	GLuint id;
};

struct Vao
{
	GLuint id;
	Vbo vbo[4];
	Ebo ebo;
};

struct GlTexture2d
{
	GLuint id;
};

struct Renderable
{
	Vao vao;
	Ubo ubo;
	GlTexture2d texture;
};


struct GraphicsData
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
};

struct Camera
{
	glm::vec3 position;
	glm::quat orientation;
};

class GraphicsEngine : public IGraphicsEngine
{
public:
	GraphicsEngine(glm::detail::uint32 width, glm::detail::uint32 height, fs::IFileSystem* fileSystem);
	virtual ~GraphicsEngine();
	
	virtual void setViewport(glm::detail::uint32 width, glm::detail::uint32 height);
	virtual void render(glm::detail::float32 delta);
	
	virtual CameraId createCamera(glm::vec3 position, glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f));
	
	virtual MeshId createStaticMesh(
		std::vector<glm::vec3> vertices,
		std::vector<glm::detail::uint32> indices,
		std::vector<glm::vec4> colors,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> textureCoordinates
	);
	virtual MeshId createAnimatedMesh(
		std::vector<glm::vec3> vertices,
		std::vector<glm::detail::uint32> indices,
		std::vector<glm::vec4> colors,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> textureCoordinates,
		std::vector<glm::ivec4> boneIds,
		std::vector<glm::vec4> boneWeights
	);
	virtual MeshId createDynamicMesh(
		std::vector<glm::vec3> vertices,
		std::vector<glm::detail::uint32> indices,
		std::vector<glm::vec4> colors,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> textureCoordinates
	);
	
	virtual SkeletonId createSkeleton(glm::detail::uint32 numberOfBones);
	
	virtual TextureId createTexture2d(std::string uri);
	
	virtual RenderableId createRenderable(MeshId meshId, TextureId textureId);
	
	virtual void rotate(const CameraId cameraId, const glm::quat& quaternion, TransformSpace relativeTo = TransformSpace::TS_LOCAL);
	virtual void rotate(const RenderableId renderableId, const glm::quat& quaternion, TransformSpace relativeTo = TransformSpace::TS_LOCAL);
	virtual void rotate(const CameraId cameraId, const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TransformSpace::TS_LOCAL);
	virtual void rotate(const RenderableId renderableId, const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TransformSpace::TS_LOCAL);
	
	virtual void translate(const CameraId cameraId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z);
	virtual void translate(const RenderableId renderableId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z);
	virtual void translate(const CameraId cameraId, const glm::vec3& trans);
	virtual void translate(const RenderableId renderableId, const glm::vec3& trans);
	
	virtual void scale(const RenderableId renderableId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z);
	virtual void scale(const RenderableId renderableId, const glm::vec3& scale);
	virtual void scale(const RenderableId renderableId, const glm::detail::float32 scale);
	
	virtual void position(const RenderableId renderableId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z);
	virtual void position(const CameraId cameraId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z);
	virtual void position(const RenderableId renderableId, const glm::vec3& position);
	virtual void position(const CameraId cameraId, const glm::vec3& position);
	
	virtual void lookAt(const RenderableId renderableId, const glm::vec3& lookAt);
	virtual void lookAt(const CameraId cameraId, const glm::vec3& lookAt);
	
	virtual void assign(const RenderableId renderableId, const SkeletonId skeletonId);
	
	virtual void update(const SkeletonId skeletonId, const void* data, glm::detail::uint32 size);
	
	virtual void setMouseRelativeMode(bool enabled);
	virtual void setCursorVisible(bool visible);
	
	virtual void processEvents();
	virtual void addEventListener(IEventListener* eventListener);
	virtual void removeEventListener(IEventListener* eventListener);

private:
	GraphicsEngine(const GraphicsEngine& other);
	
	glm::detail::uint32 width_;
	glm::detail::uint32 height_;
	
	GLuint shaderProgram_;
	
	SDL_Window* sdlWindow_;
	SDL_GLContext openglContext_;
	
	std::vector<IEventListener*> eventListeners_;
	std::vector<Renderable> renderables_;
	std::vector<Vao> vertexArrayObjects_;
	std::vector<Ubo> uniformBufferObjects_;
	std::vector<GlTexture2d> texture2dObjects_;
	Camera camera_;
	
	std::vector<GraphicsData> graphicsData_;
	glm::mat4 model_;
	glm::mat4 view_;
	glm::mat4 projection_;
	
	fs::IFileSystem* fileSystem_;
	
	GLuint createShaderProgram(std::string vertexShaderUri, std::string fragmentShaderUri);
	GLuint createShaderProgramFromSource(std::string vertexShaderSource, std::string fragmentShaderSource);
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint compileShader(std::string source, GLenum type);
	
	std::string getShaderErrorMessage(GLuint shader);
	std::string getShaderProgramErrorMessage(GLuint shaderProgram);
	
	void handleEvent(const Event& event);
	static Event convertSdlEvent(const SDL_Event& event);
	static KeySym convertSdlKeySym(const SDL_Keysym& keySym);
	static ScanCode convertSdlScancode(const SDL_Scancode& sdlScancode);
	static KeyCode convertSdlKeycode(const SDL_Keycode& sdlKeycode);
};

}

#endif /* GRAPHICSENGINE_H_ */
