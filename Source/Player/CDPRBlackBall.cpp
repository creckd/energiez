#include "CDPRBlackBall.h"
#include "Player/CDPRPlayerController.h"

CDPRBlackBall::CDPRBlackBall(CDPRPlayerController* playerController) : CDPRProjectile(playerController,"icosphere.mesh","blackball")
{
	_dragAmount = 3.0f;
}

void CDPRBlackBall::Initalize()
{
	CDPRProjectile::Initalize();

	_playerController->RegisterBlackBall(this);

	_defaultScale = Vector3(0.1f, 0.1f, 0.1f);
	_projectileNode->setScale(_defaultScale);
	_blackHoleForce = 0.0f;
}

void CDPRBlackBall::Update(float deltaTime)
{
	CDPRProjectile::Update(deltaTime);

	if(_velocity.length() <= _activationLimitVelocityMagnitude && !_activated)
	{
		_activated = true;
	}

	if(_activated)
	{
		_blackHoleForce = Math::Clamp(_blackHoleForce + deltaTime * (1.0f/_collapsingTime),0.0f,1.0f);
	}

	_projectileNode->setScale(_defaultScale * (1.0f + _blackHoleForce * _collapseScaleAmount));

	if (_blackHoleForce >= 1.0f)
		Explode();
}

void CDPRBlackBall::Explode()
{
	_playerController->UnRegisterBlackBall(this);
	Destroy();
}
