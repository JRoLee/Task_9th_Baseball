// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameModeBase.h"

#include "EngineUtils.h"
#include "Algo/RandomShuffle.h"
#include "Game/NBGameStateBase.h"
#include "Player/NBPlayerController.h"

void ANBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ANBGameStateBase* NBGameStateBase = GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameStateBase) == true)
	{
		NBGameStateBase->MulticastRPCBroadcastLoginMessage();
	}
	
	ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (IsValid(NBPlayerController) == true)
	{
		AllPlayerControllers.Add(NBPlayerController);
	}
}

void ANBGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	AnswerNumberString = GenerateAnswerNumber();
}

FString ANBGameModeBase::GenerateAnswerNumber()
{
	TArray<int32> Numbers;
	for (int32 i=1; i<= 9; i++)
	{
		Numbers.Add(i);
	}
	
	Algo::RandomShuffle(Numbers);
	
	FString Result;
	for (int32 i = 0; i < 3; i++)
	{
		Result.Append(FString::FromInt(Numbers[i]));
	}
	return Result;	
}

bool ANBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;
	
	do
	{
		if (InNumberString.Len() != 3)
		{
			break;
		}
		
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}
		
		if (bIsUnique == false)
		{
			break;
		}
		
		bCanPlay = true;
		
	} while (false);
	
	return bCanPlay;
}

FString ANBGameModeBase::JudgeResult(const FString& InAnswerNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;
	
	for (int32 i = 0; i < 3; i++)
	{
		if (InAnswerNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InAnswerNumberString.Contains(PlayerGuessChar) == true)
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	
	return FString::Printf(TEXT("%dS %dB"),StrikeCount,BallCount);
}

void ANBGameModeBase::PrintChatMessageString(ANBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeString = JudgeResult(AnswerNumberString, GuessNumberString);
		for (TActorIterator<ANBPlayerController> It(GetWorld()); It; ++It)
		{
			ANBPlayerController* NBPlayerController = *It;
			if (IsValid(NBPlayerController) == true)
			{
				FString CombineMessageString = InChatMessageString + TEXT("->") + JudgeString;
				NBPlayerController->ClientRPCPrintChatMessageString(CombineMessageString);
			}
		}
	}
	else
	{
		for (TActorIterator<ANBPlayerController> It(GetWorld()); It; ++It)
		{
			ANBPlayerController* NBPlayerController = *It;
			if (IsValid(NBPlayerController) == true)
			{
				NBPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}
