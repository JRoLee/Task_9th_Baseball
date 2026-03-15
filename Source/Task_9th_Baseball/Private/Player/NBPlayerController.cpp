// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"

#include "EngineUtils.h"
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
		ServerRPCPrintChatMessageString((InChatMassageString));
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
	for (TActorIterator<ANBPlayerController> It(GetWorld()); It; ++It)
	{
		TObjectPtr<ANBPlayerController> NBPlayerController = *It;
		if (IsValid(NBPlayerController) == true)
		{
			NBPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
}
