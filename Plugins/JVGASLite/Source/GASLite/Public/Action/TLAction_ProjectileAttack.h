// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/TLAction.h"
#include "TLAction_ProjectileAttack.generated.h"


class UAnimMontage;
class ARLCharacter;
class ATLProjectileBase;
/**
 * 
 */
UCLASS()
class GASLITE_API UTLAction_ProjectileAttack : public UTLAction
{
	GENERATED_BODY()

	protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float MontageDuration;

	UPROPERTY(EditDefaultsOnly, Category ="Attack")
	bool bUseForwardDirection;

	UFUNCTION()
	void AttackDelay_Elapsed(AActor* InstigatorCharacter);

	UFUNCTION()
	void SpawnProjectile(AActor* InstigatorCharacter, FName SocketName);

	UFUNCTION(Server, Reliable)
	void ServerSpawnProjectile(AActor* InstigatorCharacter, FName SocketName);

	UFUNCTION(BlueprintNativeEvent, Category = "OnHit")
	void OnProjectileHit(AActor* TargetedActors, ATLProjectileBase* SelfProjectile);

	public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void TriggerActionFromAnimNotify_Implementation(AActor* Instigator, FName SocketName) override;

	UTLAction_ProjectileAttack();
	
};
