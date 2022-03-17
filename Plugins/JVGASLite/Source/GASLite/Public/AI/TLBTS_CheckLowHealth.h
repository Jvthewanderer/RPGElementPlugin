// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TLBTS_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLBTS_CheckLowHealth : public UBTService
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector  HealthKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
