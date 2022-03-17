// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "TLProjectileBase.h"
#include <Components/StaticMeshComponent.h>
#include "Component/TLAttributeComponent.h"
#include <Components/SphereComponent.h>



// Sets default values
ATLExplosiveBarrel::ATLExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	blastRadius = 500.f;
	damage = -50.0;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BlastSphere"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(blastRadius);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->SetupAttachment(RootComponent); 

	// LEaving this on applies Small constant force via component ' tick; (Optional)
	RadialForceComponent->SetAutoActivate(false);

	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->ImpulseStrength = 2000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bIgnoreOwningActor = true;

	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);

	bReplicates = true;

}

// Called when the game starts or when spawned
void ATLExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &ATLExplosiveBarrel::OnBarrelHit);
}

void ATLExplosiveBarrel::OnBarrelHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* ThisIntigator = OtherActor->GetInstigator();



	if (ATLProjectileBase* MagicProjectile = Cast<ATLProjectileBase>(OtherActor))
	{
		TArray<AActor*> Result;
		SphereComp->GetOverlappingActors(Result);


		for (AActor* ThisActor : Result)
		{
			if (ThisActor == MagicProjectile)
			{
				continue;
			}

			UTLAttributeComponent* AttributeComp = Cast<UTLAttributeComponent>(ThisActor->GetComponentByClass(UTLAttributeComponent::StaticClass()));
	
			if (AttributeComp && FVector(ThisActor->GetActorLocation() - GetActorLocation()).Size() < blastRadius)
			{
				AttributeComp->ApplyHealthChange(ThisActor,damage);
			}
			RadialForceComponent->FireImpulse();
			MagicProjectile->Destroy();
		
		}

	}

}
