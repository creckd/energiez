#include "CDPRMathHelper.h"

#include "Ogre.h"

Ogre::Vector3 CDPRMathHelper::Vector3Lerp(Ogre::Vector3& srcLocation, Ogre::Vector3& destLocation, Ogre::Real Time)
{
	return srcLocation + (destLocation - srcLocation) * Time;
}
