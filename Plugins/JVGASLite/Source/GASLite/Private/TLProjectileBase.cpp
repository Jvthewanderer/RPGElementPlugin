// Fill out your copyright notice in the Description page of Project Settings.


#include "TLProjectileBase.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "../TLActionRoguelike.h"
#include "Interface/TLGameCompInterface.h"

ATLProjectileBase::ATLProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->InitialSpeed = 8000;

	FlightAudio = CreateDefaultSubobject<UAudioComponent>("FlightSound");
	FlightAudio->SetupAttachment(RootComponent);

	ImpactShakeInnerRadius = 250.f;
	ImpactShakeOuterRadius = 2500.f;

	DamageAmount = -20.0f;

	bReplicates = true;

	bIsHoming = false;
	//SetReplicates(true);

}

// Called when the game starts or when spawned
void ATLProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentHit.AddDynamic(this, &ATLProjectileBase::OnActorHit);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);

	if (bIsHoming)
	{
		if (TargetActor && MoveComp->HomingTargetComponent == nullptr)
		{
			MoveComp->HomingTargetComponent = ITLGameCompInterface::Execute_GetTargetComponent(TargetActor);
		}
		else {
			FColor TextColor = FColor::Red;
			FString ProjMsg = FString::Printf(TEXT("[%s] ProjectileTarget is Not Valid"),
				*GetNameSafe(GetOwner()));
			LogOnScreen(this, ProjMsg, TextColor, 0.0f);

		}
	}
	
}

// Called every frame
void ATLProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ATLProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		
	}

	Explode();
}

void ATLProjectileBase::Explode_Implementation()
{

	if (ensure(!IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

			UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		}
		Destroy();
	}
}

void ATLProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}