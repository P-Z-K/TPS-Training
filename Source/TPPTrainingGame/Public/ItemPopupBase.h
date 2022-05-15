// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ItemPopupBase.generated.h"

/**
 * 
 */
UCLASS()
class TPPTRAININGGAME_API UItemPopupBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWeaponName(const FText& Text);
	void SetAmmoAmount(int Amount);
	void SetAmmoType(UTexture2D* Texture);
	void SetStarsCount(int Count);

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY()
	TArray<UImage*> Stars;

	int StarsToDisplay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* WeaponName;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AmmoAmount;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* AmmoType;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* StarIcon1;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* StarIcon2;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* StarIcon3;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* StarIcon4;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* StarIcon5;
};
