// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TLBTT_Heal.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLBTT_Heal : public UBTTaskNode
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float applyedHealthDelta = 10.f;
	
};
