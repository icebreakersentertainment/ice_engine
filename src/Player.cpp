#include "Player.hpp"

namespace game
{

Player::Player()//Camera* camera) : camera_(camera)
{
}

Player::Player(/*Camera* camera,*/ int x, int y, int z)// : camera_(camera)
{
}

Player::~Player()
{
}

float Player::getX()
{
	return 0;
	//return camera_->getCamera()->getCamera()->getPosition().x;
}

float Player::getY()
{
	return 0;
	//return camera_->getCamera()->getCamera()->getPosition().y;
}

float Player::getZ()
{
	return 0;
	//return camera_->getCamera()->getCamera()->getPosition().z;
}

}
