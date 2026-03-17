// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API ANBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("DefaultName")));
	
#pragma region UI Timer
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void DecreaseRemainingTurnTime();
	
	void StartUITimer();
	
	UPROPERTY(ReplicatedUsing= OnRep_RemainingTurnTime)
	float RemainingTurnTime;
	
protected:
	UFUNCTION()
	void OnRep_RemainingTurnTime();
	
	FTimerHandle UITimerHandle;
#pragma endregion  
};
