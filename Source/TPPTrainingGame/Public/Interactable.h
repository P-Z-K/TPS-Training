// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class ABaseCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPPTRAININGGAME_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void OnCursorHoverStart() = 0;
	virtual void OnCursorHoverEnded() = 0;
	virtual void OnClicked(ABaseCharacter* Character) = 0;
};
