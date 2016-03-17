
#include "crash3d.h"
#include <cstdlib>

Crash3d::Crash3d()
    : mRoot(0),
    mLog(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),    
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mOverlaySystem(0),
    mGrid(0),
    mRaySceneQuery(0),
    mSelectedNode(0)
{
}

Crash3d::~Crash3d()
{   	
	//DestroyMaterials();

    if(mRaySceneQuery)
        mSceneMgr->destroyQuery(mRaySceneQuery);
    if(mGrid)
        delete mGrid;
    if (mTrayMgr)
        delete mTrayMgr;
    if (mCameraMan)
        delete mCameraMan;
    if (mOverlaySystem)
        delete mOverlaySystem;

    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

bool Crash3d::go()
{    
    mResourcesCfg = "./resources.cfg";
    mPluginsCfg = "./plugins.cfg";

    mRoot = new Ogre::Root(mPluginsCfg);
    mLog = Ogre::LogManager::getSingletonPtr();	

    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);    

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }    

    if(mRoot->restoreConfig() || mRoot->showConfigDialog())    
        mWindow = mRoot->initialise(true, "Crash3d");
    else return false;

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);

    mCamera = mSceneMgr->createCamera("SpectatorCamera");            
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
    mCameraMan->setStyle(OgreBites::CS_ORBIT);	

	mCamera->setNearClipDistance(200);
	mCamera->setPosition(Ogre::Vector3(0, 5000, 15000));    
    mCamera->lookAt(Ogre::Vector3(0, 0, 0));

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0.0078, 0.239, 0.231));

    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);    

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	

    Ogre::Real r = 0, g = 1, b = 0;

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<10; j++)
        {
            Ogre::ColourValue col(r, g, b);
            std::stringstream name;
            name << "material" << j + i * 10;
            Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingletonPtr()->create(name.str(), "General");
            mat->getTechnique(0)->getPass(0)->setAmbient(col);
            mat->getTechnique(0)->getPass(0)->setDiffuse(col);

            if(i == 0)
                r += 0.1;
            else g -= 0.1;
        }
    }

    for(int i=0; i<20; i++)
    {
        std::stringstream name;
        name << "sphere" << i;
        Ogre::Entity* sphere = mSceneMgr->createEntity(name.str(), "sphere.mesh");
        Ogre::SceneNode* sphereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        sphereNode->attachObject(sphere);
        sphereNode->setPosition(-5000 + std::rand() % 10000, std::rand() % 5000, -5000 + std::rand() % 10000);		        
        std::stringstream mn;
        mn << "material" << i;
        sphere->setMaterialName(mn.str());
    }

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));

    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(2000,18000,5000);
    
    mLog->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

#if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);    
    mTrayMgr->toggleAdvancedFrameStats();
    mTrayMgr->hideCursor();    

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("Session");
    items.push_back("Spectrum");
    items.push_back("Lat start");
    items.push_back("Lat end");
    items.push_back("Lon start");
    items.push_back("Lon end");
    items.push_back("Alt start");
    items.push_back("Alt end");
    items.push_back("Time start");
    items.push_back("Time end");
    items.push_back("Count");
    items.push_back("Doserate");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 300, items);
    mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
    mDetailsPanel->show();

    mGrid = new OgreGrid(mSceneMgr, "Template/Cyan");
    mGrid->attachToNode(mSceneMgr->getRootSceneNode());
    mGrid->setCellSize(1000);
    mGrid->update();
    mGrid->show();

    mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
    mRaySceneQuery->setSortByDistance(true);    

    mRoot->addFrameListener(this);
    mRoot->startRendering();    

    return true;
}

Ogre::SceneNode* Crash3d::getNodeHit(int x, int y)
{    
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(
                x / float(mWindow->getViewport(0)->getActualWidth()),
                y / float(mWindow->getViewport(0)->getActualHeight()));

    mRaySceneQuery->setRay(mouseRay);
    Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it = result.begin();

    Ogre::SceneNode* hit = 0;
    for(; it != result.end(); it++)
    {
        Ogre::String name = it->movable->getName();
        if (name.find_first_of("sphere") == 0)
        {
            hit = it->movable->getParentSceneNode();
            break;
        }
    }
    return hit;
}

bool Crash3d::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible() && mSelectedNode)   // if details panel is visible, then update its contents
        {
            Ogre::Entity* ent = static_cast<Ogre::Entity*>(mSelectedNode->getAttachedObject(0));
            mDetailsPanel->setParamValue(0, "01012016_000000");
            mDetailsPanel->setParamValue(1, ent->getName());
			std::stringstream ss;
			ss << mCamera->getPosition().x << " " << mCamera->getPosition().y << " " << mCamera->getPosition().z;
			mDetailsPanel->setParamValue(2, ss.str());
            /*            
            mDetailsPanel->setParamValue(3, ent->getName());
            mDetailsPanel->setParamValue(4, ent->getName());
            mDetailsPanel->setParamValue(5, ent->getName());
            mDetailsPanel->setParamValue(6, ent->getName());
            mDetailsPanel->setParamValue(7, ent->getName());
            mDetailsPanel->setParamValue(8, ent->getName());
            mDetailsPanel->setParamValue(9, ent->getName());
            mDetailsPanel->setParamValue(10, ent->getName());
            mDetailsPanel->setParamValue(11, ent->getName());
            */
        }
        else
        {
            mDetailsPanel->setParamValue(1, "");
        }
    }

    return true;
}

bool Crash3d::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible())
        return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if(arg.key == OIS::KC_X)
    {
        if(mSelectedNode)
        {
            mCameraMan->setTarget(mSelectedNode);
            mCamera->moveRelative(Ogre::Vector3(0, 0, 5000));
        }
    }    
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {        
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:            
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:            
            pm = Ogre::PM_POINTS;
            break;
        default:            
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);        
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    mCameraMan->injectKeyDown(arg);
    return true;
}

bool Crash3d::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);
    return true;
}

bool Crash3d::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg))
        return true;

    mCameraMan->injectMouseMove(arg);

    return true;
}

bool Crash3d::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id))
        return true;

    mCameraMan->injectMouseDown(arg, id);

    if (id == OIS::MB_Left)
    {
        Ogre::SceneNode* node = getNodeHit(arg.state.X.abs, arg.state.Y.abs);
        if(node)	
            mSelectedNode = node;        		
    }
    else if(id == OIS::MB_Right)
    {        
        mSelectedNode = 0;
    }

    return true;
}

bool Crash3d::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id))
        return true;

    mCameraMan->injectMouseUp(arg, id);

    return true;
}

// Adjust mouse clipping area
void Crash3d::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// Unattach OIS before window shutdown
void Crash3d::windowClosed(Ogre::RenderWindow* rw)
{
    // Only close for window that created OIS
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
