// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TLPowerupActor.h"
#include "TLPowerup_Action.generated.h"

class UTLAction;


/**
 * 
 */
UCLASS()
class GASLITE_API ATLPowerup_Action : public ATLPowerupActor
{
	GENERATED_BODY()

	protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Powerup")
	TSubclassOf<UTLAction> ActionToGrant;

	public:

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
