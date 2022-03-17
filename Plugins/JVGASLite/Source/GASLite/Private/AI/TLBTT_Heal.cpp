// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTT_Heal.h"
#include <AIController.h>
#include <GameFramework/Character.h>
#include "Component/TLAttributeComponent.h"

EBTNodeResult::Type UTLBTT_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!ensureMsgf(AIC,TEXT("AIController Missing! Make sure the AI Pawn has a AIController")))
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* MyPawn = Cast<ACharacter>(AIC->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UTLAttributeComponent* AttributeComp = Cast<UTLAttributeComponent>(MyPawn->GetComponentByClass(UTLAttributeComponent::StaticClass()));
	if (!ensureMsgf(AttributeComp, TEXT("AttributeComponent is Missing! Make sure the Pawn has a TLAttributeComponent")))
	{
		return EBTNodeResult::Failed;
	}

	AttributeComp->ApplyHealthChange(MyPawn, applyedHealthDelta);

	return EBTNodeResult::Succeeded;
}
