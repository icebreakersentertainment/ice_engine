#include <exception>
#include <stdexcept>
#include <system_error>

#include "graphics/GraphicsEngine.hpp"

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "logger/Logger.hpp"
#include "utilities/ImageLoader.hpp"

namespace graphics
{

GraphicsEngine::GraphicsEngine(glm::detail::uint32 width, glm::detail::uint32 height, fs::IFileSystem* fileSystem)
{
	width_ = width;
	height_ = height;
	
	fileSystem_ = fileSystem;
	
	auto errorCode = SDL_Init( SDL_INIT_VIDEO );
	
	if (errorCode != 0)
	{
		auto message = std::string("Unable to initialize SDL: ") + SDL_GetError();
		LOG_ERROR(message);
		throw std::runtime_error(message);
	}
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	
	sdlWindow_ = SDL_CreateWindow("Dark Horizon", 50, 50, width_, height_, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	if (sdlWindow_ == nullptr)
	{
		auto message = std::string("Unable to create window: ") + SDL_GetError();
		LOG_ERROR(message);
		throw std::runtime_error(message);
	}

	openglContext_ = SDL_GL_CreateContext( sdlWindow_ );
	
	if (openglContext_ == nullptr)
	{
		auto message = std::string("Unable to create OpenGL context: ") + SDL_GetError();
		LOG_ERROR(message);
		throw std::runtime_error(message);
	}
	
	glewExperimental = GL_TRUE; // Needed in core profile
	
	auto result = glewInit();
	
	if (result != GLEW_OK)
	{
		auto msg = std::string("Failed to initialize GLEW.");
		LOG_ERROR( msg );
		throw std::runtime_error(msg);
	}
	
	auto vertexShaderUri = std::string("../data/shaders/testing.vert");
	auto fragmentShaderUri = std::string("../data/shaders/testing.frag");
	
	shaderProgram_ = createShaderProgram(vertexShaderUri, fragmentShaderUri);
	
	// Set up the model, view, and projection matrices
	model_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	view_ = glm::mat4(1.0f);
	setViewport(width, height);
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
	
void GraphicsEngine::setViewport(glm::detail::uint32 width, glm::detail::uint32 height)
{
	width_ = width;
	height_ = height;
	
	projection_ = glm::perspective(glm::radians(60.0f), (glm::detail::float32)width / (glm::detail::float32)height, 0.1f, 500.f);
	
	glViewport(0, 0, width_, height_);
}

void GraphicsEngine::render(glm::detail::float32 delta)
{
	glEnable(GL_DEPTH_TEST);
	
	// Setup camera
	const glm::quat temp = glm::conjugate(camera_.orientation);

	view_ = glm::mat4_cast(temp);
	view_ = glm::translate(view_, glm::vec3(-camera_.position.x, -camera_.position.y, -camera_.position.z));
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shaderProgram_);
	
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
		  
		  std::cout << name << std::endl;
		}
	}
	
	const int modelMatrixLocation = glGetUniformLocation(shaderProgram_, "modelMatrix");
	const int pvmMatrixLocation = glGetUniformLocation(shaderProgram_, "pvmMatrix");
	const int normalMatrixLocation = glGetUniformLocation(shaderProgram_, "normalMatrix");
	
	//assert( modelMatrixLocation >= 0);
	assert( pvmMatrixLocation >= 0);
	//assert( normalMatrixLocation >= 0);
	
	glm::detail::uint32 i = 0;	
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

CameraId GraphicsEngine::createCamera(glm::vec3 position, glm::vec3 lookAt)
{
	camera_ = Camera();
	camera_.position = position;
	camera_.orientation = glm::quat();
	//camera_.orientation = glm::normalize(camera_.orientation);
	
	auto cameraId = CameraId(0);
	
	this->lookAt(cameraId, lookAt);
	
	return cameraId;
}

MeshId GraphicsEngine::createStaticMesh(
	std::vector<glm::vec3> vertices,
	std::vector<glm::detail::uint32> indices,
	std::vector<glm::vec4> colors,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> textureCoordinates
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::detail::uint32), &indices[0], GL_STATIC_DRAW); 
	
	glBindVertexArray(0);
	
	vao.ebo.count = indices.size();
	vao.ebo.mode = GL_TRIANGLES;
	vao.ebo.type =  GL_UNSIGNED_INT;
	
	vertexArrayObjects_.push_back(vao);
	auto index = vertexArrayObjects_.size() - 1;
	
	return MeshId(index);
}

MeshId GraphicsEngine::createAnimatedMesh(
		std::vector<glm::vec3> vertices,
		std::vector<glm::detail::uint32> indices,
		std::vector<glm::vec4> colors,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> textureCoordinates,
		std::vector<glm::ivec4> boneIds,
		std::vector<glm::vec4> boneWeights
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::detail::uint32), &indices[0], GL_STATIC_DRAW); 
	
	glBindVertexArray(0);
	
	vao.ebo.count = indices.size();
	vao.ebo.mode = GL_TRIANGLES;
	vao.ebo.type =  GL_UNSIGNED_INT;
	
	vertexArrayObjects_.push_back(vao);
	auto index = vertexArrayObjects_.size() - 1;
	
	return MeshId(index);
}

MeshId GraphicsEngine::createDynamicMesh(
	std::vector<glm::vec3> vertices,
	std::vector<glm::detail::uint32> indices,
	std::vector<glm::vec4> colors,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> textureCoordinates
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::detail::uint32), &indices[0], GL_STATIC_DRAW); 
	
	glBindVertexArray(0);
	
	vao.ebo.count = indices.size();
	vao.ebo.mode = GL_TRIANGLES;
	vao.ebo.type =  GL_UNSIGNED_INT;
	
	vertexArrayObjects_.push_back(vao);
	auto index = vertexArrayObjects_.size() - 1;
	
	return MeshId(index);
	*/
	return MeshId();
}

SkeletonId GraphicsEngine::createSkeleton(glm::detail::uint32 numberOfBones)
{
	Ubo ubo;
	
	glGenBuffers(1, &ubo.id);
	
	auto size = numberOfBones * sizeof(glm::mat4);
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
	
	uniformBufferObjects_.push_back(ubo);
	auto index = uniformBufferObjects_.size() - 1;
	
	return SkeletonId(index);
}

TextureId GraphicsEngine::createTexture2d(std::string uri)
{
	GlTexture2d texture;
	
	glm::detail::uint32 width = 0;
	glm::detail::uint32 height = 0;
	
	auto il = utilities::ImageLoader();
	auto image = il.loadImageData(uri);
	
	auto format = utilities::getOpenGlImageFormat(image->format);
	
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, &image->data[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	texture2dObjects_.push_back(texture);
	auto index = texture2dObjects_.size() - 1;
	
	return TextureId(index);
}

RenderableId GraphicsEngine::createRenderable(MeshId meshId, TextureId textureId)
{
	Renderable renderable;
	
	renderable.vao = vertexArrayObjects_[meshId.getId()];
	renderable.texture = texture2dObjects_[textureId.getId()];
	
	renderables_.push_back(renderable);
	auto index = renderables_.size() - 1;
	
	GraphicsData graphicsData;
	graphicsData.position = glm::vec3(0.0f, 0.0f, 0.0f);
	graphicsData.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	graphicsData.orientation = glm::quat();
	graphicsData_.push_back(graphicsData);
	
	return RenderableId(index);
}

void GraphicsEngine::rotate(const CameraId cameraId, const glm::quat& quaternion, TransformSpace relativeTo)
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
			LOG_ERROR( message );
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotate(const RenderableId renderableId, const glm::quat& quaternion, TransformSpace relativeTo)
{
	const auto id = renderableId.getId();
	
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
			LOG_ERROR( message );
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotate(const CameraId cameraId, const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo)
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
			LOG_ERROR( message );
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotate(const RenderableId renderableId, const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo)
{
	const auto id = renderableId.getId();
	
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
			LOG_ERROR( message );
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::translate(const CameraId cameraId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z)
{
	camera_.position += glm::vec3(x, y, z);
}

void GraphicsEngine::translate(const RenderableId renderableId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.position += glm::vec3(x, y, z);
}

void GraphicsEngine::translate(const CameraId cameraId, const glm::vec3& trans)
{
	camera_.position += trans;
}

void GraphicsEngine::translate(const RenderableId renderableId, const glm::vec3& trans)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.position += trans;
}


void GraphicsEngine::scale(const RenderableId renderableId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = glm::vec3(x, y, z);
}

void GraphicsEngine::scale(const RenderableId renderableId, const glm::vec3& scale)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = scale;
}

void GraphicsEngine::scale(const RenderableId renderableId, const glm::detail::float32 scale)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = glm::vec3(scale, scale, scale);
}

void GraphicsEngine::position(const RenderableId renderableId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.position = glm::vec3(x, y, z);
}

void GraphicsEngine::position(const CameraId cameraId, const glm::detail::float32 x, const glm::detail::float32 y, const glm::detail::float32 z)
{
	camera_.position = glm::vec3(x, y, z);
}

void GraphicsEngine::position(const RenderableId renderableId, const glm::vec3& position)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = position;
}

void GraphicsEngine::position(const CameraId cameraId, const glm::vec3& position)
{
	camera_.position = position;
}


void GraphicsEngine::lookAt(const RenderableId renderableId, const glm::vec3& lookAt)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	
	assert(lookAt != graphicsData.position);
	
	const glm::mat4 lookAtMatrix = glm::lookAt(graphicsData.position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	graphicsData.orientation =  glm::normalize( graphicsData.orientation * glm::quat_cast( lookAtMatrix ) );
}

void GraphicsEngine::lookAt(const CameraId cameraId, const glm::vec3& lookAt)
{
	
	assert(lookAt != camera_.position);
	
	const glm::mat4 lookAtMatrix = glm::lookAt(camera_.position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	camera_.orientation =  glm::normalize( camera_.orientation * glm::quat_cast( lookAtMatrix ) );
}

void GraphicsEngine::assign(const RenderableId renderableId, const SkeletonId skeletonId)
{
	auto& renderable = renderables_[renderableId.getId()];
	
	renderable.ubo = uniformBufferObjects_[skeletonId.getId()];
}

void GraphicsEngine::update(const SkeletonId skeletonId, const void* data, glm::detail::uint32 size)
{
	const auto& ubo = uniformBufferObjects_[skeletonId.getId()];
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	
	//void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	//memcpy( d, data, size );
	//glUnmapBuffer(GL_UNIFORM_BUFFER);
}

GLuint GraphicsEngine::createShaderProgram(std::string vertexShaderUri, std::string fragmentShaderUri)
{
	auto vertexShaderSource = fileSystem_->readAll(vertexShaderUri);
	auto fragmentShaderSource = fileSystem_->readAll(fragmentShaderUri);
	
	return createShaderProgramFromSource(vertexShaderSource, fragmentShaderSource);
}

GLuint GraphicsEngine::createShaderProgramFromSource(std::string vertexShaderSource, std::string fragmentShaderSource)
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

GLuint GraphicsEngine::createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
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
	
			LOG_ERROR( message.str() );
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
		
				LOG_WARN( message.str() );
			}
		}
	}
	
	return shaderProgram;
}

GLuint GraphicsEngine::compileShader(std::string source, GLenum type)
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
	
			LOG_ERROR( message.str() );
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
		
				LOG_WARN( message.str() );
			}
		}
	}
	
	return shader;
}

std::string GraphicsEngine::getShaderErrorMessage(GLuint shader)
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

std::string GraphicsEngine::getShaderProgramErrorMessage(GLuint shaderProgram)
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

void GraphicsEngine::setMouseRelativeMode(bool enabled)
{
	auto result = SDL_SetRelativeMouseMode(static_cast<SDL_bool>(enabled));
	
	if (result != 0)
	{
		auto message = std::string("Unable to set relative mouse mode: ") + SDL_GetError();
		LOG_ERROR(message);
		throw std::runtime_error(message);
	}
}

void GraphicsEngine::setCursorVisible(bool visible)
{
	auto toggle = (visible ? SDL_ENABLE : SDL_DISABLE);
	auto result = SDL_ShowCursor(toggle);
	
	if (result < 0)
	{
		auto message = std::string("Unable to set mouse cursor visible\\invisible: ") + SDL_GetError();
		LOG_ERROR(message);
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
			LOG_DEBUG( "Unrecognized SDL event " << event.type);
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
