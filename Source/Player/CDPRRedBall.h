#pragma once

#include "Ogre.h"

using namespace Ogre;

class CDPRRedBall : FrameListener
{
public:
	CDPRRedBall();
	void Initalize();
	void Shoot(Vector3& origin, Vector3& direction, float force);
	void Simulate(float deltaTime);

public:
	float _harmfulRange = 5.0f;
	float _energy = 1.0f;
	float _dragAmount = 1.0f;

private:
	bool frameStarted(const FrameEvent& evt) override;
private:
	const std::string MeshResourceName = "icosphere.mesh";
	const std::string MaterialResourceName = "redball";
	
	Entity* _projectileEntity = nullptr;
	SceneNode* _projectileNode = nullptr;
	
	Vector3 _velocity;

	bool _projectileFlying = false;
};