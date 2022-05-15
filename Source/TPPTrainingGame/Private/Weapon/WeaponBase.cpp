// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "CustomChannels.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Player/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "UI/ItemPopupBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	RootComponent = SkeletalMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetupAttachment(SkeletalMesh);

	Widget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	Widget->SetupAttachment(SkeletalMesh);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	Widget->SetVisibility(false);
	Popup = Cast<UItemPopupBase>(Widget->GetWidget());
	Popup->SetAmmoType(AmmoType);
	Popup->SetAmmoAmount(AmmoAmount);
	Popup->SetStarsCount(GetStarsCount());
	Popup->SetWeaponName(Name);
}

void AWeaponBase::PostInitializeComponents()
{

	Super::PostInitializeComponents();
	SetState(State);
}

int AWeaponBase::GetStarsCount()
{
	switch (Rarity)
	{
	case EWeaponRarity::Damaged: return 1;
	case EWeaponRarity::Common: return 2;
	case EWeaponRarity::Uncommon: return 3;
	case EWeaponRarity::Rare: return 4;
	case EWeaponRarity::Legendary: return 5;
	default: return 0;
	}
}

void AWeaponBase::OnCursorHoverStart()
{
	Widget->SetVisibility(true);
}

void AWeaponBase::OnCursorHoverEnded()
{
	Widget->SetVisibility(false);
}

void AWeaponBase::OnClicked(ABaseCharacter* Character)
{
	if (!Character)
	{
		return;
	}
	Character->EquipWeapon(this);
}

void AWeaponBase::Fire()
{
	if (bShouldFire)
	{
		FireBullet();
		GetWorldTimerManager().SetTimer(FiringDelayTimerHandle, this, &AWeaponBase::OnDelayEnded, FiringRate);
		bShouldFire = false;
	}
}

void AWeaponBase::FireBullet()
{
	FTransform SocketTransform = SkeletalMesh->GetSocketTransform(MuzzleFlashSocketName);
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, SocketTransform.GetLocation());
	}

	if (HipFireMontage)
	{
		CharacterThatHolds->PlayAnimMontage(HipFireMontage);
	}

	FHitResult HitResult;
	GetBeamEndLocation(SocketTransform, HitResult);
	DrawDebugSphere(GetWorld(), HitResult.Location, 5.f, 32, FColor::Green, false, 2.f);
	ShowParticles(HitResult.Location, SocketTransform);
}

void AWeaponBase::OnDelayEnded()
{
	if (!CharacterThatHolds)
	{
		return;
	}

	bShouldFire = true;
	if (CharacterThatHolds->GetIsFireButtonPressed())
	{
		Fire();
	}
	else
	{
		if (System)
		{
			System->DeactivateImmediate();
			System = nullptr;
		}
		CharacterThatHolds->StopAnimMontage(HipFireMontage);
	}
}


bool AWeaponBase::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector CrosshairsWorldPosition;
	FVector CrosshairsWorldDirection;
	if (!CharacterThatHolds->IsCrosshairsInWorldSuccess(CrosshairsWorldPosition, CrosshairsWorldDirection))
	{
		return false;
	}

	auto End = CrosshairsWorldPosition + CrosshairsWorldDirection * 50'000.f;
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	CollisionParameters.AddIgnoredActor(Owner);

	GetWorld()->LineTraceSingleByChannel(OutHitResult,
	                                     CrosshairsWorldPosition,
	                                     End,
	                                     ECC_Visibility,
	                                     CollisionParameters);

	if (OutHitResult.bBlockingHit)
	{
		OutHitLocation = OutHitResult.Location;
		return true;
	}
	OutHitLocation = End;
	return false;
}

bool AWeaponBase::GetBeamEndLocation(const FTransform& SocketTransform, FHitResult& OutHitResult)
{
	FVector OutBeamLocation;
	FHitResult CrosshairsHitResult;
	TraceUnderCrosshairs(CrosshairsHitResult, OutBeamLocation);
	//DrawDebugSphere(GetWorld(), OutBeamLocation, 10.f, 32, FColor::Red, false, 2.f);

	// Perform a second trace, this time from the gun barrel
	auto WeaponTraceEnd = OutBeamLocation;
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHitResult,
	                                     SocketTransform.GetLocation(),
	                                     WeaponTraceEnd,
	                                     ECC_Visibility,
	                                     CollisionParameters);

	// object between barrel and BeamEndPoint?
	if (!OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}
	return true;
}

void AWeaponBase::ShowParticles(const FVector& HitLocation, const FTransform& Transform)
{
	if (MuzzleFlash)
	{
		System = UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, SkeletalMesh, MuzzleFlashSocketName);
	}

	if (BeamParticles)
	{
		auto Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles,
		                                                     Transform);
		Beam->SetVectorParameter("Target", HitLocation);
	}

	if (ImpactParticles)
	{
		auto FixedHitRotation = HitLocation;
		FixedHitRotation.X += 7.f;
		// Somehow particle effect is not in the center of the HitLocation, so we have to fix it
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FixedHitRotation);
	}
}

void AWeaponBase::SetState(EWeaponState NewState)
{
	State = NewState;
	switch (State)
	{
	case EWeaponState::Pickup: OnPickupState(); break;
	case EWeaponState::EquipInterping: OnEquipInterping(); break;;
	case EWeaponState::PickedUp: OnPickedUpState(); break;
	case EWeaponState::Equipped: OnEquippedState(); break;
	case EWeaponState::Falling: OnFalling(); break;
	default: ;
	}
}

void AWeaponBase::OnPickupState()
{
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Block);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetEnableGravity(true);
	SkeletalMesh->BodyInstance.bLockXRotation = true;
	SkeletalMesh->BodyInstance.bLockYRotation = true;
	SkeletalMesh->BodyInstance.bLockZRotation = true;

	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Interactable, ECR_Block);

	CollisionSphere->SetSimulatePhysics(false);
	CollisionSphere->SetActive(true);
	CollisionSphere->SetVisibility(false);

	Widget->SetHiddenInGame(false);
}

void AWeaponBase::OnEquippedState()
{
	SkeletalMesh->BodyInstance.bLockXRotation = false;
	SkeletalMesh->BodyInstance.bLockYRotation = false;
	SkeletalMesh->BodyInstance.bLockZRotation = false;
	SkeletalMesh->SetSimulatePhysics(false);
	SkeletalMesh->SetEnableGravity(false);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	CollisionSphere->SetActive(false);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	Widget->SetHiddenInGame(true);
}

void AWeaponBase::OnPickedUpState()
{
}

void AWeaponBase::OnEquipInterping()
{
}

void AWeaponBase::OnFalling()
{
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Block);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Interactable, ECR_Ignore);
	SkeletalMesh->BodyInstance.bLockXRotation = true;
	SkeletalMesh->BodyInstance.bLockYRotation = true;
	SkeletalMesh->BodyInstance.bLockZRotation = true;
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetEnableGravity(true);
	

	CollisionSphere->SetActive(false);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
