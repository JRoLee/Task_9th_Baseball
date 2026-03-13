// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"

#include "UI/NBMainUI.h"

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetInputMode(FInputModeUIOnly());
	
	if (IsValid(MainUIWidgetClass) == true)
	{
		MainUIWidgetInstance = CreateWidget<UNBMainUI>(this, MainUIWidgetClass);
		if (IsValid(MainUIWidgetClass) == true)
		{
			MainUIWidgetInstance->AddToViewport();
		}
	}
	
}
