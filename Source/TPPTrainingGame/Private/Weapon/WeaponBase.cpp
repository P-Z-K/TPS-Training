// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "CustomChannels.h"
#include "WeaponEquippedState.h"
#include "WeaponFallingState.h"
#include "WeaponPickupState.h"
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
	Widget->SetVisibility(true);
	Popup = Cast<UItemPopupBase>(Widget->GetWidget());
	Popup->Setup(Name, AmmoAmount, AmmoType, GetStarsCount());

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
	if (CurrentState)
	{
		CurrentState->OnCursorHoverStart();
	}
}

void AWeaponBase::OnCursorHoverEnded()
{
	if (CurrentState)
	{
		CurrentState->OnCursorHoverEnded();
	}
}

void AWeaponBase::OnClicked(ABaseCharacter* Character)
{
	if (CurrentState)
	{
		CurrentState->OnClicked(Character);
	}
}

void AWeaponBase::Fire()
{
	if (bCanFire)
	{
		FireBullet();
		GetWorldTimerManager().SetTimer(FiringDelayTimerHandle, this, &AWeaponBase::OnDelayEnded, FiringRate);
		bCanFire = false;
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

	bCanFire = true;
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
	if (CurrentState)
	{
		CurrentState->OnDeinit();
		CurrentState->ConditionalBeginDestroy();
	}
	
	State = NewState;
	switch (State)
	{
	case EWeaponState::Pickup: CurrentState = NewObject<UWeaponPickupState>(); break;
	//case EWeaponState::EquipInterping: OnEquipInterping(); break;;
	//case EWeaponState::PickedUp: OnPickedUpState(); break;
	case EWeaponState::Equipped: CurrentState = NewObject<UWeaponEquippedState>(); break;
	case EWeaponState::Falling: CurrentState = NewObject<UWeaponFallingState>(); break;
	default: CurrentState = nullptr;
	}

	if (CurrentState)
	{
		CurrentState->OnInit(this, GetWorld());
	}
}
