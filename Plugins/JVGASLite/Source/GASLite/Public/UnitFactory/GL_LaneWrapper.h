// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../TLActionRoguelike.h"
#include "GL_LaneWrapper.generated.h"

class URelayPointWrapper;

ENUM_RANGE_BY_COUNT(ELanes, ELanes::Bottom);

/**
 * 
 */
UCLASS()
class GASLITE_API UGL_LaneWrapper : public UObject
{
	GENERATED_BODY()

	public:

	TMap<ELanes,URelayPointWrapper*> Lanes;

	UFUNCTION(BlueprintCallable, Category = "Common")
	void Init();
	
};
