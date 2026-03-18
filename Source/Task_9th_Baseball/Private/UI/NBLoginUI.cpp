// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NBLoginUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Player/NBLoginPlayerController.h"
#include "Player/NBPlayerController.h"

void UNBLoginUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Button_LoginButton->OnClicked.IsAlreadyBound(this, &UNBLoginUI::OnLoginButtonClicked) == false)
	{
		Button_LoginButton->OnClicked.AddDynamic(this, &UNBLoginUI::OnLoginButtonClicked);
	}
	
	if (Button_ExitButton->OnClicked.IsAlreadyBound(this, &UNBLoginUI::OnExitButtonClicked) == false)
	{
		Button_ExitButton->OnClicked.AddDynamic(this, &UNBLoginUI::OnExitButtonClicked);
	}
}

void UNBLoginUI::NativeDestruct()
{ 
	Super::NativeDestruct();
}

void UNBLoginUI::OnLoginButtonClicked()
{
	if (IsValid(EditableText_NickName) == true)
	{
		FString InputNickNameString = EditableText_NickName->GetText().ToString();
		FString PlayerNickNameString = InputNickNameString.Left(NickNameLengthLimit);
		
		TObjectPtr<APlayerController> OwningPlayer = GetOwningPlayer();
		if (IsValid(OwningPlayer) == true)
		{
			TObjectPtr<ANBPlayerController> LoginPlayerController = Cast<ANBPlayerController>(OwningPlayer);
			if (IsValid(LoginPlayerController) == true)
			{
				LoginPlayerController->ServerRPCLogInGame(PlayerNickNameString);
			}
		}
	}
}

void UNBLoginUI::OnExitButtonClicked()
{
	TObjectPtr<APlayerController> OwningPlayer = GetOwningPlayer();
	if (IsValid(OwningPlayer) == true)
	{
		TObjectPtr<ANBPlayerController> LoginPlayerController = Cast<ANBPlayerController>(OwningPlayer);
		if (IsValid(LoginPlayerController) == true)
		{
			LoginPlayerController->ExitGame();
		}
	}
}
