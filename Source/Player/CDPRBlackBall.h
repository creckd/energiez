#pragma once
#include "CDPRProjectile.h"

class CDPRPlayerController;

class CDPRBlackBall : public CDPRProjectile
{
public:
	CDPRBlackBall(CDPRPlayerController* playerController);

	void Initalize() override;
	void Update(float deltaTime) override;
	void Explode();
public:
	float _blackHoleForce = 0.0f;
	float _activationLimitVelocityMagnitude = 1.0f;
	float _collapsingTime = 3.0f;
	float _collapseScaleAmount = 20.0f;
private:
	bool _activated = false;
	Vector3 _defaultScale;

};
