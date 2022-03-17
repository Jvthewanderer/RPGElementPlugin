// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TLPowerupActor.h"
#include "TLPowerup_ManaPotion.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class GASLITE_API ATLPowerup_ManaPotion : public ATLPowerupActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "HealthPotion")
		int32 CreditCost;
	// float Mana amount?

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

	ATLPowerup_ManaPotion();

};
