// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponStateBase.generated.h"

class AWeaponBase;
class ABaseCharacter;
/**
 * 
 */
UCLASS(Abstract)
class TPPTRAININGGAME_API UWeaponStateBase : public UObject
{
	GENERATED_BODY()

public:
	UWeaponStateBase();

	virtual UWorld* GetWorld() const override;


	virtual void OnInit(AWeaponBase* Owner, UWorld* World);
	virtual void OnDeinit();
	
	virtual void OnCursorHoverStart();
	virtual void OnCursorHoverEnded();
	virtual void OnClicked(ABaseCharacter* Character);

protected:
	UPROPERTY()
	AWeaponBase* Weapon;

	UPROPERTY()
	UWorld* MyWorld;
};
