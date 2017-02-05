#ifndef IGAMEENGINE_H_
#define IGAMEENGINE_H_

#include <string>

#include "Platform.hpp"
#include "Types.hpp"
#include "entities/Entity.hpp"

namespace hercules
{

// game states
enum GameState
{
	GAME_STATE_UNKNOWN = 0,
	GAME_STATE_STARTUP,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_START_NEW_GAME,
	GAME_STATE_LOAD_NEW_GAME,
	GAME_STATE_IN_GAME
};

class IGameEngine
{
public:
	virtual ~IGameEngine()
	{
	}
	;
	
	virtual void run() = 0;
	
	virtual GameState getState() = 0;

	virtual entities::Entity* createEntity() = 0;
	virtual entities::Entity* createEntity(const std::string& name) = 0;
	virtual entities::Entity* getEntity(const std::string& name) = 0;
	
	virtual void setBootstrapScript(const std::string& className, const std::string& filename) = 0;
};

}

#endif /* IGAMEENGINE_H_ */
