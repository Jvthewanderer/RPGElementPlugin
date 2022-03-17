// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLChest.h"
#include <Components/StaticMeshComponent.h>
#include "Net/UnrealNetwork.h"

// Sets default values
ATLChest::ATLChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;

	bReplicates = true;
	//Causes warnings
	//SetReplicates(true);

}

void ATLChest::Interact_Implementation(APawn* InstigatorPawn)
{
	//throw std::logic_error("The method or operation is not implemented.");

	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ATLChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

void ATLChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ATLChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATLChest, bLidOpened);
}