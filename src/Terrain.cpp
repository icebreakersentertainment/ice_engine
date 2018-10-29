#include <cmath>

#include <FreeImage.h>

#include "ecs/PositionComponent.hpp"
#include "ecs/RigidBodyObjectComponent.hpp"
#include "ecs/GraphicsTerrainComponent.hpp"

#include "ModelLoader.hpp"

#include "Terrain.hpp"

#include "Scene.hpp"

#include "Heightfield.hpp"
#include "Image.hpp"

#include "utilities/IoUtilities.hpp"

namespace ice_engine
{

void flipVertical(Image& image)
{
	std::vector<char> data;
	data.resize(image.width()*image.height()*4);
	
	int j=data.size()-image.width()*4;
	for (int i=0; i < data.size(); i+=image.width()*4)
	{
		for (int k=0; k < image.width()*4; ++k)
		{
			data[i+k] = image.data()[j+k];
		}
		
		j-=image.width()*4;
	}
	
	image.data_ = std::move(data);
}

// Source: http://www.flashbang.se/archives/155
void saveImageToFile(fs::IFile* file, const Image& image)
{
	std::vector<char> data;
	if (image.format() == Image::Format::FORMAT_RGBA)
	{
		// Convert from RGBA to RGB
		data.resize(image.data().size()/4 * 3);
		
		int j=0;
		for (int i=0; i < data.size(); i+=3)
		{
			data[i] = image.data()[j];
			data[i+1] = image.data()[j+1];
			data[i+2] = image.data()[j+2];
			
			j+=4;
		}
	}
	else
	{
		data = image.data();
	}
	
	printf("WTF2: %i %i %i\n", image.data().size(), image.width(), image.height());
	printf("WTF2: %i\n", data.size());
	
	/*
	BYTE pixels [3*257*257];
	FIBITMAP* Image = FreeImage_ConvertFromRawBits(reinterpret_cast<BYTE*>(&data[0]), image.width(), image.height(), 3*image.width(), 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_BMP, Image, "../assets/test.bmp", 0);
	*/
	
	//glReadPixels(0,0,x,y, GL_BGR,GL_UNSIGNED_BYTE,data);// split x and y sizes into bytes
	int xa= image.width() % 256;
	int xb= (image.width()-xa)/256;
	int ya= image.height() % 256;
	int yb= (image.height()-ya)/256;//assemble the header
	unsigned char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(unsigned char)xa,(unsigned char)xb,(unsigned char)ya,(unsigned char)yb,24,0};
	
	// write header and data to file
	auto& ostream = file->getOutputStream();
	ostream.write (reinterpret_cast<char *>(header), sizeof (char)*18);
	ostream.write (reinterpret_cast<char *>(&data[0]), sizeof (char)*data.size());
	//delete[] data;
	//data=NULL;
}

void saveAlphaImageToFile(fs::IFile* file, const Image& image)
{
	std::vector<char> data;
	if (image.format() == Image::Format::FORMAT_RGBA)
	{
		// Convert from RGBA to RGB
		data.resize(image.data().size()/4 * 3);
		
		int j=0;
		for (int i=0; i < data.size(); i+=3)
		{
			data[i] = image.data()[j+3];
			data[i+1] = image.data()[j+3];
			data[i+2] = image.data()[j+3];
			
			j+=4;
		}
	}
	else
	{
		data = image.data();
	}
	
	/*
	BYTE pixels [3*257*257];
	FIBITMAP* Image = FreeImage_ConvertFromRawBits(reinterpret_cast<BYTE*>(&data[0]), image.width(), image.height(), 3*image.width(), 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_BMP, Image, "../assets/test.bmp", 0);
	*/
	
	//glReadPixels(0,0,x,y, GL_BGR,GL_UNSIGNED_BYTE,data);// split x and y sizes into bytes
	int xa= image.width() % 256;
	int xb= (image.width()-xa)/256;
	int ya= image.height() % 256;
	int yb= (image.height()-ya)/256;//assemble the header
	unsigned char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(unsigned char)xa,(unsigned char)xb,(unsigned char)ya,(unsigned char)yb,24,0};
	
	// write header and data to file
	auto& ostream = file->getOutputStream();
	ostream.write (reinterpret_cast<char *>(header), sizeof (char)*18);
	ostream.write (reinterpret_cast<char *>(&data[0]), sizeof (char)*data.size());
	//delete[] data;
	//data=NULL;
}

Terrain::Terrain(
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger,
	Scene* scene,
	HeightMap heightMap,
	SplatMap splatMap,
	DisplacementMap displacementMap,
	physics::CollisionShapeHandle collisionShapeHandle,
	pathfinding::PolygonMeshHandle polygonMeshHandle,
	pathfinding::NavigationMeshHandle navigationMeshHandle,
	graphics::IGraphicsEngine* graphicsEngine,
	pathfinding::IPathfindingEngine* pathfindingEngine,
	physics::IPhysicsEngine* physicsEngine,
	audio::IAudioEngine* audioEngine,
	audio::AudioSceneHandle audioSceneHandle,
	graphics::RenderSceneHandle renderSceneHandle,
	physics::PhysicsSceneHandle physicsSceneHandle,
	pathfinding::PathfindingSceneHandle pathfindingSceneHandle
)
	:
	audioEngine_(audioEngine),
	graphicsEngine_(graphicsEngine),
	physicsEngine_(physicsEngine),
	pathfindingEngine_(pathfindingEngine),
	properties_(properties),
	fileSystem_(fileSystem),
	logger_(logger),
	scene_(scene),
	audioSceneHandle_(audioSceneHandle),
	renderSceneHandle_(renderSceneHandle),
	physicsSceneHandle_(physicsSceneHandle),
	pathfindingSceneHandle_(pathfindingSceneHandle)
{
	//auto file = fileSystem->open("../assets/placeholders/textures/Heightmap.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
	//auto image = std::make_unique<Image>( file->getInputStream() );

	//flipVertical(*image);
	//auto collisionShapeHandle = physicsEngine_->createStaticTerrainShape(image.get());

	terrainEntity_ = scene_->createEntity();

	//ecs::RigidBodyObjectComponent pc;
	//pc.rigidBodyObjectHandle = rigidBodyObjectHandle;
	//scene_.assign(groundEntity_, gc);
	//scene_->assign(terrainEntity_, pc);
//
//	int32 dataSize = (heightMap.image()->format() == Image::Format::FORMAT_RGBA ? 4 : 3);
//
//	std::vector<char> heightMapData;
//	heightMapData.resize(heightMap.image()->width() * heightMap.image()->height());
//
//	int j=0;
//	for (int i=0; i < heightMapData.size(); ++i)
//	{
//		//heightMapData[i] = (heightMap.image()->data()[j] + heightMap.image()->data()[j+1] + heightMap.image()->data()[j+2]) / 3;
//		heightMapData[i] = heightMap.image()->data()[j+3];
//
//		j+=dataSize;
//	}

//	auto heightfield = Heightfield(*heightMap.image());
//	auto collisionShapeHandle = physicsEngine_->createStaticTerrainShape(&heightfield);

	terrainEntity_.assign<ecs::PositionComponent>(glm::vec3(-0.5f, -7.5f, -0.5f));
	terrainEntity_.assign<ecs::RigidBodyObjectComponent>(collisionShapeHandle, 0.0f, 1.0f, 1.0f);
	//auto rigidBodyObjectHandle = physicsEngine_->createRigidBodyObject(physicsSceneHandle_, collisionShapeHandle, glm::vec3(-0.5f, -7.5f, -0.5f), glm::quat(), 0.0f, 1.0f, 1.0f);
	
	//flipVertical(*image);
	//if (image->format == Image::Format::FORMAT_RGB)
	//{
	//	image = generateFormattedHeightmap(*image);
	//}

	//HeightMap heightMap(*image);
	//SplatMap splatMap(*heightMap.image(), fileSystem);
	//DisplacementMap displacementMap = DisplacementMap();
	/*
	{
		auto fileOut = fileSystem->open("../assets/im1.tga", fs::FileFlags::WRITE | fs::FileFlags::BINARY);
		saveImageToFile(fileOut.get(), *image);
		fileOut->close();
	}

	{
		auto fileOut = fileSystem->open("../assets/im2.tga", fs::FileFlags::WRITE | fs::FileFlags::BINARY);
		saveAlphaImageToFile(fileOut.get(), *image);
		fileOut->close();
	}
	*/
	
	//graphics::SplatMap splatMap = generateSplatMap(*image, fileSystem);
	//graphics::HeightMap heightMap = graphics::HeightMap(image.get());
	//graphics::DisplacementMap displacementMap;
	
	//flipVertical(*image);
	//terrainHandle_ = graphicsEngine->createTerrain(renderSceneHandle_, &heightMap, &splatMap, &displacementMap);
	//terrainEntity_.assign<ecs::GraphicsTerrainComponent>(heightMap, splatMap, displacementMap);
	
	try
	{
//		std::vector<glm::vec3> vertices;
//		std::vector<uint32> indices;
//		std::tie(vertices, indices) = detail::generateGrid(256);
//
//	    for (auto& v : vertices )
//		{
//			v.y = (((float32)(heightMap.height((uint32)v.x, (uint32)v.z)) / 255.0f) * 15.0f) - 7.5f;
//			v.x = v.x - 128.0f;
//			v.z = v.z - 128.0f;
//		}
//
//        for (int i=1; i < indices.size()-1; i+=3)
//        {
//			uint32 temp = indices[i-1];
//			indices[i-1] = indices[i+1];
//			indices[i+1] = temp;
//        }
	    
	    /*
	    vertices.clear();
		
		vertices.push_back( glm::vec3(0,0,0) );
		vertices.push_back( glm::vec3(10,0,0) );
		vertices.push_back( glm::vec3(0,0,10) );
		vertices.push_back( glm::vec3(10,0,10) );
		
		indices.clear();
		// First triangle
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
		// Second triangle
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
	    
	    
	    
	    
	    vertices.clear();
		
		vertices.push_back( glm::vec3(256,0,256) );
		vertices.push_back( glm::vec3(256,0,0) );
		vertices.push_back( glm::vec3(0,0,0) );
		vertices.push_back( glm::vec3(0,0,256) );
		
		for (auto& v : vertices )
		{
			v.x = v.x - 128.0f;
			v.z = v.z - 128.0f;
		}
		
		indices.clear();
		// First triangle
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		// Second triangle
		indices.push_back(3);
		indices.push_back(0);
		indices.push_back(2);
	    
	    /*
	
	    vertices.clear();
		
		vertices.push_back( glm::vec3(1,0,1) );
		vertices.push_back( glm::vec3(1,0,0) );
		vertices.push_back( glm::vec3(0,0,0) );
		vertices.push_back( glm::vec3(0,0,1) );
		
		indices.clear();
		// First triangle
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
		// Second triangle
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
	    */
	    /*
	    vertices.clear();
		
		vertices.push_back( glm::vec3(5.5f, -1.0f,  5.5f) );
		vertices.push_back( glm::vec3(5.5f, -1.0f, -5.5f) );
		vertices.push_back( glm::vec3(-5.5f, -1.0f, -5.5f) );
		vertices.push_back( glm::vec3(-5.5f, -1.0f,  5.5f) );
		
		indices.clear();
		// First triangle
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
		// Second triangle
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		*/
		
        //Mesh mesh;
        //mesh.vertices = std::move(vertices);
        //mesh.indices = std::move(indices);

//        pathfinding::PolygonMeshConfig polygonMeshConfig;
//        polygonMeshConfig.cellSize = 0.30;
//		polygonMeshConfig.cellHeight = 0.20;
//		polygonMeshConfig.walkableSlopeAngle = 45;
//		polygonMeshConfig.walkableHeight = (int)ceilf(2.0 / polygonMeshConfig.cellHeight); //(int)ceilf(m_agentHeight / polygonMeshConfig.cellHeight);
//		polygonMeshConfig.walkableClimb = (int)floorf(0.9 / polygonMeshConfig.cellHeight); //(int)floorf(m_agentMaxClimb / polygonMeshConfig.cellHeight);
//		polygonMeshConfig.walkableRadius = (int)ceilf(0.6 / polygonMeshConfig.cellSize); //(int)ceilf(m_agentRadius / polygonMeshConfig.cellSize);
//		polygonMeshConfig.maxEdgeLength = (int)(12 / polygonMeshConfig.cellSize); //(int)(m_edgeMaxLen / m_cellSize);
//		polygonMeshConfig.maxSimplificationError = 1.3f;
//		polygonMeshConfig.minRegionArea = (int)std::sqrt(8);
//		polygonMeshConfig.mergeRegionArea = (int)std::sqrt(20);
//		polygonMeshConfig.maxVertsPerPoly = (int)6;
//		polygonMeshConfig.detailSampleDist = 6 < 0.9f ? 0 : polygonMeshConfig.cellSize * 6; //m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
//		polygonMeshConfig.detailSampleMaxError = polygonMeshConfig.cellHeight * 1; //m_cellHeight * m_detailSampleMaxError;
		//polygonMeshHandle_ = gameEngine_->createPolygonMesh("test_terrain", vertices, indices, polygonMeshConfig);

        //polygonMeshHandle_ = pathfindingEngine->createPolygonMesh(vertices, indices, polygonMeshConfig);

//        pathfinding::NavigationMeshConfig navigationMeshConfig;
//        navigationMeshConfig.walkableHeight = 2.0f;
//        navigationMeshConfig.walkableClimb = 0.9f;
//        navigationMeshConfig.walkableRadius = 0.6f;
        //navigationMeshHandle_ = gameEngine_->createNavigationMesh(polygonMeshHandle_, navigationMeshConfig);

        //navigationMeshHandle_ = pathfindingEngine->createNavigationMesh(polygonMeshHandle_, navigationMeshConfig);

        //terrainEntity_.assign<ecs::GraphicsTerrainComponent>(std::move(heightMap), std::move(splatMap), std::move(displacementMap));
	}
	catch(const std::exception& e)
	{
		printf("what: %s\n", e.what());
		throw e;
	}
	
	//terrainEntity_ = scene_->createEntity();
	
	//GraphicsComponent gc;
	//gc.renderableHandle = renderableHandle;
	//ecs::RigidBodyObjectComponent pc;
	//pc.rigidBodyObjectHandle = rigidBodyObjectHandle;
	//scene_.assign(groundEntity_, gc);
	//scene_->assign(terrainEntity_, pc);
	
	auto crowdComponent = terrainEntity_.assign<ecs::PathfindingCrowdComponent>(pathfinding::NavigationMeshHandle(navigationMeshHandle));
	//auto crowdHandle = pathfindingEngine->createCrowd(pathfindingSceneHandle_, navigationMeshHandle_);
	crowdHandles_.push_back(crowdComponent->crowdHandle);

	/*
	graphics::PbrMaterial material;
	{
		auto file = fileSystem->open("../assets/placeholders/textures/Incense brass Metalic/incense thin_DefaultMaterial_BaseColor-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
		auto image = std::make_unique<Image>( file->getInputStream() );
	
		material.albedo = image.get();
		image.release();
	}
	
	{
		auto file = fileSystem->open("../assets/placeholders/textures/Incense brass Metalic/incense thin_DefaultMaterial_Normal-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
		auto image = std::make_unique<Image>( file->getInputStream() );
	
		material.normal = image.get();
		image.release();
	}
	{
		auto file = fileSystem->open("../assets/placeholders/textures/Incense brass Metalic/incense thin_DefaultMaterial_Metallic-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
		auto image = std::make_unique<Image>( file->getInputStream() );
	
		material.metalness = image.get();
		image.release();
	}
	{
		auto file = fileSystem->open("../assets/placeholders/textures/Incense brass Metalic/incense thin_DefaultMaterial_Roughness-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
		auto image = std::make_unique<Image>( file->getInputStream() );
	
		material.roughness = image.get();
		image.release();
	}
	*/
	/*
	{
		auto file = fileSystem->open("../assets/placeholders/textures/Incense brass Metalic/graniterockface1_Ambient_Occlusion-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
		auto image = std::make_unique<Image>( file->getInputStream() );
	
		material.ambientOcclusion = image.get();
		image.release();
	}
	*/
	/*
	if (material.albedo) flipVertical(*material.albedo);
	if (material.normal) flipVertical(*material.normal);
	if (material.metalness) flipVertical(*material.metalness);
	if (material.roughness) flipVertical(*material.roughness);
	if (material.ambientOcclusion) flipVertical(*material.ambientOcclusion);
	
	auto m = model::import("../assets/placeholders/models/Incense.obj", logger_, fileSystem_);
	auto meshHandle = graphicsEngine->createStaticMesh(m->meshes[2]);
	auto materialHandle = graphicsEngine->createMaterial(material);
	auto renderableHandle = graphicsEngine->createRenderable(renderSceneHandle_, meshHandle, materialHandle);
	graphicsEngine->translate(renderSceneHandle_, renderableHandle, glm::vec3(2, -2.0, 1));
	graphicsEngine->rotate(renderSceneHandle_, renderableHandle, -15, glm::vec3(1, 0, 0));
	//graphicsEngine->scale(renderSceneHandle_, renderableHandle, glm::vec3(20, 20, 20));
	 */
}

Terrain::~Terrain()
{
	
}

void Terrain::tick(const float32 delta)
{
	
}

const std::vector<pathfinding::CrowdHandle>& Terrain::crowds() const
{
	return crowdHandles_;
}

}
