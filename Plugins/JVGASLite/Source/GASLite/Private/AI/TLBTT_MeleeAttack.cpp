// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTT_MeleeAttack.h"
#include "AI/TLAIController.h"

UTLBTT_MeleeAttack::UTLBTT_MeleeAttack()
{
	bUseMontage = true;
}

EBTNodeResult::Type UTLBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ATLAIController* MyController = Cast<ATLAIController>(OwnerComp.GetAIOwner());
	if (!MyController)
	{
		return EBTNodeResult::Failed;
	}
	ACharacter* MyCharacter = Cast<ACharacter>(MyController->GetPawn());
	if (MyCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

	if (TargetActor == nullptr) {

		return EBTNodeResult::Failed;
	}

	if (!UTLAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	MyController->ExecutePrimaryAttack();

	return EBTNodeResult::Succeeded;
}
