// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocomotionState.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class AWeaponBase;
class UInteractionComponent;
class UTimelineComponent;
class USoundCue;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TPPTRAININGGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);
	bool IsCrosshairsInWorldSuccess(FVector& WorldPosition, FVector& WorldDirection);
	void EquipWeapon(AWeaponBase* WeaponToEquip);


#pragma region Getters
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	FORCEINLINE ELocomotionState GetCurrentLocomotionState() const { return CurrentLocomotionState; }
	FORCEINLINE bool GetIsFireButtonPressed() const {return bIsFireButtonPressed;}
	
	FORCEINLINE bool GetIsRunning() const {return bIsRunning;}
	FORCEINLINE void SetIsRunning(bool Value) {bIsRunning = Value;}
	FORCEINLINE AWeaponBase* GetCurrentWeapon() const {return  EquippedWeaponInstance;}
#pragma endregion Getters

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);

	void ActivateAiming();
	void DeactivateAiming();

	void OnCrouchButtonPressed();
	void OnCrouchButtonReleased();

	void OnRunButtonPressed();
	void OnRunButtonReleased();

	void Take();
	void DropWeapon();

	void FireWeapon();
	void SpawnDefaultWeapon();

	void OnFireButtonPressed();
	void OnFireButtonReleased();

	
	bool bIsFireButtonPressed = false;
	bool bIsRunning = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Interaction)
	UInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Combat)
	bool bIsAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta=(ClampMax=1.f, ClampMin=0.f))
	float AimingScaleFactor = 0.8f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Combat)
	AWeaponBase* EquippedWeaponInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	TSubclassOf<AWeaponBase> DefaultEquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Combat)
	FName WeaponSocketName;


#pragma region Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	ELocomotionState CurrentLocomotionState = ELocomotionState::Idle;
#pragma endregion Movement
};
