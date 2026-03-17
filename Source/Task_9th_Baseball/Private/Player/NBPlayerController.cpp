// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"
#include "Game/NBGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/NBMainUI.h"

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController() == false)
	{
		return;
	}
	
	
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

void ANBPlayerController::SetChatMassageString(const FString& InChatMassageString)
{
	ChatMassageString = InChatMassageString;
	
	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMassageString);
	}
}

void ANBPlayerController::ClientRPCPrintResultString_Implementation(const FString& InResultString)
{
	if (IsValid(MainUIWidgetInstance) == true)
	{
		MainUIWidgetInstance->SetResultText(InResultString);
	}
}

void ANBPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	if (IsValid(MainUIWidgetInstance) == true)
	{
		MainUIWidgetInstance->ShowChatLog(InChatMessageString);
	}
}

void ANBPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == true)
	{
		ANBGameModeBase* NBGameModeBase = Cast<ANBGameModeBase>(GameMode);
		if (IsValid(NBGameModeBase) == true)
		{
			NBGameModeBase->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ANBPlayerController::UpdateUITimer(float RemainingTime)
{
	if (IsValid(MainUIWidgetInstance) == true)
	{
		MainUIWidgetInstance->SetTimerText(RemainingTime);
	}
}
