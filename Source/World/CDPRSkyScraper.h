#pragma once
#include <string>
#include "Ogre.h"

using namespace Ogre;

#define BlenderToOgreUnitScale 2.0f

class CDPRSkyScraper
{
public:
	CDPRSkyScraper(SceneManager* mainSceneManager, SceneNode* cityRoot, int skyScraperHeight);
	void Spawn(Vector3 rootWorldPosition);
private:
	const std::string SkyScraperBlockMeshResource = "Cube.mesh";
	const std::string SkyScraperBlockMaterialResource = "danidiffuse";

	int _height = -1;
	SceneManager* _mainSceneManager = nullptr;
	SceneNode* _cityRootNode = nullptr;
	
	std::map<SceneNode*, Entity*> _spawnedBlocks;
	
};