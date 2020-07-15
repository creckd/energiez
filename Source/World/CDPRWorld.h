#pragma once

#include "vector"
#include "Ogre.h"
#include "CDPRSkyScraper.h"
#include "Physics/CDPRPhysics.h"

class CDPRWorld
{
public:
	void CreateWorld();

	bool RayCollidingWithAnythingInWorld(CDPRRay& ray, float& hitAtDistance);
	bool PointInAnyCollisionBoxInWorld(Vector3 point);

	std::vector<CDPRSkyScraper*> GetSpawnedSkyScrapers() { return _spawnedSkyScrapers; }
public:
	float _buildingPaddingWorldPosition = 6.0f;
	float _buildingWidth = 4.0f;

	float _terrainWidth = 100.0f;
	float _terrainHeight = 100.0f;
	int _terrainSubdivision = 20;

	Vector3 _terrainCollisionBounds[2];
	Vector3 _worldCollisionBounds[2];
private:
	void ReadCityDataFromFile();
	void BuildCity();
	void BuildTerrain();
private:
	const std::string CityTextFilePath = "city.txt";
	
	std::vector<std::vector<int>> _cityMatrix;
	std::vector<CDPRSkyScraper*> _spawnedSkyScrapers;
};
