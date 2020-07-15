#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreFrameListener.h>

class CDPRCameraController;
class CDPRWorld;
class CDPRUIManager;
class CDPRBirdManager;
class CDPRPlayerController;

using namespace Ogre;

class EnergiezApp : public OgreBites::ApplicationContextSDL, public OgreBites::InputListener
{
public:
	static EnergiezApp* GetSingletonPtr();
	
	EnergiezApp();

	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;

	void RegisterFrameListener(FrameListener* newListener);

public:
	SceneManager* _mainSceneManager = nullptr;
	CDPRCameraController* _cameraController = nullptr;
	CDPRUIManager* _uiManager = nullptr;
	CDPRWorld* _world = nullptr;
	CDPRBirdManager* _birdManager = nullptr;
	CDPRPlayerController* _playerManager = nullptr;
	
private:
	static EnergiezApp* _instance;
};