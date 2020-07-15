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

		CDPRRedBall* redBallProjectile = new CDPRRedBall();
		redBallProjectile->Initalize();
		redBallProjectile->Shoot(cameraController->GetWorldPosition(), cameraController->GetForwardVector(), _redBallShootingForce);

		return true;
	}
	
	return false;
}
