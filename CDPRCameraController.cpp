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

bool CDPRCameraController::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
	float pitchAngle;
	float pitchAngleSign;

	_cameraYawNode->yaw(Radian(1));
	_cameraPitchNode->pitch(Radian(evt.yrel));

	//_cameraNode->translate()
	//
	//this->cameraNode->translate(this->cameraYawNode->getOrientation() *
	//	this->cameraPitchNode->getOrientation() *
	//	this->mTranslateVector,
	//	Ogre::SceneNode::TS_LOCAL);
	return true;
}

bool CDPRCameraController::frameStarted(const FrameEvent& evt)
{
	_cameraYawNode->yaw(Radian(evt.timeSinceLastFrame));

	return true;
}
