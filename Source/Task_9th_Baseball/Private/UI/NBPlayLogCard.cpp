// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBPlayLogCard.h"

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Player/NBPlayerController.h"

void UNBPlayLogCard::SetPlayLogCard(FResult const& InResult)
{
	if (IsValid(TextBlock_PlayLog) == true)
	{
		TextBlock_PlayLog -> SetText(FText::FromString(InResult.InputAnswerString));
	}
	
	if (InResult.bIsOut == true)
	{
		if (IsValid(Overlay_Strike) == true)
		{
			Overlay_Strike->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (IsValid(Overlay_Ball) == true)
		{
			Overlay_Ball->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (IsValid(Overlay_Out) == true)
		{
			Overlay_Out->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (IsValid(Overlay_Out) == true)
		{
			Overlay_Out->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		if (IsValid(Overlay_Strike) == true)
		{
			if (IsValid(TextBlock_Strike) == true)
			{
				TextBlock_Strike->SetText(FText::FromString(FString::FromInt(InResult.StrikeCount)+TEXT(" S")));
			}
			Overlay_Strike->SetVisibility(ESlateVisibility::Visible);
		}
		
		if (IsValid(Overlay_Ball) == true)
		{
			if (IsValid(TextBlock_Ball) == true)
			{
				TextBlock_Ball->SetText(FText::FromString(FString::FromInt(InResult.BallCount)+TEXT(" B")));
			}
			Overlay_Ball->SetVisibility(ESlateVisibility::Visible);
		}		
	}
}
