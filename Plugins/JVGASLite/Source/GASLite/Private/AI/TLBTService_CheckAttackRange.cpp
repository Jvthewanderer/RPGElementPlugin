// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTService_CheckAttackRange.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>
#include "Interface/TLGameCompInterface.h"

void UTLBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	// Check distance between ai pawn and target actor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();

				if (ensure(AIPawn))
				{
					if (bUsePawnRadius)
					{
						float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

						bool bWithinRange = DistanceTo < ITLGameCompInterface::Execute_GetRadius(AIPawn);

						bool bHasLOS = false;
						if (bWithinRange)
						{
							bHasLOS = MyController->LineOfSightTo(TargetActor);
						}

						BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bHasLOS);
						return;
					}
				
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < targetDistance;

					bool bHasLOS = false;
					if (bWithinRange) 
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}



					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bHasLOS);
					return;
				}
			}
		}
		BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, false);
	}

}
