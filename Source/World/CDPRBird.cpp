#include "CDPRBird.h"
#include "CDPRBirdManager.h"
#include "Core/EnergiezApp.h"
#include "Physics/CDPRPhysics.h"
#include "World/CDPRWorld.h"
#include "Math/CDPRMathHelper.h"

CDPRBird::CDPRBird(CDPRBirdManager& managerReference, Entity& birdEntity, SceneNode& birdSceneNode) : _birdManager(managerReference), _birdEntity(birdEntity), _birdSceneNode(birdSceneNode)
{
}

void CDPRBird::Initialize()
{
	EnergiezApp::GetSingletonPtr()->RegisterFrameListener(this);

	_defaultSpawnPoint = _birdSceneNode.getPosition();
	
	_velocity = Vector3(Math::RangeRandom(-1,1), Math::RangeRandom(-1, 1), Math::RangeRandom(-1, 1)).normalisedCopy();
	_birdSceneNode.lookAt(_birdSceneNode.getPosition() + _velocity.normalisedCopy(), Node::TS_WORLD, Vector3::UNIT_Z);

	_position = _birdSceneNode.getPosition();
	_direction = _birdSceneNode.getOrientation() * Vector3::UNIT_Z;
}

bool CDPRBird::frameStarted(const FrameEvent& evt)
{
	WatchFlockMates();
	Move(evt);

	return true;
}

void CDPRBird::Move(const FrameEvent& evt)
{
	bool stuck = _birdSceneNode.getPosition().squaredLength() > 5000.0f;
	if (stuck)
		Destuck();
	
	if (_numberOfPerceivedFlockMates > 0) {
		_avgFlockCenter /= (float)_numberOfPerceivedFlockMates;
		
		_velocity += (_avgFlockCenter - _position).normalisedCopy();
		_velocity += _avgFlockHeading.normalisedCopy();
		_velocity += _seperationHeading.normalisedCopy();
	}

	bool headingForCollision = false;
	
	Vector3 collisionFreeWay = FindBestCollisionFreeDirection(headingForCollision);
	
	if(headingForCollision)
		_velocity += collisionFreeWay.normalisedCopy() * 500.0f;

	//Clamp to max speed
	if(_velocity.squaredLength() > _maxSpeed * _maxSpeed)
	{
		_velocity.normalise();
		_velocity *= _maxSpeed;
	}

	//Clamp to min speed
	else if (_velocity.squaredLength() < _minSpeed * _minSpeed)
	{
		_velocity.normalise();
		_velocity *= _minSpeed;
	}
	
	//_velocity = CDPRMathHelper::Vector3Lerp(_velocity, FindBestCollisionFreeDirection(), evt.timeSinceLastFrame * 100.0f);
	_birdSceneNode.translate(_velocity * evt.timeSinceLastFrame, Node::TS_WORLD);
	
//	Quaternion curentRotation = _birdSceneNode.getOrientation();
	_birdSceneNode.lookAt(_birdSceneNode.getPosition() + _velocity.normalisedCopy(), Node::TS_WORLD, Vector3::UNIT_Z);
//	Quaternion targetRotation = _birdSceneNode.getOrientation();
//	_birdSceneNode.setOrientation(Quaternion::nlerp(evt.timeSinceLastFrame * 100.0f, curentRotation, targetRotation));


	_position = _birdSceneNode.getPosition();
	_direction = _birdSceneNode.getOrientation() * Vector3::UNIT_Z;
}

void CDPRBird::Destuck()
{
	_birdSceneNode.setPosition(_defaultSpawnPoint);
}

Vector3 CDPRBird::FindBestCollisionFreeDirection(bool& headingForCollisiion)
{
	headingForCollisiion = false;
	
	int count = sizeof(sizeof(_birdManager._directions) / sizeof(_birdManager._directions[0]));
	for (int i = 0; i < count; i++)
	{

		Vector3 dir = _birdSceneNode.getOrientation() * _birdManager._directions[i];

		bool collision = false;

		CDPRRay ray = CDPRRay(_birdSceneNode.getPosition(), dir);
		float distance;

		//Check for world bounds collision
		if (CDPRPhysics::RaycastBoxBounds(EnergiezApp::GetSingletonPtr()->_world->_worldCollisionBounds, ray, distance))
		{
			if (distance < _terrainCollisionDetectionRange) {
				collision = true;
			}
		}

		//Check for terrain collision
		if (CDPRPhysics::RaycastBoxBounds(EnergiezApp::GetSingletonPtr()->_world->_terrainCollisionBounds, ray, distance))
		{
			if (distance < _terrainCollisionDetectionRange) {
				collision = true;
			}
		}

		//Check for every skyscraper collision
		if (!collision) {
			for (CDPRSkyScraper* skyScraper : EnergiezApp::GetSingletonPtr()->_world->GetSpawnedSkyScrapers()) {
				//Quick workaround for Bird bounding box
				Vector3 extendedBounds[2];
				extendedBounds[0] = skyScraper->_boxBoundPoints[0];
				extendedBounds[1] = skyScraper->_boxBoundPoints[1];
				extendedBounds[0] += (extendedBounds[0] - extendedBounds[1]).normalisedCopy() * 2.0f;
				extendedBounds[1] += (extendedBounds[1] - extendedBounds[0]).normalisedCopy() * 2.0f;
				if (CDPRPhysics::RaycastBoxBounds(extendedBounds, ray, distance))
				{
					if (distance < _terrainCollisionDetectionRange) {
						collision = true;
						break;
					}
				}
			}
		}

		if (!collision)
			return dir;
		else if (i == 0)
			headingForCollisiion = true;
	}

	return _velocity;
}

void CDPRBird::WatchFlockMates()
{
	_numberOfPerceivedFlockMates = 0;
	_avgFlockHeading = Vector3::ZERO;
	_avgFlockCenter = Vector3::ZERO;
	_seperationHeading = Vector3::ZERO;
	
	for(int i=0;i<_birdManager._spawnedBirds.size();i++)
	{
		CDPRBird* mate = _birdManager._spawnedBirds[i];
		
		if (mate == this)
			continue;
		
		float sqrDst = _position.squaredDistance(mate->_position);

		if(sqrDst < _visionDistance * _visionDistance)
		{
			_numberOfPerceivedFlockMates += 1;
			_avgFlockHeading += mate->_direction;
			_avgFlockCenter += mate->_position;

			if(sqrDst < _avoidRadius * _avoidRadius)
			{
				if(sqrDst == 0.0f)
					_seperationHeading = _velocity = Vector3(Math::RangeRandom(0, 1), Math::RangeRandom(0, 1), Math::RangeRandom(0, 1)).normalisedCopy();
				else
					_seperationHeading -= (mate->_position - _position) / Math::Clamp(sqrDst,0.1f,100.0f);
			}
		}
	}
}
