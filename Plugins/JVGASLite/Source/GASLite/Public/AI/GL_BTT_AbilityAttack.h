// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../TLActionRoguelike.h"
#include "GL_BTT_AbilityAttack.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UGL_BTT_AbilityAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
		EAttackCommand Ability;

		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
