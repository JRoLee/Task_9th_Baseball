// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

struct FResult;
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
	void StartGamePlay();
	
	void EndGamePlay();
		
	void PrintChatMessageString(ANBPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
	FString GenerateAnswerNumber();
	
	bool IsGuessNumberString(const FString& InNumberString);
	
	FString JudgeResult(const FString& InAnswerNumberString, const FString& InGuessNumberString);
	
	FResult MakeJudgeResult(const FString& InAnswerNumberString, const FString& InGuessNumberString);
	
	void IncreaseGuessCount(ANBPlayerController* InChattingPlayerController);
	
	FString SetPlayerInfoString(ANBPlayerController* InChattingPlayerController);
	
	void ResetGame();

	void JudgeGame(ANBPlayerController* InChattingPlayerController, int32 InStrikeCount);
	
protected:
	
	FGameplayTag CurrentGameState = FGameplayTag::RequestGameplayTag(FName("Game.State.Wait"));
	
	FString AnswerNumberString;
	
	TArray<TObjectPtr<ANBPlayerController>> AllPlayerControllers;
#pragma endregion
	
#pragma region TurnPlayLogic
public:
	
	void StartPlayerTurnTimer();
	
	void ChangePlayerTurn();
	
	void ChangePlayerTurnByTimer();
	
	void SetPlayerToPlay(int32 InPlayerIndex);
	
	
protected:
	
	FTimerHandle PlayerTurnTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float PlayerTurnTime = 10;
	
	int32 PlayerIndex = 0;
	
	bool bIsPlaying = false;

#pragma endregion
	
#pragma region LogIn logic
	
public:
	
	bool IsValidNickName(const FString& InNickName);
	
	UFUNCTION()
	void SetPlayerNickName(ANBPlayerController* LoginPlayerController, const FString& InPlayerNickName);

#pragma endregion
};

