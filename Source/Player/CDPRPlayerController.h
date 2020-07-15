#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"

class CDPRPlayerController : public OgreBites::InputListener
{
public:
	void Initialize();
	bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
public:
	float _redBallShootingForce = 1.0f;
};
