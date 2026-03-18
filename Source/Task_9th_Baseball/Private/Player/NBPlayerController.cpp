// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"

#include "Game/NBGameInstance.h"
#include "Game/NBGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/NBLoginUI.h"
#include "UI/NBMainUI.h"

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController() == false)
	{
		return;
	}
	
	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
	
	if (IsValid(LoginUIWidgetClass) == true)
	{
		LoginUIWidgetInstance = CreateWidget<UNBLoginUI>(this, LoginUIWidgetClass);
		if (IsValid(LoginUIWidgetInstance) == true)
		{
			LoginUIWidgetInstance->AddToViewport();
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

void ANBPlayerController::ClientRPCShowResult_Implementation(const FResult& InResult)
{
	if (IsValid(MainUIWidgetInstance) == true)
	{
		MainUIWidgetInstance->SetResultUI(InResult);
	}
}

void ANBPlayerController::ServerRPCRequestStartGame_Implementation()
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == true)
	{
		ANBGameModeBase* NBGameModeBase = Cast<ANBGameModeBase>(GameMode);
		if (IsValid(NBGameModeBase) == true)
		{
			NBGameModeBase->StartGamePlay();
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


void ANBPlayerController::ServerRPCLogInGame_Implementation(const FString& InPlayerNickName)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == true)
	{
		ANBGameModeBase* NBGameModeBase = Cast<ANBGameModeBase>(GameMode);
		if (IsValid(NBGameModeBase) == true)
		{
			NBGameModeBase->SetPlayerNickName(this, InPlayerNickName);
		}
	}
}

void ANBPlayerController::ClientRPCLogInGame_Implementation()
{
	if (IsValid(LoginUIWidgetInstance) == true)
	{
		LoginUIWidgetInstance->RemoveFromParent();
	}
	
	if (IsValid(MainUIWidgetClass) == true)
	{
		MainUIWidgetInstance = CreateWidget<UNBMainUI>(this, MainUIWidgetClass);
		if (IsValid(MainUIWidgetClass) == true)
		{
			MainUIWidgetInstance->AddToViewport();
		}
	}
}

void ANBPlayerController::ExitGame()
{
}
