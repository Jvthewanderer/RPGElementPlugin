// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "TLActionRoguelike.generated.h"

DECLARE_STATS_GROUP(TEXT("STANFORD_Game"), STATGROUP_STANFORD, STATCAT_Advanced);

static TAutoConsoleVariable<bool> CVarNetDebug(TEXT("tl.MessageDebug"), false, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

UENUM(BlueprintType)
enum class EUnitCommand : uint8
{
	UnitIdle UMETA(DisplayName = "Idle"),
	UnitMove UMETA(DisplayName = "Move"),
	UnitAttack UMETA(DisplayName = "Attack"),
	UnitMoveAttack UMETA(DisplayName = "MoveAttack"),
	UnitRecall UMETA(DisplayName = "Recall")
};

UENUM(BlueprintType)
enum class EAttackCommand : uint8
{
	PrimaryAttack UMETA(DisplayName = "PrimaryAttack"),
	PrimaryAbilityAttack UMETA(DisplayName = "PrimaryAbilityAttack"),
	SecondaryAbilityAttack UMETA(DisplayName = "SecondaryAbilityAttack"),
	TertiaryAbilityAttack UMETA(DisplayName = "TertiaryAbilityAttack"),
	SpecialAbility UMETA(DisplayName = "SpecialAbility")
};

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	Debug UMETA(DisplayName = "Debug"),
	FreeForAll UMETA(DisplayName = "FreeForAll"),
	TDM UMETA(DisplayName = "TDM"),
	MOBA UMETA(DisplayName = "MOBA"),
	ARAM UMETA(DisplayName = "ARAM")
};

UENUM(BlueprintType)
enum class ELanes : uint8
{
	Top UMETA(DisplayName = "Top"),
	Middle UMETA(DisplayName = "Middle"),
	Bottom UMETA(DisplayName = "Bottom")
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	PreMatchInitialize UMETA(DisplayName = "PreMatchInitialize"),
	PreMatchStarted UMETA(DisplayName = "PreMatchStarted"),
	MatchInitialize UMETA(DisplayName = "MatchInitialize"),
	MatchStarted UMETA(DisplayName = "MatchStarted"),
	WaitingEndGame UMETA(DisplayName = "WaitingEndGame")
};

UENUM(BlueprintType)
enum class EUnitParam : uint8
{

Health,
Mana,
HPRegen,
MPRegen,
MeleeAttack,
AbilityPower,
Defence,
AbilityDef,
AttackSpeed,
MovementSpeed
};

USTRUCT(BlueprintType)
struct FGameDesc
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	FName LevelName;

	UPROPERTY(EditAnywhere)
	int32 MaxPlayerCount;

	UPROPERTY(EditAnywhere)
	EGameModeType GameModeType;

	FGameDesc() {
		LevelName = FName(TEXT("None"));
		MaxPlayerCount = 2;
		GameModeType = EGameModeType::Debug;
	}

	FGameDesc(FName _levelName, int32 _maxPLayerCount, EGameModeType _gamemodeType){
		LevelName = _levelName;
		MaxPlayerCount = _maxPLayerCount;
		GameModeType = _gamemodeType;
	}
};

USTRUCT(BlueprintType)
struct FUnitData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> Unit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UnitRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* LevelDataTable;

	FUnitData() {
		UnitName = FName("None");
		Unit = nullptr;
		UnitRadius = 0.0f;
		LevelDataTable = nullptr;
	}

	FUnitData(FName _unitName, TSubclassOf<APawn> _unit, float _unitRadius, UDataTable* _levelDataTable) {
		UnitName = _unitName;
		Unit = _unit;
		UnitRadius = _unitRadius;
		LevelDataTable = _levelDataTable;
	
	}

};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Item;
};

USTRUCT(BlueprintType)
struct FUnitLevelData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MPRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AbilityPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AbilityDefence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed;

	FUnitLevelData() {
		Exp = 0.f;
		Health = 1.f;
		Mana = 1.f;
		HPRegen = 1.f;
		MPRegen = 1.f;
		MeleeAttack = 1.f;
		AbilityPower = 1.f;
		Defence = 1.f;
		AbilityDefence = 1.f;
		AttackSpeed = 100.f;
		MovementSpeed = 1.f;
	}

	FUnitLevelData(float _exp, float _health, float _mana, float _hpRegen, float _mpRegen, float _meleeAttack,
					float _abilityPower, float _defence, float _abilityDef, float _attackSpd, float _movementSpeed) {
		Exp = _exp;
		Health = _health;
		Mana = _mana;
		HPRegen = _hpRegen;
		MPRegen = _mpRegen;
		MeleeAttack = _meleeAttack;
		AbilityPower = _abilityPower;
		Defence = _defence;
		AbilityDefence = _abilityDef;
		AttackSpeed = _attackSpd;
		MovementSpeed = _movementSpeed;
	}

	FUnitLevelData(FUnitLevelData* a)
	{
		Exp = a->Exp;
		Health = a->Health;
		Mana = a->Mana;
		HPRegen = a->HPRegen;
		MPRegen = a->MPRegen;
		MeleeAttack = a->MeleeAttack;
		AbilityPower = a->AbilityPower;
		Defence = a->Defence;
		AbilityDefence = a->AbilityDefence;
		AttackSpeed = a->AttackSpeed;
		MovementSpeed = a->MovementSpeed;
	}

};



static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if (!CVarNetDebug.GetValueOnGameThread())
	{
		return;
	}

	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}



	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration,Color, NetPrefix + Msg);
	}

}