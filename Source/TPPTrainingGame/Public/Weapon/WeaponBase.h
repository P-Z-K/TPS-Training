// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponRarity.h"
#include "WeaponState.h"
#include "GameFramework/Actor.h"
#include "InteractionSystem/Interactable.h"
#include "WeaponBase.generated.h"

class UItemPopupBase;
class USoundCue;
class UWidgetComponent;
class USphereComponent;
class UWeaponStateBase;

UCLASS()
class TPPTRAININGGAME_API AWeaponBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	virtual void OnCursorHoverStart() override;
	virtual void OnCursorHoverEnded() override;
	virtual void OnClicked(ABaseCharacter* Character) override;

	void Fire();
	
	bool GetBeamEndLocation(const FTransform& SocketTransform, FHitResult& OutHitResult);
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);
	void ShowParticles(const FVector& HitLocation, const FTransform& Transform);

	void SetState(EWeaponState NewState);

	FORCEINLINE USkeletalMeshComponent* GetMesh() const {return SkeletalMesh;}
	FORCEINLINE UWidgetComponent* GetWidget() const {return Widget;}
	FORCEINLINE USphereComponent* GetCollisionSphere() const {return CollisionSphere;}
	FORCEINLINE void SetCharacterThatHolds(ABaseCharacter* Character) {CharacterThatHolds = Character;}

protected:
	virtual void BeginPlay() override;

	void FireBullet();
	void OnDelayEnded();
	int GetStarsCount();

	UPROPERTY()
	UWeaponStateBase* CurrentState;


	FTimerHandle FiringDelayTimerHandle;
	bool bCanFire = true;

	UPROPERTY()
	ABaseCharacter* CharacterThatHolds = nullptr;

	UPROPERTY()
	UItemPopupBase* Popup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWeaponState State = EWeaponState::Pickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Properties)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Properties, meta=(ClampMin=0, ClampMax=99))
	int AmmoAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Properties)
	UTexture2D* AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Properties)
	EWeaponRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	float FiringRate = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	USoundCue* FireSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Combat)
	FName MuzzleFlashSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	UParticleSystem* MuzzleFlash = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	UParticleSystem* ImpactParticles = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	UParticleSystem* BeamParticles = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	UParticleSystemComponent* System = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	UAnimMontage* HipFireMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	USphereComponent* CollisionSphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	UWidgetComponent* Widget = nullptr;
};
