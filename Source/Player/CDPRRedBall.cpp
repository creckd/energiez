#include "CDPRRedBall.h"

#include "Core/EnergiezApp.h"
#include "Math/CDPRMathHelper.h"

CDPRRedBall::CDPRRedBall()
{
}

void CDPRRedBall::Initalize()
{
	SceneManager* sceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;
	
	_projectileEntity = sceneManager->createEntity(MeshResourceName);
	_projectileEntity->setMaterialName(MaterialResourceName);
	_projectileEntity->setCastShadows(true);
	_projectileEntity->setVisible(false);

	_projectileNode = sceneManager->getRootSceneNode()->createChildSceneNode();

	CDPRWorld* world = EnergiezApp::GetSingletonPtr()->_world;
;
	_projectileNode->attachObject(_projectileEntity);
	_projectileNode->setScale(Vector3::UNIT_SCALE);
	EnergiezApp::GetSingletonPtr()->RegisterFrameListener(this);
}

void CDPRRedBall::Shoot(Vector3&origin, Vector3& direction, float force)
{
	_projectileNode->setPosition(origin);
	_projectileEntity->setVisible(true);
	_projectileFlying = true;
	_velocity = direction * force;
}

void CDPRRedBall::Simulate(float deltaTime)
{
	Vector3 zeroVector = Vector3::ZERO;
	if(_velocity.squaredLength() > 0.1f)
		_velocity = CDPRMathHelper::Vector3Lerp(_velocity, zeroVector, deltaTime * _dragAmount);
	else _velocity = zeroVector;

	_projectileNode->translate(_velocity);
}

bool CDPRRedBall::frameStarted(const FrameEvent& evt)
{
	if (_projectileFlying)
		Simulate(evt.timeSinceLastFrame);
	
	return true;
}
