#include "CDPRCameraController.h"
#include "Core/EnergiezApp.h"

void CDPRCameraController::Initialize()
{
	SceneManager* sceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;
	
	_cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode("MainCameraNode");
	_cameraNode->setPosition(0, _characterHeight, 0);

	_cameraYawNode = _cameraNode->createChildSceneNode("MainCameraYaw");
	_cameraPitchNode = _cameraYawNode->createChildSceneNode("MainCameraPitch");
	_cameraRollNode = _cameraPitchNode->createChildSceneNode("MainCameraRoll");
	//_cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	_mainCamera = sceneManager->createCamera("MainCamera");
	_mainCamera->setNearClipDistance(0.01); // specific to this sample
	_mainCamera->setAutoAspectRatio(true);
	
	_cameraRollNode->attachObject(_mainCamera);

	Viewport* vp = EnergiezApp::GetSingletonPtr()->getRenderWindow()->addViewport(_mainCamera);
}

void CDPRCameraController::MouseInput(float x, float y)
{
	_cameraYawNode->yaw(Radian(x));
	_cameraPitchNode->pitch(Radian(y));
}

bool CDPRCameraController::frameStarted(const FrameEvent& evt)
{
	Vector3 movementVector = _cameraYawNode->getOrientation() * _cameraPitchNode->getOrientation() * _translateVector;
	movementVector.y = 0.0f;
	movementVector.normalise();

	movementVector *= _movementSpeed;
	
	_cameraNode->translate(movementVector,
	Ogre::SceneNode::TS_LOCAL);

	Real pitchAngle;
	Real pitchAngleSign;

	pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(this->_cameraPitchNode->getOrientation().w)).valueDegrees());
	pitchAngleSign = this->_cameraPitchNode->getOrientation().x;

	if (pitchAngle > 90.0f)
	{
		if (pitchAngleSign > 0)
			this->_cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
				Ogre::Math::Sqrt(0.5f), 0, 0));
		else if (pitchAngleSign < 0)
			this->_cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
				-Ogre::Math::Sqrt(0.5f), 0, 0));
	}
	
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
