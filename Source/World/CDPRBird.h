#pragma once

#include "Ogre.h"
#include "OgreFrameListener.h"

class CDPRBirdManager;

using namespace Ogre;

class CDPRBird : public FrameListener
{
public:
	CDPRBird(CDPRBirdManager& managerReference, Entity& birdEntity, SceneNode& birdSceneNode);
	
	void Initialize();
public:
	float _terrainCollisionDetectionRange = 10.0f;
	float _visionDistance = 10.0f;
	float _avoidRadius = 5.0f;

	//for Flock calculations
	Vector3 _position = Vector3::ZERO;
	Vector3 _direction = Vector3::ZERO;

private:
	bool frameStarted(const FrameEvent& evt) override;
	void Move(const FrameEvent& evt);
	void Destuck();
	Vector3 FindBestCollisionFreeDirection(bool& headingForCollision);
	void WatchFlockMates();
	
private:
	CDPRBirdManager& _birdManager;
	Entity& _birdEntity;
	SceneNode& _birdSceneNode;

	float _minSpeed = 10.0;
	float _maxSpeed = 15.0f;
	Vector3 _velocity = Vector3::UNIT_Z;
	
	int _numberOfPerceivedFlockMates = 10;
	Vector3 _avgFlockDirection = Vector3::ZERO;
	Vector3 _avgFlockHeading = Vector3::ZERO;
	Vector3 _avgFlockCenter = Vector3::ZERO;
	Vector3 _seperationHeading = Vector3::ZERO;

	Vector3 _defaultSpawnPoint = Vector3::ZERO;
};