// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TLTowerBase.h"
#include "GL_Turret.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API AGL_Turret : public ATLTowerBase
{
	GENERATED_BODY()

	public:

		// Sets default values for this actor's properties
	AGL_Turret();

		void Tick(float DeltaTime) override;

protected:
	

		void BeginPlay() override;

		void PostInitializeComponents() override;

		UFUNCTION(BlueprintCallable, Category = "Target")
			void OnTowerRadiusBeginOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION(BlueprintCallable, Category = "Target")
			void OnTowerRadiusEndOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		UFUNCTION(BlueprintCallable, Category = "Target")
			void TowerLookAtTarget();

};
