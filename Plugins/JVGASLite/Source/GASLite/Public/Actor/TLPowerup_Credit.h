// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TLPowerupActor.h"
#include "TLPowerup_Credit.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class GASLITE_API ATLPowerup_Credit : public ATLPowerupActor
{
	GENERATED_BODY()

	protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Credit")
	int32 CreditAmount;

	public:
	
		void Interact_Implementation(APawn* InstigatorPawn) override;

		ATLPowerup_Credit();

};
