// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponFallingState.h"

#include "CustomChannels.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapon/WeaponBase.h"

void UWeaponFallingState::OnInit(AWeaponBase* Owner, UWorld* World)
{
	Super::OnInit(Owner, World);

	this->Weapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	this->Weapon->GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	this->Weapon->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	this->Weapon->GetMesh()->SetCollisionResponseToChannel(ECC_Interactable, ECR_Ignore);
	
	this->Weapon->GetMesh()->BodyInstance.bLockXRotation = true;
	this->Weapon->GetMesh()->BodyInstance.bLockYRotation = true;
	this->Weapon->GetMesh()->BodyInstance.bLockZRotation = true;
	
	this->Weapon->GetMesh()->SetSimulatePhysics(true);
	this->Weapon->GetMesh()->SetEnableGravity(true);
	

	this->Weapon->GetCollisionSphere()->SetActive(false);
	this->Weapon->GetCollisionSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->Weapon->GetWidget()->SetHiddenInGame(true);
}
