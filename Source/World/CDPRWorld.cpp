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
	BuildWalls();
}

bool CDPRWorld::RayCollidingWithAnythingInWorld(CDPRRay& ray, CDPRRayHitInfo& hitInfo)
{
	bool foundAtLeastOneCollision = false;
	float currentClosestHitDistance = 0.0f;
	//Check for world bounds collision
	//if (CDPRPhysics::RaycastBoxBounds(EnergiezApp::GetSingletonPtr()->_world->_worldCollisionBounds, ray, hitAtDistance))
	//{
	//	return true;
	//}
	float hitAtDistance;

	//Check for terrain collision
	if (CDPRPhysics::RaycastBoxBounds(EnergiezApp::GetSingletonPtr()->_world->_terrainCollisionBounds, ray, hitAtDistance))
	{
		if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
			hitInfo.hitBounds[0] = EnergiezApp::GetSingletonPtr()->_world->_terrainCollisionBounds[0];
			hitInfo.hitBounds[1] = EnergiezApp::GetSingletonPtr()->_world->_terrainCollisionBounds[1];
			hitInfo.hitdistance = hitAtDistance;
			hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
			hitInfo.hitObjectType = EHitObjectType::Terrain;

			currentClosestHitDistance = hitAtDistance;

			foundAtLeastOneCollision = true;
		}
	}
	
	//Check for every skyscraper collision
	for (CDPRSkyScraper* skyScraper : EnergiezApp::GetSingletonPtr()->_world->GetSpawnedSkyScrapers()) {
		if (CDPRPhysics::RaycastBoxBounds(skyScraper->_boxBoundPoints, ray, hitAtDistance))
		{
			if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
				hitInfo.hitBounds[0] = skyScraper->_boxBoundPoints[0];
					hitInfo.hitBounds[1] = skyScraper->_boxBoundPoints[1];
					hitInfo.hitdistance = hitAtDistance;
					hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
					hitInfo.hitObjectType = EHitObjectType::SkyScraper;

					currentClosestHitDistance = hitAtDistance;

					foundAtLeastOneCollision = true;
			}
		}
	}

	return foundAtLeastOneCollision;
}

bool CDPRWorld::PointInAnyCollisionBoxInWorld(Vector3 point)
{

	//Check for terrain collision
	if (CDPRPhysics::PointInBoxBounds(EnergiezApp::GetSingletonPtr()->_world->_terrainCollisionBounds, point))
	{
		return true;
	}

	//Check for every skyscraper collision
	for (CDPRSkyScraper* skyScraper : EnergiezApp::GetSingletonPtr()->_world->GetSpawnedSkyScrapers()) {
		if (CDPRPhysics::PointInBoxBounds(skyScraper->_boxBoundPoints, point))
		{
			return true;
		}
	}

	return false;
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

	for (auto line : lines)
	{
		std::stringstream lineStream(line);
		std::string segment;
		std::vector<int> row;


		while (std::getline(lineStream, segment, ','))
		{
			int height = std::stoi(segment);
			row.push_back(height);
		}

		if (row.size() > 0)
			_cityMatrix.push_back(row);
	}
}

void CDPRWorld::BuildCity()
{
	SceneManager* sceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;
	SceneNode* cityRootSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode("CityRoot");

	int columns = _cityMatrix.size();
	int rows = _cityMatrix[0].size();

	//Centralize City
	cityRootSceneNode->translate(Vector3(-(_buildingPaddingWorldPosition + _buildingWidth) * rows / 2, 0, -(_buildingPaddingWorldPosition + _buildingWidth) * columns / 2));

	for (int i = 0; i < _cityMatrix.size(); i++)
	{
		for (int j = 0; j < _cityMatrix[i].size(); j++)
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
		_terrainWidth, _terrainHeight, _terrainSubdivision, _terrainSubdivision,
		true,
		1, 5, 5,
		Vector3::UNIT_Z);

	_terrainCollisionBounds[0] = Vector3(-_terrainWidth / 2, -10.0f, -_terrainHeight / 2);
	_terrainCollisionBounds[1] = Vector3(_terrainWidth / 2, 0, _terrainHeight);

	_worldCollisionBounds[0] = Vector3(-_terrainWidth / 2, 0.0f, -_terrainHeight / 2);
	_worldCollisionBounds[1] = Vector3(_terrainWidth / 2, 50.0f, _terrainHeight / 2);

	Entity* groundEntity = sceneManager->createEntity("ground");
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("ground");
	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
}

void CDPRWorld::BuildWalls()
{
	SceneManager* sceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;

	Ogre::MeshPtr mMesh = MeshManager::getSingleton().load(UnitCubeMeshResource, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mMesh->buildEdgeList();

	// Back Wall
	
	Entity* wallEntity = sceneManager->createEntity(mMesh);
	wallEntity->setMaterialName(WallMaterialResource);
	wallEntity->setCastShadows(true);

	SceneNode* wallNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode->attachObject(wallEntity);
	wallNode->setScale(_terrainWidth / 2.0f, _wallHeight, _wallWidth);
	wallNode->setPosition(0, _wallHeight, (_terrainHeight / 2.0f) + (_wallWidth / 2.0f));

	// Front Wall
	
	Entity* wallEntity2 = sceneManager->createEntity(mMesh);
	wallEntity2->setMaterialName(WallMaterialResource);
	wallEntity2->setCastShadows(true);

	SceneNode* wallNode2 = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode2->attachObject(wallEntity2);
	wallNode2->setScale(_terrainWidth / 2.0f, _wallHeight, _wallWidth);
	wallNode2->setPosition(0, _wallHeight, -(_terrainHeight / 2.0f) - (_wallWidth / 2.0f));

	// Left Wall

	Entity* wallEntity3 = sceneManager->createEntity(mMesh);
	wallEntity3->setMaterialName(WallMaterialResource);
	wallEntity3->setCastShadows(true);

	SceneNode* wallNode3 = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode3->attachObject(wallEntity3);
	wallNode3->setScale(_wallWidth, _wallHeight, _terrainHeight / 2.0f);
	wallNode3->setPosition((_terrainWidth / 2.0f) + (_wallWidth / 2.0f), _wallHeight, 0);

	// Right Wall

	Entity* wallEntity4 = sceneManager->createEntity(mMesh);
	wallEntity4->setMaterialName(WallMaterialResource);
	wallEntity4->setCastShadows(true);

	SceneNode* wallNode4 = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode4->attachObject(wallEntity4);
	wallNode4->setScale(_wallWidth, _wallHeight, _terrainHeight / 2.0f);
	wallNode4->setPosition(-(_terrainWidth / 2.0f) - (_wallWidth / 2.0f), _wallHeight, 0);
}
