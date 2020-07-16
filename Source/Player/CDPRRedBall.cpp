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
	
	_playerController->RegisterRedBall(this);
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
				//_velocity += (closestBird->_position - _projectileNode->getPosition()).normalisedCopy() * deltaTime;
		}

		//_projectileNode->setScale(Vector3::UNIT_SCALE * _currentEnergy);

		//_currentEnergy -= _velocity.length();
	}
}

void CDPRRedBall::Explode()
{
	_playerController->UnRegisterRedBall(this);
	Destroy();
}

void CDPRRedBall::Consume(CDPRBird* birdToConsume)
{
	_numberOfBirdConsumed++;
	_currentEnergy += 1.0f;
	birdToConsume->Kill();
}
