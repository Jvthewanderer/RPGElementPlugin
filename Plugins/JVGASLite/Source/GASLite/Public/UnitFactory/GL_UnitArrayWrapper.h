// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GL_UnitArrayWrapper.generated.h"

class ATLAICharacter;

/**
 * 
 */
UCLASS()
class GASLITE_API UGL_UnitArrayWrapper : public UObject
{
	GENERATED_BODY()

	public:

	UPROPERTY()
	TArray<APawn*> Units;
};
