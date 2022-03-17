// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLPowerup_Action.h"
#include "Component/TLActionComponent.h"
#include "Action/TLAction.h"


void ATLPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{

	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	UTLActionComponent* ActionComp = Cast<UTLActionComponent>(InstigatorPawn->GetComponentByClass(UTLActionComponent::StaticClass()));

	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			if (ActionComp->GetAction(ActionToGrant))
			{
				FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
				GEngine->AddOnScreenDebugMessage(-1, 2.0f,FColor::Red, DebugMsg);
				return;
			}
			ActionComp->AddAction(InstigatorPawn, ActionToGrant);
			HideAndCooldownPowerup();
		}
	}

}
