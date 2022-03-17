// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TLPopUpText.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/WidgetComponent.h>
#include "UI/TLDamageTextWidget.h"

// Sets default values
ATLPopUpText::ATLPopUpText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MoveComp->InitialSpeed = 600.0f;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;

	TextWidget = CreateDefaultSubobject<UWidgetComponent>("TextWidget");
	TextWidget->SetWidgetClass(TextUIClass);

	damageDelta = 0.0f;


}

void ATLPopUpText::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.0f);

	UTLDamageTextWidget* TextUI = Cast<UTLDamageTextWidget>(TextWidget->GetUserWidgetObject());

	if (TextUI)
	{
		TextUI->SetDamageText(damageDelta);
	}


}

