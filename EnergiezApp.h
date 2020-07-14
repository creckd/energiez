#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreFrameListener.h>

using namespace Ogre;

class EnergiezApp : public OgreBites::ApplicationContext, public OgreBites::InputListener, public FrameListener
{
public:
	EnergiezApp();
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	bool frameStarted(const FrameEvent& evt) override;

private:
	SceneNode* _ninjaNode;
};