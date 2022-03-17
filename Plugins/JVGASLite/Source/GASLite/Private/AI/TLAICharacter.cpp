// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLAICharacter.h"
#include <Perception/PawnSensingComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>
#include <DrawDebugHelpers.h>
#include "Component/TLAttributeComponent.h"
#include <BrainComponent.h>
#include "UI/TLWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Component/TLActionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/TLGameCompInterface.h"
#include "Library/TLGameplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "../TLActionRoguelike.h"
#include "AI/TLAIController.h"
#include "Components/WidgetComponent.h"
#include <Components/SceneComponent.h>

// Sets default values
ATLAICharacter::ATLAICharacter()
{

	PawnSenseComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UTLActionComponent>("ActionComp");

	TargetRadius = CreateDefaultSubobject<USphereComponent>("TargetRadius");
	TargetRadius->SetupAttachment(GetMesh());
	TargetRadius->SetSphereRadius(1000.f);

	HealthBarPos = CreateDefaultSubobject<USceneComponent>("HealthBarPos");
	HealthBarPos->SetupAttachment(GetMesh());

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TargetActorKey = "TargetActor";
	TimeToHitParamName = "TimeToHit";

}

bool ATLAICharacter::IsAlive()
{
	return AttributeComp->IsAlive();
}

void ATLAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSenseComp->OnSeePawn.AddDynamic(this, &ATLAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATLAICharacter::OnAIHealthChange);
	TargetRadius->OnComponentBeginOverlap.AddDynamic(this, &ATLAICharacter::OnAIRadiusBeginOverlap);
	TargetRadius->OnComponentEndOverlap.AddDynamic(this, &ATLAICharacter::OnAIRadiusEndOverlap);
	//FindNearestTarget();

}

void ATLAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{


	Super::EndPlay(EndPlayReason);

}

USphereComponent* ATLAICharacter::GetRadiusComponent_Implementation()
{
	return TargetRadius;
}

bool ATLAICharacter::SpecialAttack_Implementation()
{
	return ActionComp->StartActionByName(this, "SpecialAttack");
}

bool ATLAICharacter::TertiaryAbility_Implementation()
{
	return ActionComp->StartActionByName(this, "Ability_03");
}

bool ATLAICharacter::SecondaryAbility_Implementation()
{
	return ActionComp->StartActionByName(this, "Ability_02");
}

bool ATLAICharacter::PrimaryAbility_Implementation()
{
	
	return ActionComp->StartActionByName(this, "Ability_01");
}

bool ATLAICharacter::PrimaryAttack_Implementation()
{

	return ActionComp->StartActionByName(this, "PrimaryAttack");
}

USkeletalMeshComponent* ATLAICharacter::GetActorMesh_Implementation()
{
	return GetMesh();
}

float ATLAICharacter::GetRadius_Implementation()
{
	return TargetRadius->GetScaledSphereRadius();
}

UTLAttributeComponent* ATLAICharacter::GetAttriblueComponent_Implementation()
{
	return AttributeComp;
}

UTLActionComponent* ATLAICharacter::GetActionComponent_Implementation()
{
	return ActionComp;
}

void ATLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATLAICharacter::OnAIRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AttributeComp)
	{
		return;
	}

// 	if (!AttributeComp->IsAlive())
// 	{
// 		return;
// 	}
// 
// 	if (GetTargetActor() != OtherActor)
// 	{
// 		if (!AttributeComp->IsFriendly(OtherActor))
// 		{
// 			SetTargetActor(OtherActor);
// 		}
// 		else
// 		{
// 			//Do Something Else...
// 		}
// 	}
}

void ATLAICharacter::OnAIRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void ATLAICharacter::OnAIHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{

// 		if (InstigatorActor != this)
// 		{
// 			SetTargetActor(InstigatorActor);
// 		}

		if (ActiveHealthBar == nullptr)
		{

			ActiveHealthBar = CreateWidget<UTLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->WorldOffset = HealthBarPos->GetRelativeLocation();
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			PawnSenseComp->OnSeePawn.RemoveDynamic(this, &ATLAICharacter::OnPawnSeen);
			AttributeComp->OnHealthChanged.RemoveDynamic(this, &ATLAICharacter::OnAIHealthChange);
			TargetRadius->OnComponentBeginOverlap.RemoveDynamic(this, &ATLAICharacter::OnAIRadiusBeginOverlap);
			TargetRadius->OnComponentEndOverlap.RemoveDynamic(this, &ATLAICharacter::OnAIRadiusEndOverlap);

			//stop Bt
			AAIController* AIC = Cast<AAIController>(GetController());

			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true); 
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			//Set lifespan
			SetLifeSpan(10.0f);
		}
	}

}

UBlackboardComponent* ATLAICharacter::GetAIControllerBlackboard()
{
	AAIController* AIC = Cast<AAIController>(GetController());

	if (!AIC)
	{
		return nullptr;
	}

	// Return if the AI pawn is dead
	if (!AttributeComp->IsAlive())
	{
		return nullptr;
	}

	return AIC->GetBlackboardComponent();
}

void ATLAICharacter::AssignTarget(AActor* NewTarget)
{
	if (!GetAIControllerBlackboard())
	{
		return;
	}

	if (NewTarget == nullptr)
	{
		return;
	}

	OriginPoint = NewTarget;

	SetTargetActor(NewTarget);
}

void ATLAICharacter::FindNearestTarget()
{
	if (!GetAIControllerBlackboard())
	{
		return;
	}

	TArray<AActor*> OutActors, OutFriendlyActors, OutEnemyActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(),UTLGameCompInterface::StaticClass(), OutActors);
	//UTLGameplayFunctionLibrary::GetAllFreindlyActors(this, OutActors, OutFriendlyActors);
	UTLGameplayFunctionLibrary::GetAllEnemyActors((AActor*)this, OutActors, OutEnemyActors);

	OutEnemyActors[UKismetMathLibrary::RandomInteger(OutEnemyActors.Num())];

	float NearestDistance = 10000000.0f;
	AActor* TargetedActor = nullptr;
	for (AActor* NearestActor : OutEnemyActors)
	{

		float Distanceto = FVector::Distance(NearestActor->GetActorLocation(), GetActorLocation());

		if (Distanceto < NearestDistance)
		{
			NearestDistance = Distanceto;
			TargetedActor = NearestActor;
		}
	}

	if (TargetedActor == nullptr)
	{
		return;
	}
		SetTargetActor(TargetedActor);
}

void ATLAICharacter::SetTargetActor(AActor* NewTarget)
{

	if (CurrentTarget != nullptr)
	{
		OldAttriComp = ITLGameCompInterface::Execute_GetAttriblueComponent(CurrentTarget);
	}
	if (NewTarget != nullptr)
	{
		NewAttriComp = ITLGameCompInterface::Execute_GetAttriblueComponent(NewTarget);
	}

	if (!GetTLAIController()) {
		return;
	}

	if (!GetAIControllerBlackboard())
	{
		if (OldAttriComp)
		{
			OldAttriComp->OnHealthChanged.RemoveDynamic(this, &ATLAICharacter::OnTargetHealthChange);
		}

		NewAttriComp->OnHealthChanged.AddDynamic(this, &ATLAICharacter::OnTargetHealthChange);

		CurrentTarget = NewTarget;

		FString WarningMsg = FString::Printf(TEXT("[%s] Warning! AI Controller Does Not Exist "),
			*GetNameSafe(this),
			NewTarget ? *GetNameSafe(NewTarget) : TEXT("None"));
		LogOnScreen(this, WarningMsg, FColor::Red, 10.0f);
		return;
	}
	// Return if the AI pawn is dead
	if (!AttributeComp->IsAlive())
	{
		GetAIControllerBlackboard()->SetValueAsObject(TargetActorKey, nullptr);
		return;
	}

	if (!NewTarget)
	{

		if (OldAttriComp)
		{
			OldAttriComp->OnHealthChanged.RemoveDynamic(this, &ATLAICharacter::OnTargetHealthChange);
		}

		CurrentTarget = nullptr;
		GetAIControllerBlackboard()->SetValueAsObject("TargetActor", nullptr);
		FString TargetMsg = FString::Printf(TEXT("[%s] Setting Key TargetActor: %s "),
			*GetNameSafe(this),
			NewTarget ? *GetNameSafe(NewTarget) : TEXT("None"));
		LogOnScreen(this, TargetMsg, FColor::Green, 10.0f);
		LogOnScreen(this, TEXT("TargetActor Invalid"), FColor::Red, 10.0f);
		return;



		GetTLAIController()->SetCommands(EUnitCommand::UnitIdle, FVector::ZeroVector, nullptr);

		//GetAIControllerBlackboard()->SetValueAsObject("TargetActor", nullptr);
		return;
	}

	NewAttriComp->OnHealthChanged.AddDynamic(this, &ATLAICharacter::OnTargetHealthChange);

	CurrentTarget = NewTarget;

	GetTLAIController()->SetCommands(EUnitCommand::UnitMoveAttack, FVector::ZeroVector, NewTarget);
}

AActor* ATLAICharacter::GetTargetActor() const
{

	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

ATLAIController* ATLAICharacter::GetTLAIController() 
{
	ATLAIController* TLAIController = Cast<ATLAIController>(GetController());

	if (!TLAIController)
	{
		return nullptr;
	}

	return TLAIController;
}

void ATLAICharacter::OnPawnSeen(APawn* Pawn)
{
	// Ignore if target already set
// 	if (GetTargetActor() != Pawn)
// 	{
// 		if (!AttributeComp->IsFriendly(Pawn)) 
// 		{
// 			SetTargetActor(Pawn);
// 
// 			MulticastPawnSeen();
// 		}
// 
// 	}



	//DrawDebugString(GetWorld(), GetActorLocation(), "Player SPOTTED", nullptr, FColor::White, 4.0f, true);

}

void ATLAICharacter::MulticastPawnSeen_Implementation()
{
	if (!SpottedWidgetClass)
	{
		return;
	}

	UTLWorldUserWidget* NewWidget = CreateWidget<UTLWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;
		// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
		// May end up behind the minion health bar otherwise.
		NewWidget->AddToViewport(10);
	}
}

void ATLAICharacter::OnTargetHealthChange_Implementation(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
// 	if (NewHealth <= 0.0f)
// 	{
// 		//Find New Target
// 		//MulticastRemoveTarget(nullptr);
// 
// 		SetTargetActor(OriginPoint);
// 	}
}
