// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBMainUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Player/NBPlayerController.h"
#include "UI/NBPlayLogCard.h"

void UNBMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (EditableText_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UNBMainUI::OnChatInputTextCommitted) == false)
	{
		EditableText_ChatInput->OnTextCommitted.AddDynamic(this, &UNBMainUI::OnChatInputTextCommitted);
	}
	
	if (Button_StartGame->OnClicked.IsAlreadyBound(this, &UNBMainUI::OnStartButtonClicked) == false)
	{
		Button_StartGame->OnClicked.AddDynamic(this, &UNBMainUI::OnStartButtonClicked);
	}	
}

void UNBMainUI::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (EditableText_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UNBMainUI::OnChatInputTextCommitted) == true)
	{
		EditableText_ChatInput->OnTextCommitted.RemoveDynamic(this, &UNBMainUI::OnChatInputTextCommitted);
	}
	
	if (Button_StartGame->OnClicked.IsAlreadyBound(this, &UNBMainUI::OnStartButtonClicked) == true)
	{
		Button_StartGame->OnClicked.RemoveDynamic(this, &UNBMainUI::OnStartButtonClicked);
	}
}

void UNBMainUI::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		TObjectPtr<APlayerController> OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			TObjectPtr<ANBPlayerController> OwningNBPlayerController = Cast<ANBPlayerController>(OwningPlayerController);
			{
				OwningNBPlayerController->SetChatMassageString(Text.ToString());
								
				EditableText_ChatInput->SetText(FText());
			}
		}
	}
}

void UNBMainUI::OnStartButtonClicked()
{
	TObjectPtr<APlayerController> OwningPlayerController = GetOwningPlayer();
	if (IsValid(OwningPlayerController) == true)
	{
		TObjectPtr<ANBPlayerController> OwningNBPlayerController = Cast<ANBPlayerController>(OwningPlayerController);
		{
			OwningNBPlayerController->ServerRPCRequestStartGame();
		}
	}
}

void UNBMainUI::ShowChatLog(const FString& InChatMessageString)
{
	if (IsValid(ScrollBox_ChatLog) == true)
	{
		TObjectPtr<UTextBlock> NewTextBlock = NewObject<UTextBlock>(this);
		
		NewTextBlock->SetFont(ChatFontInfo);
		NewTextBlock->SetAutoWrapText(true);
		NewTextBlock->SetText(FText::FromString(InChatMessageString));
		
		ScrollBox_ChatLog->AddChild(NewTextBlock);
		ScrollBox_ChatLog->ScrollToEnd();
	}
}

void UNBMainUI::SetResultText(const FString& ResultString)
{
	if (IsValid(TextBlock_Result) == true)
	{
		TextBlock_Result->SetText(FText::FromString(ResultString));
	}
}

void UNBMainUI::SetTimerText(float RemainingTime)
{
	if (IsValid(TextBlock_Timer) == true)
	{
		TextBlock_Timer->SetText(FText::FromString(FString::FromInt(RemainingTime)));
	}
}

void UNBMainUI::SetResultUI(const FResult& InResult)
{
	if (IsValid(TextBlock_InputResult) == true)
	{
		TextBlock_InputResult->SetText(FText::FromString(InResult.InputAnswerString));
	}
	if (IsValid(TextBlock_Strike) == true)
	{
		TextBlock_Strike->SetText(FText::FromString(FString::FromInt(InResult.StrikeCount)+TEXT(" S")));
	}
	if (IsValid(TextBlock_Ball) == true)
	{
		TextBlock_Ball->SetText(FText::FromString(FString::FromInt(InResult.BallCount)+TEXT(" B")));
	}
	if (IsValid(TextBlock_Out) == true)
	{
		if (InResult.bIsOut == true)
		{
			TextBlock_Out ->SetText(FText::FromString(TEXT("OUT")));
		}
		else
		{
			TextBlock_Out ->SetText(FText::FromString(TEXT("")));
		}
	}
	
	if (IsValid(ScrollBox_PlayLog) == true)
	{
		if (IsValid(PlayLogCardClass) == true)
		{
			UNBPlayLogCard* PlayLogCard = CreateWidget<UNBPlayLogCard>(this, PlayLogCardClass);
			if (IsValid(PlayLogCard) == true)
			{
				PlayLogCard->SetPlayLogCard(InResult);
				
				ScrollBox_PlayLog->AddChild(PlayLogCard);
			}
		}
	}
}
