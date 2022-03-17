// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLPowerup_HealthPotion.h"
#include "Component/TLAttributeComponent.h"
/*#include "Game/TLPlayerState.h"*/


#define LOCTEXT_NAMESPACE "InteractableActors"

ATLPowerup_HealthPotion::ATLPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditCost = 50;
}

void ATLPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UTLAttributeComponent* AttributeComp = Cast<UTLAttributeComponent>(InstigatorPawn->GetComponentByClass(UTLAttributeComponent::StaticClass()));
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
// 		// Only activate if healed successfully
// 		if (ATLPlayerState* PS = InstigatorPawn->GetPlayerState<ATLPlayerState>())
// 		{
// 			
// 			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetMaxHealth()))
// 			{
// 				HideAndCooldownPowerup();
// 			}
// 		}
	}
}

FText ATLPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UTLAttributeComponent* AttributeComp = UTLAttributeComponent::GetAttribute(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return NSLOCTEXT("InteractableActor", "HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(NSLOCTEXT("InteractableActors", "HealthPotion_InteractMessage", "Cost {0} Credits. Restore health to maximum"), CreditCost);
}

#undef LOCTEXT_NAMESPACE