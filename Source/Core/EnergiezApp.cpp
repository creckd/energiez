#include "EnergiezApp.h"

#include "Camera/CDPRCameraController.h"
#include "World/CDPRWorld.h"
#include "World/CDPRBirdManager.h"
#include "Player/CDPRPlayerController.h"
#include "UI/CDPRUIManager.h"

EnergiezApp *EnergiezApp::_instance = nullptr;

EnergiezApp::EnergiezApp() : OgreBites::ApplicationContext("Energiez")
{
	_instance = this;
}

EnergiezApp* EnergiezApp::GetSingletonPtr()
{
	return _instance;
}

void EnergiezApp::setup()
{
	OgreBites::ApplicationContext::setup();

	addInputListener(this);
	Ogre::Root* root = getRoot();
	setWindowGrab(true);

	_mainSceneManager = root->createSceneManager();

	_mainSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	_mainSceneManager->setAmbientLight(ColourValue(1, 1, 1));
	_mainSceneManager->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(_mainSceneManager);

	// SETUP CAMERA
	_cameraController = new CDPRCameraController();
	_cameraController->Initialize();
	addInputListener(_cameraController);
	RegisterFrameListener(_cameraController);

	// SETUP WORLD
	_world = new CDPRWorld();
	_world->CreateWorld();

	// SETUP UI
	_uiManager = new CDPRUIManager();
	_uiManager->Initialize();

	_birdManager = new CDPRBirdManager();
	_birdManager->Initialize();
	_birdManager->SpawnBirds(500);

	_playerManager = new CDPRPlayerController();
	_playerManager->Initialize();
	addInputListener(_playerManager);

	Light* directionalLight = _mainSceneManager->createLight("DirectionalLight");
	directionalLight->setType(Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(1, 1, 1);
	directionalLight->setSpecularColour(1, 1, 1);

	SceneNode* directionalLightNode = _mainSceneManager->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(0, -0.83, 0.75);

}

bool EnergiezApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
		return true;
	}
	return false;
}

bool EnergiezApp::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
	if (_cameraController != nullptr)
		_cameraController->MouseInput(evt.xrel * -0.01f, evt.yrel * -0.01f);

	return true;
}

void EnergiezApp::RegisterFrameListener(FrameListener* newListener)
{
	getRoot()->addFrameListener(newListener);
}

void EnergiezApp::UnRegisterFrameListener(FrameListener* newListener)
{
	getRoot()->removeFrameListener(newListener);
}
