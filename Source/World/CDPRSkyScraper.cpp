#include "World/CDPRSkyScraper.h"

#include "Core/CDPRGameInstance.h"
#include "Math/CDPRMathHelper.h"

CDPRSkyScraper::CDPRSkyScraper(SceneManager* mainSceneManager, SceneNode* cityRoot, int skyScraperHeight)
{
	_mainSceneManager = mainSceneManager;
	_cityRootNode = cityRoot;
	_currentHeight = skyScraperHeight;
}

void CDPRSkyScraper::Spawn(Vector3 localPosition, float worldSpaceWidth)
{
	_blockWidth = worldSpaceWidth;
	_localPosition = localPosition;
	
	// For simple bounding box raycast uses later...
	_boxBoundPoints[0] = _cityRootNode->getPosition() + localPosition + Vector3(-worldSpaceWidth / 2, 0, -worldSpaceWidth / 2);
	_boxBoundPoints[1] = _cityRootNode->getPosition() + localPosition + Vector3(worldSpaceWidth / 2, _currentHeight * worldSpaceWidth, worldSpaceWidth / 2);

	//for (int i = 0; i < _currentHeight; i++)
	//{
	//	Entity* spawnedSkyScraper = _mainSceneManager->createEntity(SkyScraperBlockMeshResource);
	//	spawnedSkyScraper->setMaterialName(SkyScraperBlockMaterialResource);
	//	spawnedSkyScraper->setCastShadows(true);

	//	SceneNode* spawnedSceneNode = _cityRootNode->createChildSceneNode();
	//	
	//	spawnedSceneNode->setPosition(localPosition + Vector3(0.0f, (worldSpaceWidth / 2) + (worldSpaceWidth * i), -worldSpaceWidth/2));
	//	spawnedSceneNode->attachObject(spawnedSkyScraper);
	//	spawnedSceneNode->setScale(worldSpaceWidth / BlenderToOgreUnitScale, worldSpaceWidth / BlenderToOgreUnitScale, worldSpaceWidth / BlenderToOgreUnitScale);

	//	_spawnedBlocks.push_back(new CDPRSkyScraperBlock(spawnedSceneNode, spawnedSkyScraper));
	//}

	CDPRGameInstance::GetSingletonPtr()->RegisterFrameListener(this);

	_targetHeight = _currentHeight;
	_currentHeight = 0;
	ChangeHeight(_targetHeight);
}

void CDPRSkyScraper::ChangeHeight(int newHeight)
{
	if (_currentHeight == newHeight || _currentlyTransforming)
		return;

	_currentlyTransforming = true;
	_targetHeight = newHeight;
}

bool CDPRSkyScraper::frameStarted(const FrameEvent& evt)
{
	Update(evt.timeSinceLastFrame);
	return true;
}

void CDPRSkyScraper::Update(float deltaTime)
{
	if(_currentlyTransforming)
	{
		if(_currentlyAnimatingABlock)
		{
			_blockAnimationProgress += deltaTime * _blockAnimationSpeed;
			_currentBlockAnimated->_node->setPosition(CDPRMathHelper::Vector3Lerp(_animatedNodeCurrentPosition, _animatedNodeTargetPosition, _blockAnimationProgress));
			_currentBlockAnimated->_node->setScale((CDPRMathHelper::Vector3Lerp(_animatedNodeCurrentScale, _animatedNodeTargetScale, _blockAnimationProgress)));
			
			if(_blockAnimationProgress >= 1.0f)
			{
				_currentBlockAnimated->_node->setPosition(_animatedNodeTargetPosition);
				_currentBlockAnimated->_node->setScale(_animatedNodeTargetScale);
				
				if(_targetHeight > _currentHeight)
				{
					_currentHeight++;
					_spawnedBlocks.push_back(_currentBlockAnimated);
				} else if(_targetHeight < _currentHeight)
				{
					_currentHeight--;
					_spawnedBlocks.erase(std::remove(_spawnedBlocks.begin(), _spawnedBlocks.end(), _currentBlockAnimated), _spawnedBlocks.end());
					DestroyBlock(_currentBlockAnimated);
				}
				_currentlyAnimatingABlock = false;
				//UpdateCollisionBoundingBox();
			}
		} else
		{
			if (_targetHeight > _currentHeight) {
				
				_currentBlockAnimated = CreateBlock();
				_animatedNodeTargetPosition = _localPosition + Vector3(0.0f, (_blockWidth / 2) + (_blockWidth * _currentHeight), -_blockWidth / 2);
				_animatedNodeCurrentPosition = _animatedNodeTargetPosition - (Vector3::UNIT_Y * _blockWidth);

				_animatedNodeCurrentScale = Vector3::ZERO;
				_animatedNodeTargetScale = Vector3(_blockWidth / BlenderToOgreUnitScale, _blockWidth / BlenderToOgreUnitScale, _blockWidth / BlenderToOgreUnitScale);
				
				_blockAnimationProgress = 0.0f;
				_currentlyAnimatingABlock = true;
				
			} else if(_targetHeight < _currentHeight)
			{
				_currentBlockAnimated = _spawnedBlocks[_spawnedBlocks.size()-1];
				_animatedNodeCurrentPosition = _currentBlockAnimated->_node->getPosition();
				_animatedNodeTargetPosition = _currentBlockAnimated->_node->getPosition() - (Vector3::UNIT_Y * _blockWidth/2);

				_animatedNodeTargetScale = Vector3::ZERO;
				_animatedNodeCurrentScale = _spawnedBlocks[_spawnedBlocks.size() - 1]->_node->getScale();

				_blockAnimationProgress = 0.0f;
				_currentlyAnimatingABlock = true;
			}
			else if(_currentHeight == _targetHeight) {
				//Height is same, stop transforming
				_currentlyTransforming = false;
			}
		}
	}
}

void CDPRSkyScraper::DestroyBlock(CDPRSkyScraperBlock* block)
{
	block->_node->detachAllObjects();
	CDPRGameInstance::GetSingletonPtr()->_mainSceneManager->destroyEntity(block->_entity);
	CDPRGameInstance::GetSingletonPtr()->_mainSceneManager->destroySceneNode(block->_node);
	delete block;
}

void CDPRSkyScraper::UpdateCollisionBoundingBox()
{
	// For simple bounding box raycast uses later...
	_boxBoundPoints[0] = _cityRootNode->getPosition() + _localPosition + Vector3(-_blockWidth / 2, 0, -_blockWidth / 2);
	_boxBoundPoints[1] = _cityRootNode->getPosition() + _localPosition + Vector3(_blockWidth / 2, _currentHeight * _blockWidth, _blockWidth / 2);
}

CDPRSkyScraperBlock* CDPRSkyScraper::CreateBlock()
{
	
	Entity* spawnedSkyScraper = _mainSceneManager->createEntity(SkyScraperBlockMeshResource);
	spawnedSkyScraper->setMaterialName(SkyScraperBlockMaterialResource);
	spawnedSkyScraper->setCastShadows(true);

	SceneNode* spawnedSceneNode = _cityRootNode->createChildSceneNode();
	spawnedSceneNode->setScale(_blockWidth / BlenderToOgreUnitScale, _blockWidth / BlenderToOgreUnitScale, _blockWidth / BlenderToOgreUnitScale);
	spawnedSceneNode->attachObject(spawnedSkyScraper);

	return new CDPRSkyScraperBlock(spawnedSceneNode, spawnedSkyScraper);
}
