// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TLBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

	protected:
		UPROPERTY(EditAnywhere, Category ="AI")
	FBlackboardKeySelector AttackRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float targetDistance = 2000.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	bool bUsePawnRadius = false;
	
};
