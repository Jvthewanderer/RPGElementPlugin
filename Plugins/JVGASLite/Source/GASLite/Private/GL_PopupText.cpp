// Fill out your copyright notice in the Description page of Project Settings.


#include "GL_PopupText.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "GLWC_HitText.h"

// Sets default values
AGL_PopupText::AGL_PopupText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	HitTextWidgetComp = CreateDefaultSubobject<UGLWC_HitText>("HitText");
	SetRootComponent(HitTextWidgetComp);

	ProjectileMovementComp->InitialSpeed = 600.0f;
	ProjectileMovementComp->MaxSpeed = 600.0f;

}

// Called when the game starts or when spawned
void AGL_PopupText::BeginPlay()
{
	Super::BeginPlay();
	HitTextWidgetComp->SetDamageText(GivenDelta);
	
}

// Called every frame
void AGL_PopupText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

