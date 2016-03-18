#ifndef _crash3d_h_
#define _crash3d_h_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include "grid.h"

class Crash3d : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:    

    Crash3d();
    virtual ~Crash3d();
    bool go();

protected:

    Ogre::Root *mRoot;
    Ogre::LogManager *mLog;
    Ogre::Camera *mCamera;
    Ogre::SceneManager *mSceneMgr;
    Ogre::RenderWindow *mWindow;
    Ogre::Viewport *mViewport;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    Ogre::OverlaySystem *mOverlaySystem;
    OgreGrid* mGrid;
    Ogre::RaySceneQuery *mRaySceneQuery;
    Ogre::SceneNode *mSelectedNode;

    OgreBites::InputContext mInputContext;
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;
    OgreBites::ParamsPanel* mDetailsPanel;
    bool mShutDown;

    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    Ogre::SceneNode* getNodeHit(int x, int y);

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
};

#endif // _crash3d_h_
