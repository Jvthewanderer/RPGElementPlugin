// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLAction_MeleeAttack.h"

UTLAction_MeleeAttack::UTLAction_MeleeAttack()
{
	AttackAnimDelay = 0.2f;
	ActionName = "PrimaryAttack";
}

void UTLAction_MeleeAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	MontageDuration = UTLGameplayFunctionLibrary::PlayActorAnimMontage(Instigator, AttackAnim);
}

void UTLAction_MeleeAttack::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
}

void UTLAction_MeleeAttack::TriggerAction_Implementation(AActor* Instigator)
{
	Super::TriggerAction_Implementation(Instigator);
}

void UTLAction_MeleeAttack::TriggerActionFromAnimNotify_Implementation(AActor* Instigator, FName SocketName)
{
	Super::TriggerActionFromAnimNotify_Implementation(Instigator, SocketName);
}

void UTLAction_MeleeAttack::AttackDelay_Elapsed(AActor* InstigatorCharacter)
{
	StopAction(InstigatorCharacter);
}

void UTLAction_MeleeAttack::OnAttackHit_Implementation(AActor* TargetedActor)
{

}







