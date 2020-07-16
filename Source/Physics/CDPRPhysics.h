#pragma once

#include "Ogre.h"

using namespace Ogre;

class CDPRRay
{
public:
	CDPRRay(const Vector3 &orig, const Vector3 &dir) : orig(orig), dir(dir)
	{
		invdir = 1 / dir;
		sign[0] = (invdir.x < 0);
		sign[1] = (invdir.y < 0);
		sign[2] = (invdir.z < 0);
	}

public:
	Vector3 orig, dir;
	Vector3 invdir;
	int sign[3];
};

enum EHitObjectType
{
	None,
	SkyScraper,
	WorldBounds,
	Terrain,
	Walls
};

struct CDPRRayHitInfo
{
public:
	Vector3 hitBounds[2];
	Vector3 hitPoint;
	float hitdistance;
	EHitObjectType hitObjectType;
};

class CDPRPhysics
{
public:
	static bool RaycastBoxBounds(Vector3 bounds[2], CDPRRay& r, float &t);
	static bool PointInBoxBounds(Vector3 bounds[2], Vector3& p);
};
