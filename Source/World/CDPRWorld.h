#pragma once

#include "vector"
#include "Ogre.h"
#include "CDPRSkyScraper.h"

class CDPRWorld
{
public:
	void CreateWorld();

	std::vector<CDPRSkyScraper*> GetSpawnedSkyScrapers() { return _spawnedSkyScrapers; }
public:
	float _buildingPaddingWorldPosition = 6.0f;
	float _buildingWidth = 4.0f;
private:
	void ReadCityDataFromFile();
	void BuildCity();
	void BuildTerrain();
private:
	const std::string CityTextFilePath = "city.txt";
	
	std::vector<std::vector<int>> _cityMatrix;
	std::vector<CDPRSkyScraper*> _spawnedSkyScrapers;
};
