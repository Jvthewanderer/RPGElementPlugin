// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RelayPointWrapper.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API URelayPointWrapper : public UObject
{
	GENERATED_BODY()

public:
	TArray<FTransform> RelayPoints;
	
};
