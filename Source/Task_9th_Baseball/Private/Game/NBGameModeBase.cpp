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
	StartPlayerTurnTimer();	
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
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	FString PlayerInfoString = SetPlayerInfoString(InChattingPlayerController);
	
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		IncreaseGuessCount(InChattingPlayerController);
		PlayerInfoString = SetPlayerInfoString(InChattingPlayerController);
		
		FString JudgeString = JudgeResult(AnswerNumberString, GuessNumberString);
		
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
				FString CombineMessageString = PlayerInfoString + TEXT(": ") + InChatMessageString;
				NBPlayerController->ClientRPCPrintChatMessageString(CombineMessageString);
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

void ANBGameModeBase::StartPlayerTurnTimer()
{
	ANBGameStateBase* NBGameStateBase = GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameStateBase) == true)
	{
		NBGameStateBase->RemainingTurnTime = PlayerTurnTime;
		NBGameStateBase->StartUITimer();
	}
	
	GetWorldTimerManager().SetTimer(
		PlayerTurnTimerHandle,
		this,
		&ANBGameModeBase::ChangePlayerTurn,
		PlayerTurnTime,
		false
	);
}

void ANBGameModeBase::ChangePlayerTurn()
{
}
