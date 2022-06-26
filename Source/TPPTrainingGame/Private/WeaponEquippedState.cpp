// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponEquippedState.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/ItemPopupBase.h"
#include "Weapon/WeaponBase.h"

void UWeaponEquippedState::OnInit(AWeaponBase* Owner, UWorld* World)
{
	Super::OnInit(Owner, World);

	this->Weapon->GetMesh()->BodyInstance.bLockXRotation = false;
	this->Weapon->GetMesh()->BodyInstance.bLockYRotation = false;
	this->Weapon->GetMesh()->BodyInstance.bLockZRotation = false;

	this->Weapon->GetMesh()->SetSimulatePhysics(false);
	this->Weapon->GetMesh()->SetEnableGravity(false);
	this->Weapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->Weapon->GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);

	this->Weapon->GetCollisionSphere()->SetActive(false);
	this->Weapon->GetCollisionSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->Weapon->GetCollisionSphere()->SetCollisionResponseToAllChannels(ECR_Ignore);

	this->Weapon->GetWidget()->SetHiddenInGame(true);
}
