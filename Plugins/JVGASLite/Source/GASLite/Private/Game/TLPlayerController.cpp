// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TLPlayerController.h"
#include <Blueprint/UserWidget.h>

void ATLPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		//bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		SetPause(false);
		return;
	}
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuClass)
	{
		PauseMenuInstance->AddToViewport(100);

		//bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		SetPause(true);
	}
}

void ATLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this,&ATLPlayerController::TogglePauseMenu);
}

void ATLPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ATLPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}


void ATLPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}