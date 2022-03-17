// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GL_MOBALibrary.generated.h"

/*struct FTransform;*/

/**
 * 
 */
UCLASS()
class GASLITE_API UGL_MOBALibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
 
 	UFUNCTION(BlueprintCallable, Category = "Common")
		static float GetUnitDistance(APawn* UnitOne, APawn* UnitTwo);

 	UFUNCTION(BlueprintCallable, Category = "Common")
		static float GetUnitGroundDistance(APawn* Unit, FVector Ground);

	UFUNCTION(BlueprintCallable, Category = "Common")
		static float GetUnitDistanceLength(APawn* UnitOne, APawn* UnitTwo);
	//UFUNCTION(BlueprintCallable, Category = "Common")
		//FTransform GetSymmentryTransform(FTransform Transform);
	
};
