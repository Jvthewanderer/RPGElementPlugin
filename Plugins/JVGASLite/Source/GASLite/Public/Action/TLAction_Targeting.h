// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/TLAction.h"
#include "TLAction_Targeting.generated.h"

class ATLActionTargetActor;
class ARLCharacter;

/**
 * 
 */
UCLASS()
class GASLITE_API UTLAction_Targeting : public UTLAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	TSubclassOf<ATLActionTargetActor> TargetActorClass;

	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	ATLActionTargetActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* ConfirmAttackAnim;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float MontageDuration;

	FTimerHandle TimerHandle_SetTargetLocation;

	FTimerHandle TimerHandle_AttackDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetDistance;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	bool bTraceLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	bool bOnOwningActor;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	bool bUseForwardDirection;

	UFUNCTION()
	void AttackDelay_Elapsed(AActor* InstigatorCharacter);

	UFUNCTION(Server, Reliable)
	void ServerSpawnTarget(AActor* InstigatorCharacter);




public:

	UFUNCTION()
	void SpawnTarget(AActor* InstigatorCharacter);
	
	UFUNCTION()
	void SetTargetLocation_Elapsed(AActor* InstigatorCharacter);

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	void TriggerAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void StartTrackingLocation(AActor* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void StopTrackingLocation();



	UTLAction_Targeting();
	
};
