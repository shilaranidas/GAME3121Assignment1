#pragma once
#include "Ogre.h"
//#include "OgreApplicationContext.h"
//#include "OgreInput.h"
//#include "OgreRTShaderSystem.h"
//#include "OgreTrays.h"

class Ball
{
private:
	Ogre::int32 velY;
	Ogre::int32 velX;
	Ogre::Vector3 pos;
	Ogre::SceneNode* node;

public:
	Ball();
	Ball(Ogre::Entity* ballEntity, Ogre::SceneManager* scnMgr);
	~Ball();
	Ogre::Vector3 btranslate;
	Ogre::Vector3 getPosition();
	void setPosition(Ogre::Vector3);
	Ogre::SceneNode* getShape();
	Ogre::int32 getVelX();
	void setVelX(Ogre::int32 x);
	void setVelY(Ogre::int32 y);
	Ogre::int32 getVelY();
	void update(const Ogre::FrameEvent& evt);
	void reset();
};
