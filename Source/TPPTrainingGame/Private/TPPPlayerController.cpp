// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPPlayerController.h"

#include "TPPPlayerCameraManager.h"

ATPPPlayerController::ATPPPlayerController()
{
	PlayerCameraManagerClass = ATPPPlayerCameraManager::StaticClass();
}
