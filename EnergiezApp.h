#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreFrameListener.h>

class CDPRCameraController;

using namespace Ogre;

class EnergiezApp : public OgreBites::ApplicationContext, public OgreBites::InputListener, public FrameListener
{
public:
	static EnergiezApp* GetSingletonPtr();
	
	EnergiezApp();
	
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	bool frameStarted(const FrameEvent& evt) override;

public:
	SceneManager* _mainSceneManager = nullptr;
	CDPRCameraController* _cameraController = nullptr;
	
private:
	static EnergiezApp* _instance;
	
	SceneNode* _ninjaNode;
};