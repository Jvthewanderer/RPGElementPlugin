// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLAction_Targeting.h"
#include "Component/TLActionComponent.h"
#include <Kismet/GameplayStatics.h>
#include "../TLActionRoguelike.h"
#include "Action/TLActionTargetActor.h"
#include <Camera/CameraComponent.h>
#include "RLCharacter.h"
#include "AI/TLAIController.h"




UTLAction_Targeting::UTLAction_Targeting()
{
	TargetDistance = 5000.0f;
	AttackAnimDelay = 0.2f;
	ActionName = "Ability_01";
	bTraceLocation = true;
	bOnOwningActor = false;
}

void UTLAction_Targeting::StartAction_Implementation(AActor* Instigator)
{
	bTargeting = true;

	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UTLActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);

	MontageDuration = UTLGameplayFunctionLibrary::PlayActorAnimMontage(Instigator, AttackAnim);

	if (TargetActor && !TargetActor->IsRunning())
	{
		TargetActor->StartTargeting(this);
		return;
	}

	AGL_Unit* InstigatorCharacter = Cast<AGL_Unit>(Instigator);
	if (InstigatorCharacter && !TargetActor)
	{
		// If Client
		if (GetOwningComponent()->GetOwner()->HasAuthority())
		{
			SpawnTarget(InstigatorCharacter);
		}
		
		ActionComp->SetTargeting(true);
	}

}

void UTLAction_Targeting::StopAction_Implementation(AActor* Instigator)
{
	bTargeting = false;

	Super::StopAction_Implementation(Instigator);
	
	ActionComp->SetTargeting(false);

	if (TargetActor)
	{
		TargetActor->Destroy();
		TargetActor = nullptr;
	}


}

void UTLAction_Targeting::TriggerAction_Implementation(AActor* Instigator)
{
	Super::TriggerAction_Implementation(Instigator);

	MontageDuration = UTLGameplayFunctionLibrary::PlayActorAnimMontage(Instigator, ConfirmAttackAnim);

	if (TargetActor)
	{
		TargetActor->ServerExecuteEffect();
		StartCooldown();
	}

}

void UTLAction_Targeting::AttackDelay_Elapsed(AActor* InstigatorCharacter)
{
	StopAction_Implementation(InstigatorCharacter);
}

void UTLAction_Targeting::ServerSpawnTarget_Implementation(AActor* InstigatorCharacter)
{
	SpawnTarget(InstigatorCharacter);
}

void UTLAction_Targeting::StartTrackingLocation(AActor* InstigatorCharacter)
{
	FTimerDelegate Delegate;

	Delegate.BindUFunction(this, "SetTargetLocation_Elapsed", InstigatorCharacter);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SetTargetLocation, Delegate, 0.02f, true);

}

void UTLAction_Targeting::StopTrackingLocation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_SetTargetLocation);
}

void UTLAction_Targeting::SpawnTarget(AActor* InstigatorCharacter)
{
	if (TargetActor)
	{
		return;
	}

	if (ensure(TargetActorClass))
	{
		FVector TraceStart;
		FVector TraceEnd;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Cast<APawn>(InstigatorCharacter);
		SpawnParams.Owner = InstigatorCharacter;

// 		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (InstigatorCharacter->GetPlayerCamera()->GetForwardVector() * 15);
// 		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * TargetDistance);


		ARLCharacter* PlayerCharacter = Cast<ARLCharacter>(InstigatorCharacter);
		APawn* AIPawn = Cast<APawn>(InstigatorCharacter);
		ATLAIController* AIController = Cast<ATLAIController>(AIPawn->GetController());

		if (PlayerCharacter)
		{
			TraceStart = PlayerCharacter->GetPawnViewLocation() + (PlayerCharacter->GetPlayerCamera()->GetForwardVector() * 15);
			if (bUseForwardDirection)
			{
				TraceStart = PlayerCharacter->GetActorLocation();
				TraceEnd = PlayerCharacter->GetMouseLocation();
			}
			else
			{
				TraceEnd = TraceStart + (PlayerCharacter->GetControlRotation().Vector() * 5000);
			}

		}
		else if (AIController) {

			if (AIController->GetTargetActor())
			{
				USceneComponent* TargetPoint = ITLGameCompInterface::Execute_GetTargetComponent(AIController->GetTargetActor());

				TraceStart = AIPawn->GetActorLocation();

				if (TargetPoint)
				{
					TraceEnd = TargetPoint->GetComponentLocation();
				}
				else
				{
					TraceEnd = TraceStart + (InstigatorCharacter->GetActorForwardVector() * 5000);
				}
			}

		}
		else
		{
			TraceStart = PlayerCharacter->GetPawnViewLocation() + (PlayerCharacter->GetPlayerCamera()->GetForwardVector() * 15);;
			TraceEnd = TraceStart + (PlayerCharacter->GetControlRotation().Vector() * TargetDistance);
		}

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FHitResult Hit;

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params);

		if (bBlockingHit)
		{
			TraceEnd = Hit.ImpactPoint;
		}
		else
		{
			TraceEnd = InstigatorCharacter->GetActorLocation();
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();

		if (!bTraceLocation)
		{
			TraceEnd = InstigatorCharacter->GetActorLocation();
		}


		FTransform SpawnTM = FTransform(ProjRotation, TraceEnd);
		//UGameplayStatics::SuggestProjectileVelocity(GetWorld(), InstigatorCharacter->GetControlRotation().Vector() * 5000, TraceStart, TraceEnd,200.0f);

		TargetActor = GetWorld()->SpawnActor<ATLActionTargetActor>(TargetActorClass, SpawnTM, SpawnParams);



		TargetActor->StartTargeting(this);
	}

	StartTrackingLocation(InstigatorCharacter);
	
}

void UTLAction_Targeting::SetTargetLocation_Elapsed(AActor* InstigatorCharacter)
{
	FVector TraceStart;
	FVector TraceEnd;

	if (TargetActor)
	{
		if (bTraceLocation)
		{


			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = Cast<APawn>(InstigatorCharacter);

// 			TraceStart = InstigatorCharacter->GetPawnViewLocation() + (InstigatorCharacter->GetPlayerCamera()->GetForwardVector() * 15);;
// 			TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * TargetDistance);
			FVector TraceEndZ = TraceEnd + (FVector(0,0,-100000));

			ARLCharacter* PlayerCharacter = Cast<ARLCharacter>(InstigatorCharacter);
			APawn* AIPawn = Cast<APawn>(InstigatorCharacter);
			ATLAIController* AIController = Cast<ATLAIController>(AIPawn->GetController());

			if (PlayerCharacter)
			{
				TraceStart = PlayerCharacter->GetPawnViewLocation() + (PlayerCharacter->GetPlayerCamera()->GetForwardVector() * 15);
				if (bUseForwardDirection)
				{
					TraceStart = PlayerCharacter->GetActorLocation();
					TraceEnd = PlayerCharacter->GetMouseLocation();
				}
				else
				{
					TraceEnd = TraceStart + (PlayerCharacter->GetControlRotation().Vector() * 5000);
				}

			}
			else if (AIController) {

				if (AIController->GetTargetActor())
				{
					USceneComponent* TargetPoint = ITLGameCompInterface::Execute_GetTargetComponent(AIController->GetTargetActor());

					TraceStart = AIPawn->GetActorLocation();

					if (TargetPoint)
					{
						TraceEnd = TargetPoint->GetComponentLocation();
					}
					else
					{
						TraceEnd = TraceStart + (InstigatorCharacter->GetActorForwardVector() * 5000);
					}
				}

			}
			else
			{
				TraceStart = PlayerCharacter->GetPawnViewLocation() + (PlayerCharacter->GetPlayerCamera()->GetForwardVector() * 15);;
				TraceEnd = TraceStart + (PlayerCharacter->GetControlRotation().Vector() * TargetDistance);
			}

			FCollisionShape Shape;
			Shape.SetSphere(20.0f);

			// Ignore Player
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(InstigatorCharacter);

			FCollisionObjectQueryParams ObjParams;
			ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

			FHitResult Hit, ZHit;

			bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params);

			if (bBlockingHit)
			{
				TraceEnd = Hit.ImpactPoint;
			}
			else
			{
				GetWorld()->SweepSingleByObjectType(ZHit, TraceEnd, TraceEndZ, FQuat::Identity, ObjParams, Shape, Params);
				TraceEnd = FVector(Hit.TraceEnd.X, Hit.TraceEnd.Y, ZHit.ImpactPoint.Z);
			}

			FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();

			FTransform SpawnTM = FTransform(ProjRotation, TraceEnd);

		}
		else
		{

			if (bOnOwningActor)
			{
				TraceEnd = InstigatorCharacter->GetActorLocation();
			}
			else
			{
				TraceEnd = TargetActor->GetActorLocation();
			}

			
		}

		TargetActor->SetActorLocation(TraceEnd);


	}
}

