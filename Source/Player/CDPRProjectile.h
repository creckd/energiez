#pragma once

#include "Ogre.h"

class CDPRPlayerController;
using namespace Ogre;

class CDPRProjectile : public FrameListener
{
public:
	CDPRProjectile(CDPRPlayerController* playerController, std::string meshResourceName, std::string materialResourceName);
	
	virtual void Initalize();
	virtual void Update(float deltaTime);
	virtual void Simulate(float deltaTime);

	void Shoot(Vector3& origin, Vector3& direction, float force);
	
	Vector3 GetPosition();
public:
	float _dragAmount = 1.0f;
	float _colliderSize = 2.5f;
protected:
	bool frameStarted(const FrameEvent& evt) override;
protected:
	std::string _meshResourceName = "icosphere.mesh";
	std::string _materialResourceName = "redball";

	CDPRPlayerController* _playerController;

	Entity* _projectileEntity = nullptr;
	SceneNode* _projectileNode = nullptr;

	Vector3 _velocity = Vector3::ZERO;
	Vector3 _defaultScale = Vector3::UNIT_SCALE;
	bool _projectileFlying = false;
private:
	void CollisionCheck();
};