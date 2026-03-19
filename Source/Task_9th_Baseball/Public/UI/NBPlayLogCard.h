// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBPlayLogCard.generated.h"

class UOverlay;
class UTextBlock;
struct FResult;
/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API UNBPlayLogCard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayLogCard(FResult const& InResult);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayLog;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Strike;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Strike;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Ball;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Ball;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Out;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_HomeRun;
	
	
};
