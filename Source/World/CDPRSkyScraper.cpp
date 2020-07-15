#include "World/CDPRSkyScraper.h"

CDPRSkyScraper::CDPRSkyScraper(SceneManager* mainSceneManager, SceneNode* cityRoot, int skyScraperHeight)
{
	_mainSceneManager = mainSceneManager;
	_cityRootNode = cityRoot;
	_height = skyScraperHeight;
}

void CDPRSkyScraper::Spawn(Vector3 rootWorldPosition, float worldSpaceWidth)
{
	// For simple bounding box raycast uses later...
	_boxBoundPoints[0] = rootWorldPosition + Vector3(-worldSpaceWidth / 2, 0, -worldSpaceWidth / 2);
	_boxBoundPoints[1] = rootWorldPosition + Vector3(worldSpaceWidth / 2, _height * worldSpaceWidth, worldSpaceWidth / 2);

	for (int i = 0; i < _height; i++)
	{
		Entity* spawnedSkyScraper = _mainSceneManager->createEntity(SkyScraperBlockMeshResource);
		spawnedSkyScraper->setMaterialName(SkyScraperBlockMaterialResource);
		spawnedSkyScraper->setCastShadows(true);

		SceneNode* spawnedSceneNode = _cityRootNode->createChildSceneNode();
		
		spawnedSceneNode->setPosition(rootWorldPosition + Vector3(0.0f, (worldSpaceWidth / 2) + (worldSpaceWidth * i), -worldSpaceWidth/2));
		spawnedSceneNode->attachObject(spawnedSkyScraper);
		spawnedSceneNode->setScale(worldSpaceWidth / BlenderToOgreUnitScale, worldSpaceWidth / BlenderToOgreUnitScale, worldSpaceWidth / BlenderToOgreUnitScale);

		_spawnedBlocks.emplace(spawnedSceneNode, spawnedSkyScraper);
	}
}
