// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../TLActionRoguelike.h"
#include "GL_GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FMenuPlayerDesc {
	GENERATED_BODY();

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	bool IsHost;

	UPROPERTY()
	bool IsClient;

	UPROPERTY()
	uint8 TeamID;

	FMenuPlayerDesc() {
		PlayerName = "";
		IsHost= false;
		IsClient = false;
		TeamID = 0;
	}

	FMenuPlayerDesc(FString _playerName, bool _isHost, bool _isClient, uint8 _teamID) {
		PlayerName = _playerName;
		IsHost = _isHost;
		IsClient = _isClient;
		TeamID = _teamID;
	}

};

USTRUCT(BlueprintType)
struct FMenuGameDesc {
	GENERATED_BODY();

	UPROPERTY()
		FName LevelName;

	UPROPERTY()
		EGameModeType GameModeType;

	UPROPERTY()
		int32 MaxPlayer;

	FMenuGameDesc() {
		LevelName = "None";
		GameModeType = EGameModeType::Debug;
		MaxPlayer = 10; 

	}

	FMenuGameDesc(FName _levelName, EGameModeType _gmType, int32 _maxPlayer) {
		LevelName = _levelName;
		GameModeType = _gmType;
		MaxPlayer = _maxPlayer;

	}

};

/**
 * 
 */
UCLASS()
class GASLITE_API UGL_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:

	UGL_GameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Common")
		void LocalUpdatePlayerName(FString PlayerName);

	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetLocalClientAsHost();

	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetLocalClientAsClient();

	UFUNCTION(BlueprintCallable, Category = "Common")
		void ResetHostClientSettings();

	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetLocalClientName(FString _playerName);

	UFUNCTION(BlueprintPure, Category = "Common")
		FMenuPlayerDesc GetMenuPlayerDesc();

	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetMenuPlayerDesc(FMenuPlayerDesc newPlayerDesc);

	UFUNCTION(BlueprintPure, Category = "Common")
		FMenuGameDesc GetMenuGameDesc();

	UFUNCTION(BlueprintCallable, Category = "Common")
		void SetMenuGameDesc(FMenuGameDesc newGameDesc);
	

protected:
	FMenuPlayerDesc MenuPlayerDesc;

	FMenuGameDesc MenuGameDesc;

	bool bUseLan;
	bool bIsPresence;

};
