#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreFrameListener.h>

class CDPRCameraController;
class CDPRWorld;

using namespace Ogre;

class EnergiezApp : public OgreBites::ApplicationContext, public OgreBites::InputListener
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
	CDPRWorld* _world;
	
private:
	static EnergiezApp* _instance;
};