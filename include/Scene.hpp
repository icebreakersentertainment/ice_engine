#ifndef WORLD_H_
#define WORLD_H_

#include "Types.hpp"

#include "IScene.hpp"

#include "IGameEngine.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"
#include "utilities/Properties.hpp"

namespace hercules
{

class Scene : IScene
{
public:
	Scene(IGameEngine* gameEngine, IThreadPool* threadPool, IOpenGlLoader* openGlLoader, utilities::Properties* properties);
	virtual ~Scene();

	virtual void tick(float32 elapsedTime) override;
	
private:	
	IGameEngine* gameEngine_;
	IThreadPool* threadPool_;
	IOpenGlLoader* openGlLoader_;
	utilities::Properties* properties_;
};

}

#endif /* WORLD_H_ */
