// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TLPowerupActor.h"
#include "TLPowerup_HealthPotion.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class GASLITE_API ATLPowerup_HealthPotion : public ATLPowerupActor
{
	GENERATED_BODY()

	protected:
		UPROPERTY(VisibleAnywhere, Category = "Components")
			UStaticMeshComponent* MeshComp;

		UPROPERTY(EditAnywhere, Category = "HealthPotion")
			int32 CreditCost;
		// float healt amount?

	public:
		void Interact_Implementation(APawn* InstigatorPawn) override;

		FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

		ATLPowerup_HealthPotion();
};
