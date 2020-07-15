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
	void SpawnBirds(int amountOfBirds);
public:
	Vector3 _directions[NumViewDirections];
	std::vector<CDPRBird*> _spawnedBirds;
private:
	// for gradually increasing spherical raycasts on the birds
	void CalculateSpherePointsForBirds();
	Vector3 GetRandomSpawnPointInWorld();
private:
	const std::string BirdMeshResource = "paperplane.mesh";

};
