#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_

#include "entityx/entityx.h"

//#include "ISceneManager.hpp"

#include "Types.hpp"

#include "as_wrapper/glm_bindings/Vec3.h"
#include "as_wrapper/ASRefObject.hpp"

namespace hercules
{
namespace entities
{

class GraphicsComponent : public as_wrapper::ASRefObject, public entityx::Component<GraphicsComponent> {
public:
	GraphicsComponent();//glr::ISceneManager* sceneManager);
	virtual ~GraphicsComponent();
	
	void rotate(float32 degrees, const Vec3& axis);
	void translate(const Vec3& translate);
	void setScale(float32 x, float32 y, float32 z);
	void lookAt(const Vec3& lookAt);
	
	void setPosition(const Vec3& pos);
	void setPosition(float32 x, float32 y, float32 z);
private:
	//glr::ISceneNode* sceneNode_;
	//glr::ISceneManager* sceneManager_;
};

}
}

#endif /* GRAPHICSCOMPONENT_H_ */
