#include "CDPRPhysics.h"

bool CDPRPhysics::RaycastBoxBounds(Vector3 bounds[2], CDPRRay& r, float &t)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[r.sign[0]].x - r.orig.x) * r.invdir.x;
	tmax = (bounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
	tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
	tymax = (bounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
	tzmax = (bounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	t = tmin;

	if (t < 0) {
		t = tmax;
		if (t < 0) return false;
	}

	return true;
}

bool CDPRPhysics::PointInBoxBounds(Vector3 bounds[2], Vector3& p)
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	
	if(bounds[0].x < bounds[1].x)
	{
		minX = bounds[0].x;
		maxX = bounds[1].x;
	} else
	{
		minX = bounds[1].x;
		maxX = bounds[0].x;
	}

	if (bounds[0].y < bounds[1].y)
	{
		minX = bounds[0].y;
		maxX = bounds[1].y;
	}
	else
	{
		minX = bounds[1].y;
		maxX = bounds[0].y;
	}

	if (bounds[0].z < bounds[1].z)
	{
		minX = bounds[0].z;
		maxX = bounds[1].z;
	}
	else
	{
		minX = bounds[1].z;
		maxX = bounds[0].z;
	}

	return (p.x <= maxX && p.x >= minX) && (p.y <= maxY && p.y >= minY) && (p.z <= maxZ && p.z >= minZ);
}
