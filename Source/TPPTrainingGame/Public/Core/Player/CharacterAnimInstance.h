// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocomotionState.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ABaseCharacter;
/**
 * 
 */
UCLASS()
class TPPTRAININGGAME_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	void CalculateWeaponIK();
	void CalculateMovementOffsetYaw();
	void CalculateDirection();
	ELocomotionState GetCurrentLocomotionState() const;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	ABaseCharacter* BaseCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	float CharacterYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	float CharacterYawLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	ELocomotionState CurrentLocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat)
	bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement)
	float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementOffsetYawLastFrame;
};
