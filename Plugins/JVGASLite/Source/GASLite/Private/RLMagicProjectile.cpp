// Fill out your copyright notice in the Description page of Project Settings.


#include "RLMagicProjectile.h"
#include <Components/SphereComponent.h>
#include "Component/TLAttributeComponent.h"
#include "Library/TLGameplayFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Sound/SoundCue.h"
#include "Component/TLActionComponent.h"
#include "TLProjectileBase.h"
#include "Action/TLActionEffect.h"

// Sets default values
ARLMagicProjectile::ARLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ImpactShakeInnerRadius = 250.f;
	ImpactShakeOuterRadius = 2500.f;

	InitialLifeSpan = 5.0f;

}

// Called when the game starts or when spawned
void ARLMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARLMagicProjectile::OnActorOverlap);
	
}

void ARLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");

		UTLActionComponent* ActionComp = Cast<UTLActionComponent>(OtherActor->GetComponentByClass(UTLActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}


		if(UTLGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{

			if (ImpactSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	
	 			UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
	 		}

			if (ActionComp && HasAuthority())
			{
				if (ActionEffectClass)
				{
					ActionComp->AddAction(GetInstigator(), ActionEffectClass);
				}

			}
			OnProjectileHit.Broadcast(OtherActor,this);
			Destroy();
		}
	}
}

