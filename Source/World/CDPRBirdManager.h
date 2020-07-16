#pragma once
#include <Ogre.h>
#include <vector>

using namespace Ogre;

class CDPRBird;

#define NumViewDirections 300

class CDPRBirdManager
{
public:
	void Initialize();
	void SpawnBird(Vector3 spawnPosiiton);
	void SpawnBirds(int amountOfBirds);
	void SpawnBirdsAtCustomSpawnPoint(int amountOfBirds, Vector3 spawnPoint);
public:
	Vector3 _directions[NumViewDirections];
	std::vector<CDPRBird*> _spawnedBirds;
private:
	// for gradually increasing spherical raycasts on the birds
	void CalculateSpherePointsForBirds();
	Vector3 GetRandomSpawnPointInWorld();
private:
	const std::string BirdMeshResource = "paperplane.mesh";
	const std::string BirdMaterialResource = "paperplanes";

	SceneManager* _mainSceneManager = nullptr;
	SceneNode* _birdRootNode = nullptr;
	MeshPtr _birdMeshResource;

};
