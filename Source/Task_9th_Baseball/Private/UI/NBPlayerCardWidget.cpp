// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBPlayerCardWidget.h"

#include "Components/TextBlock.h"

void UNBPlayerCardWidget::InitializePlayerCard(const FString& NickName, int32 CurrentGuessCount, int32 MaxGuessCount)
{
	if (IsValid(TextBlock_NickName) == true)
	{
		TextBlock_NickName->SetText(FText::FromString(NickName));
	}
	if (IsValid(TextBlock_GuessCount) == true)
	{
		FString CombinedString = TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
		TextBlock_GuessCount->SetText(FText::FromString(CombinedString));
	}
}