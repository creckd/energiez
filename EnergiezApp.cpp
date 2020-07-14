#include "EnergiezApp.h"

EnergiezApp::EnergiezApp() : OgreBites::ApplicationContext("Energiez")
{
}

void EnergiezApp::setup()
{
	OgreBites::ApplicationContext::setup();

	addInputListener(this);

	Ogre::Root* root = getRoot();
	Ogre::SceneManager* scnMgr = root->createSceneManager();

	scnMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

	scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->setPosition(200, 300, 400);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	Ogre::Camera* cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);

	Viewport* vp = getRenderWindow()->addViewport(cam);

	Entity* ninjaEntity = scnMgr->createEntity("Cube.mesh");
	ninjaEntity->setMaterialName("danidiffuse");
	ninjaEntity->setCastShadows(true);

	_ninjaNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	_ninjaNode->attachObject(ninjaEntity);
	_ninjaNode->setScale(Vector3(50, 50, 50));

	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"ground", RGN_DEFAULT,
		plane,
		1500, 1500, 20, 20,
		true,
		1, 5, 5,
		Vector3::UNIT_Z);

	Entity* groundEntity = scnMgr->createEntity("ground");
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Examples/Rockwall");
	scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);;

	Light* directionalLight = scnMgr->createLight("DirectionalLight");
	directionalLight->setType(Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(1, 1, 1);
	directionalLight->setSpecularColour(1, 1, 1);

	SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(0, -1, 1);
}

bool EnergiezApp::frameStarted(const Ogre::FrameEvent& evt)
{
	_ninjaNode->translate(Vector3(0.0f, evt.timeSinceLastFrame, 0.0f));
	return true;
}

bool EnergiezApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	return true;
}
