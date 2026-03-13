// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

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
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNBMainUI> MainUIWidgetClass;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNBMainUI> MainUIWidgetInstance;
};
