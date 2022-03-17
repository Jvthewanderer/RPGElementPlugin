// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLActionTargetActor.h"
#include "Net/UnrealNetwork.h"
#include "Action/TLAction.h"
#include <DrawDebugHelpers.h>
#include "Component/TLActionComponent.h"
#include "Library/TLGameplayFunctionLibrary.h"
#include "Action/TLActionEffect.h"
#include "RLCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Interface/TLGameCompInterface.h"
#include "Components/SceneComponent.h"
#include "AI/TLAIController.h"

// Sets default values
ATLActionTargetActor::ATLActionTargetActor()
{
	Radius = 100.f;
	bReplicates = true;
	SetReplicatingMovement(true);
}

void ATLActionTargetActor::EndPlay(EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

void ATLActionTargetActor::StartTargeting(UTLAction* Action)
{
	OwningAction = Action;
	MasterPlayer = Cast<AGL_Unit>(Action->GetOwningComponent()->GetOwner());
	MasterPC = MasterPlayer->GetController<APlayerController>();
	MasterAIPC = MasterPlayer->GetController<ATLAIController>();
	Damage = OwningAction->GetBaseDamage();

	FTimerDelegate Delegate;

	RepData.bIsRunning = true;
	RepData.Instigator = MasterPlayer; 
	bIsTargeting = true;

	if (MasterPC && MasterPC->HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckOverlappedActors, this, &ATLActionTargetActor::ConfirmTargetingAndContinue, 0.02f, true);
	}
	else
	{
		SetActorHiddenInGame(true);
	}

}


bool ATLActionTargetActor::ExecuteEffects_Implementation()
{
	RepData.bIsRunning = false;
	return true;
}

void ATLActionTargetActor::ServerExecuteEffect_Implementation()
{
	MulticastExecuteEffect();
}

void ATLActionTargetActor::MulticastExecuteEffect_Implementation()
{
	ExecuteEffects();
}

void ATLActionTargetActor::ConfirmTargetingAndContinue()
{

	FVector ViewLocation;
	if (/*Server*/GetOwner()->HasAuthority() && MasterPC)
	{
		GetPlayerLookingPoint(ViewLocation);
	}
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Red, false, 0.0f);
}

bool ATLActionTargetActor::GetPlayerLookingPoint(FVector& OutViewPoint)
{
	FVector ViewPoint;
	FRotator ViewRotation;
	FHitResult HitResult;
	FCollisionQueryParams QueryParam;
	QueryParam.bTraceComplex = true;

	if (MasterPC)
	{
		MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);

		if (MasterPlayer)
		{
			QueryParam.AddIgnoredActor(MasterPlayer->GetUniqueID());
		}

		bool HitTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * 5000.f, ECC_Visibility, QueryParam);
		if (HitTrace)
		{
			OutViewPoint = HitResult.ImpactPoint;
		}
		else
		{
			OutViewPoint = FVector();
		}
		
		return HitTrace;

	}
	else if(MasterAIPC)
	{
		USceneComponent* TargetPoint = ITLGameCompInterface::Execute_GetTargetComponent(MasterAIPC->GetTargetActor());

		if (MasterPlayer)
		{
			QueryParam.AddIgnoredActor(MasterPlayer->GetUniqueID());
		}

		bool HitTrace = GetWorld()->LineTraceSingleByChannel(HitResult, MasterPlayer->GetActorLocation(), TargetPoint->GetComponentLocation(), ECC_Visibility, QueryParam);
		if (HitTrace)
		{
			OutViewPoint = HitResult.ImpactPoint;
		}
		else
		{
			OutViewPoint = FVector();
		}

		return HitTrace;
	}

	return false;
}

void ATLActionTargetActor::ConfirmTargets_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CheckOverlappedActors);
	ApplyDamageToTargets();
	bIsTargeting = false;
}

void ATLActionTargetActor::ServerConfirmTargets_Implementation()
{
	ConfirmTargets();
}

void ATLActionTargetActor::CancelTargeting_Implementation()
{

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CheckOverlappedActors);
	RepData.bIsRunning = false;
	RepData.Instigator = MasterPlayer;

}

void ATLActionTargetActor::ApplyDamageToTargets()
{

	TArray<FOverlapResult> Overlaps;
	bool TraceComplex = false;

	FCollisionQueryParams CollisonQueryParams;
	CollisonQueryParams.bTraceComplex = TraceComplex;
	CollisonQueryParams.bReturnPhysicalMaterial = false;

	if (MasterPlayer)
	{
		CollisonQueryParams.AddIgnoredActor(MasterPlayer->GetUniqueID());
	}

	OverlappedActors.Empty();

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Red, false, 10.0f);
	bool HitOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		CollisonQueryParams);

	if (HitOverlap)
	{
		for (FOverlapResult Overlap : Overlaps)
		{
			APawn* PawnOverlaped = Cast<APawn>(Overlap.GetActor());
			if (PawnOverlaped && !OverlappedActors.Contains(PawnOverlaped))
			{
				OverlappedActors.Add(PawnOverlaped);
			}
		}
	}


	if (OverlappedActors.Num() > 0)
	{
		FHitResult SweepResult;
		for (AActor* TargetedActor : OverlappedActors)
		{


			UTLActionComponent* ActionComp = Cast<UTLActionComponent>(TargetedActor->GetComponentByClass(UTLActionComponent::StaticClass()));
			if (ActionComp)
			{
				if (UTLGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), TargetedActor, Damage, SweepResult))
				{
					if (ActionComp && HasAuthority())
					{
						for (TSubclassOf<UTLActionEffect> AEClass : ActionEffectClasses)
						{
							ActionComp->AddAction(GetInstigator(), AEClass);
						}

					}
				}
			}
		}
	}

	OnConfirmTargetsExecuted.Broadcast(OverlappedActors);
}

bool ATLActionTargetActor::IsRunning() const
{
	return RepData.bIsRunning;
}

void ATLActionTargetActor::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		//StartAction(RepData.Instigator);
	}
	else
	{
		ExecuteEffects();
	}
}

void ATLActionTargetActor::OnRep_bIsTargeting()
{
	if (!bIsTargeting)
	{
		ConfirmTargets();
	}
}

// Called when the game starts or when spawned
void ATLActionTargetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATLActionTargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector lookPoint;
	GetPlayerLookingPoint(lookPoint);
	//DrawDebugSphere(GetWorld(), lookPoint, Radius,32, FColor::Red);

}

void ATLActionTargetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATLActionTargetActor, RepData);
	DOREPLIFETIME(ATLActionTargetActor, bIsTargeting);
}