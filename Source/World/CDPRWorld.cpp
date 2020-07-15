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
	BuildTerrain();
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
			skyScraper->Spawn(Vector3((i * _buildingWidth) + (i * _buildingPaddingWorldPosition), 0, (j * _buildingWidth) + (j * _buildingPaddingWorldPosition)), _buildingWidth);
			
			_spawnedSkyScrapers.push_back(skyScraper);
		}
	}
}

void CDPRWorld::BuildTerrain()
{
	SceneManager* sceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;
	
	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"ground", RGN_DEFAULT,
		plane,
		150, 150, 20, 20,
		true,
		1, 5, 5,
		Vector3::UNIT_Z);

	Entity* groundEntity = sceneManager->createEntity("ground");
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Examples/Rockwall");
	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);;
}
