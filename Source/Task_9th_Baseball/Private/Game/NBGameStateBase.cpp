// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
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
