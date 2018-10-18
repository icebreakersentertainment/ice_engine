#include <string>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ModelHandle.hpp"
#include "Scene.hpp"
#include "IWindowEventListener.hpp"
#include "IKeyboardEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "graphics/gui/IGui.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "pathfinding/IPathfindingEngine.hpp"

#include "BindingDelegate.hpp"
#include "AudioEngineBindingDelegate.hpp"
#include "ScriptingEngineBindingDelegate.hpp"
#include "NetworkingEngineBindingDelegate.hpp"
#include "GraphicsEngineBindingDelegate.hpp"
#include "PhysicsEngineBindingDelegate.hpp"
#include "PathfindingEngineBindingDelegate.hpp"
#include "EntityBindingDelegate.hpp"
#include "SceneBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "HeightMap.hpp"
#include "SplatMap.hpp"
#include "DisplacementMap.hpp"
#include "Heightfield.hpp"
#include "PathfindingTerrain.hpp"

#include "scripting/IScriptingEngine.hpp"

//#include "scripting/angel_script/scriptvector/scriptvector.hpp"
//#include "scripting/angel_script/scriptmap/scriptmap.hpp"

#include "GameEngine.hpp"

#include "noise/Noise.hpp"

namespace ice_engine
{

scripting::ScriptObjectHandle createScriptObjectHandle(void* object)
{
	return scripting::ScriptObjectHandle(object);
}

namespace idebugrenderer
{
void pushLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, IDebugRenderer* debugRenderer)
{
	debugRenderer->pushLine(from, to, color);
}
}

BindingDelegate::BindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, audio::IAudioEngine* audioEngine, networking::INetworkingEngine* networkingEngine, physics::IPhysicsEngine* physicsEngine, pathfinding::IPathfindingEngine* pathfindingEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	graphicsEngine_(graphicsEngine),
	audioEngine_(audioEngine),
	networkingEngine_(networkingEngine),
	physicsEngine_(physicsEngine),
	pathfindingEngine_(pathfindingEngine)
{
}

BindingDelegate::~BindingDelegate()
{
}

static void InitConstructorPbrMaterial(PbrMaterial* memory, Image* albedo, Image* normal, Image* metalness, Image* roughness, Image* ambientOcclusion) { new(memory) PbrMaterial(albedo, normal, metalness, roughness, ambientOcclusion); }
static void InitConstructorHeightMap(HeightMap* memory, const Image& image) { new(memory) HeightMap(image); }
static void InitConstructorSplatMap(SplatMap* memory, std::vector<PbrMaterial> materialMap, Image* terrainMap) { new(memory) SplatMap(std::move(materialMap), terrainMap); }
static void InitConstructorHeightfield(Heightfield* memory, const Image& image) { new(memory) Heightfield(image); }
static void InitConstructorPathfindingTerrain(PathfindingTerrain* memory, const HeightMap& heightMap) { new(memory) PathfindingTerrain(heightMap); }
static void InitConstructorMesh(Mesh* memory, std::string name, std::vector< glm::vec3 > vertices, std::vector< uint32 > indices, std::vector< glm::vec4 > colors, std::vector< glm::vec3 > normals, std::vector< glm::vec2 > textureCoordinates, VertexBoneData vertexBoneData = VertexBoneData(), BoneData boneData = BoneData()) { new(memory) Mesh(name, vertices, indices, colors, normals, textureCoordinates, vertexBoneData, boneData); }
static void InitConstructorTexture(Texture* memory, std::string name, Image* image) { new(memory) Texture(name, image); }

void testFunction(const pathfinding::AgentParams& ap)
{
	std::cout << "hi mom " << ap << std::endl;
}

void testFunction2(const pathfinding::AgentParams ap)
{
	std::cout << "hi mom " << ap << std::endl;
}

void testFunction3(pathfinding::AgentParams ap)
{
	std::cout << "hi mom " << ap << std::endl;
}

void BindingDelegate::bind()
{
	scriptingEngine_->registerObjectType("Image", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("Image", "const vectorInt8& data()", asMETHOD(Image, data));
	scriptingEngine_->registerClassMethod("Image", "uint32 width()", asMETHOD(Image, width));
	scriptingEngine_->registerClassMethod("Image", "uint32 height()", asMETHOD(Image, height));

	scriptingEngine_->registerEnum("ImageFormat");
	scriptingEngine_->registerEnumValue("ImageFormat", "FORMAT_UNKNOWN", Image::Format::FORMAT_UNKNOWN);
	scriptingEngine_->registerEnumValue("ImageFormat", "FORMAT_RGB", Image::Format::FORMAT_RGB);
	scriptingEngine_->registerEnumValue("ImageFormat", "FORMAT_RGBA", Image::Format::FORMAT_RGBA);

	scriptingEngine_->registerObjectType("Audio", 0, asOBJ_REF | asOBJ_NOCOUNT);
	
	scriptingEngine_->registerObjectType("IFileSystem", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IFileSystem fileSystem", gameEngine_->getFileSystem());
	scriptingEngine_->registerClassMethod("IFileSystem", "bool exists(const string& in) const", asMETHOD(fs::IFileSystem, exists));
	scriptingEngine_->registerClassMethod("IFileSystem", "bool isDirectory(const string& in) const", asMETHOD(fs::IFileSystem, isDirectory));
	scriptingEngine_->registerClassMethod("IFileSystem", "void deleteFile(const string& in) const", asMETHOD(fs::IFileSystem, deleteFile));
	scriptingEngine_->registerClassMethod("IFileSystem", "void makeDirectory(const string& in) const", asMETHOD(fs::IFileSystem, makeDirectory));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getBasePath(const string& in) const", asMETHOD(fs::IFileSystem, getBasePath));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getDirectorySeperator() const", asMETHOD(fs::IFileSystem, getDirectorySeperator));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getFilename(const string& in) const", asMETHOD(fs::IFileSystem, getFilename));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getFilenameWithoutExtension(const string& in) const", asMETHOD(fs::IFileSystem, getFilenameWithoutExtension));
	scriptingEngine_->registerClassMethod("IFileSystem", "string readAll(const string& in) const", asMETHOD(fs::IFileSystem, readAll));
	scriptingEngine_->registerClassMethod("IFileSystem", "string generateTempFilename() const", asMETHOD(fs::IFileSystem, generateTempFilename));

	auto audioEngineBindingDelegate = AudioEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, audioEngine_);
	audioEngineBindingDelegate.bind();

	auto scriptingEngineBindingDelegate = ScriptingEngineBindingDelegate(logger_, scriptingEngine_);
	scriptingEngineBindingDelegate.bind();

	auto networkingEngineBindingDelegate = NetworkingEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, networkingEngine_);
	networkingEngineBindingDelegate.bind();

	auto graphicsEngineBindingDelegate = GraphicsEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, graphicsEngine_);
	graphicsEngineBindingDelegate.bind();

	auto physicsEngineBindingDelegate = PhysicsEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, physicsEngine_);
	physicsEngineBindingDelegate.bind();

	auto pathfindingEngineBindingDelegate = PathfindingEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, pathfindingEngine_);
	pathfindingEngineBindingDelegate.bind();

	// TEST
	scriptingEngine_->registerGlobalFunction("void terminate()", asFUNCTION(std::terminate), asCALL_CDECL);
	scriptingEngine_->registerGlobalFunction("void testFunction(const AgentParams& in)", asFUNCTION(testFunction), asCALL_CDECL);
	scriptingEngine_->registerGlobalFunction("void testFunction2(const AgentParams)", WRAP_FN(testFunction2), asCALL_GENERIC);
	scriptingEngine_->registerGlobalFunction("void testFunction3(AgentParams)", WRAP_FN(testFunction3), asCALL_GENERIC);

	// Register Model/Mesh/etc
	scriptingEngine_->registerObjectType("BoneData", sizeof(BoneData), asOBJ_VALUE | asGetTypeTraits<BoneData>());
	scriptingEngine_->registerObjectBehaviour("BoneData", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<BoneData>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("BoneData", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<BoneData>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("BoneData", "BoneData& opAssign(const BoneData& in)", asMETHODPR(BoneData, operator=, (const BoneData&), BoneData&));

	scriptingEngine_->registerObjectType("VertexBoneData", sizeof(VertexBoneData), asOBJ_VALUE | asGetTypeTraits<VertexBoneData>());
	scriptingEngine_->registerObjectBehaviour("VertexBoneData", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<VertexBoneData>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("VertexBoneData", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<VertexBoneData>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("VertexBoneData", "VertexBoneData& opAssign(const VertexBoneData& in)", asMETHODPR(VertexBoneData, operator=, (const VertexBoneData&), VertexBoneData&));

	scriptingEngine_->registerObjectType("Mesh", sizeof(Mesh), asOBJ_VALUE | asGetTypeTraits<Mesh>());
	scriptingEngine_->registerObjectBehaviour("Mesh", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Mesh>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("Mesh", asBEHAVE_CONSTRUCT, "void f(string, vectorVec3, vectorUInt32, vectorVec4, vectorVec3, vectorVec2, VertexBoneData = VertexBoneData(), BoneData = BoneData())", asFUNCTION(InitConstructorMesh), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Mesh", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Mesh>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Mesh", "Mesh& opAssign(const Mesh& in)", asMETHODPR(Mesh, operator=, (const Mesh&), Mesh&));
	scriptingEngine_->registerClassMethod("Mesh", "const VertexBoneData& vertexBoneData() const", asMETHOD(Mesh, vertexBoneData));

	scriptingEngine_->registerObjectType("Texture", sizeof(Texture), asOBJ_VALUE | asGetTypeTraits<Texture>());
	scriptingEngine_->registerObjectBehaviour("Texture", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Texture>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("Texture", asBEHAVE_CONSTRUCT, "void f(string, Image@)", asFUNCTION(InitConstructorTexture), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Texture", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Texture>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Texture", "Texture& opAssign(const Texture& in)", asMETHODPR(Texture, operator=, (const Texture&), Texture&));

	scriptingEngine_->registerObjectType("Skeleton", sizeof(Skeleton), asOBJ_VALUE | asGetTypeTraits<Skeleton>());
	scriptingEngine_->registerObjectBehaviour("Skeleton", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Skeleton>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("Skeleton", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Skeleton>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Skeleton", "Skeleton& opAssign(const Skeleton& in)", asMETHODPR(Skeleton, operator=, (const Skeleton&), Skeleton&));

	scriptingEngine_->registerObjectType("Animation", sizeof(Animation), asOBJ_VALUE | asGetTypeTraits<Animation>());
	scriptingEngine_->registerObjectBehaviour("Animation", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Animation>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("Animation", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Animation>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Animation", "Animation& opAssign(const Animation& in)", asMETHODPR(Animation, operator=, (const Animation&), Animation&));
	scriptingEngine_->registerClassMethod("Animation", "const string& name() const", asMETHOD(Animation, name));

	registerVectorBindings<Mesh>(scriptingEngine_, "vectorMesh", "Mesh");
	registerVectorBindings<Texture>(scriptingEngine_, "vectorTexture", "Texture");
	registerMapBindings<std::string, Animation>(scriptingEngine_, "mapStringAnimation", "string", "Animation");

	scriptingEngine_->registerObjectType("Model", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("Model", "const vectorMesh& meshes() const", asMETHOD(Model, meshes));
	scriptingEngine_->registerClassMethod("Model", "const vectorTexture& textures() const", asMETHOD(Model, textures));
	scriptingEngine_->registerClassMethod("Model", "const Skeleton& skeleton() const", asMETHOD(Model, skeleton));
	scriptingEngine_->registerClassMethod("Model", "const mapStringAnimation& animations() const", asMETHOD(Model, animations));
	
	// IGame
	scriptingEngine_->registerInterface("IGame");
	scriptingEngine_->registerInterfaceMethod("IGame", "void initialize()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void destroy()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void tick(const float)");

	// Types available in the scripting engine
	registerHandleBindings<ModelHandle>(scriptingEngine_, "ModelHandle");
	registerHandleBindings<SkeletonHandle>(scriptingEngine_, "SkeletonHandle");
	registerHandleBindings<AnimationHandle>(scriptingEngine_, "AnimationHandle");

	scriptingEngine_->registerObjectType("PbrMaterial", sizeof(PbrMaterial), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<PbrMaterial>());
	//scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<PbrMaterial>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_CONSTRUCT, "void f(Image@, Image@, Image@, Image@, Image@ = null)", asFUNCTION(InitConstructorPbrMaterial), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_CONSTRUCT, "void f(const PbrMaterial& in)", asFUNCTION(CopyConstructor<PbrMaterial>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<PbrMaterial>), asCALL_CDECL_OBJFIRST);
	registerVectorBindings<PbrMaterial>(scriptingEngine_, "vectorPbrMaterial", "PbrMaterial");

	scriptingEngine_->registerObjectType("HeightMap", sizeof(HeightMap), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<HeightMap>());
	//scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<HeightMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_CONSTRUCT, "void f(const Image& in)", asFUNCTION(InitConstructorHeightMap), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_CONSTRUCT, "void f(const HeightMap& in)", asFUNCTION(CopyConstructor<HeightMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<HeightMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerClassMethod(
		"HeightMap",
		"Image@ image()",
		asMETHODPR(HeightMap, image, (), Image*)
	);

	scriptingEngine_->registerObjectType("SplatMap", sizeof(SplatMap), asOBJ_VALUE | asGetTypeTraits<SplatMap>());
	//scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<SplatMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_CONSTRUCT, "void f(vectorPbrMaterial, Image@)", asFUNCTION(InitConstructorSplatMap), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_CONSTRUCT, "void f(const SplatMap& in)", asFUNCTION(CopyConstructor<SplatMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<SplatMap>), asCALL_CDECL_OBJFIRST);

	scriptingEngine_->registerObjectType("DisplacementMap", sizeof(DisplacementMap), asOBJ_VALUE | asGetTypeTraits<DisplacementMap>());
	scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<DisplacementMap>), asCALL_CDECL_OBJFIRST);
	//scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_CONSTRUCT, "void f(const Image& in, vectorPbrMaterial in)", asFUNCTION(InitConstructorDisplacementMap), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_CONSTRUCT, "void f(const DisplacementMap& in)", asFUNCTION(CopyConstructor<DisplacementMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<DisplacementMap>), asCALL_CDECL_OBJFIRST);

	scriptingEngine_->registerObjectType("Heightfield", sizeof(Heightfield), asOBJ_VALUE | asGetTypeTraits<Heightfield>());
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Heightfield>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_CONSTRUCT, "void f(const Image& in)", asFUNCTION(InitConstructorHeightfield), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_CONSTRUCT, "void f(const Heightfield& in)", asFUNCTION(CopyConstructor<Heightfield>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Heightfield>), asCALL_CDECL_OBJFIRST);

	scriptingEngine_->registerObjectType("PathfindingTerrain", sizeof(PathfindingTerrain), asOBJ_VALUE | asGetTypeTraits<PathfindingTerrain>());
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<PathfindingTerrain>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_CONSTRUCT, "void f(const HeightMap& in)", asFUNCTION(InitConstructorPathfindingTerrain), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_CONSTRUCT, "void f(const PathfindingTerrain& in)", asFUNCTION(CopyConstructor<PathfindingTerrain>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<PathfindingTerrain>), asCALL_CDECL_OBJFIRST);

	registerSharedFutureBindings<Image*>(scriptingEngine_, "shared_futureImage", "Image@");
	registerSharedFutureBindings<Audio*>(scriptingEngine_, "shared_futureAudio", "Audio@");
	registerSharedFutureBindings<Model*>(scriptingEngine_, "shared_futureModel", "Model@");
	registerSharedFutureBindings<ModelHandle>(scriptingEngine_, "shared_futureModelHandle", "ModelHandle");
	registerSharedFutureBindings<void>(scriptingEngine_, "shared_futureVoid", "void");

	// ITerrain bindings
	scriptingEngine_->registerObjectType("ITerrain", 0, asOBJ_REF | asOBJ_NOCOUNT);
	registerVectorBindings<pathfinding::CrowdHandle>(scriptingEngine_, "vectorCrowdHandle", "CrowdHandle");
	scriptingEngine_->registerClassMethod(
			"ITerrain",
			"const vectorCrowdHandle& crowds() const",
			asMETHODPR(ITerrain, crowds, () const, const std::vector<pathfinding::CrowdHandle>&)
		);
	/*
	scriptingEngine_->registerClassMethod(
		"IOpenGlLoader",
		"int getWorkQueueCount()",
		asMETHODPR(IOpenGlLoader, getWorkQueueCount, () const, uint32)
	);
	*/

	//scriptingEngine_->registerObjectMethod("IAudio", "Audio@ opCast()", asFUNCTION((refCast<audio::IAudio, Audio>)), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectMethod("Audio", "IAudio@ opImplCast()", asFUNCTION((refCast<Audio, audio::IAudio>)), asCALL_CDECL_OBJLAST);
	//scriptingEngine_->registerObjectMethod("IAudio", "const Audio@ opCast()", asFUNCTION((refCast<audio::IAudio, Audio>)), asCALL_CDECL_OBJLAST);
	//scriptingEngine_->registerObjectMethod("Audio", "const IAudio@ opImplCast()", asFUNCTION((refCast<Audio, audio::IAudio>)), asCALL_CDECL_OBJLAST);
	
	scriptingEngine_->registerObjectMethod("Image", "IImage@ opImplCast()", asFUNCTION((refCast<Image, graphics::IImage>)), asCALL_CDECL_OBJLAST);

	// Register function declarations
	scriptingEngine_->registerFunctionDefinition("void WorkFunction()");

	// Listeners
	scriptingEngine_->registerInterface("IWindowEventListener");
	scriptingEngine_->registerInterfaceMethod("IWindowEventListener", "bool processEvent(const WindowEvent& in)");
	scriptingEngine_->registerInterface("IKeyboardEventListener");
	scriptingEngine_->registerInterfaceMethod("IKeyboardEventListener", "bool processEvent(const KeyboardEvent& in)");
	scriptingEngine_->registerInterface("IMouseMotionEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseMotionEventListener", "bool processEvent(const MouseMotionEvent& in)");
	scriptingEngine_->registerInterface("IMouseButtonEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseButtonEventListener", "bool processEvent(const MouseButtonEvent& in)");
	scriptingEngine_->registerInterface("IMouseWheelEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseWheelEventListener", "bool processEvent(const MouseWheelEvent& in)");

	scriptingEngine_->registerInterface("IConnectEventListener");
	scriptingEngine_->registerInterfaceMethod("IConnectEventListener", "bool processEvent(const ConnectEvent& in)");
	scriptingEngine_->registerInterface("IDisconnectEventListener");
	scriptingEngine_->registerInterfaceMethod("IDisconnectEventListener", "bool processEvent(const DisconnectEvent& in)");
	scriptingEngine_->registerInterface("IMessageEventListener");
	scriptingEngine_->registerInterfaceMethod("IMessageEventListener", "bool processEvent(const MessageEvent& in)");

//	scriptingEngine_->registerInterface("ScriptObject");

	scriptingEngine_->registerGlobalFunction(
		"ScriptObjectHandle createScriptObjectHandle(IScriptObject@)",
		asFUNCTIONPR(createScriptObjectHandle, (void*), scripting::ScriptObjectHandle),
		asCALL_CDECL
	);

	scriptingEngine_->registerObjectType("Scene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	auto entityBindingDelegate = EntityBindingDelegate(logger_, scriptingEngine_, gameEngine_);
	entityBindingDelegate.bind();

	auto sceneBindingDelegate = SceneBindingDelegate(logger_, scriptingEngine_, gameEngine_, graphicsEngine_, audioEngine_, networkingEngine_, physicsEngine_, pathfindingEngine_);
	sceneBindingDelegate.bind();

	scriptingEngine_->registerInterfaceMethod("IScriptObject", "void tick(const float)");
	scriptingEngine_->registerInterfaceMethod("IScriptObject", "void update(const AgentState& in)");
	scriptingEngine_->registerInterfaceMethod("IScriptObject", "void update(const MovementRequestState& in)");
	scriptingEngine_->registerInterfaceMethod("IScriptObject", "void serialize(Entity)");
	scriptingEngine_->registerInterfaceMethod("IScriptObject", "void deserialize(Entity)");

	scriptingEngine_->registerObjectType("Noise", sizeof(noise::Noise), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<noise::Noise>());
	scriptingEngine_->registerClassMethod(
		"Noise",
		"float getValue(const float, const float) const",
		asMETHODPR(noise::Noise, getValue, (const float32, const float32) const, float32)
	);
	
	// ILogger bindings
	scriptingEngine_->registerObjectType("ILogger", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("ILogger logger", logger_);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void info(const string& in)",
		asMETHODPR(logger::ILogger, info, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void debug(const string& in)",
		asMETHODPR(logger::ILogger, debug, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void trace(const string& in)",
		asMETHODPR(logger::ILogger, trace, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void warn(const string& in)",
		asMETHODPR(logger::ILogger, warn, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void error(const string& in)",
		asMETHODPR(logger::ILogger, error, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void fatal(const string& in)",
		asMETHODPR(logger::ILogger, fatal, (const std::string&), void)
	);

	// IThreadPool bindings
	scriptingEngine_->registerObjectType("IThreadPool", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getActiveWorkerCount()",
		asMETHODPR(IThreadPool, getActiveWorkerCount, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getInactiveWorkerCount()",
		asMETHODPR(IThreadPool, getInactiveWorkerCount, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getWorkQueueSize()",
		asMETHODPR(IThreadPool, getWorkQueueSize, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getWorkQueueCount()",
		asMETHODPR(IThreadPool, getWorkQueueCount, () const, uint32)
	);

	// IOpenGlLoader bindings
	scriptingEngine_->registerObjectType("IOpenGlLoader", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IOpenGlLoader",
		"int getWorkQueueCount()",
		asMETHODPR(IOpenGlLoader, getWorkQueueCount, () const, uint32)
	);
		
	scriptingEngine_->registerObjectType("EngineStatistics", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectProperty("EngineStatistics", "float fps", asOFFSET(EngineStatistics, fps));
	scriptingEngine_->registerObjectProperty("EngineStatistics", "float renderTime", asOFFSET(EngineStatistics, renderTime));
	
	// IDebugRenderer
	scriptingEngine_->registerObjectType("IDebugRenderer", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IDebugRenderer debugRenderer", gameEngine_->getDebugRenderer());
	scriptingEngine_->registerObjectMethod(
		"IDebugRenderer",
		"void pushLine(const vec3& in, const vec3& in, const vec3& in)",
		asFUNCTION(idebugrenderer::pushLine),
		asCALL_CDECL_OBJLAST
	);
	
	// GameEngine functions available in the scripting engine
	scriptingEngine_->registerGlobalFunction(
		"const EngineStatistics@ getEngineStatistics()",
		asMETHODPR(GameEngine, getEngineStatistics, () const, const EngineStatistics&),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void setIGameInstance(IGame@)",
		asMETHODPR(GameEngine, setIGameInstance, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IGraphicsEngine@ getGraphicsEngine()",
		asMETHODPR(GameEngine, getGraphicsEngine, () const, graphics::IGraphicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IPhysicsEngine@ getPhysicsEngine()",
		asMETHODPR(GameEngine, getPhysicsEngine, () const, physics::IPhysicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IDebugRenderer@ getDebugRenderer()",
		asMETHODPR(GameEngine, getDebugRenderer, () const, IDebugRenderer*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ getBackgroundThreadPool()",
		asMETHODPR(GameEngine, getBackgroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ getForegroundThreadPool()",
		asMETHODPR(GameEngine, getForegroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IOpenGlLoader@ getOpenGlLoader()",
		asMETHODPR(GameEngine, getOpenGlLoader, () const, IOpenGlLoader*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IGui@ createGui(const string& in)",
		asMETHODPR(GameEngine, createGui, (const std::string&), graphics::gui::IGui*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyGui(const IGui@)",
		asMETHODPR(GameEngine, destroyGui, (const graphics::gui::IGui*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addWindowEventListener(IWindowEventListener@)",
		asMETHODPR(GameEngine, addWindowEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addKeyboardEventListener(IKeyboardEventListener@)",
		asMETHODPR(GameEngine, addKeyboardEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseMotionEventListener(IMouseMotionEventListener@)",
		asMETHODPR(GameEngine, addMouseMotionEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseButtonEventListener(IMouseButtonEventListener@)",
		asMETHODPR(GameEngine, addMouseButtonEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseWheelEventListener(IMouseWheelEventListener@)",
		asMETHODPR(GameEngine, addMouseWheelEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeWindowEventListener(IWindowEventListener@)",
		asMETHODPR(GameEngine, removeWindowEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeKeyboardEventListener(IKeyboardEventListener@)",
		asMETHODPR(GameEngine, removeKeyboardEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseMotionEventListener(IMouseMotionEventListener@)",
		asMETHODPR(GameEngine, removeMouseMotionEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseButtonEventListener(IMouseButtonEventListener@)",
		asMETHODPR(GameEngine, addMouseButtonEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseWheelEventListener(IMouseWheelEventListener@)",
		asMETHODPR(GameEngine, addMouseWheelEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addConnectEventListener(IConnectEventListener@)",
		asMETHODPR(GameEngine, addConnectEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addDisconnectEventListener(IDisconnectEventListener@)",
		asMETHODPR(GameEngine, addDisconnectEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMessageEventListener(IMessageEventListener@)",
		asMETHODPR(GameEngine, addMessageEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeConnectEventListener(IConnectEventListener@)",
		asMETHODPR(GameEngine, removeConnectEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeDisconnectEventListener(IDisconnectEventListener@)",
		asMETHODPR(GameEngine, addDisconnectEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMessageEventListener(IMessageEventListener@)",
		asMETHODPR(GameEngine, removeMessageEventListener, (void*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureVoid postWorkToBackgroundThreadPool(WorkFunction@)",
		asMETHODPR(GameEngine, postWorkToBackgroundThreadPool, (void*), std::shared_future<void>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Model@ importModel(const string& in, const string& in)",
		asMETHODPR(GameEngine, importModel, (const std::string&, const std::string&), Model*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureModel importModelAsync(const string& in, const string& in)",
		asMETHODPR(GameEngine, importModelAsync, (const std::string&, const std::string&), std::shared_future<Model*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Audio@ loadAudio(const string& in, const string& in)",
		asMETHODPR(GameEngine, loadAudio, (const std::string&, const std::string&), Audio*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureAudio loadAudioAsync(const string& in, const string& in)",
		asMETHODPR(GameEngine, loadAudioAsync, (const std::string&, const std::string&), std::shared_future<Audio*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Image@ createImage(const string& in, const vectorInt8& in, const uint, const uint, const ImageFormat)",
		asMETHODPR(GameEngine, createImage, (const std::string&, const std::vector<char>&, const uint32, const uint32, const Image::Format), Image*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Image@ loadImage(const string& in, const string& in)",
		asMETHODPR(GameEngine, loadImage, (const std::string&, const std::string&), Image*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureImage loadImageAsync(const string& in, const string& in)",
		asMETHODPR(GameEngine, loadImageAsync, (const std::string&, const std::string&), std::shared_future<Image*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Model@ getModel(const string& in)",
		asMETHODPR(GameEngine, getModel, (const std::string&) const, Model*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Audio@ getAudio(const string& in)",
		asMETHODPR(GameEngine, getAudio, (const std::string&) const, Audio*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Image@ getImage(const string& in)",
		asMETHODPR(GameEngine, getImage, (const std::string&) const, Image*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadModel(const string& in)",
		asMETHODPR(GameEngine, unloadModel, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadAudio(const string& in)",
		asMETHODPR(GameEngine, unloadAudio, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadImage(const string& in)",
		asMETHODPR(GameEngine, unloadImage, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ModelHandle loadStaticModel(Model@)",
		asMETHODPR(GameEngine, loadStaticModel, (const Model*), ModelHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureModelHandle loadStaticModelAsync(Model@)",
		asMETHODPR(GameEngine, loadStaticModelAsync, (const Model*), std::shared_future<ModelHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"VertexShaderHandle createVertexShader(const string& in, const string& in)",
		asMETHODPR(GameEngine, createVertexShader, (const std::string&, const std::string&), graphics::VertexShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureVertexShaderHandle createVertexShaderAsync(const string& in, const string& in)",
		asMETHODPR(GameEngine, createVertexShaderAsync, (const std::string&, const std::string&), std::shared_future<graphics::VertexShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"VertexShaderHandle createVertexShaderFromSource(const string& in, const string& in)",
		asMETHODPR(GameEngine, createVertexShaderFromSource, (const std::string&, const std::string&), graphics::VertexShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureVertexShaderHandle createVertexShaderFromSourceAsync(const string& in, const string& in)",
		asMETHODPR(GameEngine, createVertexShaderFromSourceAsync, (const std::string&, const std::string&), std::shared_future<graphics::VertexShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"FragmentShaderHandle createFragmentShader(const string& in, const string& in)",
		asMETHODPR(GameEngine, createFragmentShader, (const std::string&, const std::string&), graphics::FragmentShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureFragmentShaderHandle createFragmentShaderAsync(const string& in, const string& in)",
		asMETHODPR(GameEngine, createFragmentShaderAsync, (const std::string&, const std::string&), std::shared_future<graphics::FragmentShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"FragmentShaderHandle createFragmentShaderFromSource(const string& in, const string& in)",
		asMETHODPR(GameEngine, createFragmentShaderFromSource, (const std::string&, const std::string&), graphics::FragmentShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureFragmentShaderHandle createFragmentShaderFromSourceAsync(const string& in, const string& in)",
		asMETHODPR(GameEngine, createFragmentShaderFromSourceAsync, (const std::string&, const std::string&), std::shared_future<graphics::FragmentShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"VertexShaderHandle getVertexShader(const string& in)",
		asMETHODPR(GameEngine, getVertexShader, (const std::string&) const, graphics::VertexShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShader(const string& in)",
		asMETHODPR(GameEngine, destroyShader, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShader(const VertexShaderHandle& in)",
		asMETHODPR(GameEngine, destroyShader, (const graphics::VertexShaderHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShader(const FragmentShaderHandle& in)",
		asMETHODPR(GameEngine, destroyShader, (const graphics::FragmentShaderHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderProgramHandle createShaderProgram(const string& in, const VertexShaderHandle& in, const FragmentShaderHandle& in)",
		asMETHODPR(GameEngine, createShaderProgram, (const std::string&, const graphics::VertexShaderHandle&, const graphics::FragmentShaderHandle&), graphics::ShaderProgramHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureShaderProgramHandle createShaderProgramAsync(const string& in, const VertexShaderHandle& in, const FragmentShaderHandle& in)",
		asMETHODPR(GameEngine, createShaderProgramAsync, (const std::string&, const graphics::VertexShaderHandle&, const graphics::FragmentShaderHandle&), std::shared_future<graphics::ShaderProgramHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderProgramHandle getShaderProgram(const string& in)",
		asMETHODPR(GameEngine, getShaderProgram, (const std::string&) const, graphics::ShaderProgramHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShaderProgram(const string& in)",
		asMETHODPR(GameEngine, destroyShaderProgram, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShaderProgram(const ShaderProgramHandle& in)",
		asMETHODPR(GameEngine, destroyShaderProgram, (const graphics::ShaderProgramHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);

	scriptingEngine_->registerGlobalFunction(
		"CollisionShapeHandle createStaticPlaneShape(const string& in, const vec3& in, const float)",
		asMETHODPR(GameEngine, createStaticPlaneShape, (const std::string&, const glm::vec3&, const float32), physics::CollisionShapeHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"CollisionShapeHandle createStaticBoxShape(const string& in, const vec3& in)",
		asMETHODPR(GameEngine, createStaticBoxShape, (const std::string&, const glm::vec3&), physics::CollisionShapeHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
			"CollisionShapeHandle getStaticShape(const string& in)",
			asMETHODPR(GameEngine, getStaticShape, (const std::string&) const, physics::CollisionShapeHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
		"CollisionShapeHandle createStaticTerrainShape(const string& in, const Heightfield& in)",
		asMETHODPR(GameEngine, createStaticTerrainShape, (const std::string&, const Heightfield&), physics::CollisionShapeHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
//	scriptingEngine_->registerGlobalFunction(
//		"void destroyStaticShape(const CollisionShapeHandle& in)",
//		asMETHODPR(GameEngine, destroyStaticShape, (const physics::CollisionShapeHandle&), void)
//	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyAllStaticShapes()",
		asMETHODPR(GameEngine, destroyAllStaticShapes, (), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
			"ModelHandle createStaticModel(const string& in, const Model@)",
			asMETHODPR(GameEngine, createStaticModel, (const std::string&, const Model&), ModelHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"ModelHandle getStaticModel(const string& in)",
			asMETHODPR(GameEngine, getStaticModel, (const std::string&) const, ModelHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"AnimationHandle createAnimation(const string& in, const Animation& in)",
			asMETHODPR(GameEngine, createAnimation, (const std::string&, const Animation&), AnimationHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"AnimationHandle getAnimation(const string& in)",
			asMETHODPR(GameEngine, getAnimation, (const std::string&) const, AnimationHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"SkeletonHandle createSkeleton(const string& in, const Skeleton& in)",
			asMETHODPR(GameEngine, createSkeleton, (const std::string&, const Skeleton&), SkeletonHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"SkeletonHandle getSkeleton(const string& in)",
			asMETHODPR(GameEngine, getSkeleton, (const std::string&) const, SkeletonHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
				"void createSkeleton(const MeshHandle& in, const VertexBoneData& in)",
				asMETHODPR(GameEngine, createSkeleton, (const graphics::MeshHandle&, const graphics::ISkeleton&), void),
				asCALL_THISCALL_ASGLOBAL,
				gameEngine_
			);
	scriptingEngine_->registerGlobalFunction(
			"MeshHandle createStaticMesh(const string& in, const Mesh& in)",
			asMETHODPR(GameEngine, createStaticMesh, (const std::string&, const Mesh&), graphics::MeshHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"MeshHandle getStaticMesh(const string& in)",
			asMETHODPR(GameEngine, getStaticMesh, (const std::string&) const, graphics::MeshHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"TextureHandle createTexture(const string& in, const Texture& in)",
			asMETHODPR(GameEngine, createTexture, (const std::string&, const Texture&), graphics::TextureHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"TextureHandle getTexture(const string& in)",
			asMETHODPR(GameEngine, getTexture, (const std::string&) const, graphics::TextureHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"TerrainHandle createStaticTerrain(const string& in, const HeightMap& in, const SplatMap& in, const DisplacementMap& in)",
			asMETHODPR(GameEngine, createStaticTerrain, (const std::string&, const HeightMap&, const SplatMap&, const DisplacementMap&), graphics::TerrainHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"TerrainHandle getStaticTerrain(const string& in)",
			asMETHODPR(GameEngine, getStaticTerrain, (const std::string&) const, graphics::TerrainHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"PolygonMeshHandle createPolygonMesh(const string& in, const PathfindingTerrain& in, const PolygonMeshConfig& in)",
			asMETHODPR(GameEngine, createPolygonMesh, (const std::string&, const PathfindingTerrain& in, const pathfinding::PolygonMeshConfig&), pathfinding::PolygonMeshHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction("PolygonMeshHandle getPolygonMesh(const string& in)", asMETHOD(GameEngine, getPolygonMesh), asCALL_THISCALL_ASGLOBAL, gameEngine_);
	scriptingEngine_->registerGlobalFunction(
			"NavigationMeshHandle createNavigationMesh(const string& in, const PolygonMeshHandle& in, const NavigationMeshConfig& in)",
			asMETHODPR(GameEngine, createNavigationMesh, (const std::string&, const pathfinding::PolygonMeshHandle& in, const pathfinding::NavigationMeshConfig&), pathfinding::NavigationMeshHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"NavigationMeshHandle getNavigationMesh(const string& in)",
			asMETHODPR(GameEngine, getNavigationMesh, (const std::string&) const, pathfinding::NavigationMeshHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);

	scriptingEngine_->registerGlobalFunction(
		"Scene@ createScene(const string& in)",
		asMETHODPR(GameEngine, createScene, (const std::string&), Scene*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	scriptingEngine_->registerGlobalFunction(
		"Scene@ getScene(const string& in)",
		asMETHODPR(GameEngine, getScene, (const std::string&) const, Scene*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);

	scriptingEngine_->registerGlobalFunction(
		"void destroyScene(const string& in)",
		asMETHODPR(GameEngine, destroyScene, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyScene(Scene@)",
		asMETHODPR(GameEngine, destroyScene, (Scene* scene), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	registerVectorBindings<Scene*>(scriptingEngine_, "vectorScene", "Scene@");
	scriptingEngine_->registerGlobalFunction(
		"vectorScene getAllScenes()",
		asMETHODPR(GameEngine, getAllScenes, () const, std::vector<Scene*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	// CHEATING
	//auto pathfindingEngineBindingDelegate = PathfindingEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, pathfindingEngine_);
	//pathfindingEngineBindingDelegate.bind();
}
	
};
