// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameModeBase.h"

#include "EngineUtils.h"
#include "Algo/RandomShuffle.h"
#include "Game/NBGameStateBase.h"
#include "Player/NBPlayerController.h"
#include "Player/NBPlayerState.h"

void ANBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (IsValid(NBPlayerController) == true)
	{
		AllPlayerControllers.Add(NBPlayerController);
		
		ANBPlayerState * NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
		if (IsValid(NBPlayerState) == true)
		{
			NBPlayerState->PlayerNameString = TEXT("Player ") + FString::FromInt(AllPlayerControllers.Num());
		}
		ANBGameStateBase* NBGameStateBase = GetGameState<ANBGameStateBase>();
		
		if (IsValid(NBGameStateBase) == true)
		{
			NBGameStateBase->MulticastRPCBroadcastLoginMessage(NBPlayerState->PlayerNameString);
		}
		
		
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
	UE_LOG(LogNet, Warning, TEXT("%s"), *Result);
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
		
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				break;
			}
			
			UniqueDigits.Add(C);
		}
		
		if (UniqueDigits.Num() != 3)
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

void ANBGameModeBase::IncreaseGuessCount(ANBPlayerController* InChattingPlayerController)
{
	ANBPlayerState* NBPlayerState = InChattingPlayerController->GetPlayerState<ANBPlayerState>();
	if (IsValid(NBPlayerState) == true)
	{
		NBPlayerState->CurrentGuessCount++;
	}
}

FString ANBGameModeBase::SetPlayerInfoString(ANBPlayerController* InChattingPlayerController)
{
	FString PlayerInfoString = TEXT("Null");
	
	ANBPlayerState* NBPlayerState = InChattingPlayerController->GetPlayerState<ANBPlayerState>();
	if (IsValid(NBPlayerState) == true)
	{
		PlayerInfoString = NBPlayerState->GetPlayerInfoString();
	}
	
	return PlayerInfoString;
}

void ANBGameModeBase::PrintChatMessageString(ANBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	
	FString PlayerInfoString = SetPlayerInfoString(InChattingPlayerController);
	bool bIsHaveChance = IsHaveChance(InChattingPlayerController);
	
	if (IsGuessNumberString(InChatMessageString) == true && bIsHaveChance == true)
	{
		IncreaseGuessCount(InChattingPlayerController);
		PlayerInfoString = SetPlayerInfoString(InChattingPlayerController);
		
		FString JudgeString = JudgeResult(AnswerNumberString, InChatMessageString);
		
		int32 StrikeCount = FCString::Atoi(*JudgeString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
		
		for (TActorIterator<ANBPlayerController> It(GetWorld()); It; ++It)
		{
			ANBPlayerController* NBPlayerController = *It;
			if (IsValid(NBPlayerController) == true)
			{
				FString CombineMessageString = PlayerInfoString + TEXT(": ") + InChatMessageString + TEXT("->") + JudgeString;
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
				NotifyString = SetNotifyString(InChatMessageString, bIsHaveChance);
				FString CombineMessageString = PlayerInfoString + TEXT(": ") + InChatMessageString + NotifyString;
				NBPlayerController->ClientRPCPrintChatMessageString(CombineMessageString);
				NotifyString = TEXT("");
			}
		}
	}
}

void ANBGameModeBase::ResetGame()
{
	AnswerNumberString = GenerateAnswerNumber();
	
	for (const auto& NBPlayerController : AllPlayerControllers)
	{
		ANBPlayerState* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
		if (IsValid(NBPlayerState) == true)
		{
			NBPlayerState->CurrentGuessCount = 0;
		}
	}
}

void ANBGameModeBase::JudgeGame(ANBPlayerController* InChattingPlayerController, int32 InStrikeCount)
{
	if (InStrikeCount == 3)
	{
		ANBPlayerState* NBPlayerState = InChattingPlayerController->GetPlayerState<ANBPlayerState>();
		for (const auto& NBPlayerController : AllPlayerControllers)
		{
			if (IsValid(NBPlayerController) == true)
			{
				FString CombineMessageString = NBPlayerState->PlayerNameString + TEXT(" has won the game!");
				NBPlayerController->ClientRPCPrintResultString(CombineMessageString);
				
				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& NBPlayerController : AllPlayerControllers)
		{
			ANBPlayerState* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
			if (IsValid(NBPlayerState) == true)
			{
				if (NBPlayerState->CurrentGuessCount < NBPlayerState->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}
		
		if (bIsDraw == true)
		{
			for (const auto& NBPlayerController : AllPlayerControllers)
			{
				NBPlayerController->ClientRPCPrintResultString(TEXT("Draw..."));
				
				ResetGame();
			}
		}
	}
}

FString ANBGameModeBase::SetNotifyString(const FString& InChatMessageString, bool bIsHaveChance)
{
	FString NewNotifyString = TEXT("");
	TSet<TCHAR> UniqueDigits;
	bool bIsLatter = false;
	
	//정답 시도 아닌것 거르기
	if (InChatMessageString.Len() > 3)
	{
		return NewNotifyString;
	}
	
	//문자 포함 채팅 거르기
	for (TCHAR C : InChatMessageString)
	{
		if (FChar::IsDigit(C) == false)
		{
			bIsLatter = true;
			break;
		}
		
		UniqueDigits.Add(C);
	}
	
	if (bIsLatter == true)
	{
		return NewNotifyString;
	}
	
	if (bIsHaveChance == true)
	{
		//0 포함 거르기
		if (UniqueDigits.Contains('0'))
		{
			NewNotifyString = TEXT(" (Please enter a number 1 ~ 9.)");
			return NewNotifyString;
		}
	
		//숫자 1~2개 입력 거르기
		if (InChatMessageString.Len() < 3 )
		{
			NewNotifyString = TEXT(" (Please enter a 3-digit number.)");
			return NewNotifyString;
		}
	
		//중복 숫자 입력 거르기
		if (UniqueDigits.Num() < 3)
		{
			NewNotifyString = TEXT(" (Please enter a unique number.)");
			return NewNotifyString;
		}
	}
	else
	{
		NewNotifyString = TEXT(" (No opportunity to try.)");
		return NewNotifyString;
	}
	
	return NewNotifyString;
}

bool ANBGameModeBase::IsHaveChance(ANBPlayerController* InChattingPlayerController)
{
	bool bIsHaveChance = true;
	ANBPlayerState* NBPlayerState = InChattingPlayerController->GetPlayerState<ANBPlayerState>();
	if (IsValid(NBPlayerState) == true)
	{
		if (NBPlayerState->CurrentGuessCount >= NBPlayerState->MaxGuessCount)
		{
			bIsHaveChance = false;
		}
	}
	
	return bIsHaveChance;
}
