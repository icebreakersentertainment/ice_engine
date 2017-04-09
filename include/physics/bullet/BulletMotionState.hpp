#ifndef BULLETMOTIONSTATE_H_
#define BULLETMOTIONSTATE_H_

#include <bullet/btBulletDynamicsCommon.h>

#include "entities/Entity.hpp"
#include "IScene.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

class BulletMotionState : public btMotionState
{
public:
	BulletMotionState(const btTransform& initialPosition, entities::Entity entity, IScene* scene)
		:
		initialPosition_(initialPosition),
		entity_(entity),
		scene_(scene)
	{
	}

	virtual ~BulletMotionState()
	{
	}

	void setEntity(entities::Entity entity)
	{
		entity_ = entity;
	}

	void setGameEngine(IScene* scene)
	{
		scene_ = scene;
	}

	virtual void getWorldTransform(btTransform& worldTrans) const override
	{
		worldTrans = initialPosition_;
	}

	virtual void setWorldTransform(const btTransform& worldTrans) override
	{
		if (scene_ == nullptr)
		{
			return; // silently return before we set a node
		}
		
		initialPosition_ = worldTrans;

		const btQuaternion& rot = worldTrans.getRotation();
		const btVector3& pos = worldTrans.getOrigin();
		
		scene_->rotation(entity_, glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));
		scene_->position(entity_, pos.x(), pos.y(), pos.z());
	}

private:
	entities::Entity entity_;
	IScene* scene_;
	btTransform initialPosition_;
};

}
}
}

#endif /* BULLETMOTIONSTATE_H_ */
