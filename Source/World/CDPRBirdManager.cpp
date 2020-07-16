#include "CDPRBirdManager.h"
#include "Core/EnergiezApp.h"
#include "World/CDPRWorld.h"
#include "CDPRBird.h"

using namespace Ogre;

void CDPRBirdManager::Initialize()
{
	CalculateSpherePointsForBirds();
	
	_mainSceneManager = EnergiezApp::GetSingletonPtr()->_mainSceneManager;
	_birdRootNode = _mainSceneManager->getRootSceneNode()->createChildSceneNode("BirdsRoot");

	_birdMeshResource = MeshManager::getSingleton().load(BirdMeshResource, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	_birdMeshResource->buildEdgeList();
}

void CDPRBirdManager::SpawnBird(Vector3 spawnPosiiton)
{
	Ogre::Entity* spawnedBirdEntity = _mainSceneManager->createEntity(_birdMeshResource);
	spawnedBirdEntity->setMaterialName(BirdMaterialResource);
	spawnedBirdEntity->setCastShadows(true);

	SceneNode* spawnedSceneNode = _birdRootNode->createChildSceneNode();

	spawnedSceneNode->setPosition(spawnPosiiton);
	spawnedSceneNode->attachObject(spawnedBirdEntity);
	spawnedSceneNode->setScale(Vector3::UNIT_SCALE);

	CDPRBird* spawnedBird = new CDPRBird(*this, *spawnedBirdEntity, *spawnedSceneNode);
	spawnedBird->Initialize();

	_spawnedBirds.push_back(spawnedBird);
}

void CDPRBirdManager::SpawnBirds(int amountOfBirds)
{
	CDPRWorld* world = EnergiezApp::GetSingletonPtr()->_world;
	
	for(int i=0;i<amountOfBirds;i++)
	{
		Vector3 spawnVector = GetRandomSpawnPointInWorld();

		while (world->PointInAnyCollisionBoxInWorld(spawnVector))
			spawnVector = GetRandomSpawnPointInWorld();

		SpawnBird(spawnVector);
	}
}

void CDPRBirdManager::SpawnBirdsAtCustomSpawnPoint(int amountOfBirds, Vector3 spawnPoint)
{
	for (int i = 0; i < amountOfBirds; i++)
	{
		SpawnBird(spawnPoint);
	}
}

void CDPRBirdManager::CalculateSpherePointsForBirds()
{
	float goldenRatio = (1 + Math::Sqrt(5)) / 2;
	float angleIncrement = Math::PI * 2 * goldenRatio;

	for (int i = 0; i < NumViewDirections; i++) {
		float t = (float)i / NumViewDirections;
		float inclination = Math::ACos((1 - 2 * t)).valueRadians();
		float azimuth = angleIncrement * i;

		float x = Math::Sin(inclination) * Math::Cos(azimuth);
		float y = Math::Sin(inclination) * Math::Sin(azimuth);
		float z = Math::Cos(inclination);
		_directions[i] = Vector3(x, y, z);
	}
}

Vector3 CDPRBirdManager::GetRandomSpawnPointInWorld()
{
	CDPRWorld* world = EnergiezApp::GetSingletonPtr()->_world;
	
	float worldWidth = Math::Abs(world->_worldCollisionBounds[0].x);
	float worldHeight = Math::Abs(world->_worldCollisionBounds[1].y);
	float worldLength = Math::Abs(world->_worldCollisionBounds[0].z);

	float spawnX = Math::RangeRandom(-worldWidth / 2, worldWidth / 2);
	float spawnY = Math::RangeRandom(worldHeight / 6, worldHeight);
	float spawnZ = Math::RangeRandom(-worldLength / 2, worldLength / 2);

	return Vector3(spawnX, spawnY, spawnZ);
}
