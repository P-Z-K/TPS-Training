// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/CharacterAnimInstance.h"

#include "Core/Player/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/WeaponBase.h"


void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (BaseCharacter == nullptr)
	{
		return;
	}

	Pitch = BaseCharacter->GetBaseAimRotation().Pitch;
	Speed = BaseCharacter->GetVelocity().Size2D();
	bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();
	bIsAiming = BaseCharacter->GetIsAiming();
	CurrentLocomotionState = BaseCharacter->GetCurrentLocomotionState();
	if (Speed > 0.f)
	{
		if (BaseCharacter->GetIsRunning())
		{
			CurrentLocomotionState = ELocomotionState::Running;
		}
		MovementOffsetYawLastFrame = MovementOffsetYaw;
	}


	CalculateMovementOffsetYaw();
	CalculateWeaponIK();
	CalculateDirection();
	CurrentLocomotionState = GetCurrentLocomotionState();
}

void UCharacterAnimInstance::CalculateMovementOffsetYaw()
{
	FRotator AimRotation = BaseCharacter->GetBaseAimRotation();
	auto VelocityNormalized = BaseCharacter->GetVelocity();
	VelocityNormalized.Normalize();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(VelocityNormalized);

	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}

void UCharacterAnimInstance::CalculateWeaponIK()
{
	auto Weapon = BaseCharacter->GetCurrentWeapon();
	if (!Weapon)
	{
		return;
	}
	auto Mesh = Weapon->GetMesh();
	if (!Mesh)
	{
		return;
	}

	//Mesh->GetSocketTransform("")
}

void UCharacterAnimInstance::CalculateDirection()
{
	auto ControllerForward = BaseCharacter->GetControlRotation().Vector();
	auto Velocity = BaseCharacter->GetVelocity();
	Velocity.Normalize();

	Direction = ControllerForward.Dot(Velocity);
}

ELocomotionState UCharacterAnimInstance::GetCurrentLocomotionState() const
{
	if (BaseCharacter->bIsCrouched)
	{
		return ELocomotionState::Crouching;
	}

	if (BaseCharacter->GetIsRunning())
	{
		if (Speed > 0.f)
			return ELocomotionState::Running;
	}

	return ELocomotionState::Idle;
}
