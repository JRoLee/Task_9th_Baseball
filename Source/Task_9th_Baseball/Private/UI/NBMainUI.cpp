// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBMainUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Player/NBPlayerController.h"
#include "UI/NBPlayerCardWidget.h"
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
	
	if (IsValid(HorizontalBox_StrikeImage) == true)
	{
		for (int32 i = 0; i < HorizontalBox_StrikeImage->GetChildrenCount(); i++)
		{
			UWidget* Child = HorizontalBox_StrikeImage->GetChildAt(i);
			UImage* ImageWidget = Cast<UImage>(Child);
			if (IsValid(ImageWidget) == true)
			{
				StrikeImages.Add(ImageWidget);
			}
		}
	}
	
	if (IsValid(HorizontalBox_StrikeImage) == true)
	{
		for (int32 i = 0; i < HorizontalBox_BallImage->GetChildrenCount(); i++)
		{
			UWidget* Child = HorizontalBox_BallImage->GetChildAt(i);
			UImage* ImageWidget = Cast<UImage>(Child);
			if (IsValid(ImageWidget) == true)
			{
				BallImages.Add(ImageWidget);
			}
		}
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
				EditableText_ChatInput->SetKeyboardFocus();
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
	TempStrikeCount = InResult.StrikeCount;
	TempBallCount = InResult.BallCount;
	bIsOutTemp = InResult.bIsOut;
	
	if (IsValid(TextBlock_InputResult) == true)
	{
		TextBlock_InputResult->SetText(FText::FromString(InResult.InputAnswerString));
	}
		
	GetWorld()->GetTimerManager().SetTimer(
		StrikeTimerHandle,
		this,
		&UNBMainUI::SetStrikeResult,
		0.2f,
		false
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		BallTimerHandle,
		this,
		&UNBMainUI::SetBallResult,		
		0.4f,
		false
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		OutTimerHandle,
		this,
		&UNBMainUI::SetOutResult,
		0.6f,
		false
	);
	
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
	
	if (TempStrikeCount == 3)
	{
		TextBlock_InputResult->SetText(FText::FromString(InResult.InputAnswerString + TEXT("\n HOME RUN"))) ;
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		ResetTimerHandle,
		this,
		&UNBMainUI::ResetResult,	
		1.6f,
		false
	);
}

void UNBMainUI::SetStrikeResult()
{
	GetWorld()->GetTimerManager().ClearTimer(StrikeTimerHandle);
	
	if (TempStrikeCount == 0 || TempStrikeCount == 3)
	{
		return;
	}
	
	if (IsValid(TextBlock_Strike) == true)
	{
		TextBlock_Strike->SetText(FText::FromString(FString::FromInt(TempStrikeCount)+TEXT(" S")));
	}
	
	if (IsValid(HorizontalBox_StrikeImage) == true)
	{
		for (int32 i = 0; i < TempStrikeCount; i++)
		{
			if (IsValid(StrikeImages[i]) == true)
			{
				TObjectPtr<UTexture2D> TargetTexture2D = StrikeTexture;
				
				StrikeImages[i]->SetBrushFromTexture(TargetTexture2D);
				StrikeImages[i]->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
			}
		}
	}
	
}

void UNBMainUI::SetBallResult()
{
	GetWorld()->GetTimerManager().ClearTimer(BallTimerHandle);
	
	if (TempBallCount == 0)
	{
		return;
	}
	
	if (IsValid(TextBlock_Ball) == true)
	{
		TextBlock_Ball->SetText(FText::FromString(FString::FromInt(TempBallCount)+TEXT(" B")));
	}
	
	if (IsValid(HorizontalBox_BallImage) == true)
	{
		for (int32 i = 0; i < TempBallCount; i++)
		{
			if (IsValid(BallImages[i]) == true)
			{
				TObjectPtr<UTexture2D> TargetTexture2D = BallTexture;
				
				BallImages[i]->SetBrushFromTexture(TargetTexture2D);
				BallImages[i]->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
			}
		}
	}
	
}

void UNBMainUI::SetOutResult()
{
	GetWorld()->GetTimerManager().ClearTimer(OutTimerHandle);
	
	if (bIsOutTemp == false)
	{
		return;
	}
	
	if (IsValid(TextBlock_Out) == true)
	{
		if (bIsOutTemp == true)
		{
			TextBlock_Out ->SetText(FText::FromString(TEXT("OUT")));
			if (IsValid(Image_OutImage) == true)
			{
				TObjectPtr<UTexture2D> TargetTexture2D = OutTexture;
				
				Image_OutImage->SetBrushFromTexture(TargetTexture2D);
				Image_OutImage->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
			}
		}
	}
}

void UNBMainUI::ResetResult()
{
	GetWorld()->GetTimerManager().ClearTimer(ResetTimerHandle);
	
	TempStrikeCount = 0;
	TempBallCount = 0;
	bIsOutTemp = false;
	
	if (IsValid(TextBlock_InputResult) == true)
	{
		TextBlock_InputResult->SetText(FText::FromString(TEXT("")));
	}
	if (IsValid(TextBlock_Strike) == true)
	{
		TextBlock_Strike->SetText(FText::FromString(TEXT("")));
	}
	
	if (IsValid(TextBlock_Ball) == true)
	{
		TextBlock_Ball->SetText(FText::FromString(TEXT("")));
	}
	
	if (IsValid(TextBlock_Out) == true)
	{
		TextBlock_Out ->SetText(FText::FromString(TEXT("")));
	}
	
	TObjectPtr<UTexture2D> TargetTexture2D = BlankTexture;
	if (IsValid(HorizontalBox_StrikeImage) == true)
	{
		for (int32 i = 0; i < StrikeImages.Num(); i++)
		{
			if (IsValid(StrikeImages[i]) == true)
			{
				StrikeImages[i]->SetBrushFromTexture(TargetTexture2D);
				StrikeImages[i]->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
			}
		}
	}
	
	if (IsValid(HorizontalBox_BallImage) == true)
	{
		for (int32 i = 0; i < BallImages.Num(); i++)
		{
			if (IsValid(BallImages[i]) == true)
			{
				BallImages[i]->SetBrushFromTexture(TargetTexture2D);
				BallImages[i]->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
			}
		}
	}
	
	if (IsValid(Image_OutImage) == true)
	{
		Image_OutImage->SetBrushFromTexture(TargetTexture2D);
		Image_OutImage->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
	}
}

void UNBMainUI::RefreshPlayerList(const TArray<FString>& InNickNames, const TArray<int32>& InGuessCounts,
	const TArray<int32>& InMaxCounts)
{
	if (IsValid(ScrollBox_PlayerList) == true)
	{
		if (IsValid(PlayerCardWidgetClass) == true)
		{
			ScrollBox_PlayerList->ClearChildren();
    
			// 순서대로 카드 생성
			for (int32 i = 0; i < InNickNames.Num(); i++)
			{
				UNBPlayerCardWidget* Card = CreateWidget<UNBPlayerCardWidget>(
					this, PlayerCardWidgetClass);
				if (IsValid(Card) == false) continue;
        
				Card->InitializePlayerCard(
					InNickNames[i],
					InGuessCounts[i],
					InMaxCounts[i]);
				
				ScrollBox_PlayerList->AddChild(Card);
			}
		}
	}
}

void UNBMainUI::ResetAllUIs()
{
	ScrollBox_ChatLog->ClearChildren();
	ScrollBox_PlayLog->ClearChildren();
	ResetResult();
}
