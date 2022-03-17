// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLPowerup_ManaPotion.h"
/*#include "Game/TLPlayerState.h"*/
#include "Component/TLAttributeComponent.h"


ATLPowerup_ManaPotion::ATLPowerup_ManaPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditCost = 20;
}

void ATLPowerup_ManaPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UTLAttributeComponent* AttributeComp = Cast<UTLAttributeComponent>(InstigatorPawn->GetComponentByClass(UTLAttributeComponent::StaticClass()));
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullMana())
	{
// 		// Only activate if healed successfully
// 		if (ATLPlayerState* PS = InstigatorPawn->GetPlayerState<ATLPlayerState>())
// 		{
// 
// 			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyManaChange(this, AttributeComp->GetMaxMana()))
// 			{
// 				HideAndCooldownPowerup();
// 			}
// 		}
	}
}
