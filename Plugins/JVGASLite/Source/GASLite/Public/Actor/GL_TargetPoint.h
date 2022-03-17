// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "../TLActionRoguelike.h"
#include "GL_TargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API AGL_TargetPoint : public ATargetPoint
{
	GENERATED_BODY()

protected:

public:

	AGL_TargetPoint();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Common")
		uint8 TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
		FName UnitKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
		ELanes Lane;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
		int32 Number;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
		bool bCopy;
};
