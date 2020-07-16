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

class CDPRGameInstance : public OgreBites::ApplicationContextSDL, public OgreBites::InputListener
{
public:
	static CDPRGameInstance* GetSingletonPtr();
	
	CDPRGameInstance();

	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;

	void RegisterFrameListener(FrameListener* newListener);
	void UnRegisterFrameListener(FrameListener* newListener);

public:
	SceneManager* _mainSceneManager = nullptr;
	CDPRCameraController* _cameraController = nullptr;
	CDPRUIManager* _uiManager = nullptr;
	CDPRWorld* _world = nullptr;
	CDPRBirdManager* _birdManager = nullptr;
	CDPRPlayerController* _playerManager = nullptr;
	
private:
	static CDPRGameInstance* _instance;
};