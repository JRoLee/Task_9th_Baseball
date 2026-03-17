// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

class ANBPlayerController;
/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API ANBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()	
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void BeginPlay() override;
		
#pragma region GamePlayLogic
	
public:
		
	void PrintChatMessageString(ANBPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
	FString GenerateAnswerNumber();
	
	bool IsGuessNumberString(const FString& InNumberString);
	
	FString JudgeResult(const FString& InAnswerNumberString, const FString& InGuessNumberString);
	
	void IncreaseGuessCount(ANBPlayerController* InChattingPlayerController);
	
	FString SetPlayerInfoString(ANBPlayerController* InChattingPlayerController);
	
	void ResetGame();

	void JudgeGame(ANBPlayerController* InChattingPlayerController, int32 InStrikeCount);
	
protected:
	FString AnswerNumberString;
	
	TArray<TObjectPtr<ANBPlayerController>> AllPlayerControllers;
#pragma endregion
	
#pragma region TurnPlayLogic
public:
	
	void StartPlayerTurnTimer();
	
	void ChangePlayerTurn();
	
	
protected:
	
	FTimerHandle PlayerTurnTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float PlayerTurnTime = 10;
	
	int32 PlayerTurnIndex = 0;

#pragma endregion
	
};

