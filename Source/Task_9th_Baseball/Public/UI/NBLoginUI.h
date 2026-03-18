// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBLoginUI.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class TASK_9TH_BASEBALL_API UNBLoginUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
protected:
	
	UFUNCTION()
	void OnLoginButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_LoginButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ExitButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableText_NickName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NickName | Option")
	int32 NickNameLengthLimit = 6;
	
};
