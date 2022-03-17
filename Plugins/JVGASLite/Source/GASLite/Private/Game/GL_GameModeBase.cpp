// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GL_GameModeBase.h"
#include "../TLActionRoguelike.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Game/GL_GameStateBase.h"
#include "Game/GL_PlayerStateBase.h"
#include "RLCharacter.h"
#include "AI/TLAICharacter.h"
#include "Actor/GL_Turret.h"
#include "Actor/GL_Core.h"
#include "Component/TLAttributeComponent.h"

AGL_GameModeBase::AGL_GameModeBase()
{

	GameDesc = FGameDesc(TEXT("TestMap"), 10, EGameModeType::Debug);

	PreMatchTimer = 600;

	WaitingTimer = 30;

	MatchTimer = 0;

	EndMatchTimer = 60;

	bMatchTimerUp = false;
}

void AGL_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	//throw std::logic_error("The method or operation is not implemented.");
}

void AGL_GameModeBase::StartPlay()
{
	Super::StartPlay();
	//throw std::logic_error("The method or operation is not implemented.");

	StartPreMatchTimer();

	
}

void AGL_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//throw std::logic_error("The method or operation is not implemented.");

	UGL_GameInstance* GI = Cast<UGL_GameInstance>(GetGameInstance());
	if (GI)
	{
		if (GetBaseGameState()->PlayerArray.Num() <= GI->GetMenuGameDesc().MaxPlayer)
		{
			for (APlayerState* Player : GetBaseGameState()->PlayerArray)
			{
				AGL_PlayerStateBase* PS = Cast<AGL_PlayerStateBase>(Player);
				if (PS)
				{
					PS->ServerPlayerLogin(PS);

					// Need to get Infomation to Server of the players username
					//PS->ServerInitPlayerProperties(nullptr);
				}
			}
		}
		else 
		{
			//DestroySession()
		}
		
	}
}

void AGL_GameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	//throw std::logic_error("The method or operation is not implemented.");
	 AGL_PlayerStateBase* ExitingPS = Cast<AGL_PlayerStateBase>(Exiting->PlayerState);

	 for (APlayerState* Player : GetBaseGameState()->PlayerArray)
	 {
		 AGL_PlayerStateBase* PS = Cast<AGL_PlayerStateBase>(Player);
		 if (PS)
		 {
			 PS->ServerPlayerLogOut(ExitingPS);
		 }
	 }
}

void AGL_GameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UTLAttributeComponent* VictimActorAttr = UTLAttributeComponent::GetAttribute(VictimActor);

	// if VictimActor is a Core
	AGL_Core*  VictimCore = Cast<AGL_Core>(VictimActor);
	if (VictimCore)
	{
		return;
	}
	
	// if VictimActor is a Turret
	AGL_Turret* VictimTurret = Cast<AGL_Turret>(VictimActor);
	if (VictimTurret)
	{
		return;
	}
	
	// if VictimActor is a minion
	ATLAICharacter* VictimMinion = Cast<ATLAICharacter>(VictimActor);
	if (VictimMinion)
	{
		ARLCharacter* KillerPlayer = Cast<ARLCharacter>(Killer);

		if (KillerPlayer)
		{
			AGL_PlayerStateBase* KillerPS = Cast<AGL_PlayerStateBase>(KillerPlayer->GetPlayerState());

			HandlePlayerScore(KillerPS, 100);
		}

		return;
	}
	
	// if VictimActor is a Player
	ARLCharacter* VictimPlayer = Cast<ARLCharacter>(VictimActor);
	
	if (VictimPlayer)
	{
		AGL_PlayerStateBase*  VictimPS = Cast<AGL_PlayerStateBase>(VictimPlayer->GetPlayerState());

		ARLCharacter* KillerPlayer = Cast<ARLCharacter>(Killer);
		if (KillerPlayer)
		{
			AGL_PlayerStateBase* KillerPS = Cast<AGL_PlayerStateBase>(KillerPlayer->GetPlayerState());

			HandlePlayerDeath(VictimPS, KillerPS);
			return;
		}

		AGL_Turret* KillerTurret = Cast<AGL_Turret>(Killer);
		if (KillerTurret)
		{
			//VictimPS->ServerPlayerKilled();
			HandlePlayerDeathByUnit(VictimPS,KillerTurret);
			return;
		}

		ATLAICharacter* KillerMinion = Cast<ATLAICharacter>(Killer);
		if (KillerMinion)
		{
			//VictimPS->ServerPlayerKilled();
			HandlePlayerDeathByUnit(VictimPS, KillerMinion);
			return;
		}

		return;
	}

}

AGL_GameStateBase* AGL_GameModeBase::GetBaseGameState()
{
	return Cast<AGL_GameStateBase>(GameState);
}

APlayerController* AGL_GameModeBase::GetPlayerControllerfromPlayerState(AGL_PlayerStateBase* MPPlayerState)
{
	return Cast<APlayerController>(MPPlayerState->GetOwner());
}

void AGL_GameModeBase::HandlePlayerDeath(AGL_PlayerStateBase* VictimPlayerState, AGL_PlayerStateBase* CauserPlayerState)
{
	VictimPlayerState->MulticastUpdateDeathCount(1);
	CauserPlayerState->MulticastUpdateKillCount(1);

	VictimPlayerState->ServerPlayerKilled();
}

void AGL_GameModeBase::HandlePlayerScore(AGL_PlayerStateBase* PlayerState, int32 addedvalue)
{
	GetBaseGameState()->ServerUpdateTeamMatchScore(0, addedvalue);
}

void AGL_GameModeBase::HandlePlayerDeathByUnit(AGL_PlayerStateBase* VictimPlayerState, AActor* CauserUnit)
{
	VictimPlayerState->MulticastUpdateDeathCount(1);

	VictimPlayerState->ServerPlayerKilled();
}

void AGL_GameModeBase::StartPreMatchTimer_Implementation()
{
	//PreMatch Does Skimirish While Waiting for More Players

	FTimerDelegate PreMatchDelegate;
	PreMatchTimerValue = PreMatchTimer;

	PreMatchDelegate.BindUFunction(this, FName("PreMatchTimer_Elapsed"));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_PreMatchTimer, PreMatchDelegate, 1.0f, true);

	GetBaseGameState()->ServerUpdateMGSMatchState(EMatchState::PreMatchStarted);

	GetBaseGameState()->ServerUpdateTimer(PreMatchTimerValue);

	for (APlayerState* Player : GetBaseGameState()->PlayerArray)
	{
		if (AGL_PlayerStateBase* PS = Cast<AGL_PlayerStateBase>(Player))
		{
			PS->ServePreMatchInitialize();
		}
	}

}

void AGL_GameModeBase::PreMatchTimer_Elapsed()
{
	--PreMatchTimerValue;

	FColor TextColor = FColor::Emerald;
	FString LoginMsg = FString::Printf(TEXT("[%s] PreMatch Timer: %d"),
		*GetNameSafe(this),
		PreMatchTimerValue);
	LogOnScreen(this, LoginMsg, TextColor, 1.0f);

	GetBaseGameState()->ServerUpdateTimer(PreMatchTimerValue);


	if (PreMatchTimerValue <= 0)
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle_PreMatchTimer);

		StartWaitingTimer();
	}
}

void AGL_GameModeBase::StartWaitingTimer_Implementation()
{
	//Waiting Stops PreMatches Clears Any Player Actors in the Map and Set their View To Camera
	// This Sets up the player to choose their Character and waits Till the Timer is down

	WaitingTimerValue = WaitingTimer;
	FTimerDelegate WaitTimerDelegate;

	WaitTimerDelegate.BindUFunction(this, FName("WaitingTimer_Elapsed"));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitingTimer, WaitTimerDelegate, 1.0f, true);

	GetBaseGameState()->ServerUpdateMGSMatchState(EMatchState::MatchInitialize);

	GetBaseGameState()->ServerUpdateTimer(WaitingTimerValue);

	for (APlayerState* Player : GetBaseGameState()->PlayerArray)
	{
		if (AGL_PlayerStateBase* PS = Cast<AGL_PlayerStateBase>(Player))
		{
			PS->ServerWaitingStart();
		}
	}

}

void AGL_GameModeBase::WaitingTimer_Elapsed()
{
	--WaitingTimerValue;

	FColor TextColor = FColor::Blue;
	FString LoginMsg = FString::Printf(TEXT("[%s] Waiting Timer: %d"),
		*GetNameSafe(this),
		WaitingTimerValue);
	LogOnScreen(this, LoginMsg, TextColor, 1.0f);

	GetBaseGameState()->ServerUpdateTimer(WaitingTimerValue);

	if (WaitingTimerValue <= 0)
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle_WaitingTimer);

		StartGameMatch();
	}
}

void AGL_GameModeBase::BeginMatchTimer()
{
	MatchTimerValue = MatchTimer;

	if (MatchTimerValue <= 0)
	{
		bMatchTimerUp = true;
	}

	FTimerDelegate StartMatchDelegate;

	StartMatchDelegate.BindUFunction(this, FName("MatchTimer_Elapsed"));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_MatchTimer, StartMatchDelegate, 1.0f, true);

	GetBaseGameState()->ServerUpdateMGSMatchState(EMatchState::MatchStarted);

	GetBaseGameState()->ServerUpdateTimer(MatchTimerValue);

	for (APlayerState* Player : GetBaseGameState()->PlayerArray)
	{
		if (AGL_PlayerStateBase* PS = Cast<AGL_PlayerStateBase>(Player))
		{
			PS->ServerMatchStart();
		}
	}

	
}

void AGL_GameModeBase::MatchTimer_Elapsed()
{
	if (bMatchTimerUp)
	{
		++MatchTimerValue;
	}
	else
	{
		--MatchTimerValue;
	}

	FColor TextColor = FColor::White;
	FString LoginMsg = FString::Printf(TEXT("[%s] Match Timer: %d"),
		*GetNameSafe(this),
		MatchTimerValue);
	LogOnScreen(this, LoginMsg, TextColor, 1.0f);

	GetBaseGameState()->ServerUpdateTimer(MatchTimerValue);

	if (MatchTimerValue <= 0)
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle_MatchTimer);

		//CheckWinConditions(EWinConditons::ByTimeLimit);
		MatchEnd(EWinner::Draw);
	}
}

void AGL_GameModeBase::StartWaitEndGameTimer_Implementation()
{
	EndMatchTimerValue = EndMatchTimer;

	FTimerDelegate WaitEndDelegate;

	WaitEndDelegate.BindUFunction(this, FName("EndGameTimer_Elapsed"));

	GetBaseGameState()->ServerUpdateTimer(EndMatchTimerValue);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EndMatchTimer, WaitEndDelegate, 1.0f, true);

}

void AGL_GameModeBase::PauseMatchTimer()
{

}

void AGL_GameModeBase::SendPlayerToLobbyMenu()
{

	//When Match Timer Ends Send Player back to Lobby Menu and Clear Server

}

void AGL_GameModeBase::EndGameTimer_Elapsed()
{
	--EndMatchTimerValue;

	FColor TextColor = FColor::White;
	FString LoginMsg = FString::Printf(TEXT("[%s] End Match Timer: %d"),
		*GetNameSafe(this),
		EndMatchTimerValue);
	LogOnScreen(this, LoginMsg, TextColor, 1.0f);

	GetBaseGameState()->ServerUpdateTimer(EndMatchTimerValue);

	if (EndMatchTimerValue <= 0)
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle_EndMatchTimer);

		SendPlayerToLobbyMenu();
	}
}

void AGL_GameModeBase::StartGameMatch_Implementation()
{
	FColor TextColor = FColor::Red;
	FString LoginMsg = FString::Printf(TEXT("[%s] Start Game Match Fires Off"),
		*GetNameSafe(this),
		WaitingTimerValue);
	LogOnScreen(this, LoginMsg, TextColor, 10.0f);

	BeginMatchTimer();
}

void AGL_GameModeBase::MatchEnd_Implementation(EWinner Winner)
{
	GetBaseGameState()->ServerUpdateWinner(Winner);
	PauseMatchTimer();

	GetWorld()->GetTimerManager().PauseTimer(TimerHandle_MatchTimer);

	StartWaitEndGameTimer();

	GetBaseGameState()->ServerUpdateMGSMatchState(EMatchState::WaitingEndGame);

	for (APlayerState* Player : GetBaseGameState()->PlayerArray)
	{
		if (AGL_PlayerStateBase* PS = Cast<AGL_PlayerStateBase>(Player))
		{
			PS->ServerWaitEndMatch();
		}
	}

}

void AGL_GameModeBase::UpdateTeamMatchScore_Implementation(uint8 TeamID, int32 AddedScore)
{
	switch (TeamID)
	{
	case 0:
		if (GetBaseGameState()->GetUseDecreasingMethod())
		{
			GetBaseGameState()->ServerUpdateTeamMatchScore(1,AddedScore);
		}
		else
		{
			GetBaseGameState()->ServerUpdateTeamMatchScore(0, AddedScore);
		}
		break;
	case 1:
		if (GetBaseGameState()->GetUseDecreasingMethod())
		{
			GetBaseGameState()->ServerUpdateTeamMatchScore(0, AddedScore);
		}
		else
		{
			GetBaseGameState()->ServerUpdateTeamMatchScore(1, AddedScore);
		}
		break;
	default:
		break;
	}

	CheckWinConditions(EWinConditons::ByMatchScore);
}

void AGL_GameModeBase::CheckWinConditions_Implementation(EWinConditons WinCondiitons)
{
	switch (WinCondiitons)
	{
	case EWinConditons::ByCoreDestroyed :
		// If Opposion Core is Destroyed, Match End and the Winner shown


		break;
	case EWinConditons::ByMatchScore :
		if (GetBaseGameState()->GetUseDecreasingMethod()) {
			if (GetBaseGameState()->CurrentScoreTeam1 == 0) {

			}
		}
		break;
	case EWinConditons::ByTimeLimit :
		break;

	}
}

void AGL_GameModeBase::InitPlayerSpawns()
{

}

void AGL_GameModeBase::InitMatch_Implementation()
{

}

void AGL_GameModeBase::SpawnAIElapsed()
{

}

void AGL_GameModeBase::SpawnAIActors_Implementation(TSubclassOf<AActor> EnemySpawn)
{

}
