// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GL_Turret.h"
#include "Niagara/Public/NiagaraComponent.h"
#include <AI/TLAIController.h>

AGL_Turret::AGL_Turret()
{
	BeamColor = FLinearColor::Yellow;
// 	BeamNSystem->SetColorParameter(BeamColorName, BeamColor);
// 
// 	BeamNSystem->SetupAttachment(Mesh, SocketName);
}

void AGL_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	FColor TextColor = GetTargetActor() ? FColor::Blue : FColor::White;
// 	FString TargetMsg = FString::Printf(TEXT("[%s] TargetActor: %s : TargetLocation: %s"),
// 		*GetNameSafe(this),
// 		GetTargetActor() ? *GetNameSafe(GetTargetActor()) : TEXT("None"),
// 		GetTargetActor() ? *CurrentTarget->GetActorLocation().ToString() : TEXT("None"));
// 	LogOnScreen(this, TargetMsg, TextColor, 0.0f);

	if (TLAIController->GetTargetActor())
	{
// 		BeamNSystem->SetVectorParameter(BeamEnd, TLAIController->GetTargetActor()->GetActorLocation());
// 		BeamNSystem->SetColorParameter(BeamColorName, BeamColor);
		TowerLookAtTarget();
	}
	else
	{
		FVector OutLoc;
		FQuat OutQuat;
		Mesh->GetSocketWorldLocationAndRotation(BeamEnd, OutLoc, OutQuat);
/*		BeamNSystem->SetColorParameter(BeamColorName, FLinearColor::Black);*/
	}

}

void AGL_Turret::BeginPlay()
{
	Super::BeginPlay();

}

void AGL_Turret::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TargetRadius->OnComponentBeginOverlap.AddDynamic(this, &AGL_Turret::OnTowerRadiusBeginOverlaped);
	TargetRadius->OnComponentEndOverlap.AddDynamic(this, &AGL_Turret::OnTowerRadiusEndOverlaped);

	FVector OutLoc;
	FQuat OutQuat;
	Mesh->GetSocketWorldLocationAndRotation(BeamEnd, OutLoc, OutQuat);
/*	BeamNSystem->SetVectorParameter(BeamEnd, OutLoc);*/


}

void AGL_Turret::OnTowerRadiusBeginOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
//  if (AttributeComp->IsAlive())
// 	{
//  		if (GetTargetActor() != OtherActor)
//  		{
//  			if (!AttributeComp->IsFriendly(OtherActor))
//  			{
// 				AddTargetActor(OtherActor);
//  			}
//  
//  		}
// 	}

}

void AGL_Turret::OnTowerRadiusEndOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
// 	if (AttributeComp->IsAlive())
// 	{
// 		RemoveTargetActor(OtherActor);
// 	}
}

void AGL_Turret::TowerLookAtTarget()
{

	if (TLAIController->GetTargetActor())
	{

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

		// 		FActorSpawnParameters SpawnParams;
		// 		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// 		SpawnParams.Instigator = InstigatorCharacter;

		FVector TraceStart = GetMesh()->GetSocketLocation(SocketName);
		FVector TraceEnd = TLAIController->GetTargetActor()->GetActorLocation();
		FVector TraceEndZ = TraceEnd + (FVector(0, 0, -100000));

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);


		FHitResult Hit, ZHit;

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f, 0U, 1.0f);

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params);

		if (bBlockingHit)
		{
			TraceEnd = Hit.ImpactPoint;
		}
		else
		{
			//GetWorld()->SweepSingleByObjectType(ZHit, TraceEnd, TraceEndZ, FQuat::Identity, ObjParams, Shape, Params);
			TraceEnd = FVector(Hit.TraceEnd.X, Hit.TraceEnd.Y, ZHit.ImpactPoint.Z);
		}

		//BeamNSystem->SetVectorParameter(BeamEnd, TraceEnd);

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, TraceEnd);

	}
}
