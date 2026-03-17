// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/NBPlayerController.h"

void ANBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PlayerController) == true)
		{
			ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(PlayerController);
			if (IsValid(NBPlayerController) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game");
				NBPlayerController -> SetChatMassageString(NotificationString);
			}
		}
	}
}

void ANBGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ANBGameStateBase, RemainingTurnTime);
	
}

void ANBGameStateBase::DecreaseRemainingTurnTime()
{
	RemainingTurnTime--;
	RemainingTurnTime = FMath::Max(RemainingTurnTime, 0.0f);
}

void ANBGameStateBase::StartUITimer()
{
	GetWorldTimerManager().SetTimer(
		UITimerHandle,
		this,
		&ANBGameStateBase::DecreaseRemainingTurnTime,
		1.0f,
		true
	);
}

void ANBGameStateBase::OnRep_RemainingTurnTime()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController) == true)
	{
		ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(PlayerController);
		if (IsValid(NBPlayerController) == true)
		{
			NBPlayerController->UpdateUITimer(RemainingTurnTime);
		}
	}
}

