// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(Blueprintable)
enum class EWeaponState : uint8
{
	Pickup,
	EquipInterping,
	PickedUp,
	Equipped,
	Falling,
};
