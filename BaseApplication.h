#ifndef __BaseApplication_h_
#define __BaseApplication_h_

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

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h> //from 0.8 it's just Ogre/Renderer.h

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include <string.h>

class Level;

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    BaseApplication(void);
    ~BaseApplication(void);
    void go(void);

protected:
    CEGUI::OgreRenderer* mRenderer;

    bool setup();
    bool configure(void);
    void chooseSceneManager(void);
    void createCamera(void);
    void createFrameListener(void);
    void createScene(void);
    void destroyScene(void);
    void createViewports(void);
    void setupResources(void);
    void createResourceListener(void);
    void loadResources(void);
    void setupGUI(void);

    // Ogre::FrameListener
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    bool quit(const CEGUI::EventArgs &e);

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    Ogre::OverlaySystem *mOverlaySystem;

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;       // basic camera controller
    OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;


    //OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    //Level
    Level* level;


};
#endif // #ifndef __BaseApplication_h_
