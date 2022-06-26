// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponStateBase.h"

UWeaponStateBase::UWeaponStateBase()
{
}

UWorld* UWeaponStateBase::GetWorld() const
{
	return MyWorld;
}

void UWeaponStateBase::OnInit(AWeaponBase* Owner, UWorld* World)
{
	this->Weapon = Owner;
	MyWorld = World;
}

void UWeaponStateBase::OnDeinit()
{
}

void UWeaponStateBase::OnCursorHoverStart()
{
}

void UWeaponStateBase::OnCursorHoverEnded()
{
}

void UWeaponStateBase::OnClicked(ABaseCharacter* Character)
{
}
