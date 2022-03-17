// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTS_CheckLowHealth.h"
#include <AIController.h>
#include <GameFramework/Pawn.h>
#include "Component/TLAttributeComponent.h"
#include <BehaviorTree/BlackboardComponent.h>

void UTLBTS_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!ensure(BlackboardComp))
	{
		return;
	}

	AAIController* MyController = OwnerComp.GetAIOwner();
	APawn* MyPawn = MyController->GetPawn();
	UTLAttributeComponent* AttributeComp = Cast<UTLAttributeComponent>(MyPawn->GetComponentByClass(UTLAttributeComponent::StaticClass()));
	if (!ensureMsgf(AttributeComp,TEXT("AI Pawn missing Attribute Component. Make sure the Pawn has Attribute Component Attached.")))
	{
		return;
	}

	bool healthCheck = AttributeComp->isHealthLow();

	BlackboardComp->SetValueAsBool(HealthKey.SelectedKeyName, healthCheck);


}
