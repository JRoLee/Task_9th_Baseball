// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBMainUI.generated.h"

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
	
protected:
	
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableText_ChatInput;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Result;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ChatLog;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Timer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chat | Font")
	FSlateFontInfo ChatFontInfo;
};
