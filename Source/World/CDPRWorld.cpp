#include "World/CDPRWorld.h"
#include "World/CDPRSkyScraper.h"
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
			int height = _cityMatrix[i][j];

			if (height == 0)
				continue;

			CDPRSkyScraper* skyScraper = new CDPRSkyScraper(sceneManager, cityRootSceneNode, height);
			skyScraper->Spawn(Vector3(i * _buildingWidth * _buildingPaddingWorldPosition, 0, j * _buildingWidth * _buildingPaddingWorldPosition));
			
			_spawnedSkyScrapers.push_back(skyScraper);
		}
	}
}
