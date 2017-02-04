#ifndef GRAPHICSCOMPONENTFACTORY_H_
#define GRAPHICSCOMPONENTFACTORY_H_

#include "entities/GraphicsComponent.hpp"

namespace hercules
{
namespace entities
{

class GraphicsComponentFactory
{
public:	
	static entities::GraphicsComponent* graphicsComponentRefFactory();
	//static glr::ISceneManager* sceneManager_;

private:
	GraphicsComponentFactory();
	GraphicsComponentFactory(const GraphicsComponentFactory& other);
	~GraphicsComponentFactory();
	
	
};

}
}

#endif /* GRAPHICSCOMPONENTFACTORY_H_ */
