// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLAIController.h"
#include <Kismet/GameplayStatics.h>
#include <AIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/TLGameplayInterface.h"
#include "../TLActionRoguelike.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Component/TLAttributeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "GL_Unit.h"
#include "Interface/TLGameCompInterface.h"
#include "Actor/TLTowerBase.h"
#include "Library/GL_MOBALibrary.h"
#include "AIModule/Classes/Perception/AISense_Damage.h"

ATLAIController::ATLAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComp");

	CurrentIndex = 0;
}

void ATLAIController::BeginPlay()
{

	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

}

APawn* ATLAIController::GetNearestAlivePerceivedPawn()
{
	float Distance = 1000000.0f;
	TArray<APawn*> OutPawns;
	APawn* NearestUnit = nullptr;
	AGL_Unit* OwnerUnit = Cast<AGL_Unit>(GetPawn());
	OutPawns = GetPerceivedUnits(true);

	if(OutPawns.Num() > 0)
	{

		Distance = UGL_MOBALibrary::GetUnitDistance(GetPawn(), OutPawns[0]);

		NearestUnit = OutPawns[0];

		float tempDistance = 0.0f;
		for (APawn* CurrentPawn : OutPawns)
		{
		 tempDistance = UGL_MOBALibrary::GetUnitDistance(CurrentPawn, GetPawn());
			if (tempDistance < Distance)
			{
				Distance = tempDistance;
				NearestUnit = CurrentPawn;

			}
		}

  	FColor TextColor = FColor::Blue;
	FString TargetMsg = FString::Printf(TEXT("[%s] NearestUnit: %s : Number of Pawns: %d : Max Distance: %f"),
		*GetNameSafe(this),
		NearestUnit ? *GetNameSafe(NearestUnit) : TEXT("None"),
		OutPawns.Num(),
		Distance);
	LogOnScreen(this, TargetMsg, TextColor, 0.0f);

		return NearestUnit;
	}
	else {
		return nullptr;
	}
}

uint8 ATLAIController::GetOwnerTeam()
{
	UTLAttributeComponent* UnitAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(GetPawn());

	if (UnitAttribute)
	{
		return UnitAttribute->GetTeamID();
	}

	return Cast<AGL_Unit>(GetPawn())->UnitTeamID;
}

TArray<APawn*> ATLAIController::GetPerceivedUnits(bool bUnitAlive)
{
	TArray<AActor*> OutActorsSight, OutActorsDamage, OutActorOverlap;
	TArray<APawn*> ReturnPawn;
	bool bActorAlive = false;

	USphereComponent* PawnSphere;

	PawnSphere = ITLGameCompInterface::Execute_GetRadiusComponent(GetPawn());
	PawnSphere->GetOverlappingActors(OutActorOverlap);
	

	if (bUnitAlive)
	{
		for (AActor* SenceActor : OutActorOverlap)
		{
			AGL_Unit* SenceUnit = Cast<AGL_Unit>(SenceActor);

			if (SenceUnit)
			{

				UTLAttributeComponent* UnitAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(SenceUnit);

				if (UnitAttribute)
				{
					bActorAlive = UnitAttribute->IsAlive();
				}
				else
				{
					bActorAlive = false;
				}

				if (bActorAlive && UnitAttribute->GetTeamID() != GetOwnerTeam())
				{
					ReturnPawn.AddUnique(SenceUnit);
				}
			}
			else
			{
				ATLTowerBase* TowerUnit = Cast<ATLTowerBase>(SenceActor);

				if (TowerUnit)
				{

					UTLAttributeComponent* TowerAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(TowerUnit);

					if (TowerAttribute)
					{
						bActorAlive = TowerAttribute->IsAlive();
					}
					else
					{
						bActorAlive = false;
					}

					if (bActorAlive && TowerAttribute->GetTeamID() != GetOwnerTeam())
					{
						ReturnPawn.AddUnique(TowerUnit);
					}

				}
			}

		}
	}
	else {

		for (AActor* SenceActor : OutActorOverlap)
		{
			AGL_Unit* SenceUnit = Cast<AGL_Unit>(SenceActor);
			if (SenceUnit)
			{
				//UTLAttributeComponent* UnitAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(SenceUnit);


				ReturnPawn.AddUnique(SenceUnit);

			}
			else
			{
				ATLTowerBase* TowerUnit = Cast<ATLTowerBase>(SenceActor);

				if (TowerUnit)
				{
					//UTLAttributeComponent* TowerAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(TowerUnit);

					ReturnPawn.AddUnique(TowerUnit);


				}
			}

		}

	}

	return ReturnPawn;
// 	If Sence Sight
// 		AIPerceptionComp->GetCurrentlyPerceivedActors(AISenceSight, OutActorsSight);
// 	
// 		if (bUnitAlive)
// 		{
// 	
// 			for(AActor* SenceActor : OutActorsSight)
// 			{
// 				AGL_Unit* SenceUnit = Cast<AGL_Unit>(SenceActor);
// 	
// 				if (SenceUnit)
// 				{
// 					
// 					UTLAttributeComponent* UnitAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(SenceUnit);
// 	
// 					if (UnitAttribute)
// 					{
// 						bActorAlive = UnitAttribute->IsAlive();
// 					}
// 					else
// 					{
// 						bActorAlive = false;
// 					}
// 	
// 					if (bActorAlive)
// 					{
// 						ReturnPawn.AddUnique(SenceUnit);
// 					}
// 				}
// 				else
// 				{
// 					ATLTowerBase* TowerUnit =	Cast<ATLTowerBase>(SenceActor);
// 	
// 					if (TowerUnit)
// 					{
// 	
// 						UTLAttributeComponent* TowerAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(TowerUnit);
// 	
// 						if (TowerAttribute)
// 						{
// 							bActorAlive = TowerAttribute->IsAlive();
// 						}
// 						else
// 						{
// 							bActorAlive = false;
// 						}
// 						
// 						if (bActorAlive)
// 						{
// 							ReturnPawn.AddUnique(TowerUnit);
// 						}
// 	
// 					}
// 				}
// 	
// 			}
// 		}
// 		else {
// 	
// 			for (AActor* SenceActor : OutActorsSight)
// 			{
// 				AGL_Unit* SenceUnit = Cast<AGL_Unit>(SenceActor);
// 				if (SenceUnit)
// 				{
// 					//UTLAttributeComponent* UnitAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(SenceUnit);
// 	
// 	
// 						ReturnPawn.AddUnique(SenceUnit);
// 	
// 				}
// 				else
// 				{
// 					ATLTowerBase* TowerUnit = Cast<ATLTowerBase>(SenceActor);
// 	
// 					if (TowerUnit)
// 					{
// 						//UTLAttributeComponent* TowerAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(TowerUnit);
// 	
// 							ReturnPawn.AddUnique(TowerUnit);
// 	
// 	
// 					}
// 				}
// 	
// 			}
// 	
// 		}
// 	
// 		// If Sence Damage
// 		AIPerceptionComp->GetCurrentlyPerceivedActors(AISenceDamage, OutActorsDamage);
// 	
// 		if (bUnitAlive)
// 		{
// 	
// 			for (AActor* SenceActor : OutActorsDamage)
// 			{
// 				AGL_Unit* SenceUnit = Cast<AGL_Unit>(SenceActor);
// 	
// 				if (SenceUnit)
// 				{
// 	
// 					UTLAttributeComponent* UnitAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(SenceUnit);
// 	
// 					if (UnitAttribute)
// 					{
// 						bActorAlive = UnitAttribute->IsAlive();
// 					}
// 					else
// 					{
// 						bActorAlive = false;
// 					}
// 	
// 					if (bActorAlive)
// 					{
// 						ReturnPawn.AddUnique(SenceUnit);
// 					}
// 				}
// 				else
// 				{
// 					ATLTowerBase* TowerUnit = Cast<ATLTowerBase>(SenceActor);
// 	
// 					if (TowerUnit)
// 					{
// 	
// 						UTLAttributeComponent* TowerAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(TowerUnit);
// 	
// 						if (TowerAttribute)
// 						{
// 							bActorAlive = TowerAttribute->IsAlive();
// 						}
// 						else
// 						{
// 							bActorAlive = false;
// 						}
// 	
// 						if (bActorAlive)
// 						{
// 							ReturnPawn.AddUnique(TowerUnit);
// 						}
// 	
// 					}
// 				}
// 	
// 			}
// 		}
// 		else {
// 	
// 			for (AActor* SenceActor : OutActorsDamage)
// 			{
// 				AGL_Unit* SenceUnit = Cast<AGL_Unit>(SenceActor);
// 				if (SenceUnit)
// 				{
// 					//UTLAttributeComponent* UnitAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(SenceUnit);
// 	
// 						ReturnPawn.AddUnique(SenceUnit);
// 				}
// 				else
// 				{
// 					ATLTowerBase* TowerUnit = Cast<ATLTowerBase>(SenceActor);
// 	
// 					if (TowerUnit)
// 					{
// 						//UTLAttributeComponent* TowerAttribute = ITLGameCompInterface::Execute_GetAttriblueComponent(TowerUnit);
// 	
// 	
// 							ReturnPawn.AddUnique(TowerUnit);
// 	
// 					}
// 				}
// 	
// 			}
// 	
// 		}

//	return ReturnPawn;

}	

bool ATLAIController::ExecutePrimaryAttack_Implementation()
{
	ITLGameplayInterface* InterfaceActor = Cast<ITLGameplayInterface>(GetPawn());
	if (!InterfaceActor)
	{
		FString FailedMsg = FString::Printf(TEXT("Pawn does not have TLGamplayInterface"));
		LogOnScreen(this, FailedMsg, FColor::Red);
		return false;
	}
	ITLGameplayInterface::Execute_PrimaryAttack(GetPawn());
	return true;
}

bool ATLAIController::ExecutePrimaryAbility_Implementation()
{
	ITLGameplayInterface* InterfaceActor = Cast<ITLGameplayInterface>(GetPawn());
	if (!InterfaceActor)
	{
		FString FailedMsg = FString::Printf(TEXT("Pawn does not have TLGamplayInterface"));
		LogOnScreen(this, FailedMsg, FColor::Red);
		return false;
	}
	ITLGameplayInterface::Execute_PrimaryAbility(GetPawn());
	return true;
}

bool ATLAIController::ExecuteSecondaryAbility_Implementation()
{
	ITLGameplayInterface* InterfaceActor = Cast<ITLGameplayInterface>(GetPawn());
	if (!InterfaceActor)
	{
		FString FailedMsg = FString::Printf(TEXT("Pawn does not have TLGamplayInterface"));
		LogOnScreen(this, FailedMsg, FColor::Red);
		return false;
	}
	ITLGameplayInterface::Execute_SecondaryAbility(GetPawn());
	return true;
}

bool ATLAIController::ExecuteTertiaryAbility_Implementation()
{
	ITLGameplayInterface* InterfaceActor = Cast<ITLGameplayInterface>(GetPawn());
	if (!InterfaceActor)
	{
		FString FailedMsg = FString::Printf(TEXT("Pawn does not have TLGamplayInterface"));
		LogOnScreen(this, FailedMsg, FColor::Red);
		return false;
	}

	ITLGameplayInterface::Execute_TertiaryAbility(GetPawn());
	return true;
}

bool ATLAIController::ExecuteSpecialAttack_Implementation()
{
	ITLGameplayInterface* InterfaceActor = Cast<ITLGameplayInterface>(GetPawn());
	if (!InterfaceActor)
	{
		FString FailedMsg = FString::Printf(TEXT("Pawn does not have TLGamplayInterface"));
		LogOnScreen(this, FailedMsg, FColor::Red);
		return false;
	}

	ITLGameplayInterface::Execute_SpecialAttack(GetPawn());
	return true;
}


bool ATLAIController::SetCommands_Implementation(EUnitCommand UnitCommand, FVector TargetLocation, AActor* TargetUnit)
{
	if (!PawnAttribute)
	{
		PawnAttribute = UTLAttributeComponent::GetAttribute(GetPawn());

		if (!PawnAttribute)
		{
			return false;
		}
	}

	if (!PawnAttribute->IsAlive())
	{
		return false;
	}

	if (!GetBlackboardComponent())
	{
		return false;
	}


	Command = UnitCommand;
	uint8 IntCommand = (uint8)Command;

	GetBlackboardComponent()->SetValueAsEnum("UnitCommand", IntCommand);
	GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetUnit);
	GetBlackboardComponent()->SetValueAsVector("TargetLocation", TargetLocation);

	return true;
}

AActor* ATLAIController::GetTargetActor()
{

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("TargetActor"));
}

void ATLAIController::ResetCommand_Implementation()
{
	Command = EUnitCommand::UnitIdle;

	ResetTarget(nullptr);

	RecallTimer = 0.0f;


}

void ATLAIController::ResetTarget_Implementation(AActor* ActorTarget)
{

}

void ATLAIController::OnUnitDamaged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(),GetPawn(),InstigatorActor,Delta,FVector::ZeroVector,FVector::ZeroVector);
}

void ATLAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

ETeamAttitude::Type ATLAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (!PawnAttribute)
	{
		//Missing AttrubuteComponent
		return ETeamAttitude::Friendly;
	}
	
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{

		//UTLAttributeComponent::GetAttribute(OtherPawn)
		if (PawnAttribute->IsFriendly(OtherPawn))
		{
			return ETeamAttitude::Friendly;
		}
		else
		{
			return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Friendly;
}

void ATLAIController::InitAI(TArray<FTransform> RelayPoints)
{
	MoveRelayPoints = RelayPoints;

	UnitPawn = Cast<AGL_Unit>(GetPawn());
	PawnAttribute = UTLAttributeComponent::GetAttribute(GetPawn());

	PawnAttribute->OnHealthChanged.AddDynamic(this, &ATLAIController::OnUnitDamaged);


	if (!PawnAttribute)
	{
		return;
	}

	if (CurrentIndex < MoveRelayPoints.Num()) {

		SetCommands(EUnitCommand::UnitMove, MoveRelayPoints[CurrentIndex].GetLocation(),nullptr);

	}

}
