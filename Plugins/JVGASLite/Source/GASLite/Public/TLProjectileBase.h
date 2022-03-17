// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameplayTagContainer.h>
#include "TLProjectileBase.generated.h"

class USphereComponent;
class UParticleSystem;
class UProjectileMovementComponent;
class UAudioComponent;
class USoundCue;
class UCameraShake;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileHit, AActor*, TargetedActor, ATLProjectileBase*, SelfProjectile);

UCLASS()
class GASLITE_API ATLProjectileBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Target")
		bool bIsHoming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAudioComponent* FlightAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USoundCue* ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		float ImpactShakeOuterRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer DamageTypeTags;

	// 'virtual' so we can override this in child-classes
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;


public:	
	// Sets default values for this actor's properties
	ATLProjectileBase();

	FOnProjectileHit OnProjectileHit;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
		AActor* TargetActor;

	UFUNCTION()
		void SetDamageAmount(float newDamage) {
		DamageAmount += newDamage;
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
