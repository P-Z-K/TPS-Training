// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/TimelineComponent.h"
#include "TPPPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class TPPTRAININGGAME_API ATPPPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ATPPPlayerCameraManager();

protected:
	virtual void UpdateCamera(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float NormalFOV = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float ZoomFOV = 65.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float FOVInterpolationLength = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float CrouchInterpolationLength = 20.f;
};
