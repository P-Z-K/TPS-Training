// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemPopupBase.h"

void UItemPopupBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemPopupBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	Stars.Empty();

	Stars.Add(StarIcon1);
	Stars.Add(StarIcon2);
	Stars.Add(StarIcon3);
	Stars.Add(StarIcon4);
	Stars.Add(StarIcon5);

	for (const auto& Star : Stars)
	{
		Star->SetVisibility(ESlateVisibility::Hidden);
	}

	for (int i = 0; i < StarsToDisplay; ++i)
	{
		Stars[i]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemPopupBase::Setup(const FText& Text, int AmmoAmount, UTexture2D* Texture, int StarsCount)
{
	WeaponNameText->SetText(Text);
	this->AmmoAmountText->SetText(FText::AsNumber(AmmoAmount));
	AmmoTypeImage->SetBrushFromTexture(Texture);
	StarsToDisplay = StarsCount;
}
