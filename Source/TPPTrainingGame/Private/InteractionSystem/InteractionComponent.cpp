// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractionComponent.h"

#include "CustomChannels.h"
#include "DrawDebugHelpers.h"
#include "Core/Player/BaseCharacter.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MyOwner = Cast<ABaseCharacter>(GetOwner());
	SearchForInteractableObjects();
}

void UInteractionComponent::SearchForInteractableObjects()
{
	FVector CrosshairLocation;
	FVector CrosshairDirection;
	if (!MyOwner->IsCrosshairsInWorldSuccess(CrosshairLocation, CrosshairDirection))
	{
		return;
	}

	auto End = CrosshairLocation + CrosshairDirection * InteractionLength;
	GetWorld()->LineTraceSingleByChannel(HitResult, CrosshairLocation, End, ECC_Interactable);

	if (!HitResult.bBlockingHit)
	{
		CurrentInteractable = nullptr;
		EndInteractionForLastInteractable();
		return;
	}

	auto InteractableObj = Cast<IInteractable>(HitResult.GetActor());
	if (!InteractableObj)
	{
		CurrentInteractable = nullptr;
		EndInteractionForLastInteractable();
		return;
	}

	CurrentInteractable.SetInterface(InteractableObj);
	CurrentInteractable.SetObject(HitResult.GetActor());

	if (InteractableInLastFrame.GetObject() != CurrentInteractable.GetObject())
	{
		EndInteractionForLastInteractable();
		CurrentInteractable->OnCursorHoverStart();
	}

	InteractableInLastFrame.SetInterface(CurrentInteractable.GetInterface());
	InteractableInLastFrame.SetObject(CurrentInteractable.GetObject());
}

void UInteractionComponent::EndInteractionForLastInteractable()
{
	if (!InteractableInLastFrame)
	{
		return;
	}
	InteractableInLastFrame->OnCursorHoverEnded();
	InteractableInLastFrame.SetInterface(nullptr);
	InteractableInLastFrame.SetObject(nullptr);
}


void UInteractionComponent::TakeInteractable()
{
	if (!CurrentInteractable)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("yes"));

	CurrentInteractable->OnClicked(MyOwner);
}
