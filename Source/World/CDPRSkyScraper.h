#pragma once
#include <string>
#include "Ogre.h"

using namespace Ogre;

#define BlenderToOgreUnitScale 2.0f

class CDPRSkyScraperBlock
{
public:
	CDPRSkyScraperBlock();
	CDPRSkyScraperBlock(SceneNode* node, Entity* entity)
		: _node(node),
		  _entity(entity)
	{
	}

	SceneNode* _node;
	Entity* _entity;
};

class CDPRSkyScraper : public FrameListener
{
public:
	CDPRSkyScraper(SceneManager* mainSceneManager, SceneNode* cityRoot, int skyScraperHeight);
	void Spawn(Vector3 localPosition, float worldSpaceWidth);
	void ChangeHeight(int newHeight);

	bool frameStarted(const FrameEvent& evt) override;
public:
	//For raycast calculations later...
	Vector3 _boxBoundPoints[2];

private:
	void Update(float deltaTime);
	void DestroyBlock(CDPRSkyScraperBlock* block);
	void UpdateCollisionBoundingBox();
	CDPRSkyScraperBlock* CreateBlock();
private:
	const std::string SkyScraperBlockMeshResource = "bevelcube.mesh";
	const std::string SkyScraperBlockMaterialResource = "danidiffuse";

	float _blockWidth;
	
	int _currentHeight = -1;
	int _targetHeight = -1;

	bool _currentlyTransforming = false;
	bool _currentlyAnimatingABlock = false;
	CDPRSkyScraperBlock* _currentBlockAnimated;
	
	Vector3 _animatedNodeCurrentPosition = Vector3::ZERO;
	Vector3 _animatedNodeTargetPosition = Vector3::ZERO;

	Vector3 _animatedNodeCurrentScale = Vector3::ZERO;
	Vector3 _animatedNodeTargetScale = Vector3::ZERO;

	Vector3 _localPosition = Vector3::ZERO;

	float _blockAnimationSpeed = 4.0f;
	float _blockAnimationProgress = 0.0f;
	
	SceneManager* _mainSceneManager = nullptr;
	SceneNode* _cityRootNode = nullptr;
	
	std::vector<CDPRSkyScraperBlock*> _spawnedBlocks;
	
};
