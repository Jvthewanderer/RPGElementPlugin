// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/EngineTypes.h"
#include "../TLActionRoguelike.h"
#include "GL_GameModeBase.generated.h"

class AGL_GameStateBase;
class APlayerController;

UENUM()
enum EWinConditons {
	ByMatchScore,
	ByTimeLimit,
	ByCoreDestroyed
};

UENUM()
enum EWinner {
	Team1,
	Team2,
	Draw
};

/**
 * 
 */
UCLASS()
class GASLITE_API AGL_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGL_GameModeBase();
	
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void StartPlay() override;

	void Logout(AController* Exiting) override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	UFUNCTION(BlueprintPure, Category = "GameMode")
	AGL_GameStateBase* GetBaseGameState();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	APlayerController* GetPlayerControllerfromPlayerState(AGL_PlayerStateBase* MPPlayerState);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandlePlayerDeath(AGL_PlayerStateBase* VictimPlayerState, AGL_PlayerStateBase* CauserPlayerState);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandlePlayerScore(AGL_PlayerStateBase* PlayerState, int32 addedvalue);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandlePlayerDeathByUnit(AGL_PlayerStateBase* VictimPlayerState, AActor* CauserUnit);


	//////* Match Timers*////////

	UFUNCTION(BlueprintNativeEvent, Category = "Timer")
	void StartWaitingTimer();

	UFUNCTION(BlueprintNativeEvent, Category = "Timer")
	void StartPreMatchTimer();

	UFUNCTION()
	void BeginMatchTimer();

	UFUNCTION(BlueprintNativeEvent, Category = "Timer")
	void StartWaitEndGameTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void PauseMatchTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void SendPlayerToLobbyMenu();


	/* Match Functions*/
	UFUNCTION(BlueprintNativeEvent, Category = "Match")
	void StartGameMatch();

	UFUNCTION(BlueprintNativeEvent, Category = "Match")
	void MatchEnd(EWinner Winner);

	UFUNCTION(BlueprintNativeEvent, Category = "Match")
	void UpdateTeamMatchScore(uint8 TeamID, int32 AddedScore);

	UFUNCTION(BlueprintNativeEvent, Category = "Match")
	void CheckWinConditions(EWinConditons WinCondiitons);



	/* Spawns*/
	UFUNCTION()
	void InitPlayerSpawns();

	UFUNCTION(BlueprintNativeEvent, Category = "Match")
	void InitMatch();

	UFUNCTION()
	void SpawnAIElapsed();

	UFUNCTION(BlueprintNativeEvent, Category = "Match")
	void SpawnAIActors(TSubclassOf<AActor> EnemySpawn);


protected:
	

	/* Game Description*/

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	FGameDesc GameDesc;

	/* Timer Variables*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 PreMatchTimer;

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	int32 PreMatchTimerValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 WaitingTimer;

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	int32 WaitingTimerValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 MatchTimer;

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	int32 MatchTimerValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 EndMatchTimer;

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	int32 EndMatchTimerValue;

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	bool bMatchTimerUp;

	UPROPERTY()
	FTimerHandle TimerHandle_PreMatchTimer;

	UPROPERTY()
	FTimerHandle TimerHandle_WaitingTimer;

	UPROPERTY()
	FTimerHandle TimerHandle_MatchTimer;

	UPROPERTY()
	FTimerHandle TimerHandle_EndMatchTimer;


	//Functions
	void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void PreMatchTimer_Elapsed();

	UFUNCTION()
	void WaitingTimer_Elapsed();

	UFUNCTION()
	void MatchTimer_Elapsed();
	
	UFUNCTION()
	void EndGameTimer_Elapsed();



};
