// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDBase.h"

#include "Engine/Canvas.h"

void AHUDBase::DrawHUD()
{
	FVector2D CenterOfScreen = FVector2D(Canvas->SizeX / 2.f, Canvas->SizeY / 2.f);
	auto CenteredCrosshair = GetCenteredCrosshair(CenterOfScreen);
	DrawTexture(CrosshairTexture, CenteredCrosshair.X, CenteredCrosshair.Y, ScreenW, ScreenH, 0.f, 0.f, 1.f, 1.f);
	
	Super::DrawHUD();
}

FVector2D AHUDBase::GetCenteredCrosshair(FVector2D CenterOfScreen)
{
	CrosshairLocation = CenterOfScreen;
	CrosshairLocation.Y -= CrosshairYOffset;

	auto XPos = CenterOfScreen.X - (ScreenW / 2.f);
	auto YPos = CenterOfScreen.Y - (ScreenH / 2.f) - CrosshairYOffset;
	
	return FVector2D(XPos, YPos);
}
