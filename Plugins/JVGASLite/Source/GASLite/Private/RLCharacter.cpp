
// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Component/RLInteractionComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <DrawDebugHelpers.h>
#include "Component/TLAttributeComponent.h"
#include "Component/TLActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"


// Sets default values
ARLCharacter::ARLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<URLInteractionComponent>("InteractComp");

	AttributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UTLActionComponent>("ActionComp");


	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";

	bIsDead = false;

	bisTargeting = false;

}

USkeletalMeshComponent* ARLCharacter::GetActorMesh_Implementation()
{
	return GetMesh();
}

UTLAttributeComponent* ARLCharacter::GetAttriblueComponent_Implementation()
{
	return AttributeComp;
}

UTLActionComponent* ARLCharacter::GetActionComponent_Implementation()
{
	return ActionComp;
}

void ARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARLCharacter::OnHealthChange);
}

// Called when the game starts or when spawned
void ARLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARLCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARLCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("PrimaryAbility", IE_Pressed, this, &ARLCharacter::PrimaryAbility);
	PlayerInputComponent->BindAction("SecondaryAbility", IE_Pressed, this, &ARLCharacter::SecondaryAbility);
	PlayerInputComponent->BindAction("TertiaryAbility", IE_Pressed, this, &ARLCharacter::TertiaryAbility);
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &ARLCharacter::SpecialAttack);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARLCharacter::Jump);





}

void ARLCharacter::Jump()
{
	if (ActionComp->IsStunned())
	{
		return;
	}
	else
	{
		Super::Jump();
	}
}

void ARLCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();

	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	if (ActionComp->IsStunned())
	{
		AddMovementInput(ControlRot.Vector(), 0.0f);
	}
	else
	{
		AddMovementInput(ControlRot.Vector(), Value);
	}

}

void ARLCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();

	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	if (ActionComp->IsStunned())
	{
		AddMovementInput(ControlRot.Vector(), 0.0f);
	}
	else
	{
		AddMovementInput(RightVector, Value);
	}
}

void ARLCharacter::PrimaryAttack()
{
	if (ActionComp->IsTargeting())
	{
		ActionComp->ConfirmAction(this);
	}
	else
	{
		ActionComp->StartActionByName(this, "PrimaryAttack");
	}

}

void ARLCharacter::SpecialAttack()
{
	ActionComp->StartActionByName(this, "SpecialAttack");
}

void ARLCharacter::PrimaryAbility()
{
	ActionComp->StartActionByName(this, "Ability_01");
}

void ARLCharacter::SecondaryAbility()
{
	ActionComp->StartActionByName(this, "Ability_02");
}

void ARLCharacter::TertiaryAbility()
{
	if (ActionComp->IsTargeting())
	{
		ActionComp->CancleAction(this);
	}
	else
	{
	ActionComp->StartActionByName(this, "Ability_03");
	}
}

void ARLCharacter::OnHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		bIsDead = true;
		APlayerController* PC =Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}


FVector ARLCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}



void ARLCharacter::HealSelf(float Amount /* = 100*/)
{
	AttributeComp->ApplyHealthChange(this,Amount);
}

void ARLCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
	
}

FVector ARLCharacter::GetMouseLocation()
{
	FHitResult Hit;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,true, Hit);

	return Hit.Location;
}






