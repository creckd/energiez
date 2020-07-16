#include "CDPRRedBall.h"

#include "Core/EnergiezApp.h"
#include "Math/CDPRMathHelper.h"
#include "World/CDPRBirdManager.h"
#include "World/CDPRBird.h"
#include "CDPRPlayerController.h"

CDPRRedBall::CDPRRedBall(CDPRPlayerController* playerControllerReference) : _playerController(playerControllerReference)
{
	_playerController->RegisterProjectile(this);
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
	_defaultScale = _projectileNode->getScale();
	EnergiezApp::GetSingletonPtr()->RegisterFrameListener(this);

	_currentEnergy = _startingEnergy;
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
	if (_velocity.squaredLength() > 0.1f)
		_velocity = CDPRMathHelper::Vector3Lerp(_velocity, zeroVector, deltaTime * _dragAmount);
	else _velocity = zeroVector;

	_projectileNode->translate(_velocity);
}

void CDPRRedBall::Explode()
{
	_projectileNode->detachAllObjects();
	EnergiezApp::GetSingletonPtr()->UnRegisterFrameListener(this);
	EnergiezApp::GetSingletonPtr()->_mainSceneManager->destroyEntity(_projectileEntity);
	EnergiezApp::GetSingletonPtr()->_mainSceneManager->destroySceneNode(_projectileNode);
	_playerController->UnRegisterProjectile(this);
	delete this;
}

void CDPRRedBall::Consume(CDPRBird* birdToConsume)
{
	_numberOfBirdConsumed++;
	_currentEnergy += 1.0f;
	birdToConsume->Kill();
}

Vector3 CDPRRedBall::GetPosition()
{
	return _projectileNode->getPosition();
}

bool CDPRRedBall::frameStarted(const FrameEvent& evt)
{
	if (_projectileFlying) {
		Simulate(evt.timeSinceLastFrame);

		if(_currentEnergy <= 0.0f)
		{
			Explode();
			return true;
		}

		CDPRBirdManager* birdManager = EnergiezApp::GetSingletonPtr()->_birdManager;
		CDPRBird* closestBird = birdManager->_spawnedBirds[0];
		float closestDst = closestBird->_position.distance(_projectileNode->getPosition());
		for (auto bird : birdManager->_spawnedBirds)
		{
			if (bird == closestBird)
				continue;

			float currentDst = bird->_position.distance(_projectileNode->getPosition());
			if(currentDst <= closestDst)
			{
				closestBird = bird;
				closestDst = currentDst;
			}
		}

		if(closestDst <= _harmfulRange)
		{
			Consume(closestBird);
			if (_numberOfBirdConsumed >= _maxNumberOfBirdsConsumableBeforeExplode)
				Explode();
		} else if(closestDst <= _aggroRange)
		{
			if (_currentEnergy >= 0) {
				float boostForce = evt.timeSinceLastFrame;
				_velocity += (closestBird->_position - _projectileNode->getPosition()).normalisedCopy() * boostForce;
				_currentEnergy = Math::Clamp(_currentEnergy - boostForce,0.0f,Math::POS_INFINITY);
			}
		}

		_projectileNode->setScale(_defaultScale * _currentEnergy);

		if (_velocity.length() <= 0.5f)
			_currentEnergy -= evt.timeSinceLastFrame;
	}

	return true;
}
