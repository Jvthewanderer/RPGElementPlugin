// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action/TLAction.h"
#include "TLActionTargetActor.generated.h"



class APlayerController;
class UTLAction;
class UTLActionComponent;
class ARLCharacter;
class UTLActionEffect;
class ATLAIController;
class AGL_Unit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmTargetsExecuted, TArray<AActor*>, TargetedActors );


UCLASS()
class GASLITE_API ATLActionTargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLActionTargetActor();

	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/*Init and begin Targeting*/
	virtual void StartTargeting(UTLAction* Action);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Targeting")
	bool ExecuteEffects();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Targeting")
	void ServerExecuteEffect();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Targeting")
	void MulticastExecuteEffect();

	/** Requesting targeting data, but not necessarily stopping/destroying the task. Useful for external target data requests. */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	virtual void ConfirmTargetingAndContinue();

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	bool GetPlayerLookingPoint(FVector& OutViewPoint);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	void ConfirmTargets();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerConfirmTargets();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	void CancelTargeting();

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void ApplyDamageToTargets();
	
	UFUNCTION()
	bool IsRunning() const;

	UFUNCTION()
	void OnRep_RepData();

	UFUNCTION()
	void OnRep_bIsTargeting();

	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	APlayerController* MasterPC;

	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	ATLAIController* MasterAIPC;

	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	AGL_Unit* MasterPlayer;

	UPROPERTY()
	UTLAction* OwningAction;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(ReplicatedUsing = "OnRep_bIsTargeting")
	bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Targeting")
	float Radius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TArray<TSubclassOf<UTLActionEffect>> ActionEffectClasses;
	
	UPROPERTY(BlueprintAssignable, Category = "Targeting")
	FOnConfirmTargetsExecuted OnConfirmTargetsExecuted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_CheckOverlappedActors;



	UPROPERTY()
	TArray<AActor*> OverlappedActors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
