/** @file Week5-7-PrefabTypeDemo.cpp
 *  @brief Using Scene Manager PrefabType to create
 *
 *  How to use PrefabType
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreTrays.h"
#include <iostream>

using namespace Ogre;
using namespace OgreBites;
Ogre::Vector3 translate(0, 0, 0);
Ogre::Vector3 btranslate(0, 0, 0);
Ogre::int32 isCollide;
Ogre::int32 movDir;

class ExampleFrameListener : public Ogre::FrameListener
{
private:
    Ogre::SceneNode* _node;
    Ogre::SceneNode* _bnode;
public:

    ExampleFrameListener(Ogre::SceneNode* node, Ogre::SceneNode* bnode)
    {
        _node = node;
        _bnode = bnode;
    }

    bool frameStarted(const Ogre::FrameEvent& evt)
    {
        //moving paddle
        _node->translate(translate * evt.timeSinceLastFrame);       
        translate = Ogre::Vector3(0, 0, 0);
        
        //moving ball
        _bnode->translate(btranslate * evt.timeSinceLastFrame);
        btranslate = Ogre::Vector3(0, -10* movDir, 0);
        //collision
        AxisAlignedBox spbox = _bnode->_getWorldAABB();
        AxisAlignedBox cbbox = _node->_getWorldAABB();
        if (spbox.intersects(cbbox) && isCollide==0)
        {
            std::cout << "collide";
            isCollide = 1;
            movDir = -1;
           // btranslate = Ogre::Vector3(0, 10, 0);
        }
        return true;
    }
};

class Game
    : public ApplicationContext
    , public InputListener
{
private:
    SceneNode* paddleNode;
    Ogre::SceneNode* ballNode;
    SceneManager* scnMgr;
    Root* root;
public:
    Game();
    virtual ~Game() {}

    void setup();
    bool keyPressed(const KeyboardEvent& evt);
    void createScene();
    void createCamera();
    void createFrameListener();
    OgreBites::TrayListener myTrayListener;
    OgreBites::Label* mInfoLabel;
    OgreBites::Label* mScoreLabel;
    OgreBites::Label* mScore;
    OgreBites::Label* mLivesLabel;
    OgreBites::Label* mLives;
    Ogre::DisplayString sc = "0";
    Ogre::DisplayString l = "3";
    int score = 0;
    int lives = 3;
};


Game::Game()
    : ApplicationContext("Assignment 1")
{
    score = 0;
    lives = 3;
    isCollide = 0;//no collision
    movDir = 1;
}


void Game::setup()
{
    // do not forget to call the base first
    ApplicationContext::setup();
    addInputListener(this);

    // get a pointer to the already created root
    //Root* 
        root = getRoot();
    //SceneManager* 
        scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);
    createScene();
    createCamera();
    createFrameListener();


    


}

void Game::createScene()
{
    // -- tutorial section start --
//! [turnlights]
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    //! [turnlights]

    //! [newlight]
    Light* light = scnMgr->createLight("MainLight");
    SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    //! [newlight]

    //! [lightpos]
    lightNode->setPosition(20, 80, 50);
    //! [lightpos]


    OgreBites::TrayManager* mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow());

    //you must add this in order to add a tray
    scnMgr->addRenderQueueListener(mOverlaySystem);
    //Once you have your tray manager, make sure you relay input events to it.
    addInputListener(mTrayMgr);

    mTrayMgr->showLogo(TL_TOPRIGHT);
    mTrayMgr->showFrameStats(TL_TOPRIGHT);
    //mTrayMgr->toggleAdvancedFrameStats();

    mInfoLabel = mTrayMgr->createLabel(TL_TOP, "TInfo", "My Game Engine", 350);
    mScoreLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Score", "Score:", 150);
    mScore = mTrayMgr->createLabel(TL_TOPLEFT, "score", sc, 150);
    mLivesLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Lives", "Lives:", 150);
    mLives = mTrayMgr->createLabel(TL_TOPLEFT, "lives", l, 150);
    // a friendly reminder
    StringVector names;
    names.push_back("Help");
    mTrayMgr->createParamsPanel(TL_TOPLEFT, "Help", 100, names)->setParamValue(0, "H/F1");


    Ogre::Entity* ballEntity = scnMgr->createEntity(SceneManager::PrefabType::PT_SPHERE);
    //Ogre::SceneNode* 
        ballNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    ballNode->setPosition(0, 100, 0);
    ballNode->setScale(0.1f, 0.1f, 0.1f);
    ballNode->attachObject(ballEntity);
   

    Ogre::Entity* paddleEntity = scnMgr->createEntity(SceneManager::PrefabType::PT_PLANE);
    //Ogre::SceneNode* 
    paddleNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    paddleNode->setPosition(0, -10, 0);
    paddleNode->setScale(0.2f, 0.05f, 1.0f);
    paddleNode->attachObject(paddleEntity);

    scnMgr->showBoundingBoxes(true);

}

void Game::createCamera()
{
    //! [camera]
    SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    // create the camera
    Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    camNode->setPosition(0, 0, 140);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);
    //! [camera]


    //! [cameramove]
    camNode->setPosition(0, 47, 222);
    //! [cameramove]

}

void Game::createFrameListener()
{
    Ogre::FrameListener* FrameListener = new ExampleFrameListener(paddleNode, ballNode);
    mRoot->addFrameListener(FrameListener);
}

bool Game::keyPressed(const KeyboardEvent& evt)
{
    switch (evt.keysym.sym)
    {
    case SDLK_ESCAPE:
        getRoot()->queueEndRendering();
        break;
   
    case 'a':
    case SDLK_LEFT:
        if(paddleNode->getPosition().x > -102)
        translate = Ogre::Vector3(-30, 0, 0);     
        break;
    case 'd':
    case SDLK_RIGHT:
        if (paddleNode->getPosition().x<102)
        translate = Ogre::Vector3(30, 0, 0);        
        break;
    default:
        break;
    }
    return true;
}


int main(int argc, char** argv)
{
    try
    {
        Game app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

//! [fullsource]
