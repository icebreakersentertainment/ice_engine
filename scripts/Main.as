#include "IMain.as"

class Main : IMain
{
	Main()
	{
		println("Creating Main.");
	}
	
	void initialize()
	{
		println("Executing initialize() method.");
		
		Entity@ e = createEntity();
		//PhysicsComponent@ pc = PhysicsComponent();
		GraphicsComponent@ gc = GraphicsComponent();
		//e.addComponent(pc);
		e.addComponent(gc);
	}
	
	void destroy()
	{
		println("Executing destroy() method.");
	}
	
	void tick()
	{
		
	}
}
