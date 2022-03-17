// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATLPowerupActor::ATLPowerupActor()
{

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.f;
	bIsActive = true;

	bReplicates = true;
	//SetReplicates(true);
}

void ATLPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ATLPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	FTimerHandle TimerHandle_RespawnTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ATLPowerupActor::ShowPowerup, RespawnTime);
}

void ATLPowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();

}

FText ATLPowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ATLPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}

void ATLPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//Logic goes Here...
}

void ATLPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATLPowerupActor, bIsActive);
}