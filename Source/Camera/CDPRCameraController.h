#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreFrameListener.h"

using namespace Ogre;

class CDPRCameraController : public FrameListener, public OgreBites::InputListener
{
public:
	void Initialize();
	void MouseInput(float x, float y);
	bool frameStarted(const FrameEvent& evt) override;
	bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
	bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
	
	Vector3 GetEulerOrientation();
	Vector3 GetForwardVector();
	Vector3 GetWorldPosition();
public:
	float _movementSpeed = 0.1f;
	float _characterHeight = 1.5f;
	
	float _headBobAmount = 0.05f;
	float _headBobSpeed = 2.5f;

	float _runningHeadBobAmount = 0.05f;
	float _runningHeadBobSpeed = 20.0f;

	float _mouseSensitivity = 0.3f;

private:
	Camera* _mainCamera = nullptr;
	SceneNode* _cameraNode = nullptr;
	SceneNode* _cameraYawNode = nullptr;
	SceneNode* _cameraPitchNode = nullptr;
	SceneNode* _cameraRollNode = nullptr;
	
	Vector3 _translateVector = Vector3::ZERO;

	float _timeSinceGameLaunched = 0.0f;
};

enum
{
	KEYCODE_W = 119,
	KEYCODE_A = 97,
	KEYCODE_S = 115,
	KEYCODE_D = 100
};