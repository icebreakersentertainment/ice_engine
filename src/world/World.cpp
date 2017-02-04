#include <cstddef>
#include <sstream>
#include <thread>

#define GLM_FORCE_RADIANS
#include <glm/gtx/string_cast.hpp>

#include "world/World.hpp"
#include "Constants.hpp"

#include "noise/GameNoise.hpp"

#include "logger/Logger.hpp"

// TESTING
//#include "models/Model.hpp"

namespace hercules
{
namespace world
{

World::World(IGameEngine* game, /*glr::ISceneManager* smgr, glr::models::IModelManager* modelManager,*/ IThreadPool* threadPool, IOpenGlLoader* openGlLoader, utilities::Properties* properties)
	: filename_(std::string()), game_(game), /*smgr_(smgr), modelManager_(modelManager),*/ threadPool_(threadPool), openGlLoader_(openGlLoader), properties_(properties)
{
	//assert(smgr_ != nullptr);
	//assert(modelManager_ != nullptr);
	assert(threadPool_ != nullptr);
	assert(openGlLoader_ != nullptr);
	assert(properties_ != nullptr);

	initialize();
}

World::~World()
{
}

void World::initialize()
{
	player_ 		= nullptr;
	//terrainManager_ = nullptr;

	//nodes_ 			= std::vector< glr::ISceneNode* >();
	//models_ 		= std::vector< glr::models::IModel* >();
	
	animationTime_ 	= 0.0f;
	
	//fieldFunction_ 	= std::unique_ptr<glr::terrain::IFieldFunction>( new gamenoise::GameNoise() );
	
	loadData(filename_);
	loadRegions(player_);
}

void World::loadData(const std::string& filename)
{
	// load player
}

void World::loadRegions(Player* target)
{
	// load terrain
	
	// load environment
	
	// load sky (?)
	
	// load entities
	
	// load scripts
	
	// load global variables (?)
}

void World::createSkyBox()
{
	// Testing environment manager skybox
	/*
	auto envMgr = smgr_->getEnvironmentManager();
	envMgr->createSkyBox();
	
	if (player_ != nullptr)
	{
		envMgr->setFollowTarget( player_->getCamera()->getCamera()->getCamera() );
	}
	*/
}

void World::initializeTerrainManager()
{
	// Create our terrain manager
	/*
	glmd::int32 smoothingAlgorithm = properties_->getIntValue( std::string("glr.smoothing_algorithm"), 0 );
	
	glr::terrain::TerrainSettings terrainSettings = glr::terrain::TerrainSettings();
	
	switch (smoothingAlgorithm)
	{
		case glr::terrain::ALGORITHM_MARCHING_CUBES:
			terrainSettings.smoothingAlgorithm = glr::terrain::ALGORITHM_MARCHING_CUBES;
			break;
		
		case glr::terrain::ALGORITHM_DUAL_CONTOURING:
			if (fieldFunction_.get() == nullptr)
			{
				const std::string message = std::string("Dual Contouring algorithm requires a field function.");
				LOG_ERROR(message);
				throw glr::exception::InvalidArgumentException(message);
			}
			
			terrainSettings.smoothingAlgorithm = glr::terrain::ALGORITHM_DUAL_CONTOURING;
			break;
		
		default:
			const std::string message = std::string("Invalid smoothing algorithm set for terrain manager.");
			LOG_ERROR(message);
			throw glr::exception::InvalidArgumentException(message);
	}
	
	terrainSettings.length = 2;
	terrainSettings.width = 2;
	terrainSettings.height = 2;
	
	terrainManager_ = smgr_->getTerrainManager( fieldFunction_.get(), terrainSettings );
	
	assert(terrainManager_ != nullptr);
	
	terrainManager_->addTerrainManagerEventListener( this );
	
	if (player_ != nullptr)
	{
		terrainManager_->setFollowTarget( player_->getCamera()->getCamera()->getCamera() );
	}
	*/
}

void World::setPlayer(Player* player)
{
	if (player_ != nullptr)
	{
		std::string msg = "Player has already been set.";
		LOG_ERROR(msg);
		throw std::runtime_error(msg);
	}

	player_ = player;
	
	/*
	if (player_!= nullptr)
	{
		if (terrainManager_ != nullptr)
			terrainManager_->setFollowTarget( player_->getCamera()->getCamera()->getCamera() );
		
		auto envMgr = smgr_->getEnvironmentManager();
		envMgr->setFollowTarget( player_->getCamera()->getCamera()->getCamera() );
	}
	*/
}

void World::newWorld()
{
	LOG_DEBUG( "Loading prototype campaign data store..." );
	//dataStore_ = std::unique_ptr<pyliteserializer::SqliteDataStore>( new pyliteserializer::SqliteDataStore(std::string("../data/dark_horizon_prototype_campaign.db")) );
	
	// Load world data
	regions_ = std::vector<Region>();
	//for (int i=0; i < 4; i++)
	//{
	//	regions_[i].setId(i+1);
	//}
	
	//dataStore_->loadBulk( regions_, std::string() );
	
	//for (auto& r : regions_)
	//{		
	//	std::cout << "Region: " << r.getName() << std::endl;
	//}
	
	// Testing loading
	//regions_[0].loadMeshes();
	
	// populate world

	// add the world to the EventManager as a listener

	// load graphics scene
	/*
	smgr_->createSceneNode( std::string("test") );

	// Load our models
	//smgr_->getModelManager()->loadModel(modelsDir_ + "WoolySheep.x");
	smgr_->getModelManager()->loadModel(std::string("PlainsWolf"), std::string(modelsDir_ + "PlainsWolf.dae"));
	//smgr_->getModelManager()->loadModel(std::string("KnightSwordShield"), std::string(modelsDir_ + "KnightSword&Shield.x"));
	smgr_->getModelManager()->loadModel(std::string("VillageTree"), std::string(modelsDir_ + "VillageTree.dae"));
	smgr_->getModelManager()->loadModel(std::string("spire"), std::string(modelsDir_ + "spire.dae"));
	
	
	// Testing animation time
	animationTime_ = 0.0f;
	
	std::stringstream ss;	
	glm::vec3 pos = glm::vec3();
	
	{
		ss.clear();
		ss.str(std::string());
		ss << std::string("creature_node_test_") << nodes_.size();
		glr::ISceneNode* node = smgr_->createSceneNode(ss.str());
		
		pos = glm::vec3(4.0f, 6.0f, 8.0f) * 4.0f;
		node->setScale(0.05f, 0.05f, 0.05f);
		node->setPosition( 0.0f, 0.0f, 0.0f );
		models_.push_back( smgr_->getModelManager()->createInstance( std::string("PlainsWolf")) );
		node->attach(models_[ models_.size()-1 ].get());
		nodes_.push_back( node );

		//models_[ models_.size()-1 ]->setCurrentAnimation( nullptr );
		
		// Clamp plains wolf animation
		models_[ models_.size()-1 ]->getPlayingAnimation()->setFrameClampping(0, 88);

		assert( models_[ models_.size()-1 ].get() != nullptr );
	}
	
	
	for ( int i = 0; i < 1; i++ )
	{
		ss.clear();
		ss.str(std::string());
		ss << std::string("creature_node_test_") << nodes_.size();
		glr::ISceneNode* node = smgr_->createSceneNode(ss.str());
		pos = glm::vec3(3.0f, 0.0f, 0.0f) * (float)(i+1);
		node->translate( pos );
		node->setScale(0.05f, 0.05f, 0.05f);
		models_.push_back( smgr_->getModelManager()->createInstance(std::string("PlainsWolf")) );
		node->attach(models_[ models_.size()-1 ].get());
		nodes_.push_back( node );
		
		assert( models_[ models_.size()-1 ].get() != nullptr );
	}
	
	{
		ss.clear();
		ss.str(std::string());
		ss << std::string("creature_test_") << nodes_.size();
		glr::ISceneNode* node = smgr_->createSceneNode(ss.str());
		node->setPosition( 10.0f, 1.0f, 8.0f );
		node->setScale(0.05f, 0.05f, 0.05f);
		models_.push_back( smgr_->getModelManager()->createInstance(std::string("KnightSwordShield")) );
		node->attach(models_[ models_.size()-1 ].get());
		nodes_.push_back( node );
		
		assert( models_[ models_.size()-1 ].get() != nullptr );
	}
	
	{
		ss.clear();
		ss.str(std::string());
		ss << std::string("tree_node_test_") << nodes_.size();
		glr::ISceneNode* node = smgr_->createSceneNode(ss.str());
		
		node->setScale(0.1f, 0.1f, 0.1f);
		node->setPosition( 24.0f, 3.0f, 24.0f );
		models_.push_back( smgr_->getModelManager()->createInstance( std::string("VillageTree")) );
		node->attach(models_[ models_.size()-1 ].get());
		nodes_.push_back( node );

		assert( models_[ models_.size()-1 ].get() != nullptr );
	}
	
	
	{
		ss.clear();
		ss.str(std::string());
		ss << std::string("building_node_test_") << nodes_.size();
		glr::ISceneNode* node = smgr_->createSceneNode(ss.str());
		
		pos = glm::vec3(75.0f, 0.0f, 0.0f);
		node->setScale(0.05f, 0.05f, 0.05f);
		node->translate( pos );
		node->rotate( 225.0f, glm::vec3(0.0f, 1.0f, 0.0f) );
		models_.push_back( smgr_->getModelManager()->createInstance( std::string("spire")) );
		node->attach(models_[ models_.size()-1 ].get());
		nodes_.push_back( node );

		assert( models_[ models_.size()-1 ].get() != nullptr );
	}
	*/
	
	// Model Testing
	//smgr_->getModelManager()->loadModel(modelsDir_ + "WoolySheep.x");
	
	
	//modelManager_->loadModel(std::string("PlainsWolf"), std::string(Constants::MODELS_DIRECTORY + "PlainsWolf.dae"), false);
	/*
	modelManager_->loadModel(std::string("KnightSwordShield"), std::string(Constants::MODELS_DIRECTORY + "KnightSword&Shield.x"), false);
	auto model = modelManager_->getModelTemplate( std::string("KnightSwordShield") );
	
	auto f = [=]() {
		model->loadLocalData();
	
		openGlLoader_->postWork(
			[=] {
				model->allocateVideoMemory();
				model->pushToVideoMemory();
			}
		);
	};
	
	threadPool_->postWork( f );
	*/
	
	//modelManager_->serialize( std::string("model-manager-test.dat") );
	//modelManager_->deserialize( std::string("model-manager-test.dat") );
	
	// Testing animation time
	animationTime_ = 0.0f;
	
	std::stringstream ss;
	/*
	{
		ss.clear();
		ss.str(std::string());
		ss << std::string("creature_node_test_") << nodes_.size();
		glr::ISceneNode* node = smgr_->createSceneNode(ss.str());

		node->setScale(0.05f, 0.05f, 0.05f);
		node->setPosition( 5.0f, 5.0f, 5.0f );
		auto m = modelManager_->createInstance( std::string("KnightSwordShield") );
		assert( m != nullptr );
		models_.push_back( m );
		node->attach( m );
		nodes_.push_back( node );
		
		// Clamp plains wolf animation
		//auto animations = m->getAnimations();
		//auto a = animations[0];
		//assert(a != nullptr);
		//m->playAnimation(a, 0, 88, 0.0f);
		//m->playAnimation(a);
	}
	*/
	
	
	/*
	{
		ss.clear();
		ss.str(std::string());
		ss << std::string("creature_node_test_") << nodes_.size();
		glr::ISceneNode* node = smgr_->createSceneNode(ss.str());
		
		node->setScale(0.05f, 0.05f, 0.05f);
		node->setPosition( 5.0f, 5.0f, 5.0f );
		auto m = modelManager_->createInstance( std::string("PlainsWolf") );
		assert( m != nullptr );
		models_.push_back( m );
		node->attach( m );
		nodes_.push_back( node );
		
		// Clamp plains wolf animation
		auto animations = m->getAnimations();
		for ( auto a : animations )
		{
			std::cout << a->getName() << std::endl;
		}
		auto a = animations[0];
		assert(a != nullptr);
		m->playAnimation(a, 0, 88, 0.0f);
		//m->playAnimation(a);
	}
	*/
	// Create some lights
	/*
	auto light = smgr_->createLight( std::string("my_light1") );
	
	auto ld = glr::LightData();
	ld.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.position = glm::vec4(2.5f, 20.0f, 2.5f, 0.f);
	ld.direction = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
	light->setLightData(ld);
	
	light = smgr_->createLight( std::string("my_light2") );
	
	ld.ambient = glm::vec4(0.1f, 0.1f, 0.5f, 0.5f);
	ld.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.position = glm::vec4(2.5f, 2.5f, 2.5f, 0.f);
	ld.direction = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
	light->setLightData(ld);
	
	// Create our skybox
	createSkyBox();
	
	// Create our terrain
	initializeTerrainManager();
	
	// TESTING - generate terrain
	//threadPool_->postWork( std::bind( static_cast<void(glr::terrain::ITerrainManager::*)(bool)>(&glr::terrain::ITerrainManager::generate), terrainManager_, true ) );
	terrainManager_->generate(false);
	*/
}

/*
void World::terrainAdded(glr::terrain::ITerrain* terrain)
{
	auto f = [=]() {
		terrain->generate();
		
		if (!terrain->isEmptyOrSolid())
		{
			auto function = [=] {
				terrain->prepareOrUpdateGraphics();
				terrainManager_->moveTerrainFromProcessedToReady( terrain );
			};
			
			openGlLoader_->postWork( function );
		}
	};
	
	threadPool_->postWork( f );
	//terrain->generate();
}
*/

/*
void World::terrainRemoved(glr::terrain::ITerrain* terrain)
{
}
*/

void World::loadWorld()
{	
	
}

int counter;
void World::tick(glmd::float32 elapsedTime)
{
	counter++;
	if (counter > 100)
	{
		//threadPool_->postWork( std::bind(&glr::terrain::ITerrainManager::tick, terrainManager_) );
		counter = 0;
	}
	
	/*
	if (nodes_.size() == 0 && game_->getState() == GameState::GAME_STATE_IN_GAME)
	{
		// Create our models
		std::stringstream ss;
	
		{
			ss.clear();
			ss.str(std::string());
			ss << std::string("creature_node_test_") << nodes_.size();
			glr::ISceneNode* node = smgr_->createSceneNode(ss.str());
	
			node->setScale(0.05f, 0.05f, 0.05f);
			node->setPosition( 5.0f, 5.0f, 5.0f );
			auto m = modelManager_->createInstance( std::string("KnightSwordShield") );
			assert( m != nullptr );
			models_.push_back( m );
			node->attach( m );
			nodes_.push_back( node );
			
			// Clamp plains wolf animation
			auto animations = m->getAnimations();
			
			for (auto a : animations)
			{
				std::cout << "HERE: " << a->getName() << std::endl;
			}
			
			auto a = animations[5];
			assert(a != nullptr);
			//m->playAnimation(a, 0, 88, 0.0f);
			m->playAnimation(a);
		}
	}
	*/
	
	// Handle animations
	/*
	animationTime_ += elapsedTime;
	for ( auto model : models_ )
	{
		if ( model->getPlayingAnimation() != nullptr)
		{
			model->setAnimationTime( animationTime_ );
		}
	}
	*/

	//for ( auto node : nodes_ )
	//{
	//	if (node->getModel() != nullptr && node->getModel()->getPlayingAnimation() != nullptr)
	//		node->getModel()->getPlayingAnimation()->setAnimationTime( animationTime_ );
	//}
	
	//glr::ILight* light = smgr_->getLight("my_light1");
	//light->rotate( glm::vec3( 0.2, 0, 0 ) );

	// testing...
	//if (nodes_.size() > 0 && nodes_[0] != nullptr)
	//	nodes_[0]->rotate( 0.2f, glm::vec3( 1.0f, 0.0f, 0.0f ) );

	for (auto& r : regions_)
	{		
		r.tick(elapsedTime);
	}
}

}
}
