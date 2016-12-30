#include "entities/GraphicsComponentFactory.hpp"

namespace game
{
namespace entities
{

GraphicsComponentFactory::GraphicsComponentFactory()
{
}

GraphicsComponentFactory::GraphicsComponentFactory(const GraphicsComponentFactory& other)
{
	// Throw exception here?????
}

GraphicsComponentFactory::~GraphicsComponentFactory()
{
}

//glr::ISceneManager* GraphicsComponentFactory::sceneManager_ = nullptr;

GraphicsComponent* GraphicsComponentFactory::graphicsComponentRefFactory()
{
	return new GraphicsComponent();//GraphicsComponentFactory::sceneManager_);
}

}
}
