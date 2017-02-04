#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <memory>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "utilities/Properties.hpp"

#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"

//#include "GlrInclude.hpp"

#include "as_wrapper/AngelScript.h"
//#include "data_store/SqliteDataStore.hpp"
#include "Player.hpp"

#include "world/Region.hpp"

// TESTING
//#include "terrain/ITerrainManagerEventListener.hpp"

namespace glmd = glm::detail;

namespace hercules
{
class Game;
namespace world
{

class World// : public virtual glr::terrain::ITerrainManagerEventListener
{
public:
	World(Game* game, /*glr::ISceneManager* smgr, glr::models::IModelManager* modelManager,*/ IThreadPool* threadPool, IOpenGlLoader* openGlLoader, utilities::Properties* properties);
	virtual ~World();

	void tick(glmd::float32 elapsedTime);
	
	void setPlayer(Player* player);
	void newWorld();
	void loadWorld();
	
	//virtual void terrainAdded(glr::terrain::ITerrain* terrain);
	//virtual void terrainRemoved(glr::terrain::ITerrain* terrain);

private:	
	Player* player_;
	std::string filename_;
	Game* game_;

	// Testing nodes for models
	//std::vector< glr::ISceneNode* > nodes_;	
	// Testing animations
	glm::detail::float32 animationTime_;
	// Testing loading and storing a model
	//std::vector< glr::models::IModel* > models_;
	
	//std::unique_ptr<glr::terrain::IFieldFunction> fieldFunction_;
	
	void loadData(const std::string& filename);
	void loadRegions(Player* target);
	
	// Testing skybox
	void createSkyBox();
	
	// Testing terrain
	void initializeTerrainManager();
	
	//glr::ISceneManager* smgr_;
	//glr::models::IModelManager* modelManager_;
	//glr::terrain::ITerrainManager* terrainManager_;
	
	IThreadPool* threadPool_;
	IOpenGlLoader* openGlLoader_;
	utilities::Properties* properties_;
	
	//std::unique_ptr<pyliteserializer::SqliteDataStore> dataStore_;
	std::vector<Region> regions_;

	void initialize();
};

}
}
#endif /* WORLD_H_ */
