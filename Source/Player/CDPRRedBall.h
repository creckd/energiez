#pragma once

#include "Ogre.h"

class CDPRBird;
class CDPRPlayerController;

using namespace Ogre;

class CDPRRedBall : FrameListener
{
public:
	CDPRRedBall(CDPRPlayerController* playerControllerReference);
	
	void Initalize();
	void Shoot(Vector3& origin, Vector3& direction, float force);
	void Simulate(float deltaTime);
	void Explode();
	void Consume(CDPRBird* birdToConsume);

	Vector3 GetPosition();
public:
	float _startingEnergy = 1.0f;
	float _harmfulRange = 5.0f;
	float _aggroRange = 20.0f;
	float _dragAmount = 1.0f;

	int _maxNumberOfBirdsConsumableBeforeExplode = 5;

private:
	bool frameStarted(const FrameEvent& evt) override;
private:
	const std::string MeshResourceName = "icosphere.mesh";
	const std::string MaterialResourceName = "redball";
	
	CDPRPlayerController* _playerController;
	
	Entity* _projectileEntity = nullptr;
	SceneNode* _projectileNode = nullptr;
	
	Vector3 _velocity;
	Vector3 _defaultScale;
	bool _projectileFlying = false;

	float _currentEnergy = 1.0f;
	int _numberOfBirdConsumed = 0;
};