// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/TLAction.h"
#include "TLAction_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLAction_MeleeAttack : public UTLAction
{
	GENERATED_BODY()

	public:

	UTLAction_MeleeAttack();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	void TriggerAction_Implementation(AActor* Instigator) override;

	void TriggerActionFromAnimNotify_Implementation(AActor* Instigator, FName SocketName) override;


	protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float AttackAnimDelay;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
		float MontageDuration;

	UFUNCTION()
		void AttackDelay_Elapsed(AActor* InstigatorCharacter);

	UFUNCTION(BlueprintNativeEvent, Category = "OnHit")
	void OnAttackHit(AActor* TargetedActor);

};
