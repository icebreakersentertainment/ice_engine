#include <utility>

#include "Camera.hpp"

namespace hercules
{

Camera::Camera()//std::unique_ptr<glr::extras::FpsCamera> camera) : //camera_(std::move(camera))
{
	initialize();
}

Camera::~Camera()
{
}

void Camera::initialize()
{
}

bool Camera::isActive()
{
	return true;
}

void Camera::moveForward()
{
	////camera_->moveForward();
}

void Camera::moveBack()
{
	//camera_->moveBack();
}

void Camera::moveLeft()
{
	//camera_->moveLeft();
}

void Camera::moveRight()
{
	//camera_->moveRight();
}

void Camera::rotate(const float32& degrees, const glm::vec3& axis)
{
	//camera_->rotate( degrees / 5.5f, axis );
}

/**
 *
 */
void Camera::tick(float32 time)
{
	//camera_->tick(time);
}

/*
glr::extras::FpsCamera* Camera::getCamera()
{
	return camera_.get();
}
*/

}
