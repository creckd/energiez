#include "CDPRRedBall.h"

#include "Core/CDPRGameInstance.h"
#include "Math/CDPRMathHelper.h"
#include "World/CDPRBirdManager.h"
#include "World/CDPRBird.h"
#include "CDPRPlayerController.h"


CDPRRedBall::CDPRRedBall(CDPRPlayerController* playerController) : CDPRProjectile(playerController, "paperplane.mesh", "redball")
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

		if (_velocity.length() <= _velocityMagnitudeToKillAt)
		{
			Explode();
			return;
		}

		CDPRBirdManager* birdManager = CDPRGameInstance::GetSingletonPtr()->_birdManager;
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
		}
		else if (closestDst <= _aggroRange)
		{
				_velocity += (closestBird->_position - _projectileNode->getPosition()).normalisedCopy() * deltaTime * _chargeSpeed * (1.0f - _initialVelocityPreference);
		}

		_projectileNode->setScale(Vector3::UNIT_SCALE + _numberOfBirdConsumed * _growthAmountPerKill);

		_initialVelocityPreference = Math::Clamp(_initialVelocityPreference - deltaTime, 0.0f, 1.0f);

		_projectileNode->lookAt(GetPosition() + _velocity.normalisedCopy(), Node::TS_WORLD, Vector3::UNIT_Z);
	}
}

void CDPRRedBall::Explode()
{
	_playerController->UnRegisterRedBall(this);
	CDPRGameInstance::GetSingletonPtr()->_birdManager->SpawnBirdsAtCustomSpawnPoint(_numberOfBirdConsumed, GetPosition());
	Destroy();
}

void CDPRRedBall::Consume(CDPRBird* birdToConsume)
{
	_numberOfBirdConsumed++;
	birdToConsume->Kill();
}
