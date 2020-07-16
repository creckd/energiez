#include "CDPRBird.h"
#include "CDPRBirdManager.h"
#include "Core/EnergiezApp.h"
#include "Physics/CDPRPhysics.h"
#include "World/CDPRWorld.h"
#include "Math/CDPRMathHelper.h"
#include "Player/CDPRPlayerController.h"
#include "Player/CDPRRedBall.h"

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

void CDPRBird::Kill()
{
	_birdSceneNode.detachAllObjects();
	EnergiezApp::GetSingletonPtr()->_mainSceneManager->destroyEntity(&_birdEntity);
	EnergiezApp::GetSingletonPtr()->_mainSceneManager->destroySceneNode(&_birdSceneNode);
	EnergiezApp::GetSingletonPtr()->UnRegisterFrameListener(this);
	_birdManager._spawnedBirds.erase(std::remove(_birdManager._spawnedBirds.begin(), _birdManager._spawnedBirds.end(), this), _birdManager._spawnedBirds.end());
	delete this;
}

bool CDPRBird::frameStarted(const FrameEvent& evt)
{
	WatchFlockMates();
	WatchForRedBalls();
	Move(evt);

	return true;
}

void CDPRBird::Move(const FrameEvent& evt)
{
	bool stuck = _birdSceneNode.getPosition().squaredLength() > 7000.0f;
	if (stuck)
		Destuck();
	
	if (_numberOfPerceivedFlockMates > 0) {
		_avgFlockCenter /= (float)_numberOfPerceivedFlockMates;

		CDPRWorld* _world = EnergiezApp::GetSingletonPtr()->_world;

		CDPRRayHitInfo hitInfo;
		
		Vector3 flockCenterForce = (_avgFlockCenter - _position).normalisedCopy();
		CDPRRay ray(_position, flockCenterForce);
		flockCenterForce *= !_world->RayCollidingWithAnythingInWorld(ray, hitInfo);

		Vector3 flockHeadingForce = _avgFlockHeading.normalisedCopy();
		CDPRRay ray2(_position, flockHeadingForce);
		flockHeadingForce *= !_world->RayCollidingWithAnythingInWorld(ray, hitInfo);

		Vector3 seperationHeadingForce = _seperationHeading.normalisedCopy();
		CDPRRay ray3(_position, seperationHeadingForce);
		seperationHeadingForce *= !_world->RayCollidingWithAnythingInWorld(ray, hitInfo);

		Vector3 redBallAvoidanceForce = _redBallAvoidance.normalisedCopy();
		CDPRRay ray4(_position, redBallAvoidanceForce);
		redBallAvoidanceForce *= !_world->RayCollidingWithAnythingInWorld(ray, hitInfo);
		
		_velocity += flockCenterForce;
		_velocity += flockHeadingForce;
		_velocity += seperationHeadingForce;
		_velocity += redBallAvoidanceForce * 10.0f;
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
	
	int count = 300;
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
		//if (CDPRPhysics::RaycastBoxBounds(EnergiezApp::GetSingletonPtr()->_world->_terrainCollisionBounds, ray, distance))
		//{
		//	if (distance < _terrainCollisionDetectionRange) {
		//		collision = true;
		//	}
		//}

		//Check for every skyscraper collision
		if (!collision) {
			for (CDPRSkyScraper* skyScraper : EnergiezApp::GetSingletonPtr()->_world->GetSpawnedSkyScrapers()) {
				//Quick workaround for Bird bounding box
				//Vector3 extendedBounds[2];
				//extendedBounds[0] = skyScraper->_boxBoundPoints[0];
				//extendedBounds[1] = skyScraper->_boxBoundPoints[1];
				//extendedBounds[0].x += (extendedBounds[0] - extendedBounds[1]).normalisedCopy().x * 4.0f;
				//extendedBounds[0].z += (extendedBounds[0] - extendedBounds[1]).normalisedCopy().z * 4.0f;
				//extendedBounds[1].x += (extendedBounds[1] - extendedBounds[0]).normalisedCopy().x * 4.0f;
				//extendedBounds[1].z += (extendedBounds[1] - extendedBounds[0]).normalisedCopy().z * 4.0f;
				if (CDPRPhysics::RaycastBoxBounds(skyScraper->_boxBoundPoints, ray, distance))
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

void CDPRBird::WatchForRedBalls()
{
	_redBallAvoidance = Vector3::ZERO;
	for(auto ball : EnergiezApp::GetSingletonPtr()->_playerManager->_spawnedBalls)
	{
		float sqrDst = _position.squaredDistance(ball->GetPosition());

		if(sqrDst < _redBallDetectionDistance * _redBallDetectionDistance)
		{
			_redBallAvoidance += (_position - ball->GetPosition());
		}
	}
}
