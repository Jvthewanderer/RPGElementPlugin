// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLAction_ProjectileAttack.h"
#include "../../Public/RLCharacter.h"
#include "Action/TLAction.h"
#include "Component/TLActionComponent.h"
#include <Camera/CameraComponent.h>
#include "TLProjectileBase.h"
#include "Library/TLGameplayFunctionLibrary.h"
#include "Interface/TLGameCompInterface.h"
#include <AI/TLAIController.h>



UTLAction_ProjectileAttack::UTLAction_ProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	ActionName = "PrimaryAttack";
	bUseForwardDirection = true;
}

void UTLAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	MontageDuration = UTLGameplayFunctionLibrary::PlayActorAnimMontage(Instigator, AttackAnim);

}

void UTLAction_ProjectileAttack::AttackDelay_Elapsed(AActor* InstigatorCharacter)
{
	StopAction(InstigatorCharacter);
}

void UTLAction_ProjectileAttack::SpawnProjectile(AActor* InstigatorCharacter, FName SocketName)
{
	if (ensureAlways(ProjectileClass))
	{
		float calcDamage = 0.0f;

		USkeletalMeshComponent* ActorMesh = ITLGameCompInterface::Execute_GetActorMesh(InstigatorCharacter);
		FVector MuzzleLocation = ActorMesh->GetSocketLocation(SocketName);
		FVector TraceStart, TraceEnd;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Cast<APawn>(InstigatorCharacter);

		ARLCharacter* PlayerCharacter = Cast<ARLCharacter>(InstigatorCharacter);
		APawn* AIPawn = Cast<APawn>(InstigatorCharacter);
		ATLAIController* AIController = Cast<ATLAIController>(AIPawn->GetController());

		if (PlayerCharacter)
		{
			TraceStart = PlayerCharacter->GetPawnViewLocation() + (PlayerCharacter->GetPlayerCamera()->GetForwardVector() * 15);
			if (bUseForwardDirection)
			{
				TraceStart = MuzzleLocation;
				TraceEnd = PlayerCharacter->GetMouseLocation();
			}
			else
			{
				TraceEnd = TraceStart + (PlayerCharacter->GetControlRotation().Vector() * 5000);
			}
			
		}
		else if(AIController) {

			if (AIController->GetTargetActor())
			{
				USceneComponent* TargetPoint = ITLGameCompInterface::Execute_GetTargetComponent(AIController->GetTargetActor());

				TraceStart = MuzzleLocation;

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
			TraceStart = MuzzleLocation;
			TraceEnd = TraceStart + (InstigatorCharacter->GetActorForwardVector() * 5000);
		}

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FHitResult Hit;

		//bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params);

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - MuzzleLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);
		//AActor* ProjectileActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		if (CalcDamageObj)
		{
			calcDamage = CalcDamageObj->CalcDamage(BaseDamage);
		}
		else {
			calcDamage = BaseDamage;
		}

		ATLProjectileBase* Projectile = Cast<ATLProjectileBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(),
			ProjectileClass,
			SpawnTM,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			AIPawn));

		Projectile->SetInstigator(AIPawn);
		Projectile->SetDamageAmount(calcDamage);

		UGameplayStatics::FinishSpawningActor((AActor*)Projectile, SpawnTM);

		ATLProjectileBase* ProjectileBaseActor = Projectile;
		if (ProjectileBaseActor)
		{
			ProjectileBaseActor->OnProjectileHit.AddDynamic(this, &UTLAction_ProjectileAttack::OnProjectileHit);
			
		}
	}

	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate;

	Delegate.BindUFunction(this,"AttackDelay_Elapsed", InstigatorCharacter);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate, AttackAnimDelay, false);
}

void UTLAction_ProjectileAttack::ServerSpawnProjectile_Implementation(AActor* InstigatorCharacter, FName SocketName)
{
	SpawnProjectile(InstigatorCharacter, SocketName);
}

void UTLAction_ProjectileAttack::OnProjectileHit_Implementation(AActor* TargetedActor, ATLProjectileBase* SelfProjectile)
{
	// Does Stuff. After Stuff Happen Unbind Projectile
	SelfProjectile->OnProjectileHit.RemoveDynamic(this, &UTLAction_ProjectileAttack::OnProjectileHit);
}

void UTLAction_ProjectileAttack::TriggerActionFromAnimNotify_Implementation(AActor* Instigator, FName SocketName)
{
	Super::TriggerActionFromAnimNotify_Implementation(Instigator,SocketName);

		if (GetOwningComponent()->GetOwner()->HasAuthority())
		{
			ServerSpawnProjectile(Instigator, SocketName);
		}
}

