#include "Scene.hpp"

namespace hercules
{

Scene::Scene(IGameEngine* gameEngine, IThreadPool* threadPool, IOpenGlLoader* openGlLoader, utilities::Properties* properties)
	: gameEngine_(gameEngine), threadPool_(threadPool), openGlLoader_(openGlLoader), properties_(properties)
{
	assert(gameEngine_ != nullptr);
	assert(threadPool_ != nullptr);
	assert(openGlLoader_ != nullptr);
	assert(properties_ != nullptr);
}

Scene::~Scene()
{
}


void Scene::tick(float32 elapsedTime)
{

}

}
