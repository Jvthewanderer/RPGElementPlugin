// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/TLGameplayInterface.h"
#include "../TLActionRoguelike.h"
#include "TLAIController.generated.h"

class UBehaviorTree;
class AGL_Unit;
class UTLAttributeComponent;
class UAIPerceptionComponent;
class UAISense;

/**
 * 
 */
UCLASS()
class GASLITE_API ATLAIController : public AAIController
{
	GENERATED_BODY()
	
	protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf<UAISense> AISenceSight;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf<UAISense> AISenceDamage;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	EUnitCommand Command;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float RecallTimer;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	int32 CurrentIndex;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	AGL_Unit* UnitPawn;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	UTLAttributeComponent* PawnAttribute;

	virtual void BeginPlay();

public:

	ATLAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "AI")
	void InitAI(TArray<FTransform> RelayPoints);

	UFUNCTION(BlueprintCallable, Category = "AI")
	APawn* GetNearestAlivePerceivedPawn();

	UFUNCTION(BlueprintCallable, Category = "AI")
	uint8 GetOwnerTeam();

	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<APawn*> GetPerceivedUnits(bool bUnitAlive);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	bool ExecutePrimaryAttack();

UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	bool ExecutePrimaryAbility();

UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	bool ExecuteSecondaryAbility();

UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	bool ExecuteTertiaryAbility();

UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	bool ExecuteSpecialAttack();

UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	bool SetCommands(EUnitCommand UnitCommand, FVector TargetLocation, AActor* TargetUnit);

UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void ResetCommand();

UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void ResetTarget(AActor* ActorTarget);

UFUNCTION(BlueprintPure, Category ="AI")
	AActor* GetTargetActor();

UFUNCTION()
	void OnUnitDamaged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

UPROPERTY(BlueprintReadOnly, Category = "Ai")
	TArray<FTransform> MoveRelayPoints;


void Tick(float DeltaTime) override;

private:
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

};
