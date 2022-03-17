// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTT_RangeAttack.h"
#include <AIController.h>
#include <GameFramework/Character.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Component/TLAttributeComponent.h"
#include "Interface/TLGameCompInterface.h"
#include "TLProjectileBase.h"
#include "Kismet/GameplayStatics.h"

UTLBTT_RangeAttack::UTLBTT_RangeAttack()
{
	MaxBulletSpread = 2.f;
	SocketName = "Muzzle_01";
}


EBTNodeResult::Type UTLBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController =OwnerComp.GetAIOwner();
	if (!MyController)
	{
		return EBTNodeResult::Failed;
	}
	APawn* MyPawn = Cast<APawn>(MyController->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector MuzzleLocation = ITLGameCompInterface::Execute_GetActorMesh(MyPawn)->GetSocketLocation(SocketName);
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	
	if (TargetActor == nullptr) {

		return EBTNodeResult::Failed;
	}

	if (!UTLAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	MuzzleRotation.Pitch +=FMath::RandRange(0.0f,MaxBulletSpread);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = MyPawn;

	FTransform MuzzleTrans = FTransform(MuzzleRotation, MuzzleLocation);

	//GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
	ATLProjectileBase* Projectile = Cast<ATLProjectileBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(),
		ProjectileClass,
		MuzzleTrans,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		MyPawn));

		Projectile->TargetActor = TargetActor;
		Projectile->SetInstigator(MyPawn);

	UGameplayStatics::FinishSpawningActor((AActor*)Projectile, MuzzleTrans);

	//AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

	return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

}

