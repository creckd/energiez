#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"

class CDPRRedBall;
class CDPRBlackBall;

class CDPRPlayerController : public OgreBites::InputListener
{
public:
	void Initialize();
	bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;

	void RegisterRedBall(CDPRRedBall* ball);
	void UnRegisterRedBall(CDPRRedBall* ball);
	
	void RegisterBlackBall(CDPRBlackBall* ball);
	void UnRegisterBlackBall(CDPRBlackBall* ball);
public:
	float _redBallShootingForce = 1.0f;
	float _blackBallShootingForce = 1.0f;
	std::vector<CDPRRedBall*> _spawnedRedBalls;
	std::vector<CDPRBlackBall*> _spawnedBlackBalls;
};
