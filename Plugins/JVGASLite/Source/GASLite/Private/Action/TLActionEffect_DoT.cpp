// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLActionEffect_DoT.h"
#include "Library/TLGameplayFunctionLibrary.h"


UTLActionEffect_DoT::UTLActionEffect_DoT()
{
	Damage = 10.f;
}

void UTLActionEffect_DoT::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	Super::ExecutePeriodicEffect(Instigator);
}

void UTLActionEffect_DoT::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction(Instigator);

	FHitResult SweepResult;

	UTLGameplayFunctionLibrary::ApplyDirectionalDamage(Instigator, GetOwningComponent()->GetOwner(), Damage, SweepResult);
}

void UTLActionEffect_DoT::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction(Instigator);

}


