#include <algorithm>
#include <exception>
#include <stdexcept>
#include <system_error>

#include "graphics/custom/GraphicsEngine.hpp"

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "utilities/ImageLoader.hpp"

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
	
	auto vertexShaderUri = std::string("../data/shaders/basic_with_texture.vert");
	auto fragmentShaderUri = std::string("../data/shaders/basic_with_texture.frag");
	
	shaderProgram_ = createShaderProgram(vertexShaderUri, fragmentShaderUri);
	
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
	glEnable(GL_DEPTH_TEST);
	
	// Setup camera
	const glm::quat temp = glm::conjugate(camera_.orientation);

	view_ = glm::mat4_cast(temp);
	view_ = glm::translate(view_, glm::vec3(-camera_.position.x, -camera_.position.y, -camera_.position.z));
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shaderProgram_);
	
	/*
	{
		GLint numActiveUniforms = 0;
		glGetProgramInterfaceiv(shaderProgram_, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);
		
		std::vector<GLchar> nameData(256);
		for(int unif = 0; unif < numActiveUniforms; ++unif)
		{
			GLint arraySize = 0;
			GLenum type = 0;
			GLsizei actualLength = 0;
			glGetActiveUniform(shaderProgram_, unif, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
			std::string name((char*)&nameData[0], actualLength);
		}
	}
	*/
	
	const int modelMatrixLocation = glGetUniformLocation(shaderProgram_, "modelMatrix");
	const int pvmMatrixLocation = glGetUniformLocation(shaderProgram_, "pvmMatrix");
	const int normalMatrixLocation = glGetUniformLocation(shaderProgram_, "normalMatrix");
	
	//assert( modelMatrixLocation >= 0);
	assert( pvmMatrixLocation >= 0);
	//assert( normalMatrixLocation >= 0);
	
	uint32 i = 0;	
	for ( const auto& r : renderables_ )
	{
		const auto& graphicsData = graphicsData_[i];
		
		glm::mat4 newModel = glm::translate(model_, graphicsData.position);
		newModel = newModel * glm::mat4_cast( graphicsData.orientation );
		newModel = glm::scale(newModel, graphicsData.scale);
	
		// Send uniform variable values to the shader		
		const glm::mat4 pvmMatrix(projection_ * view_ * newModel);
		glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);
	
		glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(view_ * newModel)));
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &newModel[0][0]);
		
		if (r.ubo.id > 0)
		{
			//const int bonesLocation = glGetUniformLocation(shaderProgram_, "bones");
			//assert( bonesLocation >= 0);
			//glBindBufferBase(GL_UNIFORM_BUFFER, bonesLocation, r.ubo.id);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, r.ubo.id);
		}
		
		glBindTexture(GL_TEXTURE_2D, r.texture.id);
		glBindVertexArray(r.vao.id);
		glDrawElements(r.vao.ebo.mode, r.vao.ebo.count, r.vao.ebo.type, 0);
		glBindVertexArray(0);
		
		i++;
	}
	
	SDL_GL_SwapWindow( sdlWindow_ );
}

CameraHandle GraphicsEngine::createCamera(const glm::vec3& position, const glm::vec3& lookAt)
{
	camera_ = Camera();
	camera_.position = position;
	camera_.orientation = glm::quat();
	//camera_.orientation = glm::normalize(camera_.orientation);
	
	CameraHandle cameraHandle = CameraHandle(0);
	
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
	Vao vao;
	
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
	
	vertexArrayObjects_.push_back(vao);
	auto index = vertexArrayObjects_.size() - 1;
	
	return MeshHandle(index);
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
	Vao vao;
	
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
	
	vertexArrayObjects_.push_back(vao);
	auto index = vertexArrayObjects_.size() - 1;
	
	return MeshHandle(index);
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
	Vao vao;
	
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
	
	return MeshHandle(index);
	*/
	return MeshHandle();
}

SkeletonHandle GraphicsEngine::createSkeleton(const uint32 numberOfBones)
{
	Ubo ubo;
	
	glGenBuffers(1, &ubo.id);
	
	auto size = numberOfBones * sizeof(glm::mat4);
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
	
	uniformBufferObjects_.push_back(ubo);
	auto index = uniformBufferObjects_.size() - 1;
	
	return SkeletonHandle(index);
}

TextureHandle GraphicsEngine::createTexture2d(const utilities::Image& image)
{
	GlTexture2d texture;
	
	uint32 width = 0;
	uint32 height = 0;
	
	auto format = utilities::getOpenGlImageFormat(image.format);
	
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, &image.data[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	texture2dObjects_.push_back(texture);
	auto index = texture2dObjects_.size() - 1;
	
	return TextureHandle(index);
}

RenderableHandle GraphicsEngine::createRenderable(const MeshHandle& meshHandle, const TextureHandle& textureHandle)
{
	Renderable renderable;
	
	renderable.vao = vertexArrayObjects_[meshHandle.getId()];
	renderable.texture = texture2dObjects_[textureHandle.getId()];
	
	renderables_.push_back(renderable);
	auto index = renderables_.size() - 1;
	
	GraphicsData graphicsData;
	graphicsData.position = glm::vec3(0.0f, 0.0f, 0.0f);
	graphicsData.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	graphicsData.orientation = glm::quat();
	graphicsData_.push_back(graphicsData);
	
	return RenderableHandle(index);
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
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			graphicsData.orientation = graphicsData.orientation * glm::normalize( quaternion );
			break;
		
		case TransformSpace::TS_WORLD:
			graphicsData.orientation =  glm::normalize( quaternion ) * graphicsData.orientation;
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
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			graphicsData.orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) ) * graphicsData.orientation;
			break;
		
		case TransformSpace::TS_WORLD:
			graphicsData.orientation =  graphicsData.orientation * glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
			break;
			
		default:
			std::string message = std::string("Invalid TransformSpace type.");
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::translate(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z)
{
	camera_.position += glm::vec3(x, y, z);
}

void GraphicsEngine::translate(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.position += glm::vec3(x, y, z);
}

void GraphicsEngine::translate(const CameraHandle& cameraHandle, const glm::vec3& trans)
{
	camera_.position += trans;
}

void GraphicsEngine::translate(const RenderableHandle& renderableHandle, const glm::vec3& trans)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.position += trans;
}

void GraphicsEngine::scale(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = glm::vec3(x, y, z);
}

void GraphicsEngine::scale(const RenderableHandle& renderableHandle, const glm::vec3& scale)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = scale;
}

void GraphicsEngine::scale(const RenderableHandle& renderableHandle, const float32 scale)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = glm::vec3(scale, scale, scale);
}

void GraphicsEngine::position(const RenderableHandle& renderableHandle, const float32 x, const float32 y, const float32 z)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.position = glm::vec3(x, y, z);
}

void GraphicsEngine::position(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z)
{
	camera_.position = glm::vec3(x, y, z);
}

void GraphicsEngine::position(const RenderableHandle& renderableHandle, const glm::vec3& position)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = position;
}

void GraphicsEngine::position(const CameraHandle& cameraHandle, const glm::vec3& position)
{
	camera_.position = position;
}

void GraphicsEngine::lookAt(const RenderableHandle& renderableHandle, const glm::vec3& lookAt)
{
	const auto id = renderableHandle.getId();
	
	auto& graphicsData = graphicsData_[id];
	
	assert(lookAt != graphicsData.position);
	
	const glm::mat4 lookAtMatrix = glm::lookAt(graphicsData.position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	graphicsData.orientation =  glm::normalize( graphicsData.orientation * glm::quat_cast( lookAtMatrix ) );
}

void GraphicsEngine::lookAt(const CameraHandle& cameraHandle, const glm::vec3& lookAt)
{
	assert(lookAt != camera_.position);
	
	const glm::mat4 lookAtMatrix = glm::lookAt(camera_.position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	camera_.orientation =  glm::normalize( camera_.orientation * glm::quat_cast( lookAtMatrix ) );
}

void GraphicsEngine::assign(const RenderableHandle& renderableHandle, const SkeletonHandle& skeletonHandle)
{
	auto& renderable = renderables_[renderableHandle.getId()];
	
	renderable.ubo = uniformBufferObjects_[skeletonHandle.getId()];
}

void GraphicsEngine::update(const SkeletonHandle& skeletonHandle, const void* data, const uint32 size)
{
	const auto& ubo = uniformBufferObjects_[skeletonHandle.getId()];
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	
	//void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	//memcpy( d, data, size );
	//glUnmapBuffer(GL_UNIFORM_BUFFER);
}

GLuint GraphicsEngine::createShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	auto vertexShaderSource = fileSystem_->readAll(vertexShaderFile);
	auto fragmentShaderSource = fileSystem_->readAll(fragmentShaderFile);
	
	return createShaderProgramFromSource(vertexShaderSource, fragmentShaderSource);
}

GLuint GraphicsEngine::createShaderProgramFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	auto vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
	auto fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	
	GLuint shaderProgram = 0;
	
	try
	{
		shaderProgram = createShaderProgram(vertexShader, fragmentShader);
	}
	catch (const std::exception& e)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	
		throw e;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return shaderProgram;
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
	key.mod = keySym.mod;
	
	return key;
}

KeyCode GraphicsEngine::convertSdlKeycode(const SDL_Keycode& sdlKeycode)
{
	switch(sdlKeycode)
	{
		case SDLK_UNKNOWN:
			return KEY_UNKNOWN;
			
		case SDLK_ESCAPE:
			return KEY_ESCAPE;
		
		default:
			return KEY_UNKNOWN;
	}
}

ScanCode GraphicsEngine::convertSdlScancode(const SDL_Scancode& sdlScancode)
{
	switch(sdlScancode)
	{
		case SDL_SCANCODE_UNKNOWN:
			return SCANCODE_UNKNOWN;
			
		case SDL_SCANCODE_ESCAPE:
			return SCANCODE_ESCAPE;
		
		default:
			return SCANCODE_UNKNOWN;
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
