// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

class UNBLoginUI;

USTRUCT(BlueprintType)
struct FResult
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString InputAnswerString;
	UPROPERTY()
	int32 StrikeCount;
	UPROPERTY()
	int32 BallCount;
	UPROPERTY()
	bool bIsOut;	
	
	FResult() : InputAnswerString(""), StrikeCount(0), BallCount(0), bIsOut(false) {};
};

class UNBMainUI;
/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API ANBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void SetChatMassageString(const FString& InChatMassageString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintResultString(const FString& InResultString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCShowResult(const FResult& InResult);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCResetUI();

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCRequestStartGame();
	
	//UI Timer 표시
	UFUNCTION()
	void UpdateUITimer(float RemainingTime);
	
	TObjectPtr<UNBMainUI> GetMainUIWidgetInstance() {return MainUIWidgetInstance;}

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNBMainUI> MainUIWidgetClass;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNBMainUI> MainUIWidgetInstance;
	
#pragma region Set Player NickName
	
public:
	
	UFUNCTION(Server, Reliable)
	void ServerRPCLogInGame(const FString& InPlayerNickName);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCLogInGame();
	
	UFUNCTION()
	void ExitGame();
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNBLoginUI> LoginUIWidgetClass;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNBLoginUI> LoginUIWidgetInstance;
	
#pragma endregion 
	
#pragma region Set Player Card
	
	public:
	UFUNCTION(Client, Reliable)
	void ClientRPCRefreshPlayerList(const TArray<FString>& InNickNames, const TArray<int32>& InGuessCounts, const TArray<int32>& InMaxCounts);
#pragma endregion 
};
