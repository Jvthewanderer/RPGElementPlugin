// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/TLActionEffect.h"
#include "TLActionEffect_DoT.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLActionEffect_DoT : public UTLActionEffect
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;
	
	void ExecutePeriodicEffect_Implementation(AActor* Instigator) override;

public:
	UTLActionEffect_DoT();
		
	void StartAction_Implementation(AActor* Instigator) override;


	void StopAction_Implementation(AActor* Instigator) override;

};
