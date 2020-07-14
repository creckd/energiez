#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreFrameListener.h"

using namespace Ogre;

class CDPRCameraController : public FrameListener, public OgreBites::InputListener
{
public:
	void Initialize();
	void Update(const FrameEvent& evt);
	bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
	bool frameStarted(const FrameEvent& evt) override;

private:
	Camera* _mainCamera = nullptr;
	SceneNode* _cameraNode = nullptr;
	SceneNode* _cameraYawNode = nullptr;
	SceneNode* _cameraPitchNode = nullptr;
	SceneNode* _cameraRollNode = nullptr;
	
};