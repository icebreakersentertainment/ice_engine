#ifndef PLAYER_H_
#define PLAYER_H_

#include "Camera.hpp"

namespace hercules
{

class Player
{
public:
	Player();//Camera* camera);
	Player(/*Camera* camera, */int x, int y, int z);
	virtual ~Player();
	
	float getX();
	float getY();
	float getZ();
	
	/*
	Camera* getCamera()
	{
		return camera_;
	};
	*/

private:
	//Camera* camera_;
};

}

#endif /* PLAYER_H_ */
