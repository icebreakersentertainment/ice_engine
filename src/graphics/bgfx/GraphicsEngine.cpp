#include <array>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <system_error>

#include "graphics/bgfx/GraphicsEngine.hpp"

#include <SDL2/SDL_syswm.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <bx/crtimpl.h>

#include "utilities/ImageLoader.hpp"
#include "Platform.hpp"

namespace hercules
{
namespace graphics
{
namespace bgfx
{

struct BgfxCallbackImplementation : public ::bgfx::CallbackI
{
	
	
	BgfxCallbackImplementation(logger::ILogger* logger) : logger_(logger)
	{

	}
	
	virtual void fatal(::bgfx::Fatal::Enum _code, const char *_str) override
	{
		auto code = std::string();
		
		switch (_code)
		{
			case ::bgfx::Fatal::Enum::DebugCheck:
				code = std::string("Debug Check");
				break;
			
			case ::bgfx::Fatal::Enum::InvalidShader:
				code = std::string("Invalid Shader");
				break;
			
			case ::bgfx::Fatal::Enum::UnableToInitialize:
				code = std::string("Unable To Initialize");
				break;
			
			case ::bgfx::Fatal::Enum::UnableToCreateTexture:
				code = std::string("Unable To Create Texture");
				break;
			
			case ::bgfx::Fatal::Enum::DeviceLost:
				code = std::string("Device Lost");
				break;
			
			case ::bgfx::Fatal::Enum::Count:
				code = std::string("Count");
				break;
			
			default:
				code = std::string("Unknown");
				break;
		}
		
		logger_->fatal(code + ": " + _str);
	}
	
	virtual void traceVargs(const char *_filePath, uint16_t _line, const char *_format, va_list _argList) override
	{
		// Hopefully this buffer is big enough...
		char buffer[2048];
		
		sprintf(buffer, _format, _filePath, _line, _argList);
		
		logger_->fatal(buffer);
	}
	
	virtual uint32_t cacheReadSize(uint64_t _id) override
	{
		return 0;
	}
	
	virtual bool cacheRead(uint64_t _id, void *_data, uint32_t _size) override
	{
		return false;
	}
	
	virtual void cacheWrite(uint64_t _id, const void *_data, uint32_t _size) override
	{
		
	}
	
	virtual void screenShot(const char *_filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void *_data, uint32_t _size, bool _yflip) override
	{
		
	}
	
	virtual void captureBegin(uint32_t _width, uint32_t _height, uint32_t _pitch, ::bgfx::TextureFormat::Enum _format, bool _yflip) override
	{
		
	}
	
	virtual void captureEnd() override
	{
		
	}
	
	virtual void captureFrame(const void *_data, uint32_t _size) override
	{
		
	}

private:
	logger::ILogger* logger_;
};

GraphicsEngine::GraphicsEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger)
{
	properties_ = properties;
	fileSystem_ = fileSystem;
	logger_ = logger;
	
	width_ = properties->getIntValue(std::string("window.width"), 1024);
	height_ = properties->getIntValue(std::string("window.height"), 768);
	
	logger_->info(std::string("Width and height set to ") + std::to_string(width_) + " x " + std::to_string(height_));
	
	auto errorCode = SDL_Init(SDL_INIT_EVENTS);
	
	if (errorCode != 0)
	{
		auto message = std::string("Unable to initialize SDL: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
	
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	//SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	
	auto windowTitle = properties_->getStringValue("window.title", "Hercules");
	
	sdlWindow_ = SDL_CreateWindow(windowTitle.c_str(), 50, 50, width_, height_, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	if (sdlWindow_ == nullptr)
	{
		auto message = std::string("Unable to create window: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
	
	// TODO: fix
	auto bgfxCallbackImplementation = new BgfxCallbackImplementation(logger_);

	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(sdlWindow_, &wmi) )
	{
		auto message = std::string("Unable to get window information: ") + SDL_GetError();
		throw std::runtime_error(message);
	}

	::bgfx::PlatformData pd;
#if defined(PLATFORM_LINUX)
	pd.ndt          = wmi.info.x11.display;
	pd.nwh          = (void*)(uintptr_t)wmi.info.x11.window;
#elif defined(PLATFORM_MAC)
	pd.ndt          = nullptr;
	pd.nwh          = wmi.info.cocoa.window;
#elif defined(PLATFORM_WINDOWS)
	pd.ndt          = nullptr;
	pd.nwh          = wmi.info.win.window;
#endif
	pd.context      = nullptr;
	pd.backBuffer   = nullptr;
	pd.backBufferDS = nullptr;
	
	::bgfx::setPlatformData(pd);
	
	auto bgfxInitResult = ::bgfx::init(::bgfx::RendererType::Count, BGFX_PCI_ID_NONE, 0, bgfxCallbackImplementation);
	
	if (bgfxInitResult == false)
	{
		auto message = std::string("Unable to initialize bgfx.");
		throw std::runtime_error(message);
	}
	
	switch (::bgfx::getRendererType())
	{
		case ::bgfx::RendererType::Noop:
			logger_->info("Render type: Noop");
			break;
			
		case ::bgfx::RendererType::Direct3D9:
			logger_->info("Render type: Direct3D9");
			break;
			
		case ::bgfx::RendererType::Direct3D11:
			logger_->info("Render type: Direct3D11");
			break;
			
		case ::bgfx::RendererType::Direct3D12:
			logger_->info("Render type: Direct3D12");
			break;
			
		case ::bgfx::RendererType::Gnm:
			logger_->info("Render type: Gnm");
			break;
			
		case ::bgfx::RendererType::Metal:
			logger_->info("Render type: Metal");
			break;
			
		case ::bgfx::RendererType::OpenGL:
			logger_->info("Render type: OpenGL");
			break;
			
		case ::bgfx::RendererType::OpenGLES:
			logger_->info("Render type: OpenGLES");
			break;
			
		case ::bgfx::RendererType::Vulkan:
			logger_->info("Render type: Vulkan");
			break;
	
		case ::bgfx::RendererType::Count:
			logger_->info("Render type: Count");
			logger_->warn("Render type should not be Count!");
			break;
	}

	::bgfx::setDebug(BGFX_DEBUG_TEXT);

	auto vertexShaderFile = std::string("../data/shaders/basic.vs.sc");
	auto fragmentShaderFile = std::string("../data/shaders/basic.fs.sc");
	
	shaderProgram_ = createShaderProgram(vertexShaderFile, fragmentShaderFile);
	
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
	if (sdlWindow_)
	{
		SDL_SetWindowFullscreen( sdlWindow_, 0 );
		SDL_DestroyWindow( sdlWindow_ );
		sdlWindow_ = nullptr;
	}

	for ( const auto& r : renderables_ )
	{
		::bgfx::destroyVertexBuffer(r.vao.vbh);
		::bgfx::destroyIndexBuffer(r.vao.ibh);
	}
	
	::bgfx::destroyProgram(shaderProgram_);

	::bgfx::shutdown();
	
	SDL_Quit();
}
	
void GraphicsEngine::setViewport(const uint32 width, const uint32 height)
{
	width_ = width;
	height_ = height;
	
	SDL_SetWindowSize(sdlWindow_, width, height);
	
	projection_ = glm::perspective(glm::radians(60.0f), (float32)width / (float32)height, 0.1f, 500.f);
	
	//glViewport(0, 0, width_, height_);
	
	::bgfx::reset(width_, height_, BGFX_RESET_NONE);
	
	::bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
}

void GraphicsEngine::render(const float32 delta)
{
	/*
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
		}
	}
	
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
	*/
	
	// Use debug font to print information about this example.
	::bgfx::dbgTextClear();
	
	::bgfx::dbgTextPrintf(0, 1, 0x4f, "testing");
	::bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

	::bgfx::dbgTextPrintf(0, 4, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");

	const auto* stats = ::bgfx::getStats();
	::bgfx::dbgTextPrintf(0, 6, 0x0f, "Backbuffer %dW x %dH in pixels.",
		stats->width,
		stats->height
	);
	
	// Set view matrix
	const glm::quat temp = glm::conjugate(camera_.orientation);

	view_ = glm::mat4_cast(temp);
	view_ = glm::translate(view_, glm::vec3(-camera_.position.x, -camera_.position.y, -camera_.position.z));
	
	::bgfx::setViewTransform(0, glm::value_ptr(view_), glm::value_ptr(projection_));
	
	// Set view 0 default viewport.
	::bgfx::setViewRect(0, 0, 0, width_, height_);

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	::bgfx::touch(0);
	
	// Render stuff here
	uint32 i = 0;	
	for ( const auto& r : renderables_ )
	{
		const auto& graphicsData = graphicsData_[i];
		
		glm::mat4 newModel = glm::translate(model_, graphicsData.position);
		newModel = newModel * glm::mat4_cast( graphicsData.orientation );
		newModel = glm::scale(newModel, graphicsData.scale);
		
		::bgfx::setVertexBuffer(r.vao.vbh);
		::bgfx::setIndexBuffer(r.vao.ibh);
		
		::bgfx::setTransform(glm::value_ptr(newModel));
		
		::bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);
		
		::bgfx::submit(0, shaderProgram_);
		
		i++;
	}
	
	::bgfx::frame();
}

CameraId GraphicsEngine::createCamera(const glm::vec3& position, const glm::vec3& lookAt)
{
	camera_ = Camera();
	camera_.position = position;
	camera_.orientation = glm::quat();
	//camera_.orientation = glm::normalize(camera_.orientation);
	
	auto cameraId = CameraId(0);
	
	this->lookAt(cameraId, lookAt);
	
	return cameraId;
}

::bgfx::VertexDecl vertexDeclaration;
MeshId GraphicsEngine::createStaticMesh(
	const std::vector<glm::vec3>& vertices,
	const std::vector<uint32>& indices,
	const std::vector<glm::vec4>& colors,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& textureCoordinates
)
{
	vertexDeclaration
		.begin()
		.add(::bgfx::Attrib::Position, 3, ::bgfx::AttribType::Float)
		.add(::bgfx::Attrib::Color0, 4, ::bgfx::AttribType::Uint8, true)
		.end();
	
	Vao vao;
	
	vao.vbh = ::bgfx::createVertexBuffer(
		::bgfx::copy(&vertices[0], (vertices.size() * sizeof(glm::vec3))),
		vertexDeclaration
	);
	
	vao.ibh = ::bgfx::createIndexBuffer(
		::bgfx::copy(&indices[0], (indices.size() * sizeof(uint32)))
	);
	
	vertexArrayObjects_.push_back(vao);
	auto index = vertexArrayObjects_.size() - 1;
	
	return MeshId(index);
	
	/*
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
	
	return MeshId(index);
	*/
}

MeshId GraphicsEngine::createAnimatedMesh(
		const std::vector<glm::vec3>& vertices,
		const std::vector<uint32>& indices,
		const std::vector<glm::vec4>& colors,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& textureCoordinates,
		const std::vector<glm::ivec4>& boneIds,
		const std::vector<glm::vec4>& boneWeights
	)
{
	/*
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
	
	return MeshId(index);
	*/
	
	return MeshId();
}

MeshId GraphicsEngine::createDynamicMesh(
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
	
	return MeshId(index);
	*/
	return MeshId();
}

SkeletonId GraphicsEngine::createSkeleton(const uint32 numberOfBones)
{
	/*
	Ubo ubo;
	
	glGenBuffers(1, &ubo.id);
	
	auto size = numberOfBones * sizeof(glm::mat4);
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
	
	uniformBufferObjects_.push_back(ubo);
	auto index = uniformBufferObjects_.size() - 1;
	
	return SkeletonId(index);
	*/
	
	return SkeletonId();
}

TextureId GraphicsEngine::createTexture2d(const std::string& uri)
{
	GlTexture2d texture;
	
	texture2dObjects_.push_back(texture);
	auto index = texture2dObjects_.size() - 1;
	
	return TextureId(index);
	
	/*
	GlTexture2d texture;
	
	uint32 width = 0;
	uint32 height = 0;
	
	auto il = utilities::ImageLoader(logger_);
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
	*/
}

RenderableId GraphicsEngine::createRenderable(const MeshId meshId, const TextureId textureId)
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

void GraphicsEngine::rotate(const CameraId cameraId, const glm::quat& quaternion, const TransformSpace& relativeTo)
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

void GraphicsEngine::rotate(const RenderableId renderableId, const glm::quat& quaternion, const TransformSpace& relativeTo)
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
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::rotate(const CameraId cameraId, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo)
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

void GraphicsEngine::rotate(const RenderableId renderableId, const float32 degrees, const glm::vec3& axis, const TransformSpace& relativeTo)
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
			throw std::runtime_error(message);
	}
}

void GraphicsEngine::translate(const CameraId cameraId, const float32 x, const float32 y, const float32 z)
{
	camera_.position += glm::vec3(x, y, z);
}

void GraphicsEngine::translate(const RenderableId renderableId, const float32 x, const float32 y, const float32 z)
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


void GraphicsEngine::scale(const RenderableId renderableId, const float32 x, const float32 y, const float32 z)
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

void GraphicsEngine::scale(const RenderableId renderableId, const float32 scale)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.scale = glm::vec3(scale, scale, scale);
}

void GraphicsEngine::position(const RenderableId renderableId, const float32 x, const float32 y, const float32 z)
{
	const auto id = renderableId.getId();
	
	auto& graphicsData = graphicsData_[id];
	graphicsData.position = glm::vec3(x, y, z);
}

void GraphicsEngine::position(const CameraId cameraId, const float32 x, const float32 y, const float32 z)
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
	/*
	auto& renderable = renderables_[renderableId.getId()];
	
	renderable.ubo = uniformBufferObjects_[skeletonId.getId()];
	*/
}

void GraphicsEngine::update(const SkeletonId skeletonId, const void* data, const uint32 size)
{
	/*
	const auto& ubo = uniformBufferObjects_[skeletonId.getId()];
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	
	//void* d = glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	//memcpy( d, data, size );
	//glUnmapBuffer(GL_UNIFORM_BUFFER);
	*/
}

::bgfx::ProgramHandle GraphicsEngine::createShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	const auto vertexShaderOutputFile = compileShaderToFile(vertexShaderFile, ShaderType::VERTEX);
	const auto fragmentShaderOutputFile = compileShaderToFile(fragmentShaderFile, ShaderType::FRAGMENT);
	
	const auto vertexShaderSource = fileSystem_->readAll(vertexShaderOutputFile, true);
	const auto fragmentShaderSource = fileSystem_->readAll(fragmentShaderOutputFile, true);
	
	return createShaderProgramFromSource(vertexShaderSource, fragmentShaderSource);
	
	/*
	auto vertexShaderSource = fileSystem_->readAll(vertexShaderUri);
	auto fragmentShaderSource = fileSystem_->readAll(fragmentShaderUri);
	
	return createShaderProgramFromSource(vertexShaderSource, fragmentShaderSource);
	*/
}

std::string GraphicsEngine::compileShaderToFile(const std::string& shaderFile, const ShaderType shaderType, const std::vector<std::string>& includeDirectories) const
{
	logger_->info("Compiling vertex shader source from file '" + shaderFile + "'.");
	
	if (!fileSystem_->exists(shaderFile))
	{
		auto msg = std::string("Shader file does not exist: ") + shaderFile;
		throw std::runtime_error( msg.c_str() );
	}
	
	// Get our parameters ready
	auto outputFile = fileSystem_->generateTempFilename();
	auto extra = std::string();
	auto shaderTypeAsString = std::string();
	switch (shaderType)
	{
		case ShaderType::FRAGMENT:
			shaderTypeAsString = std::string("f");
#if defined(PLATFORM_WINDOWS)
				extra = std::string("--profile ps_4_0");
#endif
			break;
		
		case ShaderType::COMPUTE:
			throw std::runtime_error("Compute shaders not yet implemented.");
			break;
		
		case ShaderType::VERTEX:
		default:
			shaderTypeAsString = std::string("v");
#if defined(PLATFORM_WINDOWS)
				extra = std::string("--profile vs_4_0");
#endif
			break;
	}
	auto includes = std::string();
	for (const auto& dir : includeDirectories)
	{
		includes += std::string("-i ") + dir;
	}
#if defined(PLATFORM_WINDOWS)
    auto platform = std::string("windows");
    extra += std::string(" -O 3");
#elif defined(PLATFORM_MAC)
	auto platform = std::string("osx");
#elif defined(PLATFORM_LINUX)
	auto platform = std::string("linux");
#endif
	
	std::stringstream ss;
	ss << "shadercRelease";
	ss << " -f " << shaderFile;
	ss << " -o " << outputFile;
	ss << " --type " << shaderTypeAsString;
	ss << " --platform " << platform;
	ss << " " << includes;
	ss << " " << extra;
	ss << " 2>&1";
	
	logger_->info(std::string("Running shader compile command: ") + ss.str());
	
    bx::Error error;
    auto processReader = bx::ProcessReader();
    if (!processReader.open(ss.str().c_str(), &error))
    {
		throw std::runtime_error("Unable to load shader file.");
	}
	else if (!error.isOk())
    {
		auto msg = std::string("Unable to load shader file: ") + error.getMessage().getPtr();
		throw std::runtime_error(msg.c_str());
	}
	
	std::string commandOutput;
	std::array<char, 128> buffer;
	while (processReader.read(buffer.data(), 128, &error) != 0)
	{
		commandOutput += buffer.data();
	}
    
    processReader.close();
    
    int32_t result = processReader.getExitCode();
    
	if (result != EXIT_SUCCESS)
    {
		auto msg = std::string("Unable to compile shader file: ") + commandOutput;
		throw std::runtime_error(msg.c_str());
    }
    
    return outputFile;
}

::bgfx::ProgramHandle GraphicsEngine::createShaderProgramFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	auto vertexShaderMemoryBuffer = ::bgfx::copy(vertexShaderSource.c_str(), vertexShaderSource.size());
	auto fragmentShaderMemoryBuffer = ::bgfx::copy(fragmentShaderSource.c_str(), fragmentShaderSource.size());
	
	auto vertexShaderHandle = ::bgfx::createShader(vertexShaderMemoryBuffer);
	auto fragmentShaderHandle = ::bgfx::createShader(fragmentShaderMemoryBuffer);
	
	return ::bgfx::createProgram(vertexShaderHandle, fragmentShaderHandle, true);
	
	/*
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
	*/
}

std::string GraphicsEngine::getShaderErrorMessage(const GLuint shader)
{
	/*
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
	*/
	
	return std::string();
}

std::string GraphicsEngine::getShaderProgramErrorMessage(const GLuint shaderProgram)
{
	/*
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
	*/
	
	return std::string();
}

void GraphicsEngine::setMouseRelativeMode(const bool enabled)
{
	/*
	auto result = SDL_SetRelativeMouseMode(static_cast<SDL_bool>(enabled));
	
	if (result != 0)
	{
		auto message = std::string("Unable to set relative mouse mode: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
	*/
}

void GraphicsEngine::setCursorVisible(const bool visible)
{
	/*
	auto toggle = (visible ? SDL_ENABLE : SDL_DISABLE);
	auto result = SDL_ShowCursor(toggle);
	
	if (result < 0)
	{
		auto message = std::string("Unable to set mouse cursor visible\\invisible: ") + SDL_GetError();
		throw std::runtime_error(message);
	}
	*/
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
