// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../TLActionRoguelike.h"
#include "GL_PlayerStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerClientScoreboard);

USTRUCT(BlueprintType)
struct FBasePlayerStat{
	
	GENERATED_BODY();

	UPROPERTY()
	int32 KillCount;

	UPROPERTY()
	int32 DeathCount;

	UPROPERTY()
	int32 MinionKillCount;

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Gold;

	UPROPERTY()
	int32 Score;

	FBasePlayerStat() {
		KillCount = 0;
		DeathCount = 0;
		Score = 0;
		MinionKillCount = 0;
		Level = 0;
		Gold = 0;
	}

};

USTRUCT(BlueprintType)
struct FBasePlayerDesc {

	GENERATED_BODY();

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	bool IsHost;

	UPROPERTY()
	bool IsClient;

	UPROPERTY()
	uint8 TeamID;

	FBasePlayerDesc() {
		PlayerName = "";
		IsHost = false;
		IsClient = false;
		TeamID = 0;
	}

	FBasePlayerDesc(FString _playername, bool _isHost, bool _isClient, uint8 _teamID) {
		PlayerName = _playername;
		IsHost = _isHost;
		IsClient = _isClient;
		TeamID = _teamID;
	}

};



/**
 * 
 */
UCLASS()
class GASLITE_API AGL_PlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
		AGL_PlayerStateBase();


	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Player Stat")
		void ClientMatchInitializeState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Stat")
		void InitPlayerPropertiesFromInstance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Stat")
		void InitMatchInitializeFromInstance();

	UFUNCTION(BlueprintPure, Category = "Player Stat")
		int32 GetKillCount() { return BasePlayerStat.KillCount;};

	UFUNCTION(BlueprintPure, Category = "Player Stat")
		int32 GetDeathCount() { return BasePlayerStat.DeathCount; };

	UFUNCTION(BlueprintPure, Category = "Player Stat")
		float GetKDRatio();

	UFUNCTION(BlueprintPure, Category = "Player Stat")
		FString GetPlayerNameDesv() { return BasePlayerDesc.PlayerName;};

	UFUNCTION(BlueprintPure, Category = "Player Stat")
		int32 GetScore() { return BasePlayerStat.Score; };

	UFUNCTION(BlueprintPure, Category = "Player Stat")
		bool GetIsDead() {return bIsDead;};

	UFUNCTION(Server, Reliable, Category = "PlayerState")
		void ServerUpdateKillCount(int32 addedValue);

	UFUNCTION(Server, Reliable, Category = "PlayerState")
		void ServerUpdateDeathCount(int32 addedValue);

	UFUNCTION(NetMulticast, Reliable, Category = "PlayerState")
		void MulticastUpdateLevel(int32 addedValue);

	UFUNCTION(NetMulticast, Reliable, Category = "PlayerState")
		void MulticastUpdateMinionKillCount(int32 addedValue);

	UFUNCTION(NetMulticast, Reliable, Category = "PlayerState")
		void MulticastUpdateKillCount(int32 addedValue);

	UFUNCTION(NetMulticast, Reliable, Category = "PlayerState")
		void MulticastUpdateDeathCount(int32 addedValue);

	UFUNCTION(NetMulticast, Reliable, Category = "PlayerState")
		void MulticastUpdateGoldAdded(int32 addedValue);

	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetTeam(uint8 NewTeamID);

	UFUNCTION(BlueprintPure, Category = "Common")
		uint8 GetTeam(){return BasePlayerDesc.TeamID;};

	UFUNCTION(BlueprintPure, Category = "Common")
		int32 GetRespawnTimer() { return RespawnTimerValue; };

	UFUNCTION(BlueprintPure, Category = "Common")
		bool GetCanRespawn() { return CanRespawn; };

	UFUNCTION(BlueprintCallable, Category = "Common")
		void InitFromGameInst();

	void TriggerClientOpenScoreboard();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Common")
		void ServerInitPlayerProperties(FBasePlayerDesc newPlayerDesc);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Common")
		void MulticastInitPlayerProperties(FBasePlayerDesc newPlayerDesc);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Player Stat")
		void ClientInitPlayerProperties();





		//* PLayer Login/ Logout*//
	UFUNCTION(Server, Reliable)
		void ServerPlayerLogin(AGL_PlayerStateBase* PS);

	UFUNCTION(Client, Reliable)
		void ClientPlayerLogin(AGL_PlayerStateBase* PS);

	UFUNCTION(Server, Reliable)
		void ServerPlayerLogOut(AGL_PlayerStateBase* PS);

	UFUNCTION(Client, Reliable)
		void ClientPlayerLogOut(AGL_PlayerStateBase* PS);

	/* Teams*/

	UFUNCTION(Server, Reliable)
	void ServerSetTeamForPlayer(uint8 TeamId);

	UFUNCTION(NetMulticast, Reliable)
	void MuliticastSetTeamForPlayer(uint8 TeamId);


	/*Matches Functions*/

	UFUNCTION(Server, Reliable)
	void ServerMatchStart();

	UFUNCTION(Client, Reliable)
	void ClientMatchStart();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Stat")
	void InitMatchStartFromInstance();

	UFUNCTION(Server, Reliable)
		void ServePreMatchInitialize();

	UFUNCTION(Client, Reliable)
		void ClientPreMatchInitialize();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Stat")
		void InitPreMatchInitializeFromInstance();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Common")
		void ServerWaitEndMatch();

	UFUNCTION(Client, Reliable)
		void ClientWaitEndMatch();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Stat")
		void InitWaitEndMatchFromInstance();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Common")
		void ServerWaitingStart();

	UFUNCTION(Client, Reliable)
		void ClientWaitingStart();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Stat")
		void InitWaitingStartFromInstance();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Common")
		void ServerPlayerKilled();

	UFUNCTION(Client, Reliable)
		void ClientPlayerKilled();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Stat")
		void InitPlayerKilledFromInstance();

protected:

	//FDelegate OnTriggerClientOpenScoreboard;

	UPROPERTY(Replicated)
		FBasePlayerStat BasePlayerStat;

	UPROPERTY(Replicated)
		FBasePlayerDesc BasePlayerDesc;

	UPROPERTY(Replicated)
		bool bIsDead;

	UPROPERTY(Replicated)
		int32 RespawnTimerValue;

	UPROPERTY(Replicated)
		bool CanRespawn;

	UPROPERTY(BlueprintAssignable, Category = "Scoreboard")
		FOnTriggerClientScoreboard OnTriggerClientScoreboard;

	void BeginPlay() override;

};
