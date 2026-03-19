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
	
}

void ANBGameModeBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ANBGameModeBase::StartGamePlay()
{
	if (CurrentGameState == FGameplayTag::RequestGameplayTag(FName("Game.State.InGame"))) return;
	
	CurrentGameState = FGameplayTag::RequestGameplayTag(FName("Game.State.InGame"));
	for (const auto PlayerController : AllPlayerControllers)
	{
		if (IsValid(PlayerController) == true)
		{
			PlayerController->ClientRPCResetUI();
		}
	}
	ResetGame();
	SetPlayerToPlay(PlayerIndex);
}

void ANBGameModeBase::EndGamePlay()
{
	//타이머 초기화
	GetWorldTimerManager().ClearTimer(PlayerTurnTimerHandle);
	ANBGameStateBase* NBGameStateBase = GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameStateBase) == true)
	{
		NBGameStateBase->ResetUITimer();
	}
	
	CurrentGameState = FGameplayTag::RequestGameplayTag(FName("Game.State.Wait"));
}

#pragma region GamePlayLogic

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
		if (UniqueDigits.Num() < 3)
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

FResult ANBGameModeBase::MakeJudgeResult(const FString& InAnswerNumberString, const FString& InGuessNumberString)
{
	FResult NewResult;
	int32 StrikeCount = 0, BallCount = 0;
	bool bIsOut = false;
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
		bIsOut = true;
	}
	NewResult.BallCount = BallCount;
	NewResult.StrikeCount = StrikeCount;
	NewResult.bIsOut = bIsOut;
	NewResult.InputAnswerString = InGuessNumberString;
	
	return NewResult;
}

void ANBGameModeBase::IncreaseGuessCount(ANBPlayerController* InChattingPlayerController)
{
	ANBPlayerState* NBPlayerState = InChattingPlayerController->GetPlayerState<ANBPlayerState>();
	if (IsValid(NBPlayerState) == true)
	{
		NBPlayerState->CurrentGuessCount++;
	}
	
	BroadCastPlayerList();
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
	
	bool bShouldJudgeInChat = bShouldJudgeChat(InChattingPlayerController,InChatMessageString);
	
	if (bShouldJudgeInChat == true)
	{
		IncreaseGuessCount(InChattingPlayerController);
		FResult JudgeResult = MakeJudgeResult(AnswerNumberString, InChatMessageString);
		int32 StrikeCount = JudgeResult.StrikeCount;
		
		for (TActorIterator<ANBPlayerController> It(GetWorld()); It; ++It)
		{
			ANBPlayerController* NBPlayerController = *It;
			if (IsValid(NBPlayerController) == true)
			{
				NBPlayerController->ClientRPCShowResult(JudgeResult);
			}
		}
		JudgeGame(InChattingPlayerController, StrikeCount);
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
	BroadCastPlayerList();
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
				
				EndGamePlay();
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
					ChangePlayerTurn();
					break;
				}
			}
		}
		
		if (bIsDraw == true)
		{
			for (const auto& NBPlayerController : AllPlayerControllers)
			{
				NBPlayerController->ClientRPCPrintResultString(TEXT("Draw..."));
				
				EndGamePlay();
			}
		}
	}
}

bool ANBGameModeBase::bShouldJudgeChat(ANBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	bool bShouldJudgeChat = false;
	ANBPlayerState* ChatPlayerState = InChattingPlayerController->GetPlayerState<ANBPlayerState>();
	FGameplayTag ChatPlayerCurrentState = ChatPlayerState->CurrentPlayerState;
	
	do
	{
		if ( ChatPlayerCurrentState != FGameplayTag::RequestGameplayTag(FName("Player.State.MyTurn")))
		{
			break;
		}
		
		if (IsGuessNumberString(InChatMessageString) == false)
		{
			break;
		}
		
		if (CurrentGameState != FGameplayTag::RequestGameplayTag(FName("Game.State.InGame")))
		{
			break;
		}
		
		if ( ChatPlayerState->CurrentGuessCount >= ChatPlayerState->MaxGuessCount)
		{
			break;
		}
		
		bShouldJudgeChat = true;
	}
	while (false);

	return bShouldJudgeChat;
}
#pragma endregion 

#pragma region TurnPlayLogic

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
		&ANBGameModeBase::ChangePlayerTurnByTimer,
		PlayerTurnTime,
		false
	);
}

void ANBGameModeBase::ChangePlayerTurn()
{
	//타이머 초기화
	GetWorldTimerManager().ClearTimer(PlayerTurnTimerHandle);
	ANBGameStateBase* NBGameStateBase = GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameStateBase) == true)
	{
		NBGameStateBase->ResetUITimer();
	}
	
	PlayerIndex++;
	if (PlayerIndex >= AllPlayerControllers.Num())
	{
		PlayerIndex = 0;
	}
	
	SetPlayerToPlay(PlayerIndex);
}

void ANBGameModeBase::ChangePlayerTurnByTimer()
{
	IncreaseGuessCount(AllPlayerControllers[PlayerIndex]);
	ChangePlayerTurn();
}

void ANBGameModeBase::SetPlayerToPlay(int32 InPlayerIndex)
{
	if (IsValid(AllPlayerControllers[InPlayerIndex]) == false)
	{
		ChangePlayerTurn();
		return;
	}
	
	ANBPlayerState* NBCurrentPlayerState = AllPlayerControllers[InPlayerIndex]->GetPlayerState<ANBPlayerState>();
	FString CurrentPlayerNameString;
	if (IsValid(NBCurrentPlayerState) == true)
	{
		if (NBCurrentPlayerState->CurrentGuessCount >= NBCurrentPlayerState->MaxGuessCount)
		{
			ChangePlayerTurn();
			return;
		}
		CurrentPlayerNameString = NBCurrentPlayerState->PlayerNameString;
	}
	
	for (const auto& NBPlayerController : AllPlayerControllers)
	{
		if (IsValid(NBPlayerController) == true)
		{
			ANBPlayerState* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
			if (IsValid(NBPlayerState) == true)
			{
				if (NBPlayerController == AllPlayerControllers[InPlayerIndex])
				{
					NBPlayerState->CurrentPlayerState = (FGameplayTag::RequestGameplayTag(FName("Player.State.MyTurn")));
					NBPlayerController->ClientRPCPrintResultString(TEXT("My Turn"));
				}
				else
				{
					NBPlayerState->CurrentPlayerState = (FGameplayTag::RequestGameplayTag(FName("Player.State.NotMyTurn")));
					FString CombineMessageString = CurrentPlayerNameString + TEXT("'s Turn");
					NBPlayerController->ClientRPCPrintResultString(CombineMessageString);
				}
			}
		}
	}
	StartPlayerTurnTimer();
}


#pragma endregion
	
#pragma region LogIn logic

void ANBGameModeBase::BroadCastPlayerList()
{
	TArray<FString> NickNames;
	TArray<int32> GuessCounts;
	TArray<int32> MaxCounts;
    
	for (const auto& PlayerController : AllPlayerControllers)
	{
		if (IsValid(PlayerController) == false) continue;
        
		ANBPlayerState* NBPlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
		if (IsValid(NBPlayerState) == false) continue;
		if (NBPlayerState->PlayerNameString == TEXT("None")) continue;
        
		NickNames.Add(NBPlayerState->PlayerNameString);
		GuessCounts.Add(NBPlayerState->CurrentGuessCount);
		MaxCounts.Add(NBPlayerState->MaxGuessCount);
	}
    
	for (const auto& PlayerController : AllPlayerControllers)
	{
		if (IsValid(PlayerController) == true)
		{
			PlayerController->ClientRPCRefreshPlayerList(NickNames, GuessCounts, MaxCounts);
		}
	}
}

bool ANBGameModeBase::IsValidNickName(const FString& InNickName)
{
	bool bIsValidNickname = false;
	do
	{
		if (InNickName.IsEmpty() == true)
		{
			break;
		}
		
		if (InNickName.Contains(TEXT(" ")))
		{
			break;
		}
				
		bIsValidNickname = true;
	}while (false);
	
	return bIsValidNickname;
}

void ANBGameModeBase::SetPlayerNickName(ANBPlayerController* LoginPlayerController, const FString& InPlayerNickName)
{
	ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(LoginPlayerController);
	if (IsValid(NBPlayerController) == true)
	{
		AllPlayerControllers.Add(NBPlayerController);
	}
	
	ANBPlayerState* NBPlayerState = LoginPlayerController->GetPlayerState<ANBPlayerState>();
	if (IsValid(NBPlayerState) == true)
	{
		if (IsValidNickName(InPlayerNickName) == true)
		{
			NBPlayerState->PlayerNameString = InPlayerNickName;
		}
		else
		{
			int32 LoginPlayerIndex = AllPlayerControllers.Find(LoginPlayerController);
			if (LoginPlayerIndex != INDEX_NONE)
			{
				NBPlayerState->PlayerNameString = TEXT("Player ") + FString::FromInt(LoginPlayerIndex);
			}
		}
	}

	LoginPlayerController->ClientRPCLogInGame();
	
	BroadCastPlayerList();
}
	
#pragma endregion