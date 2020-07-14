#pragma once

#include "vector"
#include "Ogre.h"

class CDPRWorld
{
public:
	void CreateWorld();
public:
	float _buildingPaddingWorldPosition = 3.0f;
	float _buildingWidth = 4.0f;
private:
	void ReadCityDataFromFile();
	void BuildCity();
private:
	const std::string CityTextFilePath = "city.txt";

	//Resources
	const std::string SkyScraperBlockMeshName = "Cube.mesh";
	
	std::vector<std::vector<int>> _cityMatrix;

	std::map<Ogre::Entity*, Ogre::SceneNode*> _spawnedSkyScrapers;
	//std::vector<Ogre::Entity*> _skyScraperEntities;
};