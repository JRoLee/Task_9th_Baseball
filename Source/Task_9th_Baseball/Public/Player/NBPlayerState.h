// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API ANBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ANBPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	FString PlayerNameString;
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	
	UPROPERTY(Replicated)
	FGameplayTag CurrentPlayerState;
	
	UPROPERTY(Replicated, EditAnywhere)
	int32 MaxGuessCount;
	
	FString GetPlayerInfoString();
};
