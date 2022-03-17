// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GL_BTT_AbilityAttack.h"
#include "../TLActionRoguelike.h"
#include "Component/TLAttributeComponent.h"
#include "AI/TLAIController.h"

EBTNodeResult::Type UGL_BTT_AbilityAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	switch (Ability) {
		case EAttackCommand::PrimaryAttack :
			MyController->ExecutePrimaryAttack();
		break;

		case EAttackCommand::PrimaryAbilityAttack:
			MyController->ExecutePrimaryAbility();
			break;

		case EAttackCommand::SecondaryAbilityAttack:
			MyController->ExecuteSecondaryAbility();
			break;

		case EAttackCommand::TertiaryAbilityAttack:
			MyController->ExecuteTertiaryAbility();
			break;

		case EAttackCommand::SpecialAbility:
			MyController->ExecuteSpecialAttack();
			break;

	}

	return EBTNodeResult::Succeeded;
}
