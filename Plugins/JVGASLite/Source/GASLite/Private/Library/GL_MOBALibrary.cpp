// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/GL_MOBALibrary.h"
#include "GL_Unit.h"
#include "Interface/TLGameCompInterface.h"

float UGL_MOBALibrary::GetUnitDistance(APawn* UnitOne, APawn* UnitTwo)
{
	float Length = FVector(UnitOne->GetActorLocation() - UnitTwo->GetActorLocation()).Size();
	return Length - (ITLGameCompInterface::Execute_GetRadius(UnitOne) + ITLGameCompInterface::Execute_GetRadius(UnitTwo));
}

float UGL_MOBALibrary::GetUnitGroundDistance(APawn* Unit, FVector Ground)
{
	ITLGameCompInterface::Execute_GetRadius(Unit);

	float Length = FVector(Unit->GetActorLocation()- Ground).Size();
	return Length - ITLGameCompInterface::Execute_GetRadius(Unit);
}

float UGL_MOBALibrary::GetUnitDistanceLength(APawn* UnitOne, APawn* UnitTwo)
{

	float Length = FVector(UnitOne->GetActorLocation() - UnitTwo->GetActorLocation()).Size();

	return Length;

}