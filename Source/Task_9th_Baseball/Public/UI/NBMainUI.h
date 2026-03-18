// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBMainUI.generated.h"

struct FResult;
class UNBPlayerCardWidget;
class UButton;
class UNBPlayLogCard;
class UScrollBox;
class UTextBlock;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API UNBMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void ShowChatLog(const FString& InChatMessageString);	
	
	UFUNCTION()
	void SetResultText(const FString& ResultString);
	
	UFUNCTION()
	void SetTimerText(float RemainingTime);
	
	UFUNCTION()
	void SetResultUI(const FResult& InResult);
	
	UFUNCTION()
	void RefreshPlayerList( const TArray<FString>& InNickNames, const TArray<int32>& InGuessCounts, const TArray<int32>& InMaxCounts);
protected:
	
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void OnStartButtonClicked();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableText_ChatInput;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Result;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ChatLog;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Timer;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InputResult;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Strike;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Ball;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Out;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chat | Font")
	FSlateFontInfo ChatFontInfo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_PlayLog;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_StartGame;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_PlayerList;
	
	UPROPERTY(EditAnywhere, Category="PlayLog | Widget")
	TSubclassOf<UNBPlayLogCard> PlayLogCardClass;
	
	UPROPERTY(EditAnywhere, Category="PlayCard | Widget")
	TSubclassOf<UNBPlayerCardWidget> PlayerCardWidgetClass;
};
