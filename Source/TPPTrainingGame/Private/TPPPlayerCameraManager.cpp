// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPPlayerCameraManager.h"

ATPPPlayerCameraManager::ATPPPlayerCameraManager()
{
	bAlwaysApplyModifiers = true;
}

void ATPPPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	auto MyPawn = PCOwner ? Cast<ABaseCharacter>(PCOwner->GetPawn()) : nullptr;
	if (!MyPawn)
	{
		Super::UpdateCamera(DeltaTime);
		return;
	}

	const float TargetFOV = MyPawn->GetIsAiming() ? ZoomFOV : NormalFOV;
	DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, FOVInterpolationLength);
	SetFOV(DefaultFOV);
	
	Super::UpdateCamera(DeltaTime);
}
