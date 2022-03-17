// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TLGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTLGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASLITE_API ITLGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Executable Attacks")
	bool PrimaryAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Executable Attacks")
	bool PrimaryAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Executable Attacks")
	bool SecondaryAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Executable Attacks")
	bool TertiaryAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Executable Attacks")
	bool SpecialAttack();
};
