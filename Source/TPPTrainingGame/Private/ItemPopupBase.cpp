// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPopupBase.h"

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
	
	for(const auto& Star : Stars)
	{
		Star->SetVisibility(ESlateVisibility::Hidden);
	}

	for(int i=0; i < StarsToDisplay; ++i)
	{
		Stars[i]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemPopupBase::SetWeaponName(const FText& Text)
{
	WeaponName->SetText(Text);
}

void UItemPopupBase::SetAmmoAmount(int Amount)
{
	AmmoAmount->SetText(FText::AsNumber(Amount));
}

void UItemPopupBase::SetAmmoType(UTexture2D* Texture)
{
	AmmoType->SetBrushFromTexture(Texture);
}

void UItemPopupBase::SetStarsCount(int Count)
{
	StarsToDisplay = Count;
}
