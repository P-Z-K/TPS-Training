// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/TPPPlayerController.h"

#include "Core/Player/TPPPlayerCameraManager.h"

ATPPPlayerController::ATPPPlayerController()
{
	PlayerCameraManagerClass = ATPPPlayerCameraManager::StaticClass();
}
