#include "CDPRCameraController.h"
#include "EnergiezApp.h"

void CDPRCameraController::Initialize()
{
	SceneManager* sceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;
	
	_cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode("MainCameraNode");
	_cameraNode->setPosition(200, 300, 400);

	_cameraYawNode = _cameraNode->createChildSceneNode("MainCameraYaw");
	_cameraPitchNode = _cameraYawNode->createChildSceneNode("MainCameraPitch");
	_cameraRollNode = _cameraPitchNode->createChildSceneNode("MainCameraRoll");
	//_cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	_mainCamera = sceneManager->createCamera("MainCamera");
	_mainCamera->setNearClipDistance(5); // specific to this sample
	_mainCamera->setAutoAspectRatio(true);
	
	_cameraRollNode->attachObject(_mainCamera);

	Viewport* vp = EnergiezApp::GetSingletonPtr()->getRenderWindow()->addViewport(_mainCamera);
}

void CDPRCameraController::Update(const FrameEvent& evt)
{
	_cameraYawNode->yaw(Radian(evt.timeSinceLastFrame));
}

void CDPRCameraController::MouseInput(float x, float y)
{
	_cameraYawNode->yaw(Radian(x));
	_cameraPitchNode->pitch(Radian(y));
}

bool CDPRCameraController::frameStarted(const FrameEvent& evt)
{
	// NOTE: We multiply the mTranslateVector by the cameraPitchNode's
// orientation quaternion and the cameraYawNode's orientation
// quaternion to translate the camera accoding to the camera's
// orientation around the Y-axis and the X-axis.
		_cameraNode->translate(_cameraYawNode->getOrientation() * _cameraPitchNode->getOrientation() * _translateVector,
		Ogre::SceneNode::TS_LOCAL);
	
	return true;
}

bool CDPRCameraController::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	switch (evt.keysym.sym)
	{
	case KEYCODE_W:
		_translateVector.z = -1.0f;
		break;
	case KEYCODE_A:
		_translateVector.x = -1.0f;
		break;
	case KEYCODE_S:
		_translateVector.z = 1.0f;
		break;
	case KEYCODE_D:
		_translateVector.x = 1.0f;
		break;
	}
	
	return false;
}

bool CDPRCameraController::keyReleased(const OgreBites::KeyboardEvent& evt)
{
	switch (evt.keysym.sym)
	{
	case KEYCODE_W:
		_translateVector.z = 0.0f;
		break;
	case KEYCODE_A:
		_translateVector.x = 0.0f;
		break;
	case KEYCODE_S:
		_translateVector.z = 0.0f;
		break;
	case KEYCODE_D:
		_translateVector.x = 0.0f;
		break;
	}


	return false;
}

Vector3 CDPRCameraController::GetEulerOrientation()
{
	return Vector3(
		Ogre::Math::Floor(2 * Ogre::Degree(Ogre::Math::ACos(this->_cameraYawNode->getOrientation().w)).valueDegrees()),
		Ogre::Math::Floor(2 * Ogre::Degree(Ogre::Math::ACos(this->_cameraPitchNode->getOrientation().w)).valueDegrees()),
		Ogre::Math::Floor(2 * Ogre::Degree(Ogre::Math::ACos(this->_cameraNode->getOrientation().w)).valueDegrees())
	);
}
