#ifndef CAMERA_H_
#define CAMERA_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

//#include "GlrInclude.hpp"
#include "extras/FpsCamera.hpp"

namespace ice_engine
{

class Camera
{
public:
	Camera();//std::unique_ptr<glr::extras::FpsCamera> camera);
	virtual ~Camera();

	bool isActive();

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	
	void rotate(const float32& degrees, const glm::vec3& axis);

	void tick(float32 time);
	
	//glr::extras::FpsCamera* getCamera();

private:
	//std::unique_ptr<glr::extras::FpsCamera> camera_;

	void initialize();
};

}

#endif /* CAMERA_H_ */
