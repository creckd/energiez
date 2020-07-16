#include "CDPRPRojectile.h"
#include "Core/CDPRGameInstance.h"
#include "Math/CDPRMathHelper.h"
#include "Physics/CDPRPhysics.h"
#include "World/CDPRWorld.h"

CDPRProjectile::CDPRProjectile(CDPRPlayerController* playerController, std::string meshResourceName, std::string materialResourceName)
	: _playerController(playerController), _meshResourceName(meshResourceName), _materialResourceName(materialResourceName)
{
}

void CDPRProjectile::Initalize()
{
	SceneManager* sceneManager = CDPRGameInstance::GetSingletonPtr()->_mainSceneManager;

	_projectileEntity = sceneManager->createEntity(_meshResourceName);
	_projectileEntity->setMaterialName(_materialResourceName);
	_projectileEntity->setCastShadows(true);
	_projectileEntity->setVisible(false);

	_projectileNode = sceneManager->getRootSceneNode()->createChildSceneNode();

	CDPRWorld* world = CDPRGameInstance::GetSingletonPtr()->_world;
	;
	_projectileNode->attachObject(_projectileEntity);
	//_projectileNode->setScale(Vector3::UNIT_SCALE);
	CDPRGameInstance::GetSingletonPtr()->RegisterFrameListener(this);
}

void CDPRProjectile::Update(float deltaTime)
{
	if (_projectileFlying) {
		CollisionCheck();
		Simulate(deltaTime);
	}
}

void CDPRProjectile::Simulate(float deltaTime)
{
	Vector3 zeroVector = Vector3::ZERO;
	if (_velocity.squaredLength() > 0.1f)
		_velocity = CDPRMathHelper::Vector3Lerp(_velocity, zeroVector, deltaTime * _dragAmount);
	else _velocity = zeroVector;

	_projectileNode->translate(_velocity);
}

void CDPRProjectile::Destroy()
{
	_projectileNode->detachAllObjects();
	CDPRGameInstance::GetSingletonPtr()->UnRegisterFrameListener(this);
	CDPRGameInstance::GetSingletonPtr()->_mainSceneManager->destroyEntity(_projectileEntity);
	CDPRGameInstance::GetSingletonPtr()->_mainSceneManager->destroySceneNode(_projectileNode);
	delete this;
}

void CDPRProjectile::Shoot(Vector3& origin, Vector3& direction, float force)
{
	_projectileNode->setPosition(origin);
	_projectileEntity->setVisible(true);
	_projectileFlying = true;
	_velocity = direction * force;
}

Vector3 CDPRProjectile::GetPosition()
{
	return _projectileNode->getPosition();
}

bool CDPRProjectile::frameStarted(const FrameEvent& evt)
{
	Update(evt.timeSinceLastFrame);
	return true;
}

void CDPRProjectile::CollisionCheck()
{
	CDPRRayHitInfo hitInfo;
	CDPRRay ray(GetPosition(), _velocity.normalisedCopy());

	if (CDPRGameInstance::GetSingletonPtr()->_world->RayCollidingWithAnythingInWorld(ray, hitInfo))
	{
		if (hitInfo.hitdistance <= _colliderSize) {
			if (hitInfo.hitObjectType == EHitObjectType::SkyScraper)
			{
				Vector3 boundingBoxCenter = (hitInfo.hitBounds[0] + hitInfo.hitBounds[1]) / 2;
				boundingBoxCenter.y = GetPosition().y;

				Vector3 closestNormal = Vector3::UNIT_Z;
				float smallestDot = ((GetPosition() - boundingBoxCenter).normalisedCopy()).dotProduct(closestNormal);

				Vector3 testAxises[6] = { Vector3::UNIT_Z, Vector3::NEGATIVE_UNIT_Z,
					Vector3::UNIT_X, Vector3::NEGATIVE_UNIT_X,
					Vector3::UNIT_Y, Vector3::NEGATIVE_UNIT_Y
				};

				for(int i=0;i<6;i++)
				{
					float currentDot = ((GetPosition() - boundingBoxCenter).normalisedCopy()).dotProduct(testAxises[i]);
					if(currentDot <= smallestDot)
					{
						smallestDot = currentDot;
						closestNormal = testAxises[i];
					}
				}
				
				_velocity = -(-_velocity.reflect(closestNormal));
			} else if(hitInfo.hitObjectType == EHitObjectType::Terrain)
			{
				_velocity = -(-_velocity.reflect(Vector3::UNIT_Y));
			} else if(hitInfo.hitObjectType == EHitObjectType::Walls)
			{
				Vector3 boundingBoxCenter = (hitInfo.hitBounds[0] + hitInfo.hitBounds[1]) / 2;

				Vector3 closestNormal = Vector3::UNIT_Z;
				float smallestDot = ((boundingBoxCenter - Vector3(0,boundingBoxCenter.y,0)).normalisedCopy()).dotProduct(closestNormal);

				Vector3 testAxises[6] = { Vector3::UNIT_Z, Vector3::NEGATIVE_UNIT_Z,
					Vector3::UNIT_X, Vector3::NEGATIVE_UNIT_X,
					Vector3::UNIT_Y, Vector3::NEGATIVE_UNIT_Y
				};

				for (int i = 0; i < 6; i++)
				{
					float currentDot = ((boundingBoxCenter - Vector3(0, boundingBoxCenter.y, 0)).normalisedCopy()).dotProduct(testAxises[i]);
					if (currentDot <= smallestDot)
					{
						smallestDot = currentDot;
						closestNormal = testAxises[i];
					}
				}
				
				_velocity = -(-_velocity.reflect(closestNormal));
			} else if(hitInfo.hitObjectType == EHitObjectType::Roof)
			{
				_velocity = -(-_velocity.reflect(Vector3::NEGATIVE_UNIT_Y));
			}
		}
	}
}
