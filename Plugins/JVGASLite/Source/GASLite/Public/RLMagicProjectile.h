// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLProjectileBase.h"
#include <GameplayTagContainer.h>
#include "RLMagicProjectile.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;
class UCameraShakeBase;
class UTLActionEffect;

UCLASS()
class GASLITE_API ARLMagicProjectile : public ATLProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLMagicProjectile();

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UTLActionEffect> ActionEffectClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
