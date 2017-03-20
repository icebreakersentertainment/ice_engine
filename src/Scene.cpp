#include "Scene.hpp"

namespace hercules
{

Scene::Scene(IGameEngine* gameEngine, IThreadPool* threadPool, IOpenGlLoader* openGlLoader, utilities::Properties* properties)
	: gameEngine_(gameEngine), threadPool_(threadPool), openGlLoader_(openGlLoader), properties_(properties)
{
}

Scene::~Scene()
{
}


void Scene::tick(const float32 elapsedTime)
{

}

}
