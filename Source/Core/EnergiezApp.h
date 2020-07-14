#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreFrameListener.h>

class CDPRCameraController;
class CDPRWorld;
class CDPRUIManager;

using namespace Ogre;

class EnergiezApp : public OgreBites::ApplicationContextSDL, public OgreBites::InputListener
{
public:
	static EnergiezApp* GetSingletonPtr();
	
	EnergiezApp();

	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;

public:
	SceneManager* _mainSceneManager = nullptr;
	CDPRCameraController* _cameraController = nullptr;
	CDPRUIManager* _uiManager = nullptr;
	CDPRWorld* _world = nullptr;
	
private:
	static EnergiezApp* _instance;
};