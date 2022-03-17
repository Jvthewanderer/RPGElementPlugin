// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/TLActionEffect.h"
#include "TLActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLActionEffect_Thorns : public UTLActionEffect
{
	GENERATED_BODY()

	protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHeatlh, float Detla);

	public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UTLActionEffect_Thorns();
	
};
