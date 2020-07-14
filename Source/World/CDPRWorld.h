#pragma once

#include "vector"
#include "Ogre.h"

class CDPRSkyScraper;

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
	
	std::vector<std::vector<int>> _cityMatrix;

	std::vector<CDPRSkyScraper*> _spawnedSkyScrapers;
};