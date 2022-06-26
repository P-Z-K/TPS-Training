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
	void Setup(const FText& Text, int AmmoAmount, UTexture2D* Texture, int StarsCount);

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY()
	TArray<UImage*> Stars;

	int StarsToDisplay;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* WeaponNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AmmoAmountText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* AmmoTypeImage;

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
