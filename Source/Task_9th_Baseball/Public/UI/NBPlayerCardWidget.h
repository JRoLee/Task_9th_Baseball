// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBPlayerCardWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API UNBPlayerCardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void InitializePlayerCard(const FString& NickName, int32 CurrentGuessCount, int32 MaxGuessCount);
	
	void UpdatePlayerCardInfo(int32 CurrentGuessCount, int32 MaxGuessCount);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NickName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_GuessCount;
};
