#include "CDPRPlayerController.h"
#include "CDPRRedBall.h"
#include "Core/EnergiezApp.h"
#include "Camera/CDPRCameraController.h"


void CDPRPlayerController::Initialize()
{
}

bool CDPRPlayerController::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
	if (evt.button == 1) {
		CDPRCameraController* cameraController = EnergiezApp::GetSingletonPtr()->_cameraController;

		CDPRRedBall* redBallProjectile = new CDPRRedBall(this);
		redBallProjectile->Initalize();
		redBallProjectile->Shoot(cameraController->GetWorldPosition(), cameraController->GetForwardVector(), _redBallShootingForce);

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
