#include "test2.as"

void main() {
	println("Running create wolf script test.");
	
	Entity@ entity = world.createEntity("wolf_test");
	
	GraphicsComponent@ gc = GraphicsComponent();
	entity.addComponent(gc);
	
	//PhysicsComponent@ pc = cast<PhysicsComponent>( entity2.addComponent(COMPONENT_TYPE_PHYSICS) );
	//GraphicsComponent@ gc = cast<GraphicsComponent>( entity2.addComponent(COMPONENT_TYPE_GRAPHICS) );
	//pc.setPosition( vec3(1.0f, 1.0f, 1.0f) );
	
	gc.setPosition( vec3(1.0f, 1.0f, 1.0f) );
	gc.setModel("PlainsWolf");
	gc.setAnimation("Walk");
	
	println("Running create wolf script test.");
}
