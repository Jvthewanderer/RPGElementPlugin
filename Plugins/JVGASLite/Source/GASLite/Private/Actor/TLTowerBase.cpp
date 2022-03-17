// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLTowerBase.h"
#include "Component/TLAttributeComponent.h"
#include "Component/TLActionComponent.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Components/SphereComponent.h>
#include <BrainComponent.h>
#include <AIController.h>
#include <DrawDebugHelpers.h>
#include "UI/TLWorldUserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Components/SkeletalMeshComponent.h>
#include "Interface/TLGameCompInterface.h"
#include "../TLActionRoguelike.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Library/TLGameplayFunctionLibrary.h"
#include "AI/TLAICharacter.h"
#include "Components/WidgetComponent.h"
#include <Components/SceneComponent.h>
#include <AI/TLAIController.h>

// Sets default values
ATLTowerBase::ATLTowerBase()
{
	AttributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AttributeComp");
	
	ActionComp = CreateDefaultSubobject<UTLActionComponent>("ActionComp");

	Mesh= CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

/*	BeamNSystem = CreateDefaultSubobject<UNiagaraComponent>("BeamParticle");*/

	TargetRadius = CreateDefaultSubobject<USphereComponent>("TargetRadius");
	TargetRadius->SetupAttachment(Mesh);
	TargetRadius->SetSphereRadius(3000);

	TargetPointComponent = CreateDefaultSubobject<USceneComponent>("TargetPoints");
	TargetPointComponent->SetupAttachment(Mesh);

	HealthBarPos = CreateDefaultSubobject<USceneComponent>("HealthBarPos");
	HealthBarPos->SetupAttachment(GetMesh());

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bReplicates = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeToHitParamName = "TimeToHit";
	BeamEnd = "BeamEnd";
	SocketName = "Muzzle_01";
	BeamColorName = "BeamColor";


	TargetDistance = 10000.f;

}

void ATLTowerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATLTowerBase::OnTowerHealthChange);

}

void ATLTowerBase::OnTowerHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{

		if (InstigatorActor != this)
		{
			//ServerSetTargetActor(InstigatorActor);
		}

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

		//GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			//stop Bt
			AAIController* AIC = Cast<AAIController>(GetController());

			if (AIC)
			{
				if (AIC->GetBrainComponent())
				{
					AIC->GetBrainComponent()->StopLogic("Killed");
				}
			}

			//Set lifespan
			SetLifeSpan(4.0f);
		}
	}
}


// Called when the game starts or when spawned
void ATLTowerBase::BeginPlay()
{
	Super::BeginPlay();
	TLAIController = Cast<ATLAIController>(GetController());

	//BeamNSystem->SetColorParameter(BeamColorName, FLinearColor::Black);
}

// Called every frame
void ATLTowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USceneComponent* ATLTowerBase::GetTargetComponent_Implementation()
{
	return TargetPointComponent;
}

USphereComponent* ATLTowerBase::GetRadiusComponent_Implementation()
{
	return TargetRadius;
}

bool ATLTowerBase::PrimaryAttack_Implementation()
{

	ActionComp->StartActionByName(this, "PrimaryAttack");

	return true;
}

USkeletalMeshComponent* ATLTowerBase::GetActorMesh_Implementation()
{
	return Mesh;
}

float ATLTowerBase::GetRadius_Implementation()
{
	return TargetRadius->GetScaledSphereRadius();
}

UTLAttributeComponent* ATLTowerBase::GetAttriblueComponent_Implementation()
{
	return AttributeComp;
}

UTLActionComponent* ATLTowerBase::GetActionComponent_Implementation()
{
	return ActionComp;
}

UBlackboardComponent* ATLTowerBase::GetAIControllerBlackboard()
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

AActor* ATLTowerBase::GetTargetActor()
{
	return TLAIController->GetTargetActor();
}

void ATLTowerBase::OnTargetHealthChange_Implementation(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f)
	{
		RemoveTargetActor(nullptr);
	}
}

void ATLTowerBase::SetTargetActor(AActor* NewTarget)
{
	if (CurrentTarget != nullptr)
	{
		OldAttriComp = ITLGameCompInterface::Execute_GetAttriblueComponent(CurrentTarget);
	}
	if (NewTarget != nullptr)
	{
		NewAttriComp = ITLGameCompInterface::Execute_GetAttriblueComponent(NewTarget);
	}

	// if client, Set currentTarget using New Target.
	if (!GetAIControllerBlackboard())
	{

		if (OldAttriComp)
		{
			OldAttriComp->OnHealthChanged.RemoveDynamic(this, &ATLTowerBase::OnTargetHealthChange);
		}

		NewAttriComp->OnHealthChanged.AddDynamic(this, &ATLTowerBase::OnTargetHealthChange);

		CurrentTarget = NewTarget;

		FString WarningMsg = FString::Printf(TEXT("[%s] Warning! AI Controller Does Not Exist "),
			*GetNameSafe(this),
			NewTarget ? *GetNameSafe(NewTarget) : TEXT("None"));
		LogOnScreen(this, WarningMsg, FColor::Red, 10.0f);
		return;
	}

	// if Target is Null, Set Current Target to Nullptr
	if (!NewTarget)
	{
		if (OldAttriComp)
		{
			OldAttriComp->OnHealthChanged.RemoveDynamic(this, &ATLTowerBase::OnTargetHealthChange);
		}

		CurrentTarget = nullptr;
		GetAIControllerBlackboard()->SetValueAsObject("TargetActor", nullptr);
		FString TargetMsg = FString::Printf(TEXT("[%s] Setting Key TargetActor: %s "),
			*GetNameSafe(this),
			NewTarget ? *GetNameSafe(NewTarget) : TEXT("None"));
		LogOnScreen(this, TargetMsg, FColor::Green, 10.0f);
		LogOnScreen(this, TEXT("TargetActor Invalid"), FColor::Red, 10.0f);
		return;
	}

	if (NewTarget != CurrentTarget)
	{
		NewAttriComp->OnHealthChanged.AddDynamic(this, &ATLTowerBase::OnTargetHealthChange);
	}

	CurrentTarget = NewTarget;

	GetAIControllerBlackboard()->SetValueAsObject("TargetActor", NewTarget);
}




void ATLTowerBase::AddTargetActor(AActor* OtherActor)
{
	if (!AttributeComp->IsAlive())
	{
		MulticastSetTarget(nullptr);
/*		BeamNSystem->SetColorParameter(BeamColorName, FLinearColor::Black);*/
		return;
	}

	//If Client
	if (!CurrentTarget)
	{
		SetTargetActor(OtherActor);
/*		BeamNSystem->SetColorParameter(BeamColorName, BeamColor);*/
	}

}

void ATLTowerBase::RemoveTargetActor(AActor* OtherActor)
{
	if (!AttributeComp->IsAlive())
	{
		MulticastSetTarget(nullptr);
/*		BeamNSystem->SetColorParameter(BeamColorName, FLinearColor::Black);*/
		return;
	}

	TArray<AActor*> OverlappingActors, OutEnemyActors;
	AActor* tempTarget = nullptr;
	float ClosestTarget = TargetRadius->GetScaledSphereRadius();
	TargetRadius->GetOverlappingActors(OverlappingActors);

	if (UTLGameplayFunctionLibrary::GetAllEnemyActors((AActor*)this, OverlappingActors, OutEnemyActors))
	{

		/* TODO: Look at this again for a better way to use arrays*/

		for (AActor* OverlappedActor : OutEnemyActors)
		{

			if (OverlappedActor != this)
			{
				if (!AttributeComp->IsFriendly(OverlappedActor))
				{
					UTLAttributeComponent* AttrComp = Cast<UTLAttributeComponent>(OverlappedActor->GetComponentByClass(UTLAttributeComponent::StaticClass()));
					if (AttrComp && AttrComp->IsAlive())
					{
						float distanceTo = FVector::Distance(GetActorLocation(), OverlappedActor->GetActorLocation());
						if (distanceTo < ClosestTarget)
						{
							ClosestTarget = distanceTo;
							tempTarget = OverlappedActor;
						}
					}
				}
			}

		}

		if (tempTarget)
		{
			MulticastSetTarget(tempTarget);
			return;
		}

	}


	//MulticastSetTarget(nullptr);
/*	BeamNSystem->SetColorParameter(BeamColorName, FLinearColor::Black);*/
	return;
}

void ATLTowerBase::MulticastAddTarget_Implementation(AActor* OtherActor)
{
	AddTargetActor(OtherActor);
}

void ATLTowerBase::MulticastRemoveTarget_Implementation(AActor* OtherActor)
{
	RemoveTargetActor(OtherActor);
}


void ATLTowerBase::MulticastSetTarget_Implementation(AActor* NewTarget)
{
	SetTargetActor(NewTarget);
}

