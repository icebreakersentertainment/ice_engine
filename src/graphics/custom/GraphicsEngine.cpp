#include <algorithm>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <system_error>

#include "graphics/custom/GraphicsEngine.hpp"

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace hercules
{
namespace graphics
{
namespace custom
{

GraphicsEngine::GraphicsEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger)
{
	properties_ = properties;
	fileSystem_ = fileSystem;
	logger_ = logger;
	
	width_ = properties->getIntValue(std::string("window.width"), 1024);
	height_ = properties->getIntValue(std::string("window.height"), 768);
	
	logger_->info(std::string("Width and height set to ") + std::to_string(width_) + " x " + std::to_string(height_));
	
	auto errorCode = SDL_Init( SDL_INIT_VIDEO );
	
	if (errorCode != 0)
	{
		auto message = std::string("Unable to initialize SDL: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	
	auto windowTitle = properties_->getStringValue("window.title", "Hercules");
	
	sdlWindow_ = SDL_CreateWindow(windowTitle.c_str(), 50, 50, width_, height_, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	if (sdlWindow_ == nullptr)
	{
		auto message = std::string("Unable to create window: ") + SDL_GetError();
		throw std::runtime_error(message);
	}

	openglContext_ = SDL_GL_CreateContext( sdlWindow_ );
	
	if (openglContext_ == nullptr)
	{
		auto message = std::string("Unable to create OpenGL context: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
	
	glewExperimental = GL_TRUE; // Needed in core profile
	
	auto result = glewInit();
	
	if (result != GLEW_OK)
	{
		auto msg = std::string("Failed to initialize GLEW.");
		throw std::runtime_error(msg);
	}
	
	// Set up the model, view, and projection matrices
	model_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	view_ = glm::mat4(1.0f);
	setViewport(width_, height_);
}

GraphicsEngine::GraphicsEngine(const GraphicsEngine& other)
{
}

GraphicsEngine::~GraphicsEngine()
{
	if (openglContext_)
	{
		SDL_GL_DeleteContext(openglContext_);
		openglContext_ = nullptr;
	}
	
	if (sdlWindow_)
	{
		SDL_SetWindowFullscreen( sdlWindow_, 0 );
		SDL_DestroyWindow( sdlWindow_ );
		sdlWindow_ = nullptr;
	}

	SDL_Quit();
}
	
void GraphicsEngine::setViewport(const uint32 width, const uint32 height)
{
	width_ = width;
	height_ = height;
	
	projection_ = glm::perspective(glm::radians(60.0f), (float32)width / (float32)height, 0.1f, 500.f);
	
	glViewport(0, 0, width_, height_);
}

void GraphicsEngine::render(const float32 delta)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	
	// Setup camera
	const glm::quat temp = glm::conjugate(camera_.orientation);

	view_ = glm::mat4_cast(temp);
	view_ = glm::translate(view_, glm::vec3(-camera_.position.x, -camera_.position.y, -camera_.position.z));
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int modelMatrixLocation = 0;
	int pvmMatrixLocation = 0;
	int normalMatrixLocation = 0;
	
	//assert( modelMatrixLocation >= 0);
	//assert( pvmMatrixLocation >= 0);
	//assert( normalMatrixLocation >= 0);
	
	ShaderProgram currentShaderProgram;
	for ( const auto& r : renderables_ )
	{
		if (currentShaderProgram.id != r.shaderProgram.id)
		{
			currentShaderProgram = r.shaderProgram;
			modelMatrixLocation = glGetUniformLocation(currentShaderProgram.id, "modelMatrix");
			pvmMatrixLocation = glGetUniformLocation(currentShaderProgram.id, "pvmMatrix");
			normalMatrixLocation = glGetUniformLocation(currentShaderProgram.id, "normalMatrix");
			glUseProgram(currentShaderProgram.id);
		}
		
		glm::mat4 newModel = glm::translate(model_, r.graphicsData.position);
		newModel = newModel * glm::mat4_cast( r.graphicsData.orientation );
		newModel = glm::scale(newModel, r.graphicsData.scale);
	
		// Send uniform variable values to the shader		
		const glm::mat4 pvmMatrix(projection_ * view_ * newModel);
		glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);
	
		glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(view_ * newModel)));
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &newModel[0][0]);
		
		if (r.ubo.id > 0)
		{
			//const int bonesLocation = glGetUniformLocation(currentShaderProgram.id, "bones");
			//assert( bonesLocation >= 0);
			//glBindBufferBase(GL_UNIFORM_BUFFER, bonesLocation, r.ubo.id);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, r.ubo.id);
		}
		
		glBindTexture(GL_TEXTURE_2D, r.texture.id);
		glBindVertexArray(r.vao.id);
		glDrawElements(r.vao.ebo.mode, r.vao.ebo.count, r.vao.ebo.type, 0);
		glBindVertexArray(0);
	}
	
	SDL_GL_SwapWindow( sdlWindow_ );
}

CameraHandle GraphicsEngine::createCamera(const glm::vec3& position, const glm::vec3& lookAt)
{
	camera_ = Camera();
	camera_.position = position;
	camera_.orientation = glm::quat();
	//camera_.orientation = glm::normalize(camera_.orientation);
	
	CameraHandle cameraHandle = CameraHandle(0, 1);
	
	this->lookAt(cameraHandle, lookAt);
	
	return cameraHandle;
}

MeshHandle GraphicsEngine::createStaticMesh(
	const std::vector<glm::vec3>& vertices,
	const std::vector<uint32>& indices,
	const std::vector<glm::vec4>& colors,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& textureCoordinates
)
{
	auto handle = meshes_.create();
	auto& vao = meshes_[handle];
	
	glGenVertexArrays(1, &vao.id);
	glGenBuffers(1, &vao.vbo[0].id);
	glGenBuffers(1, &vao.ebo.id);
	
	auto size = vertices.size() * sizeof(glm::vec3);
	size += colors.size() * sizeof(glm::vec4);
	size += normals.size() * sizeof(glm::vec3);
	size += textureCoordinates.size() * sizeof(glm::vec2);
	
	glBindVertexArray(vao.id);
	glBindBuffer(GL_ARRAY_BUFFER, vao.vbo[0].id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	
	auto offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(glm::vec3), &vertices[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	offset += vertices.size() * sizeof(glm::vec3);
	glBufferSubData(GL_ARRAY_BUFFER, offset, colors.size() * sizeof(glm::vec4), &colors[0]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(1);
	
	offset += colors.size() * sizeof(glm::vec4);
	glBufferSubData(GL_ARRAY_BUFFER, offset, normals.size() * sizeof(glm::vec3), &normals[0]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(2);
	
	offset += normals.size() * sizeof(glm::vec3);
	glBufferSubData(GL_ARRAY_BUFFER, offset, textureCoordinates.size() * sizeof(glm::vec2), &textureCoordinates[0]);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(3);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ebo.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW); 
	
	glBindVertexArray(0);
	
	vao.ebo.count = indices.size();
	vao.ebo.mode = GL_TRIANGLES;
	vao.ebo.type =  GL_UNSIGNED_INT;
	
	return handle;
}

MeshHandle GraphicsEngine::createAnimatedMesh(
		const std::vector<glm::vec3>& vertices,
		const std::vector<uint32>& indices,
		const std::vector<glm::vec4>& colors,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& textureCoordinates,
		const std::vector<glm::ivec4>& boneIds,
		const std::vector<glm::vec4>& boneWeights
	)
{
	auto handle = meshes_.create();
	auto& vao = meshes_[handle];
	
	glGenVertexArrays(1, &vao.id);
	glGenBuffers(1, &vao.vbo[0].id);
	glGenBuffers(1, &vao.ebo.id);
	
	auto size = vertices.size() * sizeof(glm::vec3);
	size += colors.size() * sizeof(glm::vec4);
	size += normals.size() * sizeof(glm::vec3);
	size += textureCoordinates.size() * sizeof(glm::vec2);
	size += boneIds.size() * sizeof(glm::ivec4);
	size += boneWeights.size() * sizeof(glm::vec4);
	
	glBindVertexArray(vao.id);
	glBindBuffer(GL_ARRAY_BUFFER, vao.vbo[0].id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	
	auto offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(glm::vec3), &vertices[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	offset += vertices.size() * sizeof(glm::vec3);
	glBufferSubData(GL_ARRAY_BUFFER, offset, colors.size() * sizeof(glm::vec4), &colors[0]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(1);
	
	offset += colors.size() * sizeof(glm::vec4);
	glBufferSubData(GL_ARRAY_BUFFER, offset, normals.size() * sizeof(glm::vec3), &normals[0]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(2);
	
	offset += normals.size() * sizeof(glm::vec3);
	glBufferSubData(GL_ARRAY_BUFFER, offset, textureCoordinates.size() * sizeof(glm::vec2), &textureCoordinates[0]);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(3);
	
	offset += textureCoordinates.size() * sizeof(glm::vec2);
	glBufferSubData(GL_ARRAY_BUFFER, offset, boneIds.size() * sizeof(glm::ivec4), &boneIds[0]);
	glVertexAttribIPointer(4, 4, GL_INT, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(4);
	
	offset += boneIds.size() * sizeof(glm::ivec4);
	glBufferSubData(GL_ARRAY_BUFFER, offset, boneWeights.size() * sizeof(glm::vec4), &boneWeights[0]);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset));
	glEnableVertexAttribArray(5);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ebo.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW); 
	
	glBindVertexArray(0);
	
	vao.ebo.count = indices.size();
	vao.ebo.mode = GL_TRIANGLES;
	vao.ebo.type =  GL_UNSIGNED_INT;
	
	return handle;
}

MeshHandle GraphicsEngine::createDynamicMesh(
	const std::vector<glm::vec3>& vertices,
	const std::vector<uint32>& indices,
	const std::vector<glm::vec4>& colors,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& textureCoordinates
)
{
	/*
	auto handle = meshes_.create();
	auto& vao = meshes_[handle];
	
	glGenVertexArrays(1, &vao.id);
	glGenBuffers(1, &vao.vbo[0].id);
	glGenBuffers(1, &vao.vbo[1].id);
	glGenBuffers(1, &vao.vbo[2].id);
	glGenBuffers(1, &vao.vbo[3].id);
	glGenBuffers(1, &vao.ebo.id);
	
	glBindVertexArray(vao.id);
	glBindBuffer(GL_ARRAY_BUFFER, vao.vbo[0].id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vao.vbo[1].id);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vao.vbo[2].id);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, vao.vbo[3].id);
	glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(glm::vec2), &textureCoordinates[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ebo.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW); 
	
	glBindVertexArray(0);
	
	vao.ebo.count = indices.size();
	vao.ebo.mode = GL_TRIANGLES;
	vao.ebo.type =  GL_UNSIGNED_INT;
	
	vertexArrayObjects_.push_back(vao);
	auto index = vertexArrayObjects_.size() - 1;
	
	return handle;
	*/
	return MeshHandle();
}

SkeletonHandle GraphicsEngine::createSkeleton(const uint32 numberOfBones)
{
	auto handle = skeletons_.create();
	auto& ubo = skeletons_[handle];
	
	glGenBuffers(1, &ubo.id);
	
	auto size = numberOfBones * sizeof(glm::mat4);
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
	
	return handle;
}

TextureHandle GraphicsEngine::createTexture2d(const image::Image& image)
{
	auto handle = texture2ds_.create();
	auto& texture = texture2ds_[handle];
	
	uint32 width = 0;
	uint32 height = 0;
	
	auto format = image::getOpenGlImageFormat(image.format);
	
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, &image.data[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return handle;
}

ShaderHandle GraphicsEngine::createVertexShader(const std::string& data)
{
	logger_->debug("Creating vertex shader from data: " + data);
	return shaders_.create( compileShader(data, GL_VERTEX_SHADER) );
}

ShaderHandle GraphicsEngine::createFragmentShader(const std::string& data)
{
	logger_->debug("Creating fragment shader from data: " + data);
	return shaders_.create( compileShader(data, GL_FRAGMENT_SHADER) );
}

bool GraphicsEngine::valid(const ShaderHandle& shaderHandle) const
{
	return shaders_.valid(shaderHandle);
}

void GraphicsEngine::destroyShader(const ShaderHandle& shaderHandle)
{
	if (!shaders_.valid(shaderHandle))
	{
		throw std::runtime_error("Invalid shader handle");
	}
	
	const auto shader = shaders_[shaderHandle];
	
	glDeleteShader(shader.id);
	
	shaders_.destroy(shaderHandle);
}

ShaderProgramHandle GraphicsEngine::createShaderProgram(const ShaderHandle& vertexShaderHandle, const ShaderHandle& fragmentShaderHandle)
{
	const auto vertexShader = shaders_[vertexShaderHandle];
	const auto fragmentShader = shaders_[fragmentShaderHandle];
	
	return shaderPrograms_.create( createShaderProgram(vertexShader.id, fragmentShader.id) );
}

bool GraphicsEngine::valid(const ShaderProgramHandle& shaderProgramHandle) const
{
	return shaderPrograms_.valid(shaderProgramHandle);
}

void GraphicsEngine::destroyShaderProgram(const ShaderProgramHandle& shaderProgramHandle)
{
	if (!shaderPrograms_.valid(shaderProgramHandle))
	{
		throw std::runtime_error("Invalid shader program handle");
	}
	
	const auto shaderProgram = shaderPrograms_[shaderProgramHandle];
	
	glDeleteProgram(shaderProgram.id);
	
	shaderPrograms_.destroy(shaderProgramHandle);
}


RenderableHandle GraphicsEngine::createRenderable(const MeshHandle& meshHandle, const TextureHandle& textureHandle, const ShaderProgramHandle& shaderProgramHandle)
{
	auto handle = renderables_.create();
	auto& renderable = renderables_[handle];
	
	renderable.vao = meshes_[meshHandle];
	renderable.texture = texture2ds_[textureHandle];
	renderable.shaderProgram = shaderPrograms_[shaderProgramHandle];
	
	renderable.graphicsData.position = glm::vec3(0.0f, 0.0f, 0.0f);
	renderable.graphicsData.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	renderable.graphicsData.orientation = glm::quat();
	
	return handle;
}

void GraphicsEngine::rotate(const CameraHandle& cameraHandle, const glm::quat& quaternion, const TransformSpace& relativeTo)
{
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			camera_.orientation = camera_.orientation * glm::normalize( quaternion );
			break;
		
		case TransformSpace::TS_WORLD:
			camera_.orientation =  glm::normalize( quaternion ) * camera_.orientation;
			break;
			
		default:
			std::string message = std::string("Invalid TransformSpace type.");
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotate(const RenderableHandle& renderableHandle, const glm::quat& quaternion, const TransformSpace& relativeTo)
{
	auto& renderable = renderables_[renderableHandle];
	
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			renderable.graphicsData.orientation = renderable.graphicsData.orientation * glm::normalize( quaternion );
			break;
		
		case TransformSpace::TS_WORLD:
			renderable.graphicsData.orientation =  glm::normalize( quaternion ) * renderable.graphicsData.orientation;
			break;
			
		default:
			std::string message = std::string("Invalid TransformSpace type.");
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotate(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo)
{
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			camera_.orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) ) * camera_.orientation;
			break;
		
		case TransformSpace::TS_WORLD:
			camera_.orientation =  camera_.orientation * glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
			break;
			
		default:
			std::string message = std::string("Invalid TransformSpace type.");
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotate(const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo)
{
	auto& renderable = renderables_[renderableHandle];
	
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			renderable.graphicsData.orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) ) * renderable.graphicsData.orientation;
			break;
		
		case TransformSpace::TS_WORLD:
			renderable.graphicsData.orientation =  renderable.graphicsData.orientation * glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
			break;
			
		default:
			std::string message = std::string("Invalid TransformSpace type.");
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotation(const CameraHandle& cameraHandle, const glm::quat& quaternion)
{
	camera_.orientation = glm::normalize( quaternion );
}

void GraphicsEngine::rotation(const RenderableHandle& renderableHandle, const glm::quat& quaternion)
{
	auto& renderable = renderables_[renderableHandle];

	renderable.graphicsData.orientation = glm::normalize( quaternion );
}

void GraphicsEngine::rotation(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis)
{
	camera_.orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
}

void GraphicsEngine::rotation(const RenderableHandle& renderableHandle, const float32 degrees, const glm::vec3& axis)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.graphicsData.orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
}

glm::quat GraphicsEngine::rotation(const CameraHandle& cameraHandle) const
{
	return camera_.orientation;
}

glm::quat GraphicsEngine::rotation(const RenderableHandle& renderableHandle) const
{
	return renderables_[renderableHandle].graphicsData.orientation;
}

void GraphicsEngine::translate(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z)
{
	camera_.position += glm::vec3(x, y, z);
}

void GraphicsEngine::translate(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.graphicsData.position += glm::vec3(x, y, z);
}

void GraphicsEngine::translate(const CameraHandle& cameraHandle, const glm::vec3& trans)
{
	camera_.position += trans;
}

void GraphicsEngine::translate(const RenderableHandle& renderableHandle, const glm::vec3& trans)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.graphicsData.position += trans;
}

void GraphicsEngine::scale(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.graphicsData.scale = glm::vec3(x, y, z);
}

void GraphicsEngine::scale(const RenderableHandle& renderableHandle, const glm::vec3& scale)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.graphicsData.scale = scale;
}

void GraphicsEngine::scale(const RenderableHandle& renderableHandle, const float32 scale)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.graphicsData.scale = glm::vec3(scale, scale, scale);
}

glm::vec3 GraphicsEngine::scale(const RenderableHandle& renderableHandle) const
{
	return renderables_[renderableHandle].graphicsData.scale;
}

void GraphicsEngine::position(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z)
{
	auto& renderable = renderables_[renderableHandle];
	renderable.graphicsData.position = glm::vec3(x, y, z);
}

void GraphicsEngine::position(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z)
{
	camera_.position = glm::vec3(x, y, z);
}

void GraphicsEngine::position(const RenderableHandle& renderableHandle, const glm::vec3& position)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.graphicsData.position = position;
}

void GraphicsEngine::position(const CameraHandle& cameraHandle, const glm::vec3& position)
{
	camera_.position = position;
}

glm::vec3 GraphicsEngine::position(const RenderableHandle& renderableHandle) const
{
	return renderables_[renderableHandle].graphicsData.position;
}

glm::vec3 GraphicsEngine::position(const CameraHandle& cameraHandle) const
{
	return camera_.position;
}

void GraphicsEngine::lookAt(const RenderableHandle& renderableHandle, const glm::vec3& lookAt)
{
	auto& renderable = renderables_[renderableHandle];
	
	assert(lookAt != renderable.graphicsData.position);
	
	const glm::mat4 lookAtMatrix = glm::lookAt(renderable.graphicsData.position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	renderable.graphicsData.orientation =  glm::normalize( renderable.graphicsData.orientation * glm::quat_cast( lookAtMatrix ) );
}

void GraphicsEngine::lookAt(const CameraHandle& cameraHandle, const glm::vec3& lookAt)
{
	assert(lookAt != camera_.position);
	
	const glm::mat4 lookAtMatrix = glm::lookAt(camera_.position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	camera_.orientation =  glm::normalize( camera_.orientation * glm::quat_cast( lookAtMatrix ) );
}

void GraphicsEngine::assign(const RenderableHandle& renderableHandle, const SkeletonHandle& skeletonHandle)
{
	auto& renderable = renderables_[renderableHandle];
	
	renderable.ubo = skeletons_[skeletonHandle];
}

void GraphicsEngine::update(const SkeletonHandle& skeletonHandle, const void* data, const uint32 size)
{
	const auto& ubo = skeletons_[skeletonHandle];
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	
	//void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	//memcpy( d, data, size );
	//glUnmapBuffer(GL_UNIFORM_BUFFER);
}

GLuint GraphicsEngine::createShaderProgram(const GLuint vertexShader, const GLuint fragmentShader)
{
	auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	{
		GLint compiled = GL_FALSE;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compiled);
		
		if (!compiled)
		{
			std::stringstream message;
			message << "Could not link shader program: ";
			message << "\n" << getShaderProgramErrorMessage(shaderProgram);
	
			// Cleanup
			glDeleteProgram(shaderProgram);
	
			throw std::runtime_error(message.str());
		}
		else
		{
			auto s = getShaderProgramErrorMessage(shaderProgram);
			
			if (s.size() > 0)
			{
				std::stringstream message;
				message << "Results of linking shader program: ";
				message << "\n" << s;
		
				logger_->warn( message.str() );
			}
		}
	}
	
	return shaderProgram;
}

GLuint GraphicsEngine::compileShader(const std::string& source, const GLenum type)
{
	auto shader = glCreateShader(type);
	
	{
		const char* s = source.c_str();
		glShaderSource(shader, 1, &s, nullptr);
		glCompileShader(shader);
		
		GLint compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		
		if (!compiled)
		{
			std::stringstream message;
			message << "Could not compile shader: ";
			message << "\n" << getShaderErrorMessage(shader);
	
			// Cleanup
			glDeleteShader(shader);
	
			throw std::runtime_error(message.str());
		}
		else
		{
			auto s = getShaderErrorMessage(shader);
			
			if (s.size() > 0)
			{
				std::stringstream message;
				message << "Results of shader compilation: ";
				message << "\n" << s;
		
				logger_->warn( message.str() );
			}
		}
	}
	
	return shader;
}

std::string GraphicsEngine::getShaderErrorMessage(const GLuint shader)
{
	GLint infoLogLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (infoLogLength == 0)
	{
		return std::string();
	}

	GLchar* strInfoLog = new GLchar[infoLogLength];
	glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog);

	std::stringstream message;
	message << strInfoLog;

	delete[] strInfoLog;
	
	return message.str();
}

std::string GraphicsEngine::getShaderProgramErrorMessage(const GLuint shaderProgram)
{
	GLint infoLogLength;
	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (infoLogLength == 0)
	{
		return std::string();
	}

	GLchar* strInfoLog = new GLchar[infoLogLength ];
	glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, strInfoLog);

	std::stringstream message;
	message << strInfoLog;

	delete[] strInfoLog;
	
	return message.str();
}

void GraphicsEngine::setMouseRelativeMode(const bool enabled)
{
	auto result = SDL_SetRelativeMouseMode(static_cast<SDL_bool>(enabled));
	
	if (result != 0)
	{
		auto message = std::string("Unable to set relative mouse mode: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
}

void GraphicsEngine::setCursorVisible(const bool visible)
{
	auto toggle = (visible ? SDL_ENABLE : SDL_DISABLE);
	auto result = SDL_ShowCursor(toggle);
	
	if (result < 0)
	{
		auto message = std::string("Unable to set mouse cursor visible\\invisible: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
}

void GraphicsEngine::processEvents()
{
	SDL_Event evt;
	while( SDL_PollEvent( &evt ) )
	{
		Event event = convertSdlEvent(evt);
		this->handleEvent( event );
		/*
		switch( evt.type )
		{
			case SDL_WINDOWEVENT:
				//handleWindowEvent( evt );
				break;
			case SDL_QUIT:
				
				break;
			default:
				break;
		}
		*/
	}
}

void GraphicsEngine::handleEvent(const Event& event)
{
	for ( auto it : eventListeners_ )
	{
		it->processEvent(event);
	}
}

Event GraphicsEngine::convertSdlEvent(const SDL_Event& event)
{
	Event e;

	switch(event.type)
	{
		case SDL_QUIT:
			e.type = QUIT;
			break;
		
		case SDL_KEYDOWN:
			e.type = KEYDOWN;
			e.key.keySym = convertSdlKeySym(event.key.keysym);
			e.key.state = (event.key.state == SDL_RELEASED ? KEY_RELEASED : KEY_PRESSED);
			e.key.repeat = event.key.repeat;
			break;
		
		case SDL_KEYUP:
			e.type = KEYUP;
			e.key.keySym = convertSdlKeySym(event.key.keysym);
			e.key.state = (event.key.state == SDL_RELEASED ? KEY_RELEASED : KEY_PRESSED);
			e.key.repeat = event.key.repeat;
			break;
			
		case SDL_MOUSEMOTION:
			e.type = MOUSEMOTION;
			e.motion.x = event.motion.x;
			e.motion.y = event.motion.y;
			e.motion.xrel = event.motion.xrel;
			e.motion.yrel = event.motion.yrel;
			//e.state = (event.key.state == SDL_RELEASED ? KEY_RELEASED : KEY_PRESSED);
			
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			e.type = MOUSEBUTTONDOWN;
			e.button.button = event.button.button;
			e.button.state = event.button.state;
			e.button.clicks = event.button.clicks;
			e.button.x = event.button.x;
			e.button.y = event.button.y;
			
			break;
		
		case SDL_MOUSEBUTTONUP:
			e.type = MOUSEBUTTONUP;
			e.button.button = event.button.button;
			e.button.state = event.button.state;
			e.button.clicks = event.button.clicks;
			e.button.x = event.button.x;
			e.button.y = event.button.y;
			
			break;
		
		case SDL_MOUSEWHEEL:
			e.type = MOUSEWHEEL;
			e.wheel.x = event.wheel.x;
			e.wheel.y = event.wheel.y;
			e.wheel.direction = event.wheel.direction;
			
			break;
		
		default:
			e.type = UNKNOWN;
			break;
	}
	
	return e;
}

KeySym GraphicsEngine::convertSdlKeySym(const SDL_Keysym& keySym)
{
	KeySym key;
	
	key.sym = convertSdlKeycode(keySym.sym);
	key.scancode = convertSdlScancode(keySym.scancode);
	key.mod = convertSdlKeymod(keySym.mod);
	
	return key;
}

KeyCode GraphicsEngine::convertSdlKeycode(const SDL_Keycode& sdlKeycode)
{
	switch(sdlKeycode)
	{
		case SDLK_0:
			return KEY_0;
		case SDLK_1:
			return KEY_1;
		case SDLK_2:
			return KEY_2;
		case SDLK_3:
			return KEY_3;
		case SDLK_4:
			return KEY_4;
		case SDLK_5:
			return KEY_5;
		case SDLK_6:
			return KEY_6;
		case SDLK_7:
			return KEY_7;
		case SDLK_8:
			return KEY_8;
		case SDLK_9:
			return KEY_9;
		case SDLK_a:
			return KEY_a;
		case SDLK_AC_BACK:
			return KEY_AC_BACK;
		case SDLK_AC_BOOKMARKS:
			return KEY_AC_BOOKMARKS;
		case SDLK_AC_FORWARD:
			return KEY_AC_FORWARD;
		case SDLK_AC_HOME:
			return KEY_AC_HOME;
		case SDLK_AC_REFRESH:
			return KEY_AC_REFRESH;
		case SDLK_AC_SEARCH:
			return KEY_AC_SEARCH;
		case SDLK_AC_STOP:
			return KEY_AC_STOP;
		case SDLK_AGAIN:
			return KEY_AGAIN;
		case SDLK_ALTERASE:
			return KEY_ALTERASE;
		case SDLK_QUOTE:
			return KEY_QUOTE;
		case SDLK_APPLICATION:
			return KEY_APPLICATION;
		case SDLK_AUDIOMUTE:
			return KEY_AUDIOMUTE;
		case SDLK_AUDIONEXT:
			return KEY_AUDIONEXT;
		case SDLK_AUDIOPLAY:
			return KEY_AUDIOPLAY;
		case SDLK_AUDIOPREV:
			return KEY_AUDIOPREV;
		case SDLK_AUDIOSTOP:
			return KEY_AUDIOSTOP;
		case SDLK_b:
			return KEY_b;
		case SDLK_BACKSLASH:
			return KEY_BACKSLASH;
		case SDLK_BACKSPACE:
			return KEY_BACKSPACE;
		case SDLK_BRIGHTNESSDOWN:
			return KEY_BRIGHTNESSDOWN;
		case SDLK_BRIGHTNESSUP:
			return KEY_BRIGHTNESSUP;
		case SDLK_c:
			return KEY_c;
		case SDLK_CALCULATOR:
			return KEY_CALCULATOR;
		case SDLK_CANCEL:
			return KEY_CANCEL;
		case SDLK_CAPSLOCK:
			return KEY_CAPSLOCK;
		case SDLK_CLEAR:
			return KEY_CLEAR;
		case SDLK_CLEARAGAIN:
			return KEY_CLEARAGAIN;
		case SDLK_COMMA:
			return KEY_COMMA;
		case SDLK_COMPUTER:
			return KEY_COMPUTER;
		case SDLK_COPY:
			return KEY_COPY;
		case SDLK_CRSEL:
			return KEY_CRSEL;
		case SDLK_CURRENCYSUBUNIT:
			return KEY_CURRENCYSUBUNIT;
		case SDLK_CURRENCYUNIT:
			return KEY_CURRENCYUNIT;
		case SDLK_CUT:
			return KEY_CUT;
		case SDLK_d:
			return KEY_d;
		case SDLK_DECIMALSEPARATOR:
			return KEY_DECIMALSEPARATOR;
		case SDLK_DELETE:
			return KEY_DELETE;
		case SDLK_DISPLAYSWITCH:
			return KEY_DISPLAYSWITCH;
		case SDLK_DOWN:
			return KEY_DOWN;
		case SDLK_e:
			return KEY_e;
		case SDLK_EJECT:
			return KEY_EJECT;
		case SDLK_END:
			return KEY_END;
		case SDLK_EQUALS:
			return KEY_EQUALS;
		case SDLK_ESCAPE:
			return KEY_ESCAPE;
		case SDLK_EXECUTE:
			return KEY_EXECUTE;
		case SDLK_EXSEL:
			return KEY_EXSEL;
		case SDLK_f:
			return KEY_f;
		case SDLK_F1:
			return KEY_F1;
		case SDLK_F10:
			return KEY_F10;
		case SDLK_F11:
			return KEY_F11;
		case SDLK_F12:
			return KEY_F12;
		case SDLK_F13:
			return KEY_F13;
		case SDLK_F14:
			return KEY_F14;
		case SDLK_F15:
			return KEY_F15;
		case SDLK_F16:
			return KEY_F16;
		case SDLK_F17:
			return KEY_F17;
		case SDLK_F18:
			return KEY_F18;
		case SDLK_F19:
			return KEY_F19;
		case SDLK_F2:
			return KEY_F2;
		case SDLK_F20:
			return KEY_F20;
		case SDLK_F21:
			return KEY_F21;
		case SDLK_F22:
			return KEY_F22;
		case SDLK_F23:
			return KEY_F23;
		case SDLK_F24:
			return KEY_F24;
		case SDLK_F3:
			return KEY_F3;
		case SDLK_F4:
			return KEY_F4;
		case SDLK_F5:
			return KEY_F5;
		case SDLK_F6:
			return KEY_F6;
		case SDLK_F7:
			return KEY_F7;
		case SDLK_F8:
			return KEY_F8;
		case SDLK_F9:
			return KEY_F9;
		case SDLK_FIND:
			return KEY_FIND;
		case SDLK_g:
			return KEY_g;
		case SDLK_BACKQUOTE:
			return KEY_BACKQUOTE;
		case SDLK_h:
			return KEY_h;
		case SDLK_HELP:
			return KEY_HELP;
		case SDLK_HOME:
			return KEY_HOME;
		case SDLK_i:
			return KEY_i;
		case SDLK_INSERT:
			return KEY_INSERT;
		case SDLK_j:
			return KEY_j;
		case SDLK_k:
			return KEY_k;
		case SDLK_KBDILLUMDOWN:
			return KEY_KBDILLUMDOWN;
		case SDLK_KBDILLUMTOGGLE:
			return KEY_KBDILLUMTOGGLE;
		case SDLK_KBDILLUMUP:
			return KEY_KBDILLUMUP;
		case SDLK_KP_0:
			return KEY_KP_0;
		case SDLK_KP_00:
			return KEY_KP_00;
		case SDLK_KP_000:
			return KEY_KP_000;
		case SDLK_KP_1:
			return KEY_KP_1;
		case SDLK_KP_2:
			return KEY_KP_2;
		case SDLK_KP_3:
			return KEY_KP_3;
		case SDLK_KP_4:
			return KEY_KP_4;
		case SDLK_KP_5:
			return KEY_KP_5;
		case SDLK_KP_6:
			return KEY_KP_6;
		case SDLK_KP_7:
			return KEY_KP_7;
		case SDLK_KP_8:
			return KEY_KP_8;
		case SDLK_KP_9:
			return KEY_KP_9;
		case SDLK_KP_A:
			return KEY_KP_A;
		case SDLK_KP_AMPERSAND:
			return KEY_KP_AMPERSAND;
		case SDLK_KP_AT:
			return KEY_KP_AT;
		case SDLK_KP_B:
			return KEY_KP_B;
		case SDLK_KP_BACKSPACE:
			return KEY_KP_BACKSPACE;
		case SDLK_KP_BINARY:
			return KEY_KP_BINARY;
		case SDLK_KP_C:
			return KEY_KP_C;
		case SDLK_KP_CLEAR:
			return KEY_KP_CLEAR;
		case SDLK_KP_CLEARENTRY:
			return KEY_KP_CLEARENTRY;
		case SDLK_KP_COLON:
			return KEY_KP_COLON;
		case SDLK_KP_COMMA:
			return KEY_KP_COMMA;
		case SDLK_KP_D:
			return KEY_KP_D;
		case SDLK_KP_DBLAMPERSAND:
			return KEY_KP_DBLAMPERSAND;
		case SDLK_KP_DBLVERTICALBAR:
			return KEY_KP_DBLVERTICALBAR;
		case SDLK_KP_DECIMAL:
			return KEY_KP_DECIMAL;
		case SDLK_KP_DIVIDE:
			return KEY_KP_DIVIDE;
		case SDLK_KP_E:
			return KEY_KP_E;
		case SDLK_KP_ENTER:
			return KEY_KP_ENTER;
		case SDLK_KP_EQUALS:
			return KEY_KP_EQUALS;
		case SDLK_KP_EQUALSAS400:
			return KEY_KP_EQUALSAS400;
		case SDLK_KP_EXCLAM:
			return KEY_KP_EXCLAM;
		case SDLK_KP_F:
			return KEY_KP_F;
		case SDLK_KP_GREATER:
			return KEY_KP_GREATER;
		case SDLK_KP_HASH:
			return KEY_KP_HASH;
		case SDLK_KP_HEXADECIMAL:
			return KEY_KP_HEXADECIMAL;
		case SDLK_KP_LEFTBRACE:
			return KEY_KP_LEFTBRACE;
		case SDLK_KP_LEFTPAREN:
			return KEY_KP_LEFTPAREN;
		case SDLK_KP_LESS:
			return KEY_KP_LESS;
		case SDLK_KP_MEMADD:
			return KEY_KP_MEMADD;
		case SDLK_KP_MEMCLEAR:
			return KEY_KP_MEMCLEAR;
		case SDLK_KP_MEMDIVIDE:
			return KEY_KP_MEMDIVIDE;
		case SDLK_KP_MEMMULTIPLY:
			return KEY_KP_MEMMULTIPLY;
		case SDLK_KP_MEMRECALL:
			return KEY_KP_MEMRECALL;
		case SDLK_KP_MEMSTORE:
			return KEY_KP_MEMSTORE;
		case SDLK_KP_MEMSUBTRACT:
			return KEY_KP_MEMSUBTRACT;
		case SDLK_KP_MINUS:
			return KEY_KP_MINUS;
		case SDLK_KP_MULTIPLY:
			return KEY_KP_MULTIPLY;
		case SDLK_KP_OCTAL:
			return KEY_KP_OCTAL;
		case SDLK_KP_PERCENT:
			return KEY_KP_PERCENT;
		case SDLK_KP_PERIOD:
			return KEY_KP_PERIOD;
		case SDLK_KP_PLUS:
			return KEY_KP_PLUS;
		case SDLK_KP_PLUSMINUS:
			return KEY_KP_PLUSMINUS;
		case SDLK_KP_POWER:
			return KEY_KP_POWER;
		case SDLK_KP_RIGHTBRACE:
			return KEY_KP_RIGHTBRACE;
		case SDLK_KP_RIGHTPAREN:
			return KEY_KP_RIGHTPAREN;
		case SDLK_KP_SPACE:
			return KEY_KP_SPACE;
		case SDLK_KP_TAB:
			return KEY_KP_TAB;
		case SDLK_KP_VERTICALBAR:
			return KEY_KP_VERTICALBAR;
		case SDLK_KP_XOR:
			return KEY_KP_XOR;
		case SDLK_l:
			return KEY_l;
		case SDLK_LALT:
			return KEY_LALT;
		case SDLK_LCTRL:
			return KEY_LCTRL;
		case SDLK_LEFT:
			return KEY_LEFT;
		case SDLK_LEFTBRACKET:
			return KEY_LEFTBRACKET;
		case SDLK_LGUI:
			return KEY_LGUI;
		case SDLK_LSHIFT:
			return KEY_LSHIFT;
		case SDLK_m:
			return KEY_m;
		case SDLK_MAIL:
			return KEY_MAIL;
		case SDLK_MEDIASELECT:
			return KEY_MEDIASELECT;
		case SDLK_MENU:
			return KEY_MENU;
		case SDLK_MINUS:
			return KEY_MINUS;
		case SDLK_MODE:
			return KEY_MODE;
		case SDLK_MUTE:
			return KEY_MUTE;
		case SDLK_n:
			return KEY_n;
		case SDLK_NUMLOCKCLEAR:
			return KEY_NUMLOCKCLEAR;
		case SDLK_o:
			return KEY_o;
		case SDLK_OPER:
			return KEY_OPER;
		case SDLK_OUT:
			return KEY_OUT;
		case SDLK_p:
			return KEY_p;
		case SDLK_PAGEDOWN:
			return KEY_PAGEDOWN;
		case SDLK_PAGEUP:
			return KEY_PAGEUP;
		case SDLK_PASTE:
			return KEY_PASTE;
		case SDLK_PAUSE:
			return KEY_PAUSE;
		case SDLK_PERIOD:
			return KEY_PERIOD;
		case SDLK_POWER:
			return KEY_POWER;
		case SDLK_PRINTSCREEN:
			return KEY_PRINTSCREEN;
		case SDLK_PRIOR:
			return KEY_PRIOR;
		case SDLK_q:
			return KEY_q;
		case SDLK_r:
			return KEY_r;
		case SDLK_RALT:
			return KEY_RALT;
		case SDLK_RCTRL:
			return KEY_RCTRL;
		case SDLK_RETURN:
			return KEY_RETURN;
		case SDLK_RETURN2:
			return KEY_RETURN2;
		case SDLK_RGUI:
			return KEY_RGUI;
		case SDLK_RIGHT:
			return KEY_RIGHT;
		case SDLK_RIGHTBRACKET:
			return KEY_RIGHTBRACKET;
		case SDLK_RSHIFT:
			return KEY_RSHIFT;
		case SDLK_s:
			return KEY_s;
		case SDLK_SCROLLLOCK:
			return KEY_SCROLLLOCK;
		case SDLK_SELECT:
			return KEY_SELECT;
		case SDLK_SEMICOLON:
			return KEY_SEMICOLON;
		case SDLK_SEPARATOR:
			return KEY_SEPARATOR;
		case SDLK_SLASH:
			return KEY_SLASH;
		case SDLK_SLEEP:
			return KEY_SLEEP;
		case SDLK_SPACE:
			return KEY_SPACE;
		case SDLK_STOP:
			return KEY_STOP;
		case SDLK_SYSREQ:
			return KEY_SYSREQ;
		case SDLK_t:
			return KEY_t;
		case SDLK_TAB:
			return KEY_TAB;
		case SDLK_THOUSANDSSEPARATOR:
			return KEY_THOUSANDSSEPARATOR;
		case SDLK_u:
			return KEY_u;
		case SDLK_UNDO:
			return KEY_UNDO;
		case SDLK_UNKNOWN:
			return KEY_UNKNOWN;
		case SDLK_UP:
			return KEY_UP;
		case SDLK_v:
			return KEY_v;
		case SDLK_VOLUMEDOWN:
			return KEY_VOLUMEDOWN;
		case SDLK_VOLUMEUP:
			return KEY_VOLUMEUP;
		case SDLK_w:
			return KEY_w;
		case SDLK_WWW:
			return KEY_WWW;
		case SDLK_x:
			return KEY_x;
		case SDLK_y:
			return KEY_y;
		case SDLK_z:
			return KEY_z;
		case SDLK_AMPERSAND:
			return KEY_AMPERSAND;
		case SDLK_ASTERISK:
			return KEY_ASTERISK;
		case SDLK_AT:
			return KEY_AT;
		case SDLK_CARET:
			return KEY_CARET;
		case SDLK_COLON:
			return KEY_COLON;
		case SDLK_DOLLAR:
			return KEY_DOLLAR;
		case SDLK_EXCLAIM:
			return KEY_EXCLAIM;
		case SDLK_GREATER:
			return KEY_GREATER;
		case SDLK_HASH:
			return KEY_HASH;
		case SDLK_LEFTPAREN:
			return KEY_LEFTPAREN;
		case SDLK_LESS:
			return KEY_LESS;
		case SDLK_PERCENT:
			return KEY_PERCENT;
		case SDLK_PLUS:
			return KEY_PLUS;
		case SDLK_QUESTION:
			return KEY_QUESTION;
		case SDLK_QUOTEDBL:
			return KEY_QUOTEDBL;
		case SDLK_RIGHTPAREN:
			return KEY_RIGHTPAREN;
		case SDLK_UNDERSCORE:
			return KEY_UNDERSCORE;
		
		default:
			return KEY_UNKNOWN;
	}
}

ScanCode GraphicsEngine::convertSdlScancode(const SDL_Scancode& sdlScancode)
{
	switch(sdlScancode)
	{
		case SDL_SCANCODE_0:
			return SCANCODE_0;
		case SDL_SCANCODE_1:
			return SCANCODE_1;
		case SDL_SCANCODE_2:
			return SCANCODE_2;
		case SDL_SCANCODE_3:
			return SCANCODE_3;
		case SDL_SCANCODE_4:
			return SCANCODE_4;
		case SDL_SCANCODE_5:
			return SCANCODE_5;
		case SDL_SCANCODE_6:
			return SCANCODE_6;
		case SDL_SCANCODE_7:
			return SCANCODE_7;
		case SDL_SCANCODE_8:
			return SCANCODE_8;
		case SDL_SCANCODE_9:
			return SCANCODE_9;
		case SDL_SCANCODE_A:
			return SCANCODE_A;
		case SDL_SCANCODE_AC_BACK:
			return SCANCODE_AC_BACK;
		case SDL_SCANCODE_AC_BOOKMARKS:
			return SCANCODE_AC_BOOKMARKS;
		case SDL_SCANCODE_AC_FORWARD:
			return SCANCODE_AC_FORWARD;
		case SDL_SCANCODE_AC_HOME:
			return SCANCODE_AC_HOME;
		case SDL_SCANCODE_AC_REFRESH:
			return SCANCODE_AC_REFRESH;
		case SDL_SCANCODE_AC_SEARCH:
			return SCANCODE_AC_SEARCH;
		case SDL_SCANCODE_AC_STOP:
			return SCANCODE_AC_STOP;
		case SDL_SCANCODE_AGAIN:
			return SCANCODE_AGAIN;
		case SDL_SCANCODE_ALTERASE:
			return SCANCODE_ALTERASE;
		case SDL_SCANCODE_APOSTROPHE:
			return SCANCODE_APOSTROPHE;
		case SDL_SCANCODE_APPLICATION:
			return SCANCODE_APPLICATION;
		case SDL_SCANCODE_AUDIOMUTE:
			return SCANCODE_AUDIOMUTE;
		case SDL_SCANCODE_AUDIONEXT:
			return SCANCODE_AUDIONEXT;
		case SDL_SCANCODE_AUDIOPLAY:
			return SCANCODE_AUDIOPLAY;
		case SDL_SCANCODE_AUDIOPREV:
			return SCANCODE_AUDIOPREV;
		case SDL_SCANCODE_AUDIOSTOP:
			return SCANCODE_AUDIOSTOP;
		case SDL_SCANCODE_B:
			return SCANCODE_B;
		case SDL_SCANCODE_BACKSLASH:
			return SCANCODE_BACKSLASH;
		case SDL_SCANCODE_BACKSPACE:
			return SCANCODE_BACKSPACE;
		case SDL_SCANCODE_BRIGHTNESSDOWN:
			return SCANCODE_BRIGHTNESSDOWN;
		case SDL_SCANCODE_BRIGHTNESSUP:
			return SCANCODE_BRIGHTNESSUP;
		case SDL_SCANCODE_C:
			return SCANCODE_C;
		case SDL_SCANCODE_CALCULATOR:
			return SCANCODE_CALCULATOR;
		case SDL_SCANCODE_CANCEL:
			return SCANCODE_CANCEL;
		case SDL_SCANCODE_CAPSLOCK:
			return SCANCODE_CAPSLOCK;
		case SDL_SCANCODE_CLEAR:
			return SCANCODE_CLEAR;
		case SDL_SCANCODE_CLEARAGAIN:
			return SCANCODE_CLEARAGAIN;
		case SDL_SCANCODE_COMMA:
			return SCANCODE_COMMA;
		case SDL_SCANCODE_COMPUTER:
			return SCANCODE_COMPUTER;
		case SDL_SCANCODE_COPY:
			return SCANCODE_COPY;
		case SDL_SCANCODE_CRSEL:
			return SCANCODE_CRSEL;
		case SDL_SCANCODE_CURRENCYSUBUNIT:
			return SCANCODE_CURRENCYSUBUNIT;
		case SDL_SCANCODE_CURRENCYUNIT:
			return SCANCODE_CURRENCYUNIT;
		case SDL_SCANCODE_CUT:
			return SCANCODE_CUT;
		case SDL_SCANCODE_D:
			return SCANCODE_D;
		case SDL_SCANCODE_DECIMALSEPARATOR:
			return SCANCODE_DECIMALSEPARATOR;
		case SDL_SCANCODE_DELETE:
			return SCANCODE_DELETE;
		case SDL_SCANCODE_DISPLAYSWITCH:
			return SCANCODE_DISPLAYSWITCH;
		case SDL_SCANCODE_DOWN:
			return SCANCODE_DOWN;
		case SDL_SCANCODE_E:
			return SCANCODE_E;
		case SDL_SCANCODE_EJECT:
			return SCANCODE_EJECT;
		case SDL_SCANCODE_END:
			return SCANCODE_END;
		case SDL_SCANCODE_EQUALS:
			return SCANCODE_EQUALS;
		case SDL_SCANCODE_ESCAPE:
			return SCANCODE_ESCAPE;
		case SDL_SCANCODE_EXECUTE:
			return SCANCODE_EXECUTE;
		case SDL_SCANCODE_EXSEL:
			return SCANCODE_EXSEL;
		case SDL_SCANCODE_F:
			return SCANCODE_F;
		case SDL_SCANCODE_F1:
			return SCANCODE_F1;
		case SDL_SCANCODE_F10:
			return SCANCODE_F10;
		case SDL_SCANCODE_F11:
			return SCANCODE_F11;
		case SDL_SCANCODE_F12:
			return SCANCODE_F12;
		case SDL_SCANCODE_F13:
			return SCANCODE_F13;
		case SDL_SCANCODE_F14:
			return SCANCODE_F14;
		case SDL_SCANCODE_F15:
			return SCANCODE_F15;
		case SDL_SCANCODE_F16:
			return SCANCODE_F16;
		case SDL_SCANCODE_F17:
			return SCANCODE_F17;
		case SDL_SCANCODE_F18:
			return SCANCODE_F18;
		case SDL_SCANCODE_F19:
			return SCANCODE_F19;
		case SDL_SCANCODE_F2:
			return SCANCODE_F2;
		case SDL_SCANCODE_F20:
			return SCANCODE_F20;
		case SDL_SCANCODE_F21:
			return SCANCODE_F21;
		case SDL_SCANCODE_F22:
			return SCANCODE_F22;
		case SDL_SCANCODE_F23:
			return SCANCODE_F23;
		case SDL_SCANCODE_F24:
			return SCANCODE_F24;
		case SDL_SCANCODE_F3:
			return SCANCODE_F3;
		case SDL_SCANCODE_F4:
			return SCANCODE_F4;
		case SDL_SCANCODE_F5:
			return SCANCODE_F5;
		case SDL_SCANCODE_F6:
			return SCANCODE_F6;
		case SDL_SCANCODE_F7:
			return SCANCODE_F7;
		case SDL_SCANCODE_F8:
			return SCANCODE_F8;
		case SDL_SCANCODE_F9:
			return SCANCODE_F9;
		case SDL_SCANCODE_FIND:
			return SCANCODE_FIND;
		case SDL_SCANCODE_G:
			return SCANCODE_G;
		case SDL_SCANCODE_GRAVE:
			return SCANCODE_GRAVE;
		case SDL_SCANCODE_H:
			return SCANCODE_H;
		case SDL_SCANCODE_HELP:
			return SCANCODE_HELP;
		case SDL_SCANCODE_HOME:
			return SCANCODE_HOME;
		case SDL_SCANCODE_I:
			return SCANCODE_I;
		case SDL_SCANCODE_INSERT:
			return SCANCODE_INSERT;
		case SDL_SCANCODE_J:
			return SCANCODE_J;
		case SDL_SCANCODE_K:
			return SCANCODE_K;
		case SDL_SCANCODE_KBDILLUMDOWN:
			return SCANCODE_KBDILLUMDOWN;
		case SDL_SCANCODE_KBDILLUMTOGGLE:
			return SCANCODE_KBDILLUMTOGGLE;
		case SDL_SCANCODE_KBDILLUMUP:
			return SCANCODE_KBDILLUMUP;
		case SDL_SCANCODE_KP_0:
			return SCANCODE_KP_0;
		case SDL_SCANCODE_KP_00:
			return SCANCODE_KP_00;
		case SDL_SCANCODE_KP_000:
			return SCANCODE_KP_000;
		case SDL_SCANCODE_KP_1:
			return SCANCODE_KP_1;
		case SDL_SCANCODE_KP_2:
			return SCANCODE_KP_2;
		case SDL_SCANCODE_KP_3:
			return SCANCODE_KP_3;
		case SDL_SCANCODE_KP_4:
			return SCANCODE_KP_4;
		case SDL_SCANCODE_KP_5:
			return SCANCODE_KP_5;
		case SDL_SCANCODE_KP_6:
			return SCANCODE_KP_6;
		case SDL_SCANCODE_KP_7:
			return SCANCODE_KP_7;
		case SDL_SCANCODE_KP_8:
			return SCANCODE_KP_8;
		case SDL_SCANCODE_KP_9:
			return SCANCODE_KP_9;
		case SDL_SCANCODE_KP_A:
			return SCANCODE_KP_A;
		case SDL_SCANCODE_KP_AMPERSAND:
			return SCANCODE_KP_AMPERSAND;
		case SDL_SCANCODE_KP_AT:
			return SCANCODE_KP_AT;
		case SDL_SCANCODE_KP_B:
			return SCANCODE_KP_B;
		case SDL_SCANCODE_KP_BACKSPACE:
			return SCANCODE_KP_BACKSPACE;
		case SDL_SCANCODE_KP_BINARY:
			return SCANCODE_KP_BINARY;
		case SDL_SCANCODE_KP_C:
			return SCANCODE_KP_C;
		case SDL_SCANCODE_KP_CLEAR:
			return SCANCODE_KP_CLEAR;
		case SDL_SCANCODE_KP_CLEARENTRY:
			return SCANCODE_KP_CLEARENTRY;
		case SDL_SCANCODE_KP_COLON:
			return SCANCODE_KP_COLON;
		case SDL_SCANCODE_KP_COMMA:
			return SCANCODE_KP_COMMA;
		case SDL_SCANCODE_KP_D:
			return SCANCODE_KP_D;
		case SDL_SCANCODE_KP_DBLAMPERSAND:
			return SCANCODE_KP_DBLAMPERSAND;
		case SDL_SCANCODE_KP_DBLVERTICALBAR:
			return SCANCODE_KP_DBLVERTICALBAR;
		case SDL_SCANCODE_KP_DECIMAL:
			return SCANCODE_KP_DECIMAL;
		case SDL_SCANCODE_KP_DIVIDE:
			return SCANCODE_KP_DIVIDE;
		case SDL_SCANCODE_KP_E:
			return SCANCODE_KP_E;
		case SDL_SCANCODE_KP_ENTER:
			return SCANCODE_KP_ENTER;
		case SDL_SCANCODE_KP_EQUALS:
			return SCANCODE_KP_EQUALS;
		case SDL_SCANCODE_KP_EQUALSAS400:
			return SCANCODE_KP_EQUALSAS400;
		case SDL_SCANCODE_KP_EXCLAM:
			return SCANCODE_KP_EXCLAM;
		case SDL_SCANCODE_KP_F:
			return SCANCODE_KP_F;
		case SDL_SCANCODE_KP_GREATER:
			return SCANCODE_KP_GREATER;
		case SDL_SCANCODE_KP_HASH:
			return SCANCODE_KP_HASH;
		case SDL_SCANCODE_KP_HEXADECIMAL:
			return SCANCODE_KP_HEXADECIMAL;
		case SDL_SCANCODE_KP_LEFTBRACE:
			return SCANCODE_KP_LEFTBRACE;
		case SDL_SCANCODE_KP_LEFTPAREN:
			return SCANCODE_KP_LEFTPAREN;
		case SDL_SCANCODE_KP_LESS:
			return SCANCODE_KP_LESS;
		case SDL_SCANCODE_KP_MEMADD:
			return SCANCODE_KP_MEMADD;
		case SDL_SCANCODE_KP_MEMCLEAR:
			return SCANCODE_KP_MEMCLEAR;
		case SDL_SCANCODE_KP_MEMDIVIDE:
			return SCANCODE_KP_MEMDIVIDE;
		case SDL_SCANCODE_KP_MEMMULTIPLY:
			return SCANCODE_KP_MEMMULTIPLY;
		case SDL_SCANCODE_KP_MEMRECALL:
			return SCANCODE_KP_MEMRECALL;
		case SDL_SCANCODE_KP_MEMSTORE:
			return SCANCODE_KP_MEMSTORE;
		case SDL_SCANCODE_KP_MEMSUBTRACT:
			return SCANCODE_KP_MEMSUBTRACT;
		case SDL_SCANCODE_KP_MINUS:
			return SCANCODE_KP_MINUS;
		case SDL_SCANCODE_KP_MULTIPLY:
			return SCANCODE_KP_MULTIPLY;
		case SDL_SCANCODE_KP_OCTAL:
			return SCANCODE_KP_OCTAL;
		case SDL_SCANCODE_KP_PERCENT:
			return SCANCODE_KP_PERCENT;
		case SDL_SCANCODE_KP_PERIOD:
			return SCANCODE_KP_PERIOD;
		case SDL_SCANCODE_KP_PLUS:
			return SCANCODE_KP_PLUS;
		case SDL_SCANCODE_KP_PLUSMINUS:
			return SCANCODE_KP_PLUSMINUS;
		case SDL_SCANCODE_KP_POWER:
			return SCANCODE_KP_POWER;
		case SDL_SCANCODE_KP_RIGHTBRACE:
			return SCANCODE_KP_RIGHTBRACE;
		case SDL_SCANCODE_KP_RIGHTPAREN:
			return SCANCODE_KP_RIGHTPAREN;
		case SDL_SCANCODE_KP_SPACE:
			return SCANCODE_KP_SPACE;
		case SDL_SCANCODE_KP_TAB:
			return SCANCODE_KP_TAB;
		case SDL_SCANCODE_KP_VERTICALBAR:
			return SCANCODE_KP_VERTICALBAR;
		case SDL_SCANCODE_KP_XOR:
			return SCANCODE_KP_XOR;
		case SDL_SCANCODE_L:
			return SCANCODE_L;
		case SDL_SCANCODE_LALT:
			return SCANCODE_LALT;
		case SDL_SCANCODE_LCTRL:
			return SCANCODE_LCTRL;
		case SDL_SCANCODE_LEFT:
			return SCANCODE_LEFT;
		case SDL_SCANCODE_LEFTBRACKET:
			return SCANCODE_LEFTBRACKET;
		case SDL_SCANCODE_LGUI:
			return SCANCODE_LGUI;
		case SDL_SCANCODE_LSHIFT:
			return SCANCODE_LSHIFT;
		case SDL_SCANCODE_M:
			return SCANCODE_M;
		case SDL_SCANCODE_MAIL:
			return SCANCODE_MAIL;
		case SDL_SCANCODE_MEDIASELECT:
			return SCANCODE_MEDIASELECT;
		case SDL_SCANCODE_MENU:
			return SCANCODE_MENU;
		case SDL_SCANCODE_MINUS:
			return SCANCODE_MINUS;
		case SDL_SCANCODE_MODE:
			return SCANCODE_MODE;
		case SDL_SCANCODE_MUTE:
			return SCANCODE_MUTE;
		case SDL_SCANCODE_N:
			return SCANCODE_N;
		case SDL_SCANCODE_NUMLOCKCLEAR:
			return SCANCODE_NUMLOCKCLEAR;
		case SDL_SCANCODE_O:
			return SCANCODE_O;
		case SDL_SCANCODE_OPER:
			return SCANCODE_OPER;
		case SDL_SCANCODE_OUT:
			return SCANCODE_OUT;
		case SDL_SCANCODE_P:
			return SCANCODE_P;
		case SDL_SCANCODE_PAGEDOWN:
			return SCANCODE_PAGEDOWN;
		case SDL_SCANCODE_PAGEUP:
			return SCANCODE_PAGEUP;
		case SDL_SCANCODE_PASTE:
			return SCANCODE_PASTE;
		case SDL_SCANCODE_PAUSE:
			return SCANCODE_PAUSE;
		case SDL_SCANCODE_PERIOD:
			return SCANCODE_PERIOD;
		case SDL_SCANCODE_POWER:
			return SCANCODE_POWER;
		case SDL_SCANCODE_PRINTSCREEN:
			return SCANCODE_PRINTSCREEN;
		case SDL_SCANCODE_PRIOR:
			return SCANCODE_PRIOR;
		case SDL_SCANCODE_Q:
			return SCANCODE_Q;
		case SDL_SCANCODE_R:
			return SCANCODE_R;
		case SDL_SCANCODE_RALT:
			return SCANCODE_RALT;
		case SDL_SCANCODE_RCTRL:
			return SCANCODE_RCTRL;
		case SDL_SCANCODE_RETURN:
			return SCANCODE_RETURN;
		case SDL_SCANCODE_RETURN2:
			return SCANCODE_RETURN2;
		case SDL_SCANCODE_RGUI:
			return SCANCODE_RGUI;
		case SDL_SCANCODE_RIGHT:
			return SCANCODE_RIGHT;
		case SDL_SCANCODE_RIGHTBRACKET:
			return SCANCODE_RIGHTBRACKET;
		case SDL_SCANCODE_RSHIFT:
			return SCANCODE_RSHIFT;
		case SDL_SCANCODE_S:
			return SCANCODE_S;
		case SDL_SCANCODE_SCROLLLOCK:
			return SCANCODE_SCROLLLOCK;
		case SDL_SCANCODE_SELECT:
			return SCANCODE_SELECT;
		case SDL_SCANCODE_SEMICOLON:
			return SCANCODE_SEMICOLON;
		case SDL_SCANCODE_SEPARATOR:
			return SCANCODE_SEPARATOR;
		case SDL_SCANCODE_SLASH:
			return SCANCODE_SLASH;
		case SDL_SCANCODE_SLEEP:
			return SCANCODE_SLEEP;
		case SDL_SCANCODE_SPACE:
			return SCANCODE_SPACE;
		case SDL_SCANCODE_STOP:
			return SCANCODE_STOP;
		case SDL_SCANCODE_SYSREQ:
			return SCANCODE_SYSREQ;
		case SDL_SCANCODE_T:
			return SCANCODE_T;
		case SDL_SCANCODE_TAB:
			return SCANCODE_TAB;
		case SDL_SCANCODE_THOUSANDSSEPARATOR:
			return SCANCODE_THOUSANDSSEPARATOR;
		case SDL_SCANCODE_U:
			return SCANCODE_U;
		case SDL_SCANCODE_UNDO:
			return SCANCODE_UNDO;
		case SDL_SCANCODE_UNKNOWN:
			return SCANCODE_UNKNOWN;
		case SDL_SCANCODE_UP:
			return SCANCODE_UP;
		case SDL_SCANCODE_V:
			return SCANCODE_V;
		case SDL_SCANCODE_VOLUMEDOWN:
			return SCANCODE_VOLUMEDOWN;
		case SDL_SCANCODE_VOLUMEUP:
			return SCANCODE_VOLUMEUP;
		case SDL_SCANCODE_W:
			return SCANCODE_W;
		case SDL_SCANCODE_WWW:
			return SCANCODE_WWW;
		case SDL_SCANCODE_X:
			return SCANCODE_X;
		case SDL_SCANCODE_Y:
			return SCANCODE_Y;
		case SDL_SCANCODE_Z:
			return SCANCODE_Z;
		case SDL_SCANCODE_INTERNATIONAL1:
			return SCANCODE_INTERNATIONAL1;
		case SDL_SCANCODE_INTERNATIONAL2:
			return SCANCODE_INTERNATIONAL2;
		case SDL_SCANCODE_INTERNATIONAL3:
			return SCANCODE_INTERNATIONAL3;
		case SDL_SCANCODE_INTERNATIONAL4:
			return SCANCODE_INTERNATIONAL4;
		case SDL_SCANCODE_INTERNATIONAL5:
			return SCANCODE_INTERNATIONAL5;
		case SDL_SCANCODE_INTERNATIONAL6:
			return SCANCODE_INTERNATIONAL6;
		case SDL_SCANCODE_INTERNATIONAL7:
			return SCANCODE_INTERNATIONAL7;
		case SDL_SCANCODE_INTERNATIONAL8:
			return SCANCODE_INTERNATIONAL8;
		case SDL_SCANCODE_INTERNATIONAL9:
			return SCANCODE_INTERNATIONAL9;
		case SDL_SCANCODE_LANG1:
			return SCANCODE_LANG1;
		case SDL_SCANCODE_LANG2:
			return SCANCODE_LANG2;
		case SDL_SCANCODE_LANG3:
			return SCANCODE_LANG3;
		case SDL_SCANCODE_LANG4:
			return SCANCODE_LANG4;
		case SDL_SCANCODE_LANG5:
			return SCANCODE_LANG5;
		case SDL_SCANCODE_LANG6:
			return SCANCODE_LANG6;
		case SDL_SCANCODE_LANG7:
			return SCANCODE_LANG7;
		case SDL_SCANCODE_LANG8:
			return SCANCODE_LANG8;
		case SDL_SCANCODE_LANG9:
			return SCANCODE_LANG9;
		case SDL_SCANCODE_NONUSBACKSLASH:
			return SCANCODE_NONUSBACKSLASH;
		case SDL_SCANCODE_NONUSHASH:
			return SCANCODE_NONUSHASH;
		
		default:
			return SCANCODE_UNKNOWN;
	}
}

KeyMod GraphicsEngine::convertSdlKeymod(const uint16 sdlKeymod)
{
	switch(sdlKeymod)
	{
		case KMOD_NONE:
			return KEYMOD_NONE;
		case KMOD_LSHIFT:
			return KEYMOD_LSHIFT;
		case KMOD_RSHIFT:
			return KEYMOD_RSHIFT;
		case KMOD_LCTRL:
			return KEYMOD_LCTRL;
		case KMOD_RCTRL:
			return KEYMOD_RCTRL;
		case KMOD_LALT:
			return KEYMOD_LALT;
		case KMOD_RALT:
			return KEYMOD_RALT;
		case KMOD_LGUI:
			return KEYMOD_LGUI;
		case KMOD_RGUI:
			return KEYMOD_RGUI;
		case KMOD_NUM:
			return KEYMOD_NUM;
		case KMOD_CAPS:
			return KEYMOD_CAPS;
		case KMOD_MODE:
			return KEYMOD_MODE;
		case KMOD_RESERVED:
			return KEYMOD_RESERVED;
		case KMOD_CTRL:
			return KEYMOD_CTRL;
		case KMOD_SHIFT:
			return KEYMOD_SHIFT;
		case KMOD_ALT:
			return KEYMOD_ALT;
		case KMOD_GUI:
			return KEYMOD_GUI;
		
		default:
			return KEYMOD_NONE;
	}
}

void GraphicsEngine::addEventListener(IEventListener* eventListener)
{
	if (eventListener == nullptr)
	{
		throw std::invalid_argument("IEventListener cannot be null.");
	}
	
	eventListeners_.push_back(eventListener);
}

void GraphicsEngine::removeEventListener(IEventListener* eventListener)
{
	auto it = std::find(eventListeners_.begin(), eventListeners_.end(), eventListener);
	
	if (it != eventListeners_.end())
	{
		eventListeners_.erase( it );
	}
}

}
}
}
