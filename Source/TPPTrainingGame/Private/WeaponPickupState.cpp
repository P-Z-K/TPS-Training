// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickupState.h"

#include "CustomChannels.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Player/BaseCharacter.h"
#include "UI/ItemPopupBase.h"
#include "Weapon/WeaponBase.h"

void UWeaponPickupState::OnInit(AWeaponBase* Owner, UWorld* World)
{
	Super::OnInit(Owner, World);

	this->Weapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	this->Weapon->GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	this->Weapon->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	this->Weapon->GetMesh()->SetSimulatePhysics(true);
	this->Weapon->GetMesh()->SetEnableGravity(true);
	
	this->Weapon->GetMesh()->BodyInstance.bLockXRotation = true;
	this->Weapon->GetMesh()->BodyInstance.bLockYRotation = true;
	this->Weapon->GetMesh()->BodyInstance.bLockZRotation = true;

	this->Weapon->GetCollisionSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->Weapon->GetCollisionSphere()->SetCollisionResponseToAllChannels(ECR_Overlap);
	this->Weapon->GetCollisionSphere()->SetCollisionResponseToChannel(ECC_Interactable, ECR_Block);

	this->Weapon->GetCollisionSphere()->SetSimulatePhysics(false);
	this->Weapon->GetCollisionSphere()->SetActive(true);
	this->Weapon->GetCollisionSphere()->SetVisibility(false);

	this->Weapon->GetWidget()->SetHiddenInGame(true);
}

void UWeaponPickupState::OnCursorHoverStart()
{
	Super::OnCursorHoverStart();
	this->Weapon->GetWidget()->SetHiddenInGame(false);
}

void UWeaponPickupState::OnCursorHoverEnded()
{
	Super::OnCursorHoverEnded();
	this->Weapon->GetWidget()->SetHiddenInGame(true);
}

void UWeaponPickupState::OnClicked(ABaseCharacter* Character)
{
	Super::OnClicked(Character);

	if (Character)
	{
		Character->EquipWeapon(Weapon);
	}
}
