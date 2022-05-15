// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"

/**
 * 
 */
UCLASS()
class TPPTRAININGGAME_API AHUDBase : public AHUD
{
	GENERATED_BODY()
public:
	FORCEINLINE FVector2D GetCrosshairLocation() const {return CrosshairLocation;}
	
protected:
	virtual void DrawHUD() override;

	FVector2D GetCenteredCrosshair(FVector2D CenterOfScreen);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* CrosshairTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScreenW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScreenH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrosshairYOffset;
	
	FVector2D CrosshairLocation;
};
