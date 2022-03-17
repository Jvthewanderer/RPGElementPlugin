// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameplayTagContainer.h>
#include "GLCalcDamage.generated.h"


class UTLActionComponent;
class UTLAttributeComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class GASLITE_API UGLCalcDamage : public UObject
{
	GENERATED_BODY()

public:

	UGLCalcDamage();

	void Initialize(UTLActionComponent* NewActionComp);

	UFUNCTION(BlueprintNativeEvent,Category = "Calculation")
	float CalcDamage(float baseDamage);
	
	protected:
	UFUNCTION(BlueprintPure, Category = "Calaculation")
	AActor* GetOwner();

	UFUNCTION(BlueprintPure, Category = "Calaculation")
	FGameplayTagContainer GetOwnersGameplayTags();
	
	UFUNCTION(BlueprintPure, Category = "Calaculation")
	UTLAttributeComponent* GetOwnerAttribute();

	UFUNCTION(BlueprintPure, Category = "Calaculation")
	float GetOwnerHealth();

	UFUNCTION(BlueprintPure, Category = "Calaculation")
	float GetAttackTypeAttribute();

	UFUNCTION(BlueprintNativeEvent, Category = "Calaculation")
	float GetAddionalDamage();
protected:

	UTLActionComponent* ActionComp;

	UTLAttributeComponent* AttributeComponent;

	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer BoostedTags;

	/*Action can only start if OwingAction has none of these Tags applied*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer ReduceTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTag AbilityTag;
};
