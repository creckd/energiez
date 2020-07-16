#include "World/CDPRWorld.h"
#include <algorithm>
#include <sstream>
#include <string>
#include "fstream"
#include "Core/CDPRGameInstance.h"

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

	float hitAtDistance;

	// WALL COLLISION CHECKS
	if (CDPRPhysics::RaycastBoxBounds(_frontWallBounds, ray, hitAtDistance))
	{
		if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
			hitInfo.hitBounds[0] = _frontWallBounds[0];
			hitInfo.hitBounds[1] = _frontWallBounds[1];
			hitInfo.hitdistance = hitAtDistance;
			hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
			hitInfo.hitObjectType = EHitObjectType::Walls;

			currentClosestHitDistance = hitAtDistance;

			foundAtLeastOneCollision = true;
		}
	}

	if (CDPRPhysics::RaycastBoxBounds(_backWallBounds, ray, hitAtDistance))
	{
		if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
			hitInfo.hitBounds[0] = _backWallBounds[0];
			hitInfo.hitBounds[1] = _backWallBounds[1];
			hitInfo.hitdistance = hitAtDistance;
			hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
			hitInfo.hitObjectType = EHitObjectType::Walls;

			currentClosestHitDistance = hitAtDistance;

			foundAtLeastOneCollision = true;
		}
	}

	if (CDPRPhysics::RaycastBoxBounds(_rightWallBounds, ray, hitAtDistance))
	{
		if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
			hitInfo.hitBounds[0] = _rightWallBounds[0];
			hitInfo.hitBounds[1] = _rightWallBounds[1];
			hitInfo.hitdistance = hitAtDistance;
			hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
			hitInfo.hitObjectType = EHitObjectType::Walls;

			currentClosestHitDistance = hitAtDistance;

			foundAtLeastOneCollision = true;
		}
	}

	if (CDPRPhysics::RaycastBoxBounds(_leftWallBounds, ray, hitAtDistance))
	{
		if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
			hitInfo.hitBounds[0] = _leftWallBounds[0];
			hitInfo.hitBounds[1] = _leftWallBounds[1];
			hitInfo.hitdistance = hitAtDistance;
			hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
			hitInfo.hitObjectType = EHitObjectType::Walls;

			currentClosestHitDistance = hitAtDistance;

			foundAtLeastOneCollision = true;
		}
	}

	if (CDPRPhysics::RaycastBoxBounds(_roofBounds, ray, hitAtDistance))
	{
		if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
			hitInfo.hitBounds[0] = _roofBounds[0];
			hitInfo.hitBounds[1] = _roofBounds[1];
			hitInfo.hitdistance = hitAtDistance;
			hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
			hitInfo.hitObjectType = EHitObjectType::Roof;

			currentClosestHitDistance = hitAtDistance;

			foundAtLeastOneCollision = true;
		}
	}

	//Check for terrain collision
	if (CDPRPhysics::RaycastBoxBounds(CDPRGameInstance::GetSingletonPtr()->_world->_terrainCollisionBounds, ray, hitAtDistance))
	{
		if (foundAtLeastOneCollision && hitAtDistance <= currentClosestHitDistance || !foundAtLeastOneCollision) {
			hitInfo.hitBounds[0] = CDPRGameInstance::GetSingletonPtr()->_world->_terrainCollisionBounds[0];
			hitInfo.hitBounds[1] = CDPRGameInstance::GetSingletonPtr()->_world->_terrainCollisionBounds[1];
			hitInfo.hitdistance = hitAtDistance;
			hitInfo.hitPoint = ray.orig + ray.dir.normalisedCopy() * hitInfo.hitdistance;
			hitInfo.hitObjectType = EHitObjectType::Terrain;

			currentClosestHitDistance = hitAtDistance;

			foundAtLeastOneCollision = true;
		}
	}
	
	//Check for every skyscraper collision
	for (CDPRSkyScraper* skyScraper : CDPRGameInstance::GetSingletonPtr()->_world->GetSpawnedSkyScrapers()) {
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
	if (CDPRPhysics::PointInBoxBounds(CDPRGameInstance::GetSingletonPtr()->_world->_terrainCollisionBounds, point))
	{
		return true;
	}

	//Check for every skyscraper collision
	for (CDPRSkyScraper* skyScraper : CDPRGameInstance::GetSingletonPtr()->_world->GetSpawnedSkyScrapers()) {
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
	SceneManager* sceneManager = CDPRGameInstance::GetSingletonPtr()->_mainSceneManager;
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
	SceneManager* sceneManager = CDPRGameInstance::GetSingletonPtr()->_mainSceneManager;

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
	SceneManager* sceneManager = CDPRGameInstance::GetSingletonPtr()->_mainSceneManager;

	Ogre::MeshPtr mMesh = MeshManager::getSingleton().load(UnitCubeMeshResource, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mMesh->buildEdgeList();

	// Back Wall
	
	Entity* wallEntity = sceneManager->createEntity(mMesh);
	wallEntity->setMaterialName(WallMaterialResource);
	wallEntity->setCastShadows(true);

	SceneNode* wallNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode->attachObject(wallEntity);

	Vector3 backWallCenter = Vector3(0,_wallHeight, (_terrainHeight / 2.0f) + (_wallWidth));
	Vector3 backWallScale = Vector3(_terrainWidth / 2.0f, _wallHeight, _wallWidth);
	
	wallNode->setPosition(backWallCenter);
	wallNode->setScale(backWallScale);
	
	_backWallBounds[0] = backWallCenter - backWallScale;
	_backWallBounds[1] = backWallCenter + backWallScale;

	//High invisible collision walls
	_backWallBounds[1].y *= _mapRoofHeightInWorldPosition;

	// Front Wall
	
	Entity* wallEntity2 = sceneManager->createEntity(mMesh);
	wallEntity2->setMaterialName(WallMaterialResource);
	wallEntity2->setCastShadows(true);

	SceneNode* wallNode2 = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode2->attachObject(wallEntity2);

	Vector3 frontWallCenter = Vector3(0, _wallHeight, -(_terrainHeight / 2.0f) - (_wallWidth));
	Vector3 frontWallScale = Vector3(_terrainWidth / 2.0f, _wallHeight, _wallWidth);
	
	wallNode2->setPosition(frontWallCenter);
	wallNode2->setScale(frontWallScale);

	_frontWallBounds[0] = frontWallCenter - frontWallScale;
	_frontWallBounds[1] = frontWallCenter + frontWallScale;
	
	//High invisible collision walls
	_frontWallBounds[1].y *= _mapRoofHeightInWorldPosition;
	
	// Left Wall

	Entity* wallEntity3 = sceneManager->createEntity(mMesh);
	wallEntity3->setMaterialName(WallMaterialResource);
	wallEntity3->setCastShadows(true);

	SceneNode* wallNode3 = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode3->attachObject(wallEntity3);

	Vector3 leftWallCenter = Vector3((_terrainWidth / 2.0f) + (_wallWidth), _wallHeight, 0);
	Vector3 leftWallScale = Vector3(_wallWidth, _wallHeight, _terrainHeight / 2.0f);
	
	wallNode3->setPosition(leftWallCenter);
	wallNode3->setScale(leftWallScale);

	_leftWallBounds[0] = leftWallCenter - leftWallScale;
	_leftWallBounds[1] = leftWallCenter + leftWallScale;

	//High invisible collision walls
	_leftWallBounds[1].y *= _mapRoofHeightInWorldPosition;

	// Right Wall

	Entity* wallEntity4 = sceneManager->createEntity(mMesh);
	wallEntity4->setMaterialName(WallMaterialResource);
	wallEntity4->setCastShadows(true);

	SceneNode* wallNode4 = sceneManager->getRootSceneNode()->createChildSceneNode();
	wallNode4->attachObject(wallEntity4);

	Vector3 rightWallCenter = Vector3(-(_terrainWidth / 2.0f) - (_wallWidth), _wallHeight, 0);
	Vector3 rightWallScale = Vector3(_wallWidth, _wallHeight, _terrainHeight / 2.0f);
	
	wallNode4->setPosition(rightWallCenter);
	wallNode4->setScale(rightWallScale);

	_rightWallBounds[0] = rightWallCenter - rightWallScale;
	_rightWallBounds[1] = rightWallCenter + rightWallScale;

	//High invisible collision walls
	_rightWallBounds[1].y *= _mapRoofHeightInWorldPosition;

	//Invisible top collision wall
	
	_roofBounds[0] = Vector3(-_terrainWidth / 2.0f, _mapRoofHeightInWorldPosition, -_terrainHeight / 2.0f);
	_roofBounds[1] = Vector3(_terrainWidth / 2.0f, _mapRoofHeightInWorldPosition + _wallWidth, _terrainHeight / 2.0f);
}
