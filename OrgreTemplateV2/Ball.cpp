#include "Ball.h"
Ball::Ball()
{
}
Ball::Ball(Ogre::Entity* ent, Ogre::SceneManager* scnMgr)
{
	node = scnMgr->getRootSceneNode()->createChildSceneNode();
	node->setPosition(0, 100, 0);
	node->setScale(0.1f, 0.1f, 0.1f);

	node->attachObject(ent);
	reset();
	
	pos = node->getPosition();
	
}
Ball::~Ball()
{
}
void Ball::reset() {
	velY = 1;

	velX = 0;
	btranslate = Ogre::Vector3(0, 0, 0);
}
Ogre::Vector3 Ball::getPosition()
{
	return pos;
}
void Ball::setPosition(Ogre::Vector3 p)
{
	pos = p;
}

Ogre::SceneNode* Ball::getShape()
{
	return node;
}

Ogre::int32 Ball::getVelX()
{
	return velX;
}

void Ball::setVelX(Ogre::int32 x)
{
	velX = x;
}

Ogre::int32 Ball::getVelY()
{
	return velY;
}
void Ball::setVelY(Ogre::int32 y)
{
	velY = y;
}
void Ball::update(const Ogre::FrameEvent& evt)
{
	btranslate = Ogre::Vector3(-10 * velX, -10 * velY, 0);
    //moving ball
    if (node->getPosition().y > 135)
    {
        velY = 1;
    }
    
    if (node->getPosition().x > 112)
    {
        velX = 1;
       // std::cout << "greater then 112" << std::endl;
    }
    if (node->getPosition().x < -112)
    {
        //std::cout << "less then -112" << std::endl;
        velX = -1;
    }
	node->translate(btranslate * evt.timeSinceLastFrame);
}
