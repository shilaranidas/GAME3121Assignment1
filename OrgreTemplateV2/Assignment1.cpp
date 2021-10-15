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

class ExampleFrameListener : public Ogre::FrameListener
{
private:
    Ogre::SceneNode* _node;
public:

    ExampleFrameListener(Ogre::SceneNode* node)
    {
        _node = node;
    }

    bool frameStarted(const Ogre::FrameEvent& evt)
    {
        _node->translate(translate * evt.timeSinceLastFrame);
       
        translate = Ogre::Vector3(0, 0, 0);
        return true;
    }
};

class BasicTutorial1
    : public ApplicationContext
    , public InputListener
{
private:
    SceneNode* paddleNode;
    SceneManager* scnMgr;
    Root* root;
public:
    BasicTutorial1();
    virtual ~BasicTutorial1() {}

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


BasicTutorial1::BasicTutorial1()
    : ApplicationContext("Assignment 1")
{
    score = 0;
    lives = 3;
}


void BasicTutorial1::setup()
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

void BasicTutorial1::createScene()
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
    Ogre::SceneNode* ballNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    ballNode->setPosition(0, 0, 0);
    ballNode->setScale(0.2f, 0.2f, 0.2f);
    ballNode->attachObject(ballEntity);

    Ogre::Entity* paddleEntity = scnMgr->createEntity(SceneManager::PrefabType::PT_PLANE);
    //Ogre::SceneNode* 
    paddleNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    paddleNode->setPosition(0, -10, 0);
    paddleNode->setScale(0.2f, 0.05f, 1.0f);
    paddleNode->attachObject(paddleEntity);

    scnMgr->showBoundingBoxes(true);

}

void BasicTutorial1::createCamera()
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

void BasicTutorial1::createFrameListener()
{
    Ogre::FrameListener* FrameListener = new ExampleFrameListener(paddleNode);
    mRoot->addFrameListener(FrameListener);
}

bool BasicTutorial1::keyPressed(const KeyboardEvent& evt)
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
        BasicTutorial1 app;
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