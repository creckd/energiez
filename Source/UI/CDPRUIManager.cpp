#include "CDPRUIManager.h"
#include "Core/CDPRGameInstance.h"
#include "OgreOverlay.h"
#include "OgreOverlaySystem.h"
#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayContainer.h"

void CDPRUIManager::Initialize()
{
	CDPRGameInstance* gameInstance = CDPRGameInstance::GetSingletonPtr();
	gameInstance->_mainSceneManager->addRenderQueueListener(gameInstance->getOverlaySystem());

	CreateMainOverlay();
}

void CDPRUIManager::CreateMainOverlay()
{
	//Setup Crosshair
	OverlayManager& overlayManager = OverlayManager::getSingleton();
	Overlay* overlay = overlayManager.create("MainOverlay");
	OverlayElement* overlayElement = overlayManager.createOverlayElement("Panel", "PanelName");
	
	OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(overlayElement);
	
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setHorizontalAlignment(GHA_CENTER);
	panel->setVerticalAlignment(GVA_CENTER);

	int crossHairWidth = 100;
	
	panel->setPosition(-crossHairWidth/2, -crossHairWidth/2);
	panel->setDimensions(crossHairWidth, crossHairWidth);
	panel->setMaterialName("Dani/Crosshair");

	overlay->add2D(panel);
	overlay->show();
}
