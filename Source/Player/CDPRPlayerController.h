#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"

class CDPRRedBall;

class CDPRPlayerController : public OgreBites::InputListener
{
public:
	void Initialize();
	bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;

	void RegisterProjectile(CDPRRedBall* ball);
	void UnRegisterProjectile(CDPRRedBall* ball);
public:
	float _redBallShootingForce = 1.0f;
	float _blackBallShootingForce = 1.0f;
	std::vector<CDPRRedBall*> _spawnedBalls;
};
