// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TLBTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	protected:
		bool bUseMontage;


	public: 
		UTLBTT_MeleeAttack();

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* AttackMontage;

		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
