#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "Types.hpp"

//#include "GlrInclude.hpp"

namespace ice_engine
{
namespace extras
{

class FpsCamera
{
public:
	FpsCamera();//ICamera* camera, float32 speed);
	virtual ~FpsCamera();

	bool isActive();

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	
	//ICamera* getCamera();
	
	void rotate(const float32& degrees, const glm::vec3& axis);

	/**
	 * 
	 */
	void tick(float32 time);

private:
	//ICamera* camera_;
	float32 speed_;

	void initialize();
};

}
}

#endif /* FPSCAMERA_H_ */
