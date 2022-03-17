// Fill out your copyright notice in the Description page of Project Settings.


#include "GL_Unit.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGL_Unit::AGL_Unit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetPointComponent = CreateDefaultSubobject<USceneComponent>("TargetPoint");
	TargetPointComponent->SetupAttachment(GetMesh());

	UnitRadius = 500.f;

	UnitTeamID = 255;

}


USceneComponent* AGL_Unit::GetTargetComponent_Implementation()
{
	return TargetPointComponent;
}

float AGL_Unit::GetRadius_Implementation()
{
	return UnitRadius;
}

void AGL_Unit::UnitInit_Implementation(FName _unitKey, uint8 TeamID, AGL_PlayerStateBase* CommanderPlayerState, AGL_Unit* OwnerUnit)
{

}

void AGL_Unit::SetUnitCommand(TEnumAsByte<ECommand> MicroCommand, FVector Location, AGL_Unit* Unit)
{
	GetCharacterMovement()->StopMovementImmediately();
}

float AGL_Unit::GetUnitParam(EUnitParam Param)
{
	return 0.0f;
}

void AGL_Unit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGL_Unit, UnitKey);
}