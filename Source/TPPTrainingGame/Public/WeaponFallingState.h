// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponStateBase.h"
#include "WeaponFallingState.generated.h"

/**
 * 
 */
UCLASS()
class TPPTRAININGGAME_API UWeaponFallingState : public UWeaponStateBase
{
	GENERATED_BODY()

public:
	virtual void OnInit(AWeaponBase* Owner, UWorld* World) override;
};
