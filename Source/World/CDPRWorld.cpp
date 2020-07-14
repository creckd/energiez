#include "World/CDPRWorld.h"
#include <algorithm>
#include <sstream>
#include <string>
#include "fstream"
#include "Core/EnergiezApp.h"

using namespace Ogre;

void CDPRWorld::CreateWorld()
{
	ReadCityDataFromFile();
	BuildCity();
}

void CDPRWorld::ReadCityDataFromFile()
{
	std::ifstream file(CityTextFilePath);
	std::string str;
	std::vector<std::string> lines;
	
	while (std::getline(file, str))
	{
		// Ignore commented lines and spaces
		if (str.rfind("//", 0) != 0) {
			str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
			lines.push_back(str);
		}
	}

	for(auto line : lines)
	{
		std::stringstream lineStream(line);
		std::string segment;
		std::vector<int> row;

		
		while (std::getline(lineStream, segment, ','))
		{
			int height = std::stoi(segment);
			row.push_back(height);
		}

		if(row.size() > 0)
			_cityMatrix.push_back(row);
	}
}

void CDPRWorld::BuildCity()
{
	SceneManager* sceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;
	SceneNode* cityRootSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode("CityRoot");
	
	for(int i=0;i<_cityMatrix.size();i++)
	{
		for(int j=0;j<_cityMatrix[i].size();j++)
		{
			Entity* spawnedSkyScraper = sceneManager->createEntity(SkyScraperBlockMeshName);
			spawnedSkyScraper->setMaterialName("danidiffuse");
			spawnedSkyScraper->setCastShadows(true);

			SceneNode* spawnedSceneNode = cityRootSceneNode->createChildSceneNode();
			spawnedSceneNode->setPosition(Vector3(i * _buildingWidth * _buildingPaddingWorldPosition, 0, j * _buildingWidth * _buildingPaddingWorldPosition));
			spawnedSceneNode->attachObject(spawnedSkyScraper);
			spawnedSceneNode->setScale(1.0, 1.0 * _cityMatrix[i][j], 1.0);

			_spawnedSkyScrapers.emplace(spawnedSkyScraper, spawnedSceneNode);
		}
	}
}
