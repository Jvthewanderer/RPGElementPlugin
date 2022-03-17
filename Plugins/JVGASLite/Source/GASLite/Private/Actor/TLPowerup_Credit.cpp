// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLPowerup_Credit.h"
#include <Components/StaticMeshComponent.h>
/*#include "Game/TLPlayerState.h"*/

ATLPowerup_Credit::ATLPowerup_Credit()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditAmount = 80;
}

void ATLPowerup_Credit::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
// 	if (ATLPlayerState* PS = InstigatorPawn->GetPlayerState<ATLPlayerState>())
// 	{
// 		PS->AddCredits(CreditAmount);
// 		HideAndCooldownPowerup();
// 	}
}

