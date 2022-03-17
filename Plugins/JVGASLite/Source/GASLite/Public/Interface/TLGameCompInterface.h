// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TLGameCompInterface.generated.h"

class UTLActionComponent;
class UTLAttributeComponent;
class USceneComponent;
class USphereComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTLGameCompInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASLITE_API ITLGameCompInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UTLActionComponent* GetActionComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UTLAttributeComponent* GetAttriblueComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USphereComponent* GetRadiusComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMeshComponent* GetActorMesh();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USceneComponent* GetTargetComponent();
	 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetRadius();
};
