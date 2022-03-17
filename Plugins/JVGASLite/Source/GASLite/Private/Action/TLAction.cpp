// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLAction.h"
#include "Component/TLActionComponent.h"
#include "../TLActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include <GameFramework/GameStateBase.h>
#include "ActionCalc/GLCalcDamage.h"


UTLAction::UTLAction()
{
	BaseDamage = -10.0f;
	RecastTime = 0.0f;
}

void UTLAction::Initialize(UTLActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;

	if (CalcDamageClass)
	{
		CalcDamageObj = NewObject<UGLCalcDamage>(GetOwningComponent()->GetOwner(), CalcDamageClass);

		CalcDamageObj->Initialize(NewActionComp);
	}
}

bool UTLAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning() || GetTimeRemaining() > 0.0f)
	{
		return false;
	}
	
	UTLActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

float UTLAction::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();

	if (GS && TimeStarted > 0.0f)
	{
		float EndTime = TimeStarted + RecastTime;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return -1.0f;
}

void UTLAction::StartCooldown()
{
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
}

void UTLAction::ClearCooldown()
{
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = 0.0f;
	}
}

void UTLAction::ReduceCooldown(float reduceTime)
{
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted += reduceTime;
	}
}

void UTLAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UTLActionComponent* Comp =GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	StartCooldown();

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(),this);
}

void UTLAction::StopAction_Implementation(AActor* Instigator)
{

	//UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UTLActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(),this);
}

void UTLAction::TriggerAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action Triggered: %s"), *GetNameSafe(this));
	
	bTargeting = false;
	//RepData.bIsTargeting = true;
	//RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionTriggered.Broadcast(GetOwningComponent(),this);
}

void UTLAction::TriggerActionFromAnimNotify_Implementation(AActor* Instigator, FName SocketName)
{
	UE_LOG(LogTemp, Log, TEXT("Action Triggered From Anim Notify: %s"), *GetNameSafe(this));
}


UWorld* UTLAction::GetWorld() const
{
	//Outer is Set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;

}

UTLActionComponent* UTLAction::GetOwningComponent() const
{
	return ActionComp;
}

float UTLAction::GetBaseDamage()
{
	if (CalcDamageObj)
	{
		return CalcDamageObj->CalcDamage(BaseDamage);
	}

	return BaseDamage;
}

void UTLAction::OnRep_RepData()
{	
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UTLAction::OnRep_bTargeting()
{
	if (!bTargeting)
	{
		TriggerAction(RepData.Instigator);
	}
}

bool UTLAction::IsRunning() const
{
	return RepData.bIsRunning;
}


bool UTLAction::IsTargeting() const
{
	return bTargeting;
}

void UTLAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTLAction, RepData);
	DOREPLIFETIME(UTLAction, bTargeting);
	DOREPLIFETIME(UTLAction, ActionComp);
	DOREPLIFETIME(UTLAction, TimeStarted);
}