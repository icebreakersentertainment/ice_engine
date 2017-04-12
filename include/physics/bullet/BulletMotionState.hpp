#ifndef BULLETMOTIONSTATE_H_
#define BULLETMOTIONSTATE_H_

#include <bullet/btBulletDynamicsCommon.h>

#include "physics/IMotionStateListener.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

class BulletMotionState : public btMotionState
{
public:
	BulletMotionState(const btTransform& initialPosition, std::unique_ptr<IMotionStateListener> motionStateListener = nullptr)
		:
		initialPosition_(initialPosition),
		motionStateListener_(std::move(motionStateListener))
	{
	}

	virtual ~BulletMotionState()
	{
	}

	virtual void getWorldTransform(btTransform& worldTrans) const override
	{
		worldTrans = initialPosition_;
	}

	virtual void setWorldTransform(const btTransform& worldTrans) override
	{
		if (motionStateListener_.get() == nullptr)
		{
			return;
		}
		
		initialPosition_ = worldTrans;

		const btQuaternion& rot = worldTrans.getRotation();
		const btVector3& pos = worldTrans.getOrigin();
		
		motionStateListener_->update(glm::vec3(pos.x(), pos.y(), pos.z()), glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));
	}

private:
	btTransform initialPosition_;
	std::unique_ptr<IMotionStateListener> motionStateListener_;
};

}
}
}

#endif /* BULLETMOTIONSTATE_H_ */
