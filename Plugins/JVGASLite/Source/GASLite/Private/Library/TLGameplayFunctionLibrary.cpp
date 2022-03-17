// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/TLGameplayFunctionLibrary.h"
#include "Component/TLAttributeComponent.h"
#include "Interface/TLGameCompInterface.h"
#include <Kismet/GameplayStatics.h>
#include"../TLActionRoguelike.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

static TAutoConsoleVariable<float> CVarImpactDirectional(TEXT("tl.DamageImpactDirectional"), 1.f, TEXT("The Impact on how far the target actor goes."), ECVF_Cheat);

bool UTLGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UTLAttributeComponent* AttributeComp = UTLAttributeComponent::GetAttribute(TargetActor);

	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, DamageAmount);
	}

	return false;
}

bool UTLGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	float ImpactPower = CVarImpactDirectional.GetValueOnGameThread();
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * ImpactPower, HitResult.ImpactPoint,HitResult.BoneName);
		}
		return true;
	}
	
	 return false;
}

bool UTLGameplayFunctionLibrary::GetAllFreindlyActors(AActor* CurrentActor, const TArray<AActor*> Actors, TArray<AActor*>& OutFriendlyActors)
{

	if (Actors.IsEmpty())
	{
		return false;
	}

	for (AActor* FriendActor : Actors)
	{

		if (ITLGameCompInterface::Execute_GetAttriblueComponent(CurrentActor)->IsFriendly(FriendActor))
		{
			FString TargetMsg = FString::Printf(TEXT("[%s] IN enemy Array: %s "),
				*GetNameSafe(CurrentActor),
				*GetNameSafe(FriendActor));
			LogOnScreen(CurrentActor, TargetMsg, FColor::Emerald, 10.0f);
			OutFriendlyActors.Add(FriendActor);
		}
	}

	return true;
}

bool UTLGameplayFunctionLibrary::GetAllEnemyActors(AActor* CurrentActor, const TArray<AActor*> Actors, TArray<AActor*>& OutEnemyActors)
{

	if (Actors.IsEmpty())
	{
		return false;
	}

	for (AActor* EnemyActor : Actors)
	{
		if (!ITLGameCompInterface::Execute_GetAttriblueComponent(CurrentActor)->IsFriendly(EnemyActor))
		{
			FString TargetMsg = FString::Printf(TEXT("[%s] IN enemy Array: %s "),
				*GetNameSafe(CurrentActor),
				*GetNameSafe(EnemyActor));
			LogOnScreen(CurrentActor, TargetMsg, FColor::Emerald, 10.0f);
			
			OutEnemyActors.Add(EnemyActor);
		}
	}

	return true;
}

bool UTLGameplayFunctionLibrary::SetActorTeamID(AActor* CurrentActor, uint8 TeamID)
{
	UTLAttributeComponent* AttributeComp = ITLGameCompInterface::Execute_GetAttriblueComponent(CurrentActor);
	if (AttributeComp)
	{
		AttributeComp->SetTeamID(TeamID);
	}
	else {
		return false;
	}

	return true;
}

float UTLGameplayFunctionLibrary::PlayActorAnimMontage(AActor* CurrentActor, class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* Mesh = ITLGameCompInterface::Execute_GetActorMesh(CurrentActor);
	UAnimInstance* AnimInstance = (Mesh) ? Mesh->GetAnimInstance() : nullptr;
	if (AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		if (Duration > 0.f)
		{
			// Start at a given Section.
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}

			return Duration;
		}
	}

	return 0.f;
}
