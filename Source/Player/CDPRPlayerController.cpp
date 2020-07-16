#include "CDPRPlayerController.h"
#include "CDPRRedBall.h"
#include "CDPRBlackBall.h"
#include "Core/CDPRGameInstance.h"
#include "Camera/CDPRCameraController.h"


void CDPRPlayerController::Initialize()
{
}

bool CDPRPlayerController::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
	//Left Click
	if (evt.button == 1) {
		CDPRCameraController* cameraController = CDPRGameInstance::GetSingletonPtr()->_cameraController;

		CDPRRedBall* redBallProjectile = new CDPRRedBall(this);
		redBallProjectile->Initalize();
		redBallProjectile->Shoot(cameraController->GetWorldPosition(), cameraController->GetForwardVector(), _redBallShootingForce);

		return true;

	//Right Click
	} else if(evt.button == 3)
	{
		CDPRCameraController* cameraController = CDPRGameInstance::GetSingletonPtr()->_cameraController;

		CDPRBlackBall* blackBallProjectile = new CDPRBlackBall(this);
		blackBallProjectile->Initalize();
		blackBallProjectile->Shoot(cameraController->GetWorldPosition(), cameraController->GetForwardVector(), _blackBallShootingForce);

		return true;
		
	}
	
	return false;
}

void CDPRPlayerController::RegisterRedBall(CDPRRedBall* ball)
{
	_spawnedRedBalls.push_back(ball);
}

void CDPRPlayerController::UnRegisterRedBall(CDPRRedBall* ball)
{
	_spawnedRedBalls.erase(std::remove(_spawnedRedBalls.begin(), _spawnedRedBalls.end(), ball), _spawnedRedBalls.end());
}

void CDPRPlayerController::RegisterBlackBall(CDPRBlackBall* ball)
{
	_spawnedBlackBalls.push_back(ball);
}

void CDPRPlayerController::UnRegisterBlackBall(CDPRBlackBall* ball)
{
	_spawnedBlackBalls.erase(std::remove(_spawnedBlackBalls.begin(), _spawnedBlackBalls.end(), ball), _spawnedBlackBalls.end());
}
