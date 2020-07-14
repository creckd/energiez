#include "World/CDPRSkyScraper.h"

CDPRSkyScraper::CDPRSkyScraper(SceneManager* mainSceneManager, SceneNode* cityRoot, int skyScraperHeight)
{
	_mainSceneManager = mainSceneManager;
	_cityRootNode = cityRoot;
	_height = skyScraperHeight;
}

void CDPRSkyScraper::Spawn(Vector3 rootWorldPosition)
{
	for(int i=0;i<_height;i++)
	{
		Entity* spawnedSkyScraper = _mainSceneManager->createEntity(SkyScraperBlockMeshResource);
		spawnedSkyScraper->setMaterialName(SkyScraperBlockMaterialResource);
		spawnedSkyScraper->setCastShadows(true);

		SceneNode* spawnedSceneNode = _cityRootNode->createChildSceneNode();
		spawnedSceneNode->setPosition(rootWorldPosition + Vector3(0.0f,(BlenderToOgreUnitScale/2.0f) + (i * BlenderToOgreUnitScale),0.0f));
		spawnedSceneNode->attachObject(spawnedSkyScraper);
		spawnedSceneNode->setScale(1.0, 1.0, 1.0);

		_spawnedBlocks.emplace(spawnedSceneNode, spawnedSkyScraper);
	}
}
