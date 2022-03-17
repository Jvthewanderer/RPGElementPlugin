// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TLActionComponent.h"
#include "Action/TLAction.h"
#include "../TLActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include <Engine/ActorChannel.h>


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);


UTLActionComponent::UTLActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UTLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UTLAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}

	}
	

}


void UTLActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UTLAction*>ActionCopy = Actions;
	for (UTLAction* Action : ActionCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}

	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UTLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// 	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
		
	// Draw All Actions
	for (UTLAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(Action->GetOuter()));
		LogOnScreen(this,ActionMsg, TextColor, 0.0f);
	}
}	

void UTLActionComponent::AddAction(AActor* Instigator, TSubclassOf<UTLAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	//Skip for Clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UTLAction* NewAction = NewObject<UTLAction>(GetOwner(), ActionClass);
	if(NewAction)
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}

}

void UTLActionComponent::RemoveAction(UTLAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;	
	}
	Actions.Remove(ActionToRemove);
}

UTLAction* UTLActionComponent::GetAction(TSubclassOf<UTLAction> ActionClass) const
{
	for (UTLAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

UTLAction* UTLActionComponent::GetActionByName(FName ActionName) const 
{

	for (UTLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			return Action;
		}
	}

	return nullptr;
}

bool UTLActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for(UTLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator)) 
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FailedMsg);
				continue;
			}

			//Is Client
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
			//BookMark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			// Do Cosmetic thing So player don't feel like they're waiting for an action to happen.
			Action->StartAction(Instigator);

			return true;
		}
	}
	return false;
}

bool UTLActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UTLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{

				//Is Client
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);

				return true;
			}
		}
	}
	return false;
}

bool UTLActionComponent::TriggerActionByName(AActor* Instigator, FName ActionName)
{
	for (UTLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{

			//Is Client
			if (!GetOwner()->HasAuthority())
			{
				ServerTriggerAction(Instigator, ActionName);
			}

			Action->TriggerAction(Instigator);

			return true;
		}


	}
	return false;
}

bool UTLActionComponent::TriggerActionFromAnimNotifyByName(AActor* Instigator, FName ActionName, FName SocketName)
{
	for (UTLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{


			//Is Client
			if (!GetOwner()->HasAuthority())
			{
				ServerTriggerActionFromAnimNotify(Instigator,ActionName, SocketName);
			}

			Action->TriggerActionFromAnimNotify(Instigator, SocketName);

			return true;
		}


	}
	return false;
}

bool UTLActionComponent::ConfirmAction(AActor* Instigator)
{
	if (IsTargeting())
	{
		for (UTLAction* Action : Actions)
		{
			if (Action && Action->IsTargeting())
			{

				//Is Client
				if (!GetOwner()->HasAuthority())
				{
					ServerConfirmAction(Instigator);
				}

				Action->TriggerAction(Instigator);


				return true;
			}
		}

	}
	
	return false;
}

bool UTLActionComponent::CancleAction(AActor* Instigator)
{
	if (IsTargeting())
	{
		for (UTLAction* Action : Actions)
		{
			if (Action && Action->IsTargeting())
			{

				//Is Client
				if (!GetOwner()->HasAuthority())
				{
					ServerCancleAction(Instigator);
				}

				Action->StopAction(Instigator);

				return true;
			}
		}

	}

	return false;
}

UTLActionComponent* UTLActionComponent::GetActionComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UTLActionComponent>(FromActor->GetComponentByClass(UTLActionComponent::StaticClass()));
	}

	return nullptr;
}


void UTLActionComponent::SetTargeting(bool NewTargeting)
{
	bIsTargeting = NewTargeting;
}

void UTLActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UTLActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator,ActionName);
}

void UTLActionComponent::ServerTriggerAction_Implementation(AActor* Instigator, FName ActionName)
{
	TriggerActionByName(Instigator, ActionName);
}

void UTLActionComponent::ServerTriggerActionFromAnimNotify_Implementation(AActor* Instigator, FName ActionName, FName SocketName)
{
	TriggerActionFromAnimNotifyByName(Instigator, ActionName, SocketName);
}

void UTLActionComponent::ServerConfirmAction_Implementation(AActor* Instigator)
{
	ConfirmAction(Instigator);
}

void UTLActionComponent::ServerCancleAction_Implementation(AActor* Instigator)
{
	CancleAction(Instigator);
}

bool UTLActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UTLAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action,*Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}


void UTLActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTLActionComponent, Actions);
}
