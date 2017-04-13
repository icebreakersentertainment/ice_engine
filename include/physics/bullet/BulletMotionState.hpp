#ifndef BULLETMOTIONSTATE_H_
#define BULLETMOTIONSTATE_H_

#include <bullet/btBulletDynamicsCommon.h>

#include "physics/IMotionChangeListener.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

class BulletMotionState : public btMotionState
{
public:
	BulletMotionState(const btTransform& initialPosition, std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr)
		:
		initialPosition_(initialPosition),
		motionStateListener_(std::move(motionStateListener))
	{
	}

	virtual ~BulletMotionState()
	{
	}
	
	/**
	 * Call this to update the physics simulation from an external source.
	 */
	void position(const glm::vec3& position)
	{
		const btVector3 pos = btVector3(position.x, position.y, position.z);
		
		initialPosition_.setOrigin(pos);
	}
	
	/**
	 * Call this to update the physics simulation from an external source.
	 */
	void orientation(const glm::quat& orientation)
	{
		const btQuaternion rot = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
		
		initialPosition_.setRotation(rot);
	}
	
	/**
	 * Call this to update the physics simulation from an external source.
	 */
	void setPositionAndOrientation(const glm::vec3& position, const glm::quat& orientation)
	{
		const btQuaternion rot = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
		const btVector3 pos = btVector3(position.x, position.y, position.z);
		initialPosition_ = btTransform(rot, pos);
	}

	btTransform getWorldTransform() const
	{
		return initialPosition_;
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

		const btQuaternion rot = worldTrans.getRotation();
		const btVector3 pos = worldTrans.getOrigin();
		
		motionStateListener_->update(glm::vec3(pos.x(), pos.y(), pos.z()), glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));
	}

private:
	btTransform initialPosition_;
	std::unique_ptr<IMotionChangeListener> motionStateListener_;
};

}
}
}

#endif /* BULLETMOTIONSTATE_H_ */
