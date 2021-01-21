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
#include "ITextInputEventListener.hpp"
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

#include "resources/EngineResourceManager.MeshHandle.hpp"

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

static void InitConstructorImage(Image* memory, const std::vector<byte>& data, const uint32 width, const uint32 height, const IImage::Format format) { new(memory) Image(data, width, height, format); }
static void InitConstructorPbrMaterial(PbrMaterial* memory, IImage* albedo, IImage* normal, IImage* metalness, IImage* roughness, IImage* ambientOcclusion) { new(memory) PbrMaterial(albedo, normal, metalness, roughness, ambientOcclusion); }
static void InitConstructorHeightMap(HeightMap* memory, const IImage& image) { new(memory) HeightMap(image); }
static void InitConstructorHeightMap(HeightMap* memory, const std::vector<uint8>& imageData, const uint32 width, const uint32 height) { new(memory) HeightMap(imageData, width, height); }
static void InitConstructorSplatMap(SplatMap* memory, std::vector<PbrMaterial> materialMap, IImage* terrainMap) { new(memory) SplatMap(std::move(materialMap), terrainMap); }
static void InitConstructorHeightfield(Heightfield* memory, const IImage& image) { new(memory) Heightfield(image); }
static void InitConstructorPathfindingTerrain(PathfindingTerrain* memory, const HeightMap& heightMap) { new(memory) PathfindingTerrain(heightMap); }
static void InitConstructorMesh(Mesh* memory, std::string name, std::vector< glm::vec3 > vertices, std::vector< uint32 > indices, std::vector< glm::vec4 > colors, std::vector< glm::vec3 > normals, std::vector< glm::vec2 > textureCoordinates, VertexBoneData vertexBoneData = VertexBoneData(), BoneData boneData = BoneData()) { new(memory) Mesh(name, vertices, indices, colors, normals, textureCoordinates, vertexBoneData, boneData); }
static void InitConstructorTexture(Texture* memory, std::string name, IImage* image) { new(memory) Texture(name, image); }

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

template<typename T, typename V>
class UniquePtrRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }

	static void InitConstructor(T* memory, const uint64 id) { new(memory) T(id); }

	static void CopyConstructor(T* memory, const T& other) { new(memory) T(other); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }

	static V* get(T* u) { return u->get(); }
};

/**
 * Register our unique_ptr bindings.
 */
template<typename T>
void registerUniquePtrBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& scriptTypeName)
{
	typedef UniquePtrRegisterHelper<std::unique_ptr<T>, T> UniquePtrBase;

	scriptingEngine->registerObjectType(name.c_str(), sizeof(T), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<T>());
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(UniquePtrBase::DefaultConstructor), asCALL_CDECL_OBJFIRST);
//	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(ref@)", asFUNCTION(HandleBase::InitConstructor), asCALL_CDECL_OBJFIRST);
//	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(UniquePtrBase::DefaultDestructor), asCALL_CDECL_OBJFIRST);
	// scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(std::unique_ptr<T>, operator=, (const std::unique_ptr<T>&), std::unique_ptr<T>&));
	scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(std::unique_ptr<T>, operator=, (std::unique_ptr<T>&&), std::unique_ptr<T>&));
	scriptingEngine->registerObjectMethod(name.c_str(), scriptTypeName + "@ get()", asFUNCTION(UniquePtrBase::get), asCALL_CDECL_OBJFIRST);
	// scriptingEngine->registerClassMethod(name.c_str(), "const " + scriptTypeName + "@+ get() const", asMETHODPR(T, get, () const, void*));
	// scriptingEngine->registerClassMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool ));
	// scriptingEngine->registerClassMethod(name.c_str(), "bool opEquals(const " + name + "& in) const", asMETHODPR(T, operator==, (const T&) const, bool));
}

template<typename T>
class TemplateTypeRegisterHelper
{
public:
    static void DefaultConstructor(T* memory) { new(memory) T(); }

    static void InitConstructor(T* memory, const uint64 id) { new(memory) T(id); }

    static void CopyConstructor(T* memory, const T& other) { new(memory) T(other); }
    static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
};

/**
 * Register our template type bindings.
 */
template<typename T = std::nullptr_t>
void registerTemplateTypeBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& scriptTypeName, const asDWORD objectTypeFlags = asOBJ_REF | asOBJ_NOCOUNT, const asEObjTypeFlags objectTypeTraitsFlag = asOBJ_APP_CLASS_ALLINTS)
{
    const int32 size = objectTypeFlags & asOBJ_REF ? 0 : sizeof(T);
    const asDWORD typeFlags = objectTypeFlags | asOBJ_TEMPLATE | (objectTypeFlags & asOBJ_REF ? 0 : objectTypeTraitsFlag | asGetTypeTraits<T>());
    scriptingEngine->registerObjectType(name.c_str(), size, typeFlags);

    if ((objectTypeFlags & asOBJ_NOCOUNT) == 0)
    {
//        scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(TemplateTypeRegisterHelper::DefaultConstructor), asCALL_CDECL_OBJFIRST);
//    	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(ref@)", asFUNCTION(HandleBase::InitConstructor), asCALL_CDECL_OBJFIRST);
//    	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
    }
}

/**
 * Register our template type specialization bindings.
 */
template<typename T = std::nullptr_t>
void registerTemplateTypeSpecializationBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& scriptTypeName, const asDWORD objectTypeFlags = asOBJ_REF | asOBJ_NOCOUNT, const asEObjTypeFlags objectTypeTraitsFlag = asOBJ_APP_CLASS_ALLINTS)
{
    const int32 size = objectTypeFlags & asOBJ_REF ? 0 : sizeof(T);
    const asDWORD typeFlags = objectTypeFlags | (objectTypeFlags & asOBJ_REF ? 0 : objectTypeTraitsFlag | asGetTypeTraits<T>());
    scriptingEngine->registerObjectType(name.c_str(), size, typeFlags);

    if ((objectTypeFlags & asOBJ_NOCOUNT) == 0)
    {
//        scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(TemplateTypeRegisterHelper::DefaultConstructor), asCALL_CDECL_OBJFIRST);
//    	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(ref@)", asFUNCTION(HandleBase::InitConstructor), asCALL_CDECL_OBJFIRST);
//    	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
    }
}

/**
 * Register our EngineResourceManager template type bindings.
 */
void registerEngineResourceManagerTemplateTypeBindings(scripting::IScriptingEngine* scriptingEngine)
{
    const std::string className = "EngineResourceManager<class T>";
    registerTemplateTypeBindings(scriptingEngine, className.c_str(), "IFile");
}

//template class EngineResourceManager<graphics::MeshHandle>;
//template const graphics::MeshHandle& EngineResourceManager<graphics::MeshHandle>::create(const std::string&, const graphics::IMesh&);

//template<typename T>
//class EngineResourceManagerTemplateTypeRegisterHelper
//{
//public:
//    static const graphics::MeshHandle& create(EngineResourceManager<graphics::MeshHandle>* engineResourceManager, const std::string& name, const graphics::IMesh& mesh)
//    {
//        printf("TRYING...\n");
//        return engineResourceManager->create(name, mesh);
//    }
//};

/**
 * Register our EngineResourceManager template type specialization bindings.
 */
template<typename T>
void registerEngineResourceManagerTemplateTypeSpecializationBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name)
{
    const std::string className = detail::format("EngineResourceManager<%s>", name);
//    registerTemplateTypeBindings(scriptingEngine, className.c_str(), "IFile");

    registerTemplateTypeSpecializationBindings(scriptingEngine, className, "IFile");
//    EngineResourceManager<T>, create, (const std::string&, const graphics::IMesh&), const T&

//    template class EngineResourceManager<T>::create<const graphics::IMesh&>;

//    scriptingEngine->registerClassMethod(className.c_str(), detail::format("const %s& create(const string& in, const IMesh& in)", name), asMETHODPR(EngineResourceManager<T>, create, (const std::string&, const graphics::IMesh&), const T&));

//    scriptingEngine->registerObjectMethod(className.c_str(), detail::format("const %s& create(const string& in, const IMesh& in)", name), asFUNCTION(EngineResourceManagerTemplateTypeRegisterHelper::create), asCALL_CDECL_OBJFIRST);

    scriptingEngine->registerClassMethod(className.c_str(), "void destroy(const string& in)", asMETHODPR(EngineResourceManager<T>, destroy, (const std::string&), void));
    scriptingEngine->registerClassMethod(className.c_str(), detail::format("void destroy(const %s& in)", name), asMETHODPR(EngineResourceManager<T>, destroy, (const T&), void));
    scriptingEngine->registerClassMethod(className.c_str(), "void destroyAll()", asMETHOD(EngineResourceManager<T>, destroyAll));
    scriptingEngine->registerClassMethod(className.c_str(), "bool exists(const string& in) const", asMETHODPR(EngineResourceManager<T>, exists, (const std::string&) const, bool));
    scriptingEngine->registerClassMethod(className.c_str(), detail::format("const %s& get(const string& in) const", name), asMETHODPR(EngineResourceManager<T>, get, (const std::string&) const, const T&));
}

void mrtest(int* a, std::string* errs)
{
    *a = 10;
    *errs = std::string("wtffffff");
}

const std::vector<byte>& iImageDataProxy(const IImage* image)
{
    return image->data();
}

uint32 iImageWidthProxy(const IImage* image)
{
    return image->width();
}

uint32 iImageHeightProxy(const IImage* image)
{
    return image->height();
}

void BindingDelegate::bind()
{
    scriptingEngine_->registerGlobalFunction("void mrtest(int& out, string& out)", asFUNCTION(mrtest), asCALL_CDECL);

	scriptingEngine_->registerObjectType("IImage", 0, asOBJ_REF | asOBJ_NOCOUNT);

    scriptingEngine_->registerObjectMethod("IImage", "const vectorUInt8& data() const", asFUNCTION(iImageDataProxy), asCALL_CDECL_OBJFIRST);
    scriptingEngine_->registerObjectMethod("IImage", "uint32 width() const", asFUNCTION(iImageWidthProxy), asCALL_CDECL_OBJFIRST);
    scriptingEngine_->registerObjectMethod("IImage", "uint32 height() const", asFUNCTION(iImageHeightProxy), asCALL_CDECL_OBJFIRST);
//	scriptingEngine_->registerClassMethod("IImage", "const vectorUInt8& data() const", asMETHOD(IImage, data));
//	scriptingEngine_->registerClassMethod("IImage", "uint32 width() const", asMETHOD(IImage, width));
//	scriptingEngine_->registerClassMethod("IImage", "uint32 height() const", asMETHOD(IImage, height));

    scriptingEngine_->registerEnum("IImageFormat");
    scriptingEngine_->registerEnumValue("IImageFormat", "FORMAT_UNKNOWN", IImage::Format::FORMAT_UNKNOWN);
    scriptingEngine_->registerEnumValue("IImageFormat", "FORMAT_RGB", IImage::Format::FORMAT_RGB);
    scriptingEngine_->registerEnumValue("IImageFormat", "FORMAT_RGBA", IImage::Format::FORMAT_RGBA);

    scriptingEngine_->registerObjectType("Image", sizeof(Image), asOBJ_VALUE | asGetTypeTraits<Image>());
    scriptingEngine_->registerObjectBehaviour("Image", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Image>), asCALL_CDECL_OBJLAST);
    scriptingEngine_->registerObjectBehaviour("Image", asBEHAVE_CONSTRUCT, "void f(const vectorUInt8& in, const uint32, const uint32, const IImageFormat)", asFUNCTION(InitConstructorImage), asCALL_CDECL_OBJFIRST);
    scriptingEngine_->registerObjectBehaviour("Image", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Image>), asCALL_CDECL_OBJLAST);
    scriptingEngine_->registerClassMethod("Image", "Image& opAssign(const Image& in)", asMETHODPR(Image, operator=, (const Image&), Image&));
    scriptingEngine_->registerClassMethod("Image", "const vectorUInt8& data() const", asMETHOD(Image, data));
    scriptingEngine_->registerClassMethod("Image", "uint32 width() const", asMETHOD(Image, width));
    scriptingEngine_->registerClassMethod("Image", "uint32 height() const", asMETHOD(Image, height));

	scriptingEngine_->registerObjectType("Audio", 0, asOBJ_REF | asOBJ_NOCOUNT);

    scriptingEngine_->registerEnum("FileFlags");
    scriptingEngine_->registerEnumValue("FileFlags", "READ", fs::FileFlags::READ);
    scriptingEngine_->registerEnumValue("FileFlags", "WRITE", fs::FileFlags::WRITE);
    scriptingEngine_->registerEnumValue("FileFlags", "APPEND", fs::FileFlags::APPEND);
    scriptingEngine_->registerEnumValue("FileFlags", "BINARY", fs::FileFlags::BINARY);

    scriptingEngine_->registerObjectType("IFile", 0, asOBJ_REF | asOBJ_NOCOUNT);
    scriptingEngine_->registerClassMethod("IFile", "string path()", asMETHOD(fs::IFile, path));
    scriptingEngine_->registerClassMethod("IFile", "void close()", asMETHOD(fs::IFile, close));
    scriptingEngine_->registerClassMethod("IFile", "void write(const string& in)", asMETHODPR(fs::IFile, write, (const std::string&), void));
    scriptingEngine_->registerClassMethod("IFile", "string readAll()", asMETHOD(fs::IFile, readAll));
    registerUniquePtrBindings<fs::IFile>(scriptingEngine_, "unique_ptrIFile", "IFile");

	scriptingEngine_->registerObjectType("IFileSystem", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IFileSystem fileSystem", gameEngine_->fileSystem());
	scriptingEngine_->registerClassMethod("IFileSystem", "bool exists(const string& in) const", asMETHOD(fs::IFileSystem, exists));
	scriptingEngine_->registerClassMethod("IFileSystem", "bool isDirectory(const string& in) const", asMETHOD(fs::IFileSystem, isDirectory));
	scriptingEngine_->registerClassMethod("IFileSystem", "vectorString list(const string& in) const", asMETHOD(fs::IFileSystem, list));
	scriptingEngine_->registerClassMethod("IFileSystem", "void deleteFile(const string& in) const", asMETHOD(fs::IFileSystem, deleteFile));
	scriptingEngine_->registerClassMethod("IFileSystem", "void makeDirectory(const string& in) const", asMETHOD(fs::IFileSystem, makeDirectory));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getBasePath(const string& in) const", asMETHOD(fs::IFileSystem, getBasePath));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getDirectorySeperator() const", asMETHOD(fs::IFileSystem, getDirectorySeperator));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getTempDirectory() const", asMETHOD(fs::IFileSystem, getTempDirectory));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getFilename(const string& in) const", asMETHOD(fs::IFileSystem, getFilename));
	scriptingEngine_->registerClassMethod("IFileSystem", "string getFilenameWithoutExtension(const string& in) const", asMETHOD(fs::IFileSystem, getFilenameWithoutExtension));
	scriptingEngine_->registerClassMethod("IFileSystem", "string readAll(const string& in) const", asMETHOD(fs::IFileSystem, readAll));
	scriptingEngine_->registerClassMethod("IFileSystem", "unique_ptrIFile open(const string& in, const int32) const", asMETHOD(fs::IFileSystem, open));
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
	scriptingEngine_->registerGlobalFunction("void terminate()", asFUNCTIONPR(std::terminate, (), void), asCALL_CDECL);
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
    scriptingEngine_->registerObjectMethod("Mesh", "IMesh@ opImplCast()", asFUNCTION((refCast<Mesh, graphics::IMesh>)), asCALL_CDECL_OBJLAST);
    scriptingEngine_->registerObjectMethod("Mesh", "const IMesh@ opImplCast() const", asFUNCTION((refCast<Mesh, graphics::IMesh>)), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Mesh", "const VertexBoneData& vertexBoneData() const", asMETHOD(Mesh, vertexBoneData));

	scriptingEngine_->registerObjectType("Texture", sizeof(Texture), asOBJ_VALUE | asGetTypeTraits<Texture>());
	scriptingEngine_->registerObjectBehaviour("Texture", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Texture>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("Texture", asBEHAVE_CONSTRUCT, "void f(string, IImage@)", asFUNCTION(InitConstructorTexture), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Texture", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Texture>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Texture", "Texture& opAssign(const Texture& in)", asMETHODPR(Texture, operator=, (const Texture&), Texture&));

	scriptingEngine_->registerObjectType("Skeleton", sizeof(Skeleton), asOBJ_VALUE | asGetTypeTraits<Skeleton>());
	scriptingEngine_->registerObjectBehaviour("Skeleton", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Skeleton>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("Skeleton", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Skeleton>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Skeleton", "Skeleton& opAssign(const Skeleton& in)", asMETHODPR(Skeleton, operator=, (const Skeleton&), Skeleton&));

	scriptingEngine_->registerObjectType("AnimatedBoneNode", sizeof(AnimatedBoneNode), asOBJ_VALUE | asGetTypeTraits<AnimatedBoneNode>());
    scriptingEngine_->registerObjectBehaviour("AnimatedBoneNode", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<AnimatedBoneNode>), asCALL_CDECL_OBJLAST);
    scriptingEngine_->registerObjectBehaviour("AnimatedBoneNode", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<AnimatedBoneNode>), asCALL_CDECL_OBJLAST);
    scriptingEngine_->registerClassMethod("AnimatedBoneNode", "AnimatedBoneNode& opAssign(const AnimatedBoneNode& in)", asMETHODPR(AnimatedBoneNode, operator=, (const AnimatedBoneNode&), AnimatedBoneNode&));
    scriptingEngine_->registerObjectProperty("AnimatedBoneNode", "string name", asOFFSET(AnimatedBoneNode, name));
    scriptingEngine_->registerObjectProperty("AnimatedBoneNode", "vectorDouble positionTimes", asOFFSET(AnimatedBoneNode, positionTimes));
    scriptingEngine_->registerObjectProperty("AnimatedBoneNode", "vectorDouble rotationTimes", asOFFSET(AnimatedBoneNode, rotationTimes));
    scriptingEngine_->registerObjectProperty("AnimatedBoneNode", "vectorDouble scalingTimes", asOFFSET(AnimatedBoneNode, scalingTimes));
    scriptingEngine_->registerObjectProperty("AnimatedBoneNode", "vectorVec3 positions", asOFFSET(AnimatedBoneNode, positions));
    scriptingEngine_->registerObjectProperty("AnimatedBoneNode", "vectorQuat rotations", asOFFSET(AnimatedBoneNode, rotations));
    scriptingEngine_->registerObjectProperty("AnimatedBoneNode", "vectorVec3 scalings", asOFFSET(AnimatedBoneNode, scalings));

    registerUnorderedMapBindings<std::string, AnimatedBoneNode>(scriptingEngine_, "unordered_mapStringAnimatedBoneNode", "string", "AnimatedBoneNode");

	scriptingEngine_->registerObjectType("Animation", sizeof(Animation), asOBJ_VALUE | asGetTypeTraits<Animation>());
	scriptingEngine_->registerObjectBehaviour("Animation", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Animation>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectBehaviour("Animation", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Animation>), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerClassMethod("Animation", "Animation& opAssign(const Animation& in)", asMETHODPR(Animation, operator=, (const Animation&), Animation&));
	scriptingEngine_->registerClassMethod("Animation", "const string& name() const", asMETHOD(Animation, name));
	scriptingEngine_->registerClassMethod("Animation", "double duration() const", asMETHOD(Animation, duration));
	scriptingEngine_->registerClassMethod("Animation", "double ticksPerSecond() const", asMETHOD(Animation, ticksPerSecond));
	scriptingEngine_->registerClassMethod("Animation", "const unordered_mapStringAnimatedBoneNode& animatedBoneNodes() const", asMETHOD(Animation, animatedBoneNodes));

	registerVectorBindings<Mesh>(scriptingEngine_, "vectorMesh", "Mesh");
	registerVectorBindings<Texture>(scriptingEngine_, "vectorTexture", "Texture");
    registerUnorderedMapBindings<std::string, Animation>(scriptingEngine_, "unordered_mapStringAnimation", "string", "Animation");

	scriptingEngine_->registerObjectType("Model", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("Model", "const vectorMesh& meshes() const", asMETHOD(Model, meshes));
	scriptingEngine_->registerClassMethod("Model", "const vectorTexture& textures() const", asMETHOD(Model, textures));
	scriptingEngine_->registerClassMethod("Model", "const Skeleton& skeleton() const", asMETHOD(Model, skeleton));
	scriptingEngine_->registerClassMethod("Model", "const unordered_mapStringAnimation& animations() const", asMETHOD(Model, animations));

	// IGame
	scriptingEngine_->registerInterface("IGame");
	scriptingEngine_->registerInterfaceMethod("IGame", "void initialize()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void destroy()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void tick(const float)");

	// Types available in the scripting engine
	registerHandleBindings<ModelHandle>(scriptingEngine_, "ModelHandle");
	registerHandleBindings<SkeletonHandle>(scriptingEngine_, "SkeletonHandle");
	registerHandleBindings<AnimationHandle>(scriptingEngine_, "AnimationHandle");
	registerHandleBindings<graphics::BonesHandle>(scriptingEngine_, "BonesHandle");

	scriptingEngine_->registerObjectType("PbrMaterial", sizeof(PbrMaterial), asOBJ_VALUE | asGetTypeTraits<PbrMaterial>());
	//scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<PbrMaterial>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_CONSTRUCT, "void f(IImage@, IImage@, IImage@, IImage@, IImage@ = null)", asFUNCTION(InitConstructorPbrMaterial), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_CONSTRUCT, "void f(const PbrMaterial& in)", asFUNCTION(CopyConstructor<PbrMaterial>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PbrMaterial", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<PbrMaterial>), asCALL_CDECL_OBJFIRST);
	registerVectorBindings<PbrMaterial>(scriptingEngine_, "vectorPbrMaterial", "PbrMaterial");

	scriptingEngine_->registerObjectType("HeightMap", sizeof(HeightMap), asOBJ_VALUE | asGetTypeTraits<HeightMap>());
	scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<HeightMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_CONSTRUCT, "void f(const IImage& in)", asFUNCTIONPR(InitConstructorHeightMap, (HeightMap*, const IImage&), void), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("HeightMap",asBEHAVE_CONSTRUCT, "void f(const vectorUInt8& in, const uint32, const uint32)", asFUNCTIONPR(InitConstructorHeightMap, (HeightMap*, const std::vector<uint8>&, const uint32, const uint32), void), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_CONSTRUCT, "void f(const HeightMap& in)", asFUNCTION(CopyConstructor<HeightMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("HeightMap", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<HeightMap>), asCALL_CDECL_OBJFIRST);
    scriptingEngine_->registerClassMethod("HeightMap", "HeightMap& opAssign(const HeightMap& in)", asMETHOD(HeightMap, operator=));
	scriptingEngine_->registerClassMethod(
		"HeightMap",
		"IImage@ image()",
		asMETHODPR(HeightMap, image, (), IImage*)
	);

	scriptingEngine_->registerObjectType("SplatMap", sizeof(SplatMap), asOBJ_VALUE | asGetTypeTraits<SplatMap>());
	scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<SplatMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_CONSTRUCT, "void f(vectorPbrMaterial, IImage@)", asFUNCTION(InitConstructorSplatMap), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_CONSTRUCT, "void f(const SplatMap& in)", asFUNCTION(CopyConstructor<SplatMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("SplatMap", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<SplatMap>), asCALL_CDECL_OBJFIRST);
    scriptingEngine_->registerClassMethod("SplatMap", "SplatMap& opAssign(const SplatMap& in)", asMETHOD(SplatMap, operator=));
    scriptingEngine_->registerClassMethod("SplatMap", "const vectorIPbrMaterial& materialMap() const", asMETHOD(SplatMap, materialMap));

	scriptingEngine_->registerObjectType("DisplacementMap", sizeof(DisplacementMap), asOBJ_VALUE | asGetTypeTraits<DisplacementMap>());
	scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<DisplacementMap>), asCALL_CDECL_OBJFIRST);
	//scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_CONSTRUCT, "void f(const IImage& in, vectorPbrMaterial in)", asFUNCTION(InitConstructorDisplacementMap), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_CONSTRUCT, "void f(const DisplacementMap& in)", asFUNCTION(CopyConstructor<DisplacementMap>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("DisplacementMap", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<DisplacementMap>), asCALL_CDECL_OBJFIRST);

	scriptingEngine_->registerObjectType("Heightfield", sizeof(Heightfield), asOBJ_VALUE | asGetTypeTraits<Heightfield>());
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<Heightfield>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_CONSTRUCT, "void f(const IImage& in)", asFUNCTION(InitConstructorHeightfield), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_CONSTRUCT, "void f(const Heightfield& in)", asFUNCTION(CopyConstructor<Heightfield>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Heightfield", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<Heightfield>), asCALL_CDECL_OBJFIRST);
    scriptingEngine_->registerClassMethod("Heightfield", "Heightfield& opAssign(const Heightfield& in)", asMETHOD(Heightfield, operator=));

	scriptingEngine_->registerObjectType("PathfindingTerrain", sizeof(PathfindingTerrain), asOBJ_VALUE | asGetTypeTraits<PathfindingTerrain>());
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<PathfindingTerrain>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_CONSTRUCT, "void f(const HeightMap& in)", asFUNCTION(InitConstructorPathfindingTerrain), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_CONSTRUCT, "void f(const PathfindingTerrain& in)", asFUNCTION(CopyConstructor<PathfindingTerrain>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PathfindingTerrain", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<PathfindingTerrain>), asCALL_CDECL_OBJFIRST);
    scriptingEngine_->registerClassMethod("PathfindingTerrain", "PathfindingTerrain& opAssign(const PathfindingTerrain& in)", asMETHOD(PathfindingTerrain, operator=));

	registerSharedFutureBindings<IImage*>(scriptingEngine_, "shared_futureImage", "IImage@");
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

	scriptingEngine_->registerObjectMethod("Image", "IImage@ opImplCast()", asFUNCTION((refCast<Image, IImage>)), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectMethod("Image", "GraphicsIImage@ opImplCast()", asFUNCTION((refCast<Image, graphics::IImage>)), asCALL_CDECL_OBJLAST);

	// Register function declarations
	scriptingEngine_->registerFunctionDefinition("void WorkFunction()");

	// Listeners
	scriptingEngine_->registerInterface("IWindowEventListener");
	scriptingEngine_->registerInterfaceMethod("IWindowEventListener", "bool processEvent(const WindowEvent& in)");
	scriptingEngine_->registerInterface("IKeyboardEventListener");
	scriptingEngine_->registerInterfaceMethod("IKeyboardEventListener", "bool processEvent(const KeyboardEvent& in)");
	scriptingEngine_->registerInterface("ITextInputEventListener");
	scriptingEngine_->registerInterfaceMethod("ITextInputEventListener", "bool processEvent(const TextInputEvent& in)");
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

    scriptingEngine_->registerInterface("IScriptingEngineDebugHandler");
    scriptingEngine_->registerInterfaceMethod("IScriptingEngineDebugHandler", "void tick(const float)");

//	scriptingEngine_->registerInterface("ScriptObject");

	scriptingEngine_->registerGlobalFunction(
		"ScriptObjectHandle createScriptObjectHandle(IScriptObject@)",
		asFUNCTIONPR(createScriptObjectHandle, (void*), scripting::ScriptObjectHandle),
		asCALL_CDECL
	);

//    registerUnorderedMapBindings<std::string, std::string>(scriptingEngine_, "unordered_mapStringString", "string", "string");

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
	scriptingEngine_->registerGlobalProperty("IDebugRenderer debugRenderer", gameEngine_->debugRenderer());
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
		"IGraphicsEngine@ graphicsEngine()",
		asMETHODPR(GameEngine, graphicsEngine, () const, graphics::IGraphicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IPhysicsEngine@ physicsEngine()",
		asMETHODPR(GameEngine, physicsEngine, () const, physics::IPhysicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ backgroundThreadPool()",
		asMETHODPR(GameEngine, backgroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ foregroundThreadPool()",
		asMETHODPR(GameEngine, foregroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IOpenGlLoader@ openGlLoader()",
		asMETHODPR(GameEngine, openGlLoader, () const, IOpenGlLoader*),
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
		"void addTextInputEventListener(ITextInputEventListener@)",
		asMETHODPR(GameEngine, addTextInputEventListener, (void*), void),
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
		"void removeTextInputEventListener(ITextInputEventListener@)",
		asMETHODPR(GameEngine, removeTextInputEventListener, (void*), void),
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
            "void addScriptingEngineDebugHandler(IScriptingEngineDebugHandler@)",
            asMETHODPR(GameEngine, addScriptingEngineDebugHandler, (void*), void),
            asCALL_THISCALL_ASGLOBAL,
            gameEngine_
    );
    scriptingEngine_->registerGlobalFunction(
            "void removeScriptingEngineDebugHandler(IScriptingEngineDebugHandler@)",
            asMETHODPR(GameEngine, removeScriptingEngineDebugHandler, (const void*), void),
            asCALL_THISCALL_ASGLOBAL,
            gameEngine_
    );
	scriptingEngine_->registerGlobalFunction(
		"shared_futureVoid postWorkToForegroundThreadPool(WorkFunction@)",
		asMETHODPR(GameEngine, postWorkToForegroundThreadPool, (void*), std::shared_future<void>),
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
            "shared_futureVoid postWorkToOpenGlWorker(WorkFunction@)",
            asMETHODPR(GameEngine, postWorkToOpenGlWorker, (void*), std::shared_future<void>),
            asCALL_THISCALL_ASGLOBAL,
            gameEngine_
    );






    // EngineResourceManager<graphics::MeshHandle>
//    registerTemplateTypeBindings(scriptingEngine_, "EngineResourceManager<class T>", "IFile");
//    registerTemplateTypeSpecializationBindings(scriptingEngine_, "EngineResourceManager<MeshHandle>", "IFile");
    registerEngineResourceManagerTemplateTypeBindings(scriptingEngine_);
    registerEngineResourceManagerTemplateTypeSpecializationBindings<graphics::MeshHandle>(scriptingEngine_, "MeshHandle");
    scriptingEngine_->registerClassMethod("EngineResourceManager<MeshHandle>", detail::format("const %s& create(const string& in, const IMesh& in)", "MeshHandle"), asMETHODPR(EngineResourceManager<graphics::MeshHandle>, create, (const std::string&, const graphics::IMesh&), const graphics::MeshHandle&));

//    scriptingEngine_->registerObjectMethod("EngineResourceManager<MeshHandle>", detail::format("const %s& create(const string& in, const IMesh& in)", "MeshHandle"), asFUNCTION(EngineResourceManagerTemplateTypeRegisterHelper::create), asCALL_CDECL_OBJFIRST);

    scriptingEngine_->registerGlobalFunction(
            "EngineResourceManager<MeshHandle>@ engineResourceManagerMeshHandle()",
            asMETHOD(GameEngine, engineResourceManager<graphics::MeshHandle>),
            asCALL_THISCALL_ASGLOBAL,
            gameEngine_
    );











	scriptingEngine_->registerGlobalFunction(
		"Model@ importModel(const string& in, const string& in)",
		asMETHOD(GameEngine, importModel),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureModel importModelAsync(const string& in, const string& in)",
		asMETHOD(GameEngine, importModelAsync),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Audio@ loadAudio(const string& in, const string& in)",
		asMETHOD(GameEngine, loadAudio),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureAudio loadAudioAsync(const string& in, const string& in)",
		asMETHOD(GameEngine, loadAudioAsync),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IImage@ createImage(const string& in, const vectorUInt8& in, const uint, const uint, const IImageFormat)",
		asMETHOD(GameEngine, createImage),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IImage@ loadImage(const string& in, const string& in)",
		asMETHOD(GameEngine, loadImage),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureImage loadImageAsync(const string& in, const string& in)",
		asMETHOD(GameEngine, loadImageAsync),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Model@ getModel(const string& in)",
		asMETHOD(GameEngine, getModel),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Audio@ getAudio(const string& in)",
		asMETHOD(GameEngine, getAudio),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IImage@ getImage(const string& in)",
		asMETHOD(GameEngine, getImage),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadModel(const string& in)",
		asMETHOD(GameEngine, unloadModel),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadAudio(const string& in)",
		asMETHOD(GameEngine, unloadAudio),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadImage(const string& in)",
		asMETHOD(GameEngine, unloadImage),
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
		"void destroy(const string& in)",
		asMETHODPR(GameEngine, destroyShader, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroy(const VertexShaderHandle& in)",
		asMETHODPR(GameEngine, destroyShader, (const graphics::VertexShaderHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroy(const FragmentShaderHandle& in)",
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
//	scriptingEngine_->registerGlobalFunction(
//		"void destroy(const string& in)",
//		asMETHODPR(GameEngine, destroyShaderProgram, (const std::string&), void),
//		asCALL_THISCALL_ASGLOBAL,
//		gameEngine_
//	);
	scriptingEngine_->registerGlobalFunction(
		"void destroy(const ShaderProgramHandle& in)",
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
	 	"CollisionShapeHandle createStaticSphereShape(const string& in, const float)",
	 	asMETHODPR(GameEngine, createStaticSphereShape, (const std::string&, const float32), physics::CollisionShapeHandle),
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
//		"void destroy(const CollisionShapeHandle& in)",
//		asMETHODPR(GameEngine, destroy, (const physics::CollisionShapeHandle&), void)
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
			"SkyboxHandle createStaticSkybox(const string& in, const IImage& in, const IImage& in, const IImage& in, const IImage& in, const IImage& in, const IImage& in)",
			asMETHODPR(GameEngine, createStaticSkybox, (const std::string&, const IImage&, const IImage&, const IImage&, const IImage&, const IImage&, const IImage&), graphics::SkyboxHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"SkyboxHandle getStaticSkybox(const string& in)",
			asMETHODPR(GameEngine, getStaticSkybox, (const std::string&) const, graphics::SkyboxHandle),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"SoundHandle createSound(const string& in, const Audio& in)",
			asMETHOD(GameEngine, createSound),
			asCALL_THISCALL_ASGLOBAL,
			gameEngine_
		);
	scriptingEngine_->registerGlobalFunction(
			"SoundHandle getSound(const string& in)",
			asMETHOD(GameEngine, getSound),
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
		"Scene@ createScene(const string& in, const vectorString& in = vectorString(), const string& in = \"\")",
		asMETHODPR(GameEngine, createScene, (const std::string&, const std::vector<std::string>&, const std::string&), Scene*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Scene@ createScene(const string& in, const ModuleHandle, const string& in = \"\")",
		asMETHODPR(GameEngine, createScene, (const std::string&, const scripting::ModuleHandle, const std::string&), Scene*),
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
		"void destroyScene(const Scene@)",
		asMETHODPR(GameEngine, destroyScene, (const Scene*), void),
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
