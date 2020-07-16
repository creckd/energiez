#include "CDPRRedBall.h"

#include "Core/EnergiezApp.h"
#include "Math/CDPRMathHelper.h"
#include "World/CDPRBirdManager.h"
#include "World/CDPRBird.h"
#include "CDPRPlayerController.h"


CDPRRedBall::CDPRRedBall(CDPRPlayerController* playerController) : CDPRProjectile(playerController, "icosphere.mesh", "redball")
{

}

void CDPRRedBall::Initalize()
{
	CDPRProjectile::Initalize();
	
	_playerController->RegisterProjectile(this);
}

void CDPRRedBall::Update(float deltaTime)
{
	CDPRProjectile::Update(deltaTime);
	
	if (_projectileFlying) {

		if (_currentEnergy <= 0.0f)
		{
			Explode();
			return;
		}

		CDPRBirdManager* birdManager = EnergiezApp::GetSingletonPtr()->_birdManager;
		CDPRBird* closestBird = birdManager->_spawnedBirds[0];
		float closestDst = closestBird->_position.distance(_projectileNode->getPosition());
		for (auto bird : birdManager->_spawnedBirds)
		{
			if (bird == closestBird)
				continue;

			float currentDst = bird->_position.distance(_projectileNode->getPosition());
			if (currentDst <= closestDst)
			{
				closestBird = bird;
				closestDst = currentDst;
			}
		}

		if (closestDst <= _harmfulRange)
		{
			Consume(closestBird);
			if (_numberOfBirdConsumed >= _maxNumberOfBirdsConsumableBeforeExplode)
				Explode();
		}
		else if (closestDst <= _aggroRange)
		{
			if (_currentEnergy >= 0) {
				float boostForce = deltaTime;
				_velocity += (closestBird->_position - _projectileNode->getPosition()).normalisedCopy() * boostForce;
				_currentEnergy = Math::Clamp(_currentEnergy - boostForce, 0.0f, Math::POS_INFINITY);
			}
		}

		_projectileNode->setScale(_defaultScale * _currentEnergy);

		if (_velocity.length() <= 0.5f)
			_currentEnergy -= deltaTime;
	}
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
