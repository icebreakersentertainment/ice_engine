#ifndef BULLETMOTIONSTATE_H_
#define BULLETMOTIONSTATE_H_

#include <bullet/btBulletDynamicsCommon.h>

#include "entities/Entity.hpp"
#include "IGameEngine.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

class BulletMotionState : public btMotionState
{
public:
	BulletMotionState(const btTransform& initialPosition, entities::Entity entity, IGameEngine* gameEngine)
		:
		initialPosition_(initialPosition),
		entity_(entity),
		gameEngine_(gameEngine)
	{
	}

	virtual ~BulletMotionState()
	{
	}

	void setEntity(entities::Entity entity)
	{
		entity_ = entity;
	}

	void setGameEngine(IGameEngine* gameEngine)
	{
		gameEngine_ = gameEngine;
	}

	virtual void getWorldTransform(btTransform& worldTrans) const override
	{
		worldTrans = initialPosition_;
	}

	virtual void setWorldTransform(const btTransform& worldTrans) override
	{
		if (gameEngine_ == nullptr)
		{
			return; // silently return before we set a node
		}
		
		initialPosition_ = worldTrans;

		const btQuaternion& rot = worldTrans.getRotation();
		const btVector3& pos = worldTrans.getOrigin();
		
		gameEngine_->rotation(entity_, glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));
		gameEngine_->position(entity_, pos.x(), pos.y(), pos.z());
	}

private:
	entities::Entity entity_;
	IGameEngine* gameEngine_;
	btTransform initialPosition_;
};

}
}
}

#endif /* BULLETMOTIONSTATE_H_ */
