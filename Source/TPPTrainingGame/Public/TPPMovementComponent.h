// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPPMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TPPTRAININGGAME_API UTPPMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Character Movement (Custom)")
	float SpeedWhileAimingModifier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Character Movement (Custom)")
	float SpeedWhileRunningModifier = 1.2f;
	
public:
	virtual float GetMaxSpeed() const override;
};
