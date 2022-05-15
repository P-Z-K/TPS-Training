// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Player//BaseCharacter.h"
#include "Interactable.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPPTRAININGGAME_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	void TakeInteractable();

protected:
	virtual void BeginPlay() override;

	void SearchForInteractableObjects();
	void EndInteractionForLastInteractable();

	UPROPERTY()
	ABaseCharacter* MyOwner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Interaction)
	float InteractionLength;

	UPROPERTY()
	TScriptInterface<IInteractable> CurrentInteractable;

	UPROPERTY()
	TScriptInterface<IInteractable> InteractableInLastFrame;

	FHitResult HitResult;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
