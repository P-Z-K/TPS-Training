// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/TPPMovementComponent.h"
#include "Core/Player/BaseCharacter.h"


float UTPPMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	const auto Character = Cast<ABaseCharacter>(PawnOwner);
	if (!CharacterOwner)
	{
		return MaxSpeed;
	}

	if (Character->GetIsAiming())
	{
		MaxSpeed *= SpeedWhileAimingModifier;
	}
	if (Character->GetIsRunning())
	{
		MaxSpeed *= SpeedWhileRunningModifier;
	}
	
	return MaxSpeed;
}
