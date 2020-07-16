#include "CDPRPlayerController.h"
#include "CDPRRedBall.h"
#include "CDPRBlackBall.h"
#include "Core/EnergiezApp.h"
#include "Camera/CDPRCameraController.h"


void CDPRPlayerController::Initialize()
{
}

bool CDPRPlayerController::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
	//Left Click
	if (evt.button == 1) {
		CDPRCameraController* cameraController = EnergiezApp::GetSingletonPtr()->_cameraController;

		CDPRRedBall* redBallProjectile = new CDPRRedBall(this);
		redBallProjectile->Initalize();
		redBallProjectile->Shoot(cameraController->GetWorldPosition(), cameraController->GetForwardVector(), _redBallShootingForce);

		return true;

	//Right Click
	} else if(evt.button == 3)
	{
		CDPRCameraController* cameraController = EnergiezApp::GetSingletonPtr()->_cameraController;

		CDPRBlackBall* blackBallProjectile = new CDPRBlackBall(this);
		blackBallProjectile->Initalize();
		blackBallProjectile->Shoot(cameraController->GetWorldPosition(), cameraController->GetForwardVector(), _blackBallShootingForce);

		return true;
		
	}
	
	return false;
}

void CDPRPlayerController::RegisterProjectile(CDPRRedBall* ball)
{
	_spawnedBalls.push_back(ball);
}

void CDPRPlayerController::UnRegisterProjectile(CDPRRedBall* ball)
{
	_spawnedBalls.erase(std::remove(_spawnedBalls.begin(), _spawnedBalls.end(), ball), _spawnedBalls.end());
}
