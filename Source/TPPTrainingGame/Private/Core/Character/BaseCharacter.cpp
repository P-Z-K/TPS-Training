// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Core/Player/TPPMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InteractionSystem/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/HUDBase.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponState.h"
#include "Engine/EngineTypes.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTPPMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// Setting up a spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 115.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.f, 50.f, 70.f);

	// Setting up a camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// Don't rotate when the controller rotates. Let the controller only affects the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.15f;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
}

bool ABaseCharacter::IsCrosshairsInWorldSuccess(FVector& WorldPosition, FVector& WorldDirection)
{
	if (!GetWorld())
	{
		return false;
	}
	auto PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return false;
	}
	FVector2D CrosshairLocation = Cast<AHUDBase>(PC->GetHUD())->GetCrosshairLocation();

	auto bResult = UGameplayStatics::DeprojectScreenToWorld(PC, CrosshairLocation, WorldPosition,
	                                                        WorldDirection);
	return bResult;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultWeapon();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);


	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::OnFireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::OnFireButtonReleased);

	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &ABaseCharacter::ActivateAiming);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &ABaseCharacter::DeactivateAiming);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::OnCrouchButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::OnCrouchButtonReleased);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ABaseCharacter::OnRunButtonPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABaseCharacter::OnRunButtonReleased);

	PlayerInputComponent->BindAction("Take", IE_Pressed, this, &ABaseCharacter::Take);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &ABaseCharacter::DropWeapon);
}

void ABaseCharacter::MoveForward(float Value)
{
	if (Value == 0.f)
	{
		return;
	}
	AddMovementInput(GetActorForwardVector(), Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Value == 0.f)
	{
		return;
	}
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetActorRotation()), Value);
}

void ABaseCharacter::Turn(float Value)
{
	if (Value == 0.f) return;

	if (bIsAiming)
	{
		Value *= AimingScaleFactor;
	}
	AddControllerYawInput(Value);
}

void ABaseCharacter::LookUp(float Value)
{
	if (Value == 0.f) return;

	if (bIsAiming)
	{
		Value *= AimingScaleFactor;
	}
	AddControllerPitchInput(Value);
}

void ABaseCharacter::ActivateAiming()
{
	if (!EquippedWeaponInstance)
	{
		return;
	}
	bIsAiming = true;
}

void ABaseCharacter::DeactivateAiming()
{
	if (!EquippedWeaponInstance)
	{
		return;
	}
	bIsAiming = false;
}

void ABaseCharacter::OnFireButtonReleased()
{
	bIsFireButtonPressed = false;
}

void ABaseCharacter::OnFireButtonPressed()
{
	bIsFireButtonPressed = true;
	FireWeapon();
}

void ABaseCharacter::OnCrouchButtonPressed()
{
	if (bIsRunning)
	{
		return;
	}

	Crouch();
}

void ABaseCharacter::OnCrouchButtonReleased()
{
	if (bIsRunning)
	{
		return;
	}

	UnCrouch();
}

void ABaseCharacter::OnRunButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}

	bIsRunning = true;
}

void ABaseCharacter::OnRunButtonReleased()
{
	bIsRunning = false;
}

void ABaseCharacter::Take()
{
	InteractionComponent->TakeInteractable();
}

void ABaseCharacter::DropWeapon()
{
	if (!EquippedWeaponInstance)
	{
		return;
	}
	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, true);
	EquippedWeaponInstance->DetachFromActor(Rules);
	EquippedWeaponInstance->SetState(EWeaponState::Falling);
	EquippedWeaponInstance = nullptr;
}

void ABaseCharacter::EquipWeapon(AWeaponBase* WeaponToEquip)
{
	if (!WeaponToEquip)
	{
		return;
	}

	if (EquippedWeaponInstance)
	{
		EquippedWeaponInstance->SetActorHiddenInGame(true);
		EquippedWeaponInstance->Destroy();
	}
	EquippedWeaponInstance = WeaponToEquip;
	EquippedWeaponInstance->SetCharacterThatHolds(this);
	EquippedWeaponInstance->SetState(EWeaponState::Equipped);

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	                                EAttachmentRule::SnapToTarget, true);
	EquippedWeaponInstance->AttachToComponent(GetMesh(), Rules, WeaponSocketName);
}

void ABaseCharacter::FireWeapon()
{
	if (!EquippedWeaponInstance)
	{
		return;
	}

	EquippedWeaponInstance->Fire();
}

void ABaseCharacter::SpawnDefaultWeapon()
{
	if (!DefaultEquippedWeapon)
	{
		return;
	}
	auto SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(DefaultEquippedWeapon);
	EquipWeapon(SpawnedWeapon);
}
