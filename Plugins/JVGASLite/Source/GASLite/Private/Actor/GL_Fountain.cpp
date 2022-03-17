// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GL_Fountain.h"
#include "RLCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Component/TLAttributeComponent.h"

// Sets default values
AGL_Fountain::AGL_Fountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RadiusComponent = CreateDefaultSubobject<USphereComponent>("RadiusComp");
	RadiusComponent->SetupAttachment(RootComponent);
	RadiusComponent->SetSphereRadius(1000);

	TeamID = 255;

	Percentage = 0.1f;

	HealTimer = 1.f;

}

// Called when the game starts or when spawned
void AGL_Fountain::BeginPlay()
{
	Super::BeginPlay();

	TimerDel.BindUFunction(this, FName("HealTeamPlayers"));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDel, HealTimer, true);
	
}

// Called every frame
void AGL_Fountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGL_Fountain::HealTeamPlayers()
{
	TArray<AActor*> OutActors;
	RadiusComponent->GetOverlappingActors(OutActors, ARLCharacter::StaticClass());

	for (AActor* Player : OutActors)
	{

		UTLAttributeComponent* PlayerAttribute = UTLAttributeComponent::GetAttribute(Player);
		if (PlayerAttribute->IsAlive() && PlayerAttribute->GetTeamID() == TeamID)
		{
			PlayerAttribute->ApplyHealthChange(Player, PlayerAttribute->GetMaxHealth() * Percentage);
			PlayerAttribute->ApplyManaChange(Player, PlayerAttribute->GetMaxMana() * Percentage);
		}
	}
}

