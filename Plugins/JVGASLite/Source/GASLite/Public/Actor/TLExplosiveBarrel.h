// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "TLExplosiveBarrel.generated.h"

class URadialForceComponent;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class GASLITE_API ATLExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Spells|Exploding Barrel")
		void OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion")
	float blastRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion")
	float damage;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Explosion")
		URadialForceComponent* RadialForceComponent = nullptr;
};
