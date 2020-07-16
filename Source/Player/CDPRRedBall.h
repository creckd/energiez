#pragma once

#include "Ogre.h"
#include "CDPRProjectile.h"

class CDPRBird;
class CDPRPlayerController;

using namespace Ogre;

class CDPRRedBall : public CDPRProjectile
{
public:
	CDPRRedBall(CDPRPlayerController* playerController);
	
	void Initalize() override;
	void Update(float deltaTime) override;
	void Explode();
	void Consume(CDPRBird* birdToConsume);
public:
	float _harmfulRange = 5.0f;
	float _aggroRange = 20.0f;
	float _velocityMagnitudeToKillAt = 0.1f;
	float _growthAmountPerKill = 0.05f;
	float _chargeSpeed = 5.0f;
	float _maxSpeed = 15.0f;
private:
	float _initialVelocityPreference = 1.0f;
	int _numberOfBirdConsumed = 0;
};