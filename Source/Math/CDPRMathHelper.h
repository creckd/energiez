#pragma once
#include <Ogre.h>

class CDPRMathHelper
{
public:
	static Ogre::Vector3 Vector3Lerp(Ogre::Vector3 &srcLocation, Ogre::Vector3 &destLocation, Ogre::Real Time);
};
