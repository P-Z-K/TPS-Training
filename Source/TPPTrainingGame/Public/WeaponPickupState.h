// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponStateBase.h"
#include "WeaponPickupState.generated.h"

/**
 * 
 */
UCLASS()
class TPPTRAININGGAME_API UWeaponPickupState : public UWeaponStateBase
{
	GENERATED_BODY()

public:
	virtual void OnInit(AWeaponBase* Owner, UWorld* World) override;
	virtual void OnCursorHoverStart() override;
	virtual void OnCursorHoverEnded() override;
	virtual void OnClicked(ABaseCharacter* Character) override;
};
