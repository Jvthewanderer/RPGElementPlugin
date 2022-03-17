// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../TLActionRoguelike.h"
#include "GL_GameStateBase.generated.h"

USTRUCT(BlueprintType)
struct FBaseMatchDesc
{
	GENERATED_BODY();

	UPROPERTY()
	int32 MatchTimeLimit;

	UPROPERTY()
	int32 MatchScoreLimit;

	UPROPERTY()
		bool UseMSDecreasingMethod;

	UPROPERTY()
		bool AutoSpawn;

	UPROPERTY()
		bool UseWaveSpawn;

	UPROPERTY()
		int32 RespawnTime;

	FBaseMatchDesc(int32 _matchTimeLimit, int32 _matchScoreLimit, bool _MSDecreasingMethod, bool _autoSpawn, bool _useWaveSpawn, int32 _respawnTime) {
		MatchTimeLimit = _matchTimeLimit;
		MatchScoreLimit = _matchScoreLimit;
		UseMSDecreasingMethod = _MSDecreasingMethod;
		AutoSpawn = _autoSpawn;
		UseWaveSpawn = _useWaveSpawn;
		RespawnTime = _respawnTime;
	}

	FBaseMatchDesc() {
		MatchTimeLimit = 0;
		MatchScoreLimit = 0;
		UseMSDecreasingMethod = false;
		AutoSpawn = true;
		UseWaveSpawn = false;
		RespawnTime = 5;
	}

};

USTRUCT(BlueprintType)
struct FGameStateDesc 
{
	GENERATED_BODY();

	UPROPERTY()
	FName LevelName;

	UPROPERTY()
	EGameModeType GameModeType;

	FGameStateDesc() {
		LevelName= FName(TEXT("None"));
		GameModeType = EGameModeType::Debug;
	}

	FGameStateDesc(FName _levelMap, EGameModeType _gmType) {
		LevelName = _levelMap;
		GameModeType = _gmType;
	}
};

/**
 * 
 */
UCLASS()
class GASLITE_API AGL_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	public:

	AGL_GameStateBase();

	UFUNCTION(BlueprintPure, Category = "Common")
		bool GetUseDecreasingMethod(){ return BaseMatchDesc.UseMSDecreasingMethod; };

	UFUNCTION(BlueprintPure, Category = "Common")
		int32 GetMatchScoreLimit() {return BaseMatchDesc.MatchScoreLimit;};

	UFUNCTION(Server, Reliable, Category = "Common")
		void ServerUpdateTeamMatchScore(uint8 TeamID, int32 Score);

	UFUNCTION(NetMulticast, Reliable, Category = "Common")
		void MulticastUpdateTeamMatchScore(int32 MatchScoreTeam1, int32 MatchScoreTeam2);

	UFUNCTION(BlueprintCallable, Category = "Common")
		int32 UpdateTeamScore(int32 CurrentTeamScore, int32 AddedScore);

	UFUNCTION(Server, Reliable, Category = "Common")
		void ServerUpdateWinner(EWinner CurrentMatchWinner);

	UFUNCTION(Server, Reliable, Category = "Common")
		void ServerUpdateMGSMatchState(EMatchState MatchState);

	UFUNCTION(NetMulticast, Reliable, Category = "Common")
		void MulticastUpdateMatchState(EMatchState MatchState);

	UFUNCTION(Server, Reliable, Category = "Common")
		void ServerHandleMatchState();

	UFUNCTION(Server, Reliable, Category = "Common")
		void ServerInitializeTeamScore();

	UFUNCTION(NetMulticast, Reliable, Category = "Common")
		void MulticastInitializeTeamScore(int32 MatchScoreTeam1, int32 MatchScoreTeam2);

	UFUNCTION(Server, Reliable, Category = "Common")
		void ServerUpdateTimer(int32 GameTimer);


	UFUNCTION(NetMulticast, Reliable, Category = "Common")
		void MulticastUpdateTimer(int32 GameTimer);


	UFUNCTION(BlueprintPure, Category = "Common")
		EMatchState GetCurrentMatchState(){return CurrentMatchState;};

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Common")
		int32 CurrentRoundCount;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Common")
		int32 CurrentScoreTeam1;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Common")
		int32 CurrentScoreTeam2;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Common")
		int32 CurrentTime;


protected:


	UPROPERTY(Replicated)
		FBaseMatchDesc BaseMatchDesc;

	UPROPERTY(Replicated)
		FGameStateDesc GameStateDesc;

	UPROPERTY(Replicated)
		EMatchState CurrentMatchState;

};
