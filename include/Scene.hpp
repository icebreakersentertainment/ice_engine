#ifndef SCENE_H_
#define SCENE_H_

#include "Types.hpp"

#include "IScene.hpp"

#include "IGameEngine.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"
#include "utilities/Properties.hpp"

namespace hercules
{

class Scene : public IScene
{
public:
	Scene(IGameEngine* gameEngine, IThreadPool* threadPool, IOpenGlLoader* openGlLoader, utilities::Properties* properties);
	virtual ~Scene();

	virtual void tick(const float32 elapsedTime) override;
	
private:	
	IGameEngine* gameEngine_;
	IThreadPool* threadPool_;
	IOpenGlLoader* openGlLoader_;
	utilities::Properties* properties_;
};

}

#endif /* SCENE_H_ */
