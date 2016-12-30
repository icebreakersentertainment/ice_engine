#include <iostream>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "entities/GraphicsComponent.hpp"

namespace game
{
namespace entities
{

GraphicsComponent::GraphicsComponent()//glr::ISceneManager* sceneManager) : sceneManager_(sceneManager)
{
	//assert(sceneManager_ != nullptr);

	std::cout << "Created GraphicsComponent." << std::endl;
	////sceneNode_ = smgr_->getSceneNode( name );

	////sceneNode_ = sceneManager_->createSceneNode("testing1234");
}

GraphicsComponent::~GraphicsComponent()
{
	//sceneManager_->destroySceneNode( //sceneNode_ );
}

void GraphicsComponent::rotate(float degrees, const Vec3& axis)
{
	//sceneNode_->rotate(degrees, glm::vec3(axis.x, axis.y, axis.z));
}

void GraphicsComponent::translate(const Vec3& translate)
{
	//sceneNode_->translate(glm::vec3(translate.x, translate.y, translate.z));
}

void GraphicsComponent::setScale(float x, float y, float z)
{
	//sceneNode_->setScale(x, y, z);
}

void GraphicsComponent::lookAt(const Vec3& lookAt)
{
	//sceneNode_->lookAt(glm::vec3(lookAt.x, lookAt.y, lookAt.z));
}

void GraphicsComponent::setPosition(const Vec3& pos)
{
	//sceneNode_->setPosition(pos.x, pos.y, pos.z);
}

void GraphicsComponent::setPosition(float x, float y, float z)
{
	//sceneNode_->setPosition(x, y, z);
}

}
}
