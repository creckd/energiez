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
	void MouseInput(float x, float y);
	bool frameStarted(const FrameEvent& evt) override;
	bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
	bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
	
	Vector3 GetEulerOrientation();

public:
	float _cameraSpeed = 1.0f;

private:
	Camera* _mainCamera = nullptr;
	SceneNode* _cameraNode = nullptr;
	SceneNode* _cameraYawNode = nullptr;
	SceneNode* _cameraPitchNode = nullptr;
	SceneNode* _cameraRollNode = nullptr;
	
	Vector3 _translateVector = Vector3::ZERO;
};

enum
{
	KEYCODE_W = 119,
	KEYCODE_A = 97,
	KEYCODE_S = 115,
	KEYCODE_D = 100
};