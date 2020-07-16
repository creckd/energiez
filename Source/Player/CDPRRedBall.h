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
	float _startingEnergy = 1.0f;
	float _harmfulRange = 5.0f;
	float _aggroRange = 20.0f;

	int _maxNumberOfBirdsConsumableBeforeExplode = 5;
private:
	float _currentEnergy = 1.0f;
	int _numberOfBirdConsumed = 0;
};